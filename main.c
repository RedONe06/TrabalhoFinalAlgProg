/* Este Trabalho Final busca consolidar o aprendizado em relação ao conteúdo teórico e prático abordado e exercitar a
habilidade de trabalho em equipe, consistindo na implementação de um jogo na linguagem de programação C.
O jogo a implementado é uma versão simplificada do jogo BomberMan, utilizando a biblioteca gráfica RayLib*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"
#include "canvas.h"
#include "structs.h"
#include "filemanager.h"
#include "constants.h"

/************ Protótipos *************/
void colocarBomba(JOGADOR *jogador, MAPA *mapa);
void verificaTimerDasBombas(MAPA *mapa, JOGADOR *jogador);
void explodirBomba(MAPA *mapa, BOMBA *bomba, JOGADOR *jogador);
// void novoJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[], BAU baus[50]); //cria um novo jogo
void controlarMovimentacao(MAPA *mapa, JOGADOR *jogador);
void controlarMenu(bool *menuEstaRodando, int nivel, MAPA *mapa, JOGADOR *jogador);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);
bool direcaoEstaLivre(MAPA *mapa, POSICAO posicaoAtual, int direcao);
void proximoNivel(int nivel, MAPA *mapa, JOGADOR *jogado);
void perderVida(JOGADOR *jogador);

int main()
{
    int nivel = 1;
    bool rodando = true; // Enquanto rodando == True o jogo irá rodar
    bool menuEstaRodando = false;

    MAPA mapa;
    mapa.nBaus = 0;
    mapa.nInimigos = 0;
    mapa.nParedesDestrutiveis = 0;
    mapa.nParedesIndestrutives = 0;

    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;
    jogador.bombas[0].ativa = false;
    jogador.bombas[1].ativa = false;
    jogador.bombas[2].ativa = false;

    lerMapaDoArquivo(nivel, &mapa);

    // Printa a matriz para verificar se o mapa foi recebido corretamente
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%c", mapa.matriz[i][j]);
        }
        printf("\n");
    }

    // Inicia a tela do jogo no tamanho especificado pelas variáveis
    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window");

    // Jogo formatado para 60 fps
    SetTargetFPS(60);

    // While principal do jogo
    while (rodando)
    {
        /****** DESENHO ******/
        BeginDrawing();
        ClearBackground(RAYWHITE); // Desenha o fundo branco

        desenharBarraStatus(&jogador);
        if (!menuEstaRodando) desenharMapa(&mapa, &jogador);
        else desenharMenu();

        /**** PERSONAGEM ****/
        //----------------------------------------------------------------------------------
        if(!menuEstaRodando)
        {
            controlarMovimentacao(&mapa, &jogador);
            if (IsKeyPressed(KEY_B)) colocarBomba(&jogador, &mapa);
            verificaTimerDasBombas(&mapa, &jogador);
        }
        //----------------------------------------------------------------------------------
        /**** MENU ****/
        if (!menuEstaRodando && IsKeyPressed(KEY_TAB)) // Ativar menu
        {
            menuEstaRodando = true;
        }
        else if (menuEstaRodando)
        {
            controlarMenu(&menuEstaRodando, nivel, &mapa, &jogador);
        }

        if (IsKeyPressed(KEY_ESCAPE)) rodando = false; // Sair do jogo
        //----------------------------------------------------------------------------------

        /**** MUDAR DE FASE ****/
        if (jogador.nChaves >= 5) proximoNivel(nivel, &mapa, &jogador);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    return 0;
}

/*********** Funções ************/

bool direcaoEstaLivre(MAPA *mapa, POSICAO posicaoAtual, int direcao)
{
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa->matriz[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return true;
        break;
    case 1: // Cima
        if (mapa->matriz[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return true;
        break;
    case 2: // Esquerda
        if (mapa->matriz[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return true;
        break;
    default: // Direita
        if (mapa->matriz[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return true;
    }

    return false;
}

POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao)
{
    POSICAO novaPosicao;

    switch(direcao)
    {
    case 0: // Baixo
        novaPosicao.col = posicaoAtual.col;
        novaPosicao.lin = posicaoAtual.lin + 1;
        break;
    case 1: // Cima
        novaPosicao.col = posicaoAtual.col;
        novaPosicao.lin = posicaoAtual.lin - 1;
        break;
    case 2: // Esquerda
        novaPosicao.col = posicaoAtual.col - 1;
        novaPosicao.lin = posicaoAtual.lin;
        break;
    default: // Direita
        novaPosicao.col = posicaoAtual.col + 1;
        novaPosicao.lin = posicaoAtual.lin;
    }

    return novaPosicao;
}

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, MAPA *mapa)
{
    // Se não tiver bombas ou espaço para colocar bomba, não colocar bomba
    if (jogador->nBombas <= 0 || !direcaoEstaLivre(mapa, jogador->posicao, jogador->direcao)) return;

    // Define a posição que a bomba vai ficar
    POSICAO posicaoBomba = acharProximaPosicao(jogador->posicao, jogador->direcao);
    // Pega o index da bomba baseado na quantidade disponível no arsenal
    int ultimaBomba = jogador->nBombas - 1;

    jogador->bombas[ultimaBomba].tempoInicio = time(NULL); // Armazena o tempo atual
    jogador->bombas[ultimaBomba].posicao.lin = posicaoBomba.lin;
    jogador->bombas[ultimaBomba].posicao.col = posicaoBomba.col;
    jogador->bombas[ultimaBomba].ativa = true;

    mapa->matriz[posicaoBomba.lin][posicaoBomba.col] = 'X'; // Coloca um B para marcar a bomba no mapa
    jogador->nBombas--; // Decrementa o número de bombas do arsenal
}

void verificaTimerDasBombas(MAPA *mapa, JOGADOR *jogador)
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (jogador->bombas[i].ativa == false) continue;

        double intervaloEmSegundos = difftime(time(NULL), jogador->bombas[i].tempoInicio)/10;
        intervaloEmSegundos = (intervaloEmSegundos - floor(intervaloEmSegundos)) * 10;
        if ((int)intervaloEmSegundos > 3)
        {
            // Se já se passaram 3 segundos do tempo de início da bomba, explodir ela
            explodirBomba(mapa, &jogador->bombas[i], jogador);
        }
    }
}

void explodirBomba(MAPA *mapa, BOMBA *bomba, JOGADOR *jogador)
{
    // Limpa o espaço da bomba e marca como um espaço vazio
    mapa->matriz[bomba->posicao.lin][bomba->posicao.col] = ' ';
    bomba->ativa = false;
    jogador->nBombas++;
    /*
    int i,j;

    for (i=-2; i<3; i++)
    {
        for (j=-2; j<3; j++)
        {
        switch(mapa->matriz[bomba->posicao.lin + i][bomba->posicao.col + j])
        {
        case 'D':
            mapa->matriz[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 10;
            break;
        case 'B':
            mapa->mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 10;
            break;
        case 'K':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = 'C';
            contadores[2]--;
            jogador->pontuacao += 10;
            break;
        case 'E':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 20;
            contadores[4]--;
            break;
        case 'J':
            perderVida(jogador);
            break;
        case 'X':
            //explodirBomba();
            break;
        default:
            break;

        }
        }
    }


    //desenharExplosao(mapa, jogador);*/
}


void controlarMovimentacao(MAPA *mapa, JOGADOR *jogador)
{
    if (IsKeyPressed(KEY_RIGHT) && direcaoEstaLivre(mapa, jogador->posicao, 3))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
        jogador->direcao = 3; // Direita
    }
    else if (IsKeyPressed(KEY_LEFT) && direcaoEstaLivre(mapa, jogador->posicao, 2))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
        jogador->direcao = 2; // Esquerda
    }
    else if (IsKeyPressed(KEY_UP) && direcaoEstaLivre(mapa, jogador->posicao, 1))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
        jogador->direcao = 1; // Cima
    }
    else if (IsKeyPressed(KEY_DOWN) && direcaoEstaLivre(mapa, jogador->posicao, 0))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
        jogador->direcao = 0; // Baixo
    }
}

void controlarMenu(bool *menuEstaRodando, int nivel, MAPA *mapa, JOGADOR *jogador)
{
    if(IsKeyPressed(KEY_N))
    {
        //novoJogo(nivel, mapa, jogador, contadores, baus);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_C))
    {
        printf("\nCarregar jogo (Implementar)");
        // carregarJogo(nivel, mapa, jogador, contadores, baus);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_S))
    {
        printf("\nSalvar jogo (Implementar)");
        // salvarJogo(nivel, mapa, jogador, contadores);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_Q))
    {
        printf("\nSaindo do jogo");
        CloseWindow();
    }
    else if (IsKeyPressed(KEY_V))
    {
        printf("Voltando ao jogo");
        *menuEstaRodando = false;
    }
}

/* Inicia o jogo do zero */
void novoJogo(int nivel, MAPA *mapa, JOGADOR *jogador)
{
    // Reinicia os contadores
    // contadores[0] = contadores[1] = contadores[2] = contadores[3] = contadores[4] = 0;
    nivel = 0;

    // Reinicia o jogador
    jogador->nVidas = 3;
    jogador->nBombas = 3;
    jogador->pontuacao = 0;
    jogador->nChaves = 0;

    // Reinicia as bombas
    jogador->bombas[0].ativa = false;
    jogador->bombas[1].ativa = false;
    jogador->bombas[2].ativa = false;

    // Lê novamente o arquivo e recria do 0
    //lerMapaDoArquivo(nivel, mapa);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

void proximoNivel(int nivel, MAPA *mapa, JOGADOR *jogador)
{
    nivel++;
    jogador->nChaves = 0;
    //desenharProxNivel();
    //lerMapaDoArquivo(nivel, mapa, contadores, baus);
}

void perderVida(JOGADOR *jogador)
{
    if (jogador->nVidas > 1)
    {
        jogador->nVidas--;
        jogador->pontuacao -= 100;
        if (jogador->pontuacao < 0) jogador->pontuacao = 0;
    }
    else
    {
        CloseWindow();
        printf("\n\n\t\t- Game over -");
    }
}
