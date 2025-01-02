#include <stdio.h>
#include "canvas.h"
#include "raylib.h"

#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

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
            case 'E': // Chave
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                break;
            default:
                continue;
            }
        }
    }
}
