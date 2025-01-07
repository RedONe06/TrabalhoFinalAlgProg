#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declaração de funções
void lerMapaDoArquivo(int nivel, char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50]);
void salvarJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[]);
void carregarJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[], BAU baus[50]);

#endif
