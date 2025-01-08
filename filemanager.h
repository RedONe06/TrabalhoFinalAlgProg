#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declaração de funções
void lerMapaDoArquivo(int nivel, MAPA *mapa);
void salvarJogo(int nivel, MAPA *mapa, JOGADOR *jogador);
void carregarJogo(int nivel, MAPA *mapa, JOGADOR *jogador);

#endif
