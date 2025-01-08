#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declara��o de fun��es
void lerMapaDoArquivo(int nivel, MAPA *mapa);
void salvarJogo(int nivel, MAPA *mapa, JOGADOR *jogador);
void carregarJogo(int nivel, MAPA *mapa, JOGADOR *jogador);

#endif
