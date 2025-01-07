#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declara��o de fun��es
void lerMapaDoArquivo(int nivel, char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50]);
void salvarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]);
void carregarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]);

#endif
