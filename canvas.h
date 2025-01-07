#ifndef CANVAS_H
#include "structs.h"
#define CANVAS_H

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Declara��o de fun��es
void desenharMapa(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
void desenharMenu();
void desenharProxNivel();
void desenharBarraStatus(JOGADOR *jogador);
void desenharExplosao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador, POSICAO posBomba);
#endif
