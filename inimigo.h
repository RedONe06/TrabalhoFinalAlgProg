#ifndef INIMIGO_H
#define INIMIGO_H
#include "structs.h"
#include "constants.h"

void verificaTimerDosInimigos(MAPA *mapa, JOGADOR *jogador);
void moverInimigos(MAPA *mapa, JOGADOR *jogador);
void moverInimigo(MAPA *mapa, INIMIGO *inimigo, JOGADOR *jogador);
void matarInimigo(MAPA *mapa, POSICAO posicao);

#endif // INIMIGO_H
