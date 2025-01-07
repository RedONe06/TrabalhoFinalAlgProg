#include <stdio.h>
#include "canvas.h"
#include "raylib.h"

#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Dimensões máximas da tela do jogo em px
#define ALTURA 600
#define LARGURA 1200

// Número máximo de conteúdos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

void desenharMapa(char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            switch (mapa[i][j])
            {
            case 'J': // Jogador
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, GREEN);
                jogador->posicao.lin = i;
                jogador->posicao.col = j;
                break;
            case 'W': // Parede indestrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, SKYBLUE);
                break;
            case 'D': // Parede destrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;
            case 'B': // Baú
            case 'K': // Baú com chave
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
                break;
            case 'E': // Inimigo
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                break;
            case 'X':
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLACK);
            default:
                continue;
            }
        }
    }
}

void desenharBarraStatus(JOGADOR *jogador)
{
    DrawText(TextFormat("Vidas: %d\tBombas: %d", jogador->nVidas, jogador->nBombas), 50, 540, 20, BLACK);
    DrawText(TextFormat("Chaves: %d", jogador->nChaves), LARGURA / 2.2, 540, 20, BLACK);
    DrawText(TextFormat("Pontuacao: %d", jogador->pontuacao), LARGURA - 200, 540, 20, BLACK);
}

void desenharMenu()
{
    int yTitulo = ALTURA / 4;
    int xTitulo = LARGURA / 2;
    int xTexto = LARGURA / 3;

    DrawText("Menu", xTitulo, yTitulo, 40, BLACK);
    DrawText("[N] Novo jogo", xTexto, yTitulo + 30, 20, BLACK);
    DrawText("[C] Carregar jogo", xTexto, yTitulo + 55, 20, BLACK);
    DrawText("[S] Salvar jogo", xTexto, yTitulo + 80, 20, BLACK);
    DrawText("[Q] Sair do jogo", xTexto, yTitulo + 105, 20, BLACK);
    DrawText("[V] Voltar", xTexto, yTitulo + 130, 20, BLACK);
}

void desenharProxNivel()
{
    clock_t tempo = clock() + CLOCKS_PER_SEC * 4;
    while (tempo > clock()){
        DrawText("AVANCANDO PARA ", LARGURA / 5 , ALTURA / 2 - 85 , 80, BLACK);
        DrawText("A PRÓXIMA FASE", LARGURA / 5 , ALTURA / 2 , 80, BLACK);
    }
}

void desenharExplosao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador, POSICAO posBomba)
{
    /*
    for(int i = 0; i < 3; i++)
    {
        POSICAO proximaPosicao = acharProximaPosicao(posBomba, 0);
        char conteudo = mapa[proximaPosicao.lin][proximaPosicao.col];
        if (conteudo == 'J')
        {
            printf("\nMatar jogador");
            break;
        }
        else if(conteudo == 'W') {}
        else if(conteudo == 'D') {}
        else if(conteudo == 'B') {}
        else if(conteudo == 'K') {}
        else if(conteudo == 'E') {}
        else if(conteudo == 'X') {}
    }*/
}
