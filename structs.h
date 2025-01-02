#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Defini��o de boolean
#define True 1
#define False 0

// Defini��o de posi��o gen�rica no mapa linha e coluna
typedef struct
{
    int lin;
    int col;
} POSICAO;

// Informa��es do jogador
typedef struct
{
    int nVidas; // N�mero de vidas dispon�veis
    int nBombas; // N�mero de bombas restantes
    int pontuacao; // Pontua��o atual do jogador
    int nChaves; // N�mero de chaves encontradas
    POSICAO posicao; // Posicao no mapa
} JOGADOR;

// Informa��es de uma bomba
typedef struct
{
    bool ativa; // Status atual da bomba
    float tempoInicio; // O tempo em que a bomba foi colocada
    POSICAO posicao;
} BOMBA;

#endif