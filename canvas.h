#ifndef CANVAS_H
#include "structs.h"
#define CANVAS_H

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Declaração de funções
void desenharParedes(char mapa[LINHAS][COLUNAS]);
void desenharInimigos(char mapa[LINHAS][COLUNAS], POSICAO posicoes[]);
void desenharBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[]);
void desenharChaves(char mapa[LINHAS][COLUNAS], POSICAO posicoes[]);

#endif
