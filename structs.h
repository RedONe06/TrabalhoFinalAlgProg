#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

// Defini��o de posi��o gen�rica no mapa linha e coluna
typedef struct
{
    int lin;
    int col;
    int direcao;
} POSICAO;

// Informa��es de uma bomba
typedef struct
{
    bool ativa;
    time_t tempoInicio; // O tempo em que a bomba foi colocada
    POSICAO posicao;
} BOMBA;

//Informa��es dos baus
typedef struct
{
    POSICAO posicao;
    bool temChave;
} BAU;

typedef struct{
    POSICAO posicao;
} INIMIGO;

typedef struct
{
    char matriz[LINHAS][COLUNAS];
    BAU baus[50];
    int nBaus;
    INIMIGO inimigos[MAX_INIMIGOS];
    int nInimigos;
    int nParedesDestrutiveis;
    int nParedesIndestrutives;
} MAPA;

// Informa��es do jogador
typedef struct
{
    int nVidas; // N�mero de vidas dispon�veis
    int nBombas; // N�mero de bombas restantes
    int pontuacao; // Pontua��o atual do jogador
    int nChaves; // N�mero de chaves encontradas
    POSICAO posicao; // Posicao no mapa
    int direcao; // 0 = Para cima, 1 = Para baixo, 2 = Para esquerda, 3 = Para direita
    BOMBA bombas[3];
} JOGADOR;

#endif
