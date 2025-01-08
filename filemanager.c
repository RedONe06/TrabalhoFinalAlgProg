#include "structs.h"
#include "constants.h"
#include <stdio.h>

/* Lê o arquivo *de texto* do mapa e transforma em um mapa matriz para a utilização no jogo */
void lerMapaDoArquivo(int idMapa, MAPA *mapa)
{
    FILE *arq;
    char nomeArquivo[50];
    sprintf(nomeArquivo, "mapa%d.txt", idMapa);
    arq = fopen(nomeArquivo, "r");

    if (arq == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return;
    }
    else
    {
        for (int i = 0; i < LINHAS; i++)
        {
            for (int j = 0; j < COLUNAS; j++)
            {
                mapa->matriz[i][j] = getc(arq); // Busca cada caractere do arquivo
                if (mapa->matriz[i][j] == 'E') // Mapeia posição inicial dos inimigos
                {
                    mapa->inimigos[mapa->nInimigos].posicao.lin = i;
                    mapa->inimigos[mapa->nInimigos].posicao.col = j;
                    mapa->nInimigos++;
                }
            }

            getc(arq); // Pega o caractere \n após a leitura de cada linha
        }

        fclose(arq);
    }
}

/* Salva o jogo em um arquivo binario */
void salvarJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[])
{
    int i;
    int lin, col;

    printf("\nSalvando o jogo...");

    // Cria um arquivo para salvar o jogo
    FILE *save;
    save = fopen("JogoSalvo.bomb","wb");

    if (save == NULL)
    {
        printf("\nErro ao salvar o jogo.");
    }
    else
    {
        // Salva as informações...
//        fwrite(nivel, sizeof(int), 1, save); //... do nivel
        fwrite(jogador, sizeof(int), 4, save); // ... do jogador
        fwrite(jogador->bombas, sizeof(BOMBA), MAX_BOMBAS, save); // ... das bombas

        // ... do mapa
        for (i = 0; i < LINHAS; i++)
        {
            fwrite(mapa, sizeof(char), 60, save);
            fwrite("\n", sizeof(char), 1, save);
        }

        fclose(save);

        printf("\n  Sucesso!");
    }
}

/* Carrega o jogo salvo de um arquivo binário */
void carregarJogo(int nivel, MAPA *mapa, JOGADOR *jogador)
{
    printf("\nCarregando o jogo...");

    // Lê o arquivo do jogo salvo
    FILE *arq;
    arq = fopen("JogoSalvo.bomb","rb");

    if (arq == NULL)
    {
        printf("\nErro ao carregar o jogo salvo.");
    }
    else
    {
        // Carrega as informações...
        fread(jogador, sizeof(int), 4, arq); // ... do jogador
        fread(jogador->bombas, sizeof(BOMBA), MAX_BOMBAS, arq); // ... das bombas
        fread(mapa, sizeof(char), TAMANHO_MAPA, arq); // ... do mapa

        fclose(arq);

        printf("\n  Jogo carregado com sucesso!");
    }

    // Cria o mapa de novo
    lerMapaDoArquivo(nivel, mapa);
}

void acharMapasDisponiveis(int idMapas[MAX_MAPAS], int *nMapas)
{
    for(int i = 1; i <= MAX_MAPAS; i++)
    {
        FILE *arq;
        char nomeArquivo[50];
        sprintf(nomeArquivo, "mapa%d.txt", i);
        arq = fopen(nomeArquivo, "r");

        if (arq == NULL)
        {
            continue;
        }
        else
        {
            fclose(arq);
            idMapas[*nMapas] = i;
            nMapas++;
        }
    }
}
