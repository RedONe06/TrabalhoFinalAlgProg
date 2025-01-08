#ifndef MAPA_H
#define MAPA_H
#include "structs.h"
#include "constants.h"

bool direcaoEstaLivre(MAPA *mapa, POSICAO posicaoAtual, int direcao);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);
MAPA iniciarMapa();
void matarInimigo();
void quebrarBau(MAPA *mapa);

#endif // MAPA_H
