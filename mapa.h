#ifndef MAPA_H
#define MAPA_H
#include "structs.h"
#include "constants.h"

bool direcaoEstaLivre(MAPA *mapa, POSICAO posicaoAtual, int direcao);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);

#endif // MAPA_H
