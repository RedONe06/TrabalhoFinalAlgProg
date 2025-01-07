#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declaração de funções
void lerMapaDoArquivo(int nivel, char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50]);
void salvarJogo(char mapa[LINHAS][COLUNAS],  JOGADOR *jogador, BOMBA bombas[], int contadores[], BAU baus[50]);
void carregarJogo(char mapa[LINHAS][COLUNAS], JOGADOR *jogador, BOMBA bombas[], int contadores[], BAU baus[50]);

#endif
