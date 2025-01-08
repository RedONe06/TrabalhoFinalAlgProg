#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "structs.h"
#include "constants.h"

void proximoNivel(int nivel, MAPA *mapa, JOGADOR *jogador);
void novoJogo(int nivel, MAPA *mapa, JOGADOR *jogador);
void controlarMenu(bool *menuEstaRodando, int nivel, MAPA *mapa, JOGADOR *jogador);
bool jaPassouSegundos(time_t tempoInicial, int segundos);

#endif // GAMEMANAGER_H
