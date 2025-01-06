#ifndef CANVAS_H
#include "structs.h"
#define CANVAS_H

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Declaração de funções
void desenharMapa(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
void desenharMenu();
void desenharBarraStatus(JOGADOR *jogador);
#endif
