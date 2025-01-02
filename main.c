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
#define MAX_CHAVES 3
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
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); // menu de pausa
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[]);//coloca as bombas no mapa
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa); //explode as bombas
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); //cria um novo jogo
void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);

/************* Main **************/
int main()
{
    bool rodando = true; // Enquanto rodando == True o jogo irá rodar

    char mapa[LINHAS][COLUNAS];

    // Vetor para contar o numero de  paredes indestrutiveis, destrutiveis, baus com chaves, baus sem chaves e inimigos.
    int contadores[5] = {0, 0, 0, 0, 0};
    /*[0] = paredes indestrutiveis
      [1] = paredes destrutiveis
      [2] = baus com chaves
      [3] = baus sem chaves
      [4] = inimigos */

    lerMapaDoArquivo("bombermap.txt", mapa);

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

    BOMBA bombas[MAX_BOMBAS] = { 0 }; // Inicia as bombas como inativas

    // Informações sobre o jogador
    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;

    // Criar o mapa adicionando as posições dos objetos às respectivas structs
    //criarMapa(mapa, &posJogador, posInimigos, posChaves, contadores);

    // While principal do jogo
    while (rodando)
    {
        /****** Desenhar ******/
        BeginDrawing();
        ClearBackground(RAYWHITE); // Desenha o fundo branco

        DrawText(TextFormat("Vidas: %d\tBombas: %d\tPontos: %d\tChaves: %d", jogador.nVidas, jogador.nBombas, jogador.pontuacao, jogador.nChaves), 0, 520, 20, BLACK);//escreve vidas, bombas, pontos e chaves.
        desenharMapa(mapa, &jogador);

        /**** MOVER O PERSONAGEM + MENU ****/
        //----------------------------------------------------------------------------------
        controlarMovimentacao(mapa, &jogador);
        if (IsKeyPressed(KEY_SPACE)) colocarBomba(&jogador, mapa, bombas);
        if (IsKeyPressed(KEY_TAB)) menu(&rodando, mapa, &posJogador, posInimigos, posChaves, &jogador, bombas, contadores); //abrir menu
        if (IsKeyPressed(KEY_ESCAPE)) rodando = False;//sair do jogo (apenas para testar)
        //----------------------------------------------------------------------------------

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    return 0;
}

/*********** Funções ************/

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[])
{
    int lin = jogador->posicao.lin;
    int col = jogador->posicao.col;

    // Se não tiver bombas, não há para colocar
    if (jogador->nBombas <= 0) return;

    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (bombas[i].ativa)
        {
            continue;
        }

        // Para cada bomba inativa
        if (mapa[lin][col] == ' ')  // Verifica se o espaço do jogador está vazio
        {
            lin = jogador->posicao.lin;
            col = jogador->posicao.col;

            bombas[i].ativa = True; // Ativa a bomba
            bombas[i].tempoInicio = 0; // Armazena o tempo atual
            bombas[i].posicao.lin = lin;
            bombas[i].posicao.col = col;

            mapa[lin][col] = 'B'; // Coloca um B para marcar a bomba no mapa
            jogador->nBombas--; // Decrementa o número de bombas
            break;
        }

    }
}

/* Verifica se os 3 segundos da bomba se passaram */
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (bombas[i].ativa)
        {
            if (4 >= 3.0) /**PRECISO DE ALGO UE CONTE POR 3 SEGUNDOS SEM PARAR COMPLETAMENTE O JOGO**/
            {

                explodirBomba(mapa, bombas[i].posicao.lin, bombas[i].posicao.col, bombas[i].ativa);//explode a bomba

                jogador->nBombas++; //retorna a bomba pro inventario
                bombas[i].ativa = false; //bomba fica como inativa
            }
        }
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa)
{
    // Se não está ativa, não pode explodir
    if (!ativa) return;

    // Limpa o espaço da bomba e marca como um espaço vazio
    mapa[lin][col] = ' ';

    /* Aplica os efeitos da explosões em todas as direções */

    // Cima (exemplo de como todos os fors funcionam)*/
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaLinha = lin - i;
        char blocoAtual = mapa[novaLinha][col];

        // Se encontrar uma parede indestrutivel
        if (novaLinha >= 0 && blocoAtual == 'W')
        {
            break; //Para a explosão (Não destrói nada atrás da parede)
        }
        else
        {
            // Se !'W' desenha uma explosão
            DrawRectangle(novaLinha, col, TAM_BLOCO, TAM_BLOCO, ORANGE);// 20x20 Cor: laranja
        }

        if (novaLinha < 0) continue;

        // Se a parede for destrutível, destroi ela
        if (blocoAtual == 'D')
        {
            mapa[lin - i][col] = ' ';
            break; //Para a explosão (Não destrói nada atrás da parede)
        }

        // Se encontrar outra bomba, explode ela imediatamente
        if (blocoAtual == 'B')
        {
            mapa[lin - i][col] = ' '; //marca como espaço vazio
            explodirBomba(mapa, novaLinha, col, true); //explode aquela bomba
        }
    }

    // Baixo
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaLinha = lin + i;
        char blocoAtual = mapa[novaLinha][col];

        if (novaLinha < LINHAS && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaLinha, col, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaLinha < LINHAS && blocoAtual == 'D')
        {
            mapa[lin + i][col] = ' ';
            break;
        }
        if (novaLinha >= 0 && blocoAtual == 'B')
        {
            mapa[lin + i][col] = ' ';
            explodirBomba(mapa, novaLinha, col, true);
        }
    }

    // Esquerda
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaColuna = col - i;
        char blocoAtual = mapa[lin][novaColuna];

        if (novaColuna >= 0 && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaColuna, lin, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaColuna < 0) continue;

        if (blocoAtual == 'D')
        {
            mapa[lin][col - i] = ' ';
            break;
        }
        if (blocoAtual == 'B')
        {
            mapa[lin][col - i] = ' ';
            explodirBomba(mapa, lin, novaColuna, true);
        }
    }

    // Direita
    for (int i = 1; i <= 3; i++)
    {
        int novaColuna = col + i;
        char blocoAtual = mapa[lin][novaColuna];

        if (novaColuna < COLUNAS && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaColuna, lin, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaColuna < COLUNAS && blocoAtual == 'D')
        {
            mapa[lin][col + i] = ' ';
            break;
        }
        if (novaColuna >= 0 && blocoAtual == 'B')
        {
            mapa[lin][col + i] = ' ';
            explodirBomba(mapa, lin, novaColuna, true);
        }
    }
}

/* Roda o menu de pausa */
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    printf("\nEntrou no menu.");
    *rodando = false;
    int keyPressed = 0;
    char opcao = ' ';
    bool i = true;

    do
    {
        keyPressed = GetCharPressed();
        opcao = (char)keyPressed;
        opcao = toupper(opcao); // Normalização da opção

        switch (opcao)
        {
        case 'N': // Iniciar um novo jogo do zero
            novoJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'C': // Carregar um arquivo binario de um jogo salvo
            carregarJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'S': // Salvar o jogo (PRECISA SER EM UM ARQUIVO BINÁRIO)
            salvarJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'Q':  // Sair do jogo sem salvar
            printf("\nFechando...");
            CloseWindow();
            *rodando = false;
            i = false;
            printf("  Sucesso!\n");
            break;

        case 'V': // Retornar ao jogo
            printf("\nRetornou ao jogo!");
            i = false;
            *rodando = true;
            break;

        default:
            printf("\nComando incompreendido");
        }
    }
    while (i == true);
}

/* Inicia o jogo do zero */
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    // Reinicia os contadores
    contadores[0] = contadores[1] = contadores[2] = contadores[3] = contadores[4] = 0;

    // Reinicia o jogador
    jogador->nVidas = 3;
    jogador->nBombas = 3;
    jogador->pontuacao = 0;
    jogador->nChaves = 0;

    // Reinicia as bombas
    for (int i = 0; i<MAX_BOMBAS; i++)
        bombas[i].ativa = false;

    // Lê novamente o arquivo e recria do 0
    lerMapaDoArquivo("bombermap.txt", mapa);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    int linha = jogador->posicao.lin;
    int coluna = jogador->posicao.col;

    char proxDireita = mapa[linha][coluna + 1];
    char proxEsquerda = mapa[linha][coluna - 1];
    char proxCima = mapa[linha - 1][coluna];
    char proxBaixo = mapa[linha + 1][coluna];

    if (IsKeyPressed(KEY_RIGHT) && proxDireita == ' ')
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
    }
    else if (IsKeyPressed(KEY_LEFT) && proxEsquerda == ' ')
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
    }
    else if (IsKeyPressed(KEY_UP) && proxCima == ' ')
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
    }
    else if (IsKeyPressed(KEY_DOWN) && proxBaixo == ' ')
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
    }
}
