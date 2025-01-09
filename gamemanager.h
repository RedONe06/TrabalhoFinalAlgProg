#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "structs.h"
#include "constants.h"

void proximoNivel(int nivel, MAPA *mapa, JOGADOR *jogador);
void novoJogo(MAPA *mapa, JOGADOR *jogador, bool *trocandoDeNivel, int *niveisPassados, int indexMapas[MAX_MAPAS]);
void controlarMenu(bool *menuEstaRodando, MAPA *mapa, JOGADOR *jogador, bool *trocandoDeNivel, int *niveisPassados, int indexMapas[MAX_MAPAS]);
bool jaPassouSegundos(time_t tempoInicial, int segundos);
void iniciarVariaveis(MAPA *mapa, JOGADOR *jogador);
void trocarDeNivel(MAPA *mapa, int indexMapas[MAX_MAPAS], int *niveisDisponiveis, int *niveisPassados);
void terminarJogo();

#endif // GAMEMANAGER_H
