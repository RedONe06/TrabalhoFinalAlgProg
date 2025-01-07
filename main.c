/* Este Trabalho Final busca consolidar o aprendizado em relação ao conteúdo teórico e prático abordado e exercitar a
habilidade de trabalho em equipe, consistindo na implementação de um jogo na linguagem de programação C.
O jogo a implementado é uma versão simplificada do jogo BomberMan, utilizando a biblioteca gráfica RayLib*/

/*********** Includes e Defines ***********/

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

// Número máximo de conteúdos
#define MAX_BOMBAS 3
#define MAX_CHAVES 5
#define MAX_INIMIGOS 5

// Dimensões máximas da tela do jogo em px
#define ALTURA 600
#define LARGURA 1200

// Resoluções em px
#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Outras definições
#define ALCANCE_BLOCOS_BOMBA 3

/************ Protótipos *************/
void criarInimigo(POSICAO posInimigos[], int lin, int col, int num);
void criarChave(POSICAO posChaves[], int lin, int col, int num);
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS]);
void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[]);
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA *bomba, JOGADOR *jogador, int contadores[]);
void novoJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[], BAU baus[50]); //cria um novo jogo
void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
void controlarMenu(bool *menuEstaRodando, int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador,  int contadores[], BAU baus[50]);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);
bool direcaoEstaLivre(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao);
int objetoNaDirecao(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao);
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[]);
void mudarDirecaoInimigos(POSICAO posInimigos[], clock_t *timer, int contadores[]);
void proximoNivel(int nivel, char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50], JOGADOR *jogado);
void perderVida(JOGADOR *jogador);

/************* Main **************/
int main()
{
    clock_t timer_inimigos = 1 + clock();
    clock_t timer_direcao = clock();

    int nivel = 1; //qual Mapa0X.txt abrir

    bool rodando = true; // Enquanto rodando == True o jogo irá rodar
    bool menuEstaRodando = false;

    char mapa[LINHAS][COLUNAS];

    // Vetor para contar o numero de  paredes indestrutiveis, destrutiveis, baus com chaves, baus sem chaves e inimigos.
    int contadores[5] = {0, 0, 0, 0, 0};
    /*[0] = paredes indestrutiveis
      [1] = paredes destrutiveis
      [2] = baus com chaves e sem chaves
      [3] = chaves
      [4] = inimigos */

    BAU baus[50];

    lerMapaDoArquivo(nivel, mapa, contadores, baus);

    // Printa a matriz para verificar se o mapa foi recebido corretamente
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }

    // Inicia a tela do jogo no tamanho especificado pelas variáveis
    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window");

    // Jogo formatado para 60 fps
    SetTargetFPS(60);

    // Posições dos inimigos, chaves e jogador
    POSICAO posInimigos[MAX_INIMIGOS];
    POSICAO posChaves[MAX_CHAVES];
    POSICAO posJogador;

    // Informações sobre o jogador
    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;
    jogador.bombas[0].ativa = false;
    jogador.bombas[1].ativa = false;
    jogador.bombas[2].ativa = false;

    // While principal do jogo
    while (rodando)
    {

        /****** DESENHO ******/
        BeginDrawing();
        ClearBackground(RAYWHITE); // Desenha o fundo branco

        desenharBarraStatus(&jogador);
        if (!menuEstaRodando) desenharMapa(mapa, &jogador);
        else desenharMenu();

        /**** PERSONAGEM ****/
        //----------------------------------------------------------------------------------
        if(!menuEstaRodando)
        {
            controlarMovimentacao(mapa, &jogador);
            if (IsKeyPressed(KEY_B)) colocarBomba(&jogador, mapa);
            verificaTimerDasBombas(mapa, &jogador, contadores);
        }
        //----------------------------------------------------------------------------------
        /**** MENU ****/
        if (!menuEstaRodando && IsKeyPressed(KEY_TAB)) // Ativar menu
        {
            menuEstaRodando = true;
        }
        else if (menuEstaRodando)
        {
            controlarMenu(&menuEstaRodando, nivel, mapa, &jogador, contadores, baus);
        }

        if (IsKeyPressed(KEY_ESCAPE)) rodando = false; // Sair do jogo
        //----------------------------------------------------------------------------------

        /**** MUDAR DE FASE ****/
        if (jogador.nChaves >= 5) proximoNivel(nivel, mapa, contadores, baus, &jogador);


        //verificaTimerDasBombas(mapa, bombas, &jogador);
        EndDrawing();

        //----------------------------------------------------------------------------------

        if (clock() >= timer_direcao) mudarDirecaoInimigos(posInimigos, &timer_direcao, contadores);
        //if (clock() >= timer_inimigos) moverInimigos(posInimigos, &timer_inimigos, mapa, &jogador, contadores);

        // printf("\nTimer Aleatorio: %ld, Timer Inimigos: %ld, Clock Atual: %ld", timer_direcao, timer_inimigos, clock());
    }



    return 0;
}

/*********** Funções ************/

/* A cada 3 segundos escolhe uma direcao nova aleatoria para cada inimigo*/
void mudarDirecaoInimigos(POSICAO posInimigos[], clock_t *timer, int contadores[])
{
    int i, num;

    for (i = 0; i < contadores[4]; i++)
    {
        num = rand() % 4 + 1;
        posInimigos[i].direcao = num;
    }

    *timer = 3 * CLOCKS_PER_SEC + clock();
}

/* A cada segundo, se possível, move o inimigo na direção determinada*/
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS],JOGADOR *jogador,int contadores[])
{
    int i;
    bool moveu;
    for (i=0; i < contadores[4]; i++)
    {
        int tentativas = 0; //Contador de tentativas para o código não ficar em um loop infinito caso tenha paredes nas 4 direções
        moveu = False;
        while(!moveu && tentativas < 4)
        {
            switch (posInimigos[i].direcao)
            {
            case 1:

                if (mapa[posInimigos[i].lin + 1][posInimigos[i].col] == ' ') //Verifica se há uma parede no caminho
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' '; //limpa a região onde estava
                    posInimigos[i].lin++; // muda a posição
                    moveu = True; //muda a booleana
                }
                else if (mapa[posInimigos[i].lin + 1][posInimigos[i].col] == 'J')  // se encontrar o jogador...
                {
                    perderVida(jogador); // ...tira uma vida dele
                }

                break;
            case 2:

                if (mapa[posInimigos[i].lin][posInimigos[i].col + 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col++;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin][posInimigos[i].col + 1] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            case 3:

                if (mapa[posInimigos[i].lin - 1][posInimigos[i].col] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].lin--;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin - 1][posInimigos[i].col] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            case 4:

                if (mapa[posInimigos[i].lin][posInimigos[i].col - 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col--;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin ][posInimigos[i].col - 1] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            }
            if (!moveu) //se encontrou uma parede ele muda de direção e aumenta as tentativas
            {
                posInimigos[i].direcao = (posInimigos[i].direcao % 4) + 1;
                tentativas++;
            }
        }
        mapa[posInimigos[i].lin][posInimigos[i].col] = 'E'; //coloca um 'E' na nova posição
    }


    *timer = clock() + CLOCKS_PER_SEC; // atualiza o timer
}

/* Dado um mapa, uma posicao atual e uma direção para movimento, checa se a posicao seguinte é possível (está vazia) ou não */
int objetoNaDirecao(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao)
{
    // 0 == Parede, 1 == Livre, 2 == Inimigo, 3 == Chave
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return 1;
        else if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == 'E') return 2;
        else if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == 'C') return 3;
        break;
    case 1: // Cima
        if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return 1;
        else if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == 'E') return 2;
        else if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == 'C') return 3;
        break;
    case 2: // Esquerda
        if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return 1;
        else if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == 'E') return 2;
        else if (mapa[posicaoAtual.lin ][posicaoAtual.col - 1] == 'C') return 3;
        break;
    default: // Direita
        if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return 1;
        else if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == 'E') return 2;
        else if (mapa[posicaoAtual.lin ][posicaoAtual.col + 1] == 'C') return 3;
    }

    return 0;
}

bool direcaoEstaLivre(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao)
{
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return true;
        break;
    case 1: // Cima
        if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return true;
        break;
    case 2: // Esquerda
        if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return true;
        break;
    default: // Direita
        if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return true;
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
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS])
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

    mapa[posicaoBomba.lin][posicaoBomba.col] = 'X'; // Coloca um B para marcar a bomba no mapa
    jogador->nBombas--; // Decrementa o número de bombas do arsenal
}

void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[])
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (jogador->bombas[i].ativa == false) continue;

        double intervaloEmSegundos = difftime(time(NULL), jogador->bombas[i].tempoInicio)/10;
        intervaloEmSegundos = (intervaloEmSegundos - floor(intervaloEmSegundos)) * 10;
        if ((int)intervaloEmSegundos > 3)
        {
            // Se já se passaram 3 segundos do tempo de início da bomba, explodir ela
            explodirBomba(mapa, &jogador->bombas[i], jogador, contadores);
        }
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA *bomba, JOGADOR *jogador, int contadores[])
{
    int i,j;
    // Limpa o espaço da bomba e marca como um espaço vazio
    mapa[bomba->posicao.lin][bomba->posicao.col] = ' ';
    for (i=-2; i<3; i++)
    {
        for (j=-2; j<3; j++)
        {
        switch(mapa[bomba->posicao.lin + i][bomba->posicao.col + j])
        {
        case 'D':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 10;
            break;
        case 'B':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            contadores[2]--;
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

    bomba->ativa = false;
    jogador->nBombas++;
    //desenharExplosao(mapa, jogador);
}


void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    if (IsKeyPressed(KEY_RIGHT))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 3))
        {
        case 1: // se encontrar um espaço livre
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
            jogador->direcao = 3; // Direita
            break;
        case 2: // se encontrar um inimigo
            perderVida(jogador);
            break;
        case 3: // se encontrar uma chave
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin ][jogador->posicao.col +1] = 'J';
            jogador->nChaves++;
            jogador->pontuacao += 50;
            jogador->direcao = 3;
            break;
        }
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        switch (direcaoEstaLivre(mapa, jogador->posicao, 2))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin ][jogador->posicao.col - 1] = 'J';
            jogador->direcao = 2; // Esquerda
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
            jogador->nChaves++;
            jogador->pontuacao += 50;
            jogador->direcao = 2;
            break;
        }
    }
    else if (IsKeyPressed(KEY_UP))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 1))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
            jogador->direcao = 1; // Cima
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
            jogador->nChaves++;
            jogador->pontuacao += 50;
            jogador->direcao = 1;
            break;
        }
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 0))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
            jogador->direcao = 0; // Baixo
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
            jogador->nChaves++;
            jogador->pontuacao += 50;
            jogador->direcao = 0;
            break;
        }
    }
}

void controlarMenu(bool *menuEstaRodando, int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador,  int contadores[], BAU baus[50])
{
    if(IsKeyPressed(KEY_N))
    {
        novoJogo(nivel, mapa, jogador, contadores, baus);
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
void novoJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[], BAU baus[50])
{
    // Reinicia os contadores
    contadores[0] = contadores[1] = contadores[2] = contadores[3] = contadores[4] = 0;
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
    lerMapaDoArquivo(nivel, mapa, contadores, baus);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

void proximoNivel(int nivel,char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50], JOGADOR *jogador)
{
    nivel++;
    jogador->nChaves = 0;
    //desenharProxNivel();
    lerMapaDoArquivo(nivel, mapa, contadores, baus);
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
