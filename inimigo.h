#ifndef INIMIGO_H
#define INIMIGO_H
#include "structs.h"
#include "constants.h"

void verificaTimerDosInimigos(MAPA *mapa);
void moverInimigos(MAPA *mapa);
void moverInimigo(MAPA *mapa, INIMIGO *inimigo);

#endif // INIMIGO_H
