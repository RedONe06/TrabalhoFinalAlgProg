#ifndef CANVAS_H
#include "structs.h"
#define CANVAS_H

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Declara��o de fun��es
void desenharParedes(char mapa[LINHAS][COLUNAS]);
void desenharInimigos(char mapa[LINHAS][COLUNAS], POSICAO posicoes[]);
void desenharBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[]);
void desenharChaves(char mapa[LINHAS][COLUNAS], POSICAO posicoes[]);

#endif
