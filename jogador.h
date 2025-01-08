#ifndef JOGADOR_H
#define JOGADOR_H
#include "structs.h"
#include "constants.h"

void controlarMovimentacao(MAPA *mapa, JOGADOR *jogador);
void perderVida(JOGADOR *jogador);
JOGADOR iniciarJogador();
void matarJogador();

#endif // JOGADOR_H
