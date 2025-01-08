#ifndef CANVAS_H
#define CANVAS_H
#include "structs.h"
#include "constants.h"

// Declaração de funções
void desenharMapa(MAPA *mapa, JOGADOR *jogador);
void desenharMenu();
void desenharProxNivel();
void desenharBarraStatus(JOGADOR *jogador);
void desenharExplosao(MAPA *mapa, JOGADOR *jogador, POSICAO posBomba);
#endif
