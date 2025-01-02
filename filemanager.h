#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "structs.h"

// Declaração de funções
void lerMapaDoArquivo(char *nomeArquivo, char mapa[LINHAS][COLUNAS]);
void salvarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]);
void carregarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]);

#endif
