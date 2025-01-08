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

// Definição de posição genérica no mapa linha e coluna
typedef struct
{
    int lin;
    int col;
    int direcao;
} POSICAO;

// Informações de uma bomba
typedef struct
{
    bool ativa;
    time_t tempoInicio; // O tempo em que a bomba foi colocada
    POSICAO posicao;
} BOMBA;

//Informações dos baus
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

// Informações do jogador
typedef struct
{
    int nVidas; // Número de vidas disponíveis
    int nBombas; // Número de bombas restantes
    int pontuacao; // Pontuação atual do jogador
    int nChaves; // Número de chaves encontradas
    POSICAO posicao; // Posicao no mapa
    int direcao; // 0 = Para cima, 1 = Para baixo, 2 = Para esquerda, 3 = Para direita
    BOMBA bombas[3];
} JOGADOR;

#endif
