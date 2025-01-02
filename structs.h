#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Definição de boolean
#define True 1
#define False 0

// Definição de posição genérica no mapa linha e coluna
typedef struct
{
    int lin;
    int col;
} POSICAO;

// Informações do jogador
typedef struct
{
    int nVidas; // Número de vidas disponíveis
    int nBombas; // Número de bombas restantes
    int pontuacao; // Pontuação atual do jogador
    int nChaves; // Número de chaves encontradas
    POSICAO posicao; // Posicao no mapa
    int direcao; // 0 = Para cima, 1 = Para baixo, 2 = Para esquerda, 3 = Para direita
} JOGADOR;

// Informações de uma bomba
typedef struct
{
    time_t tempoInicio; // O tempo em que a bomba foi colocada
    POSICAO posicao;
} BOMBA;

#endif
