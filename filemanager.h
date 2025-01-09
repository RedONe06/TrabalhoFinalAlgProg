#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declara��o de fun��es
void lerMapaDoArquivo(int nivel, MAPA *mapa);
void salvarJogo(int *niveisPassados, MAPA *mapa, JOGADOR *jogador);
void carregarJogo(int *niveisPassados, MAPA *mapa, JOGADOR *jogador);
void acharMapasDisponiveis(int idMapas[MAX_MAPAS], int *nMapas);

#endif
