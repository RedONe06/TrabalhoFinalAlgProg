#ifndef BOMBA_H
#define BOMBA_H
#include "structs.h"
#include "constants.h"

void colocarBomba(JOGADOR *jogador, MAPA *mapa);
void verificaTimerDasBombas(MAPA *mapa, JOGADOR *jogador);
void explodirBomba(MAPA *mapa, BOMBA *bomba, JOGADOR *jogador);

#endif // BOMBA_H
