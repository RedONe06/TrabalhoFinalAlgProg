#include <stdio.h>
#include "canvas.h"
#include "raylib.h"

#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Número máximo de conteúdos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

void desenharParedes(char mapa[LINHAS][COLUNAS])
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            switch (mapa[i][j])
            {
            case 'W': // Parede indestrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, SKYBLUE);
                break;
            case 'D': // Parede destrutivel
                DrawRectangle(i * TAM_BLOCO, j * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;
            default:
                continue;
            }
        }
    }
}

void desenharInimigos(char mapa[LINHAS][COLUNAS], POSICAO posicoes[])
{
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        DrawRectangle(posicoes[i].col, posicoes[i].lin, TAM_BLOCO, TAM_BLOCO, RED);
    }

}

void desenharBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[])
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (bombas[i].ativa)   //caso a bomba esteja ativa desenha ela
        {
            DrawRectangle(bombas[i].posicao.col * TAM_BLOCO, bombas[i].posicao.lin * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
        }
    }
}

void desenharChaves(char mapa[LINHAS][COLUNAS], POSICAO posicoes[])
{
    // Desenha as chaves
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        DrawRectangle(posicoes[i].col, posicoes[i].lin, TAM_BLOCO, TAM_BLOCO, GOLD);
    }
}
