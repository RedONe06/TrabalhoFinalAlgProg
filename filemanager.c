#include "structs.h"
#include "constants.h"
#include <stdio.h>

/* L� o arquivo *de texto* do mapa e transforma em um mapa matriz para a utiliza��o no jogo */
void lerMapaDoArquivo(int idMapa, MAPA *mapa)
{
    printf("\nLendo mapa [%d]", idMapa);
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
                if (mapa->matriz[i][j] == 'E') // Mapeia posi��o inicial dos inimigos
                {
                    mapa->inimigos[mapa->nInimigos].posicao.lin = i;
                    mapa->inimigos[mapa->nInimigos].posicao.col = j;
                    mapa->nInimigos++;
                }
            }

            getc(arq); // Pega o caractere \n ap�s a leitura de cada linha
        }

        fclose(arq);
    }
}

/* Salva o jogo em um arquivo binario */
void salvarJogo(int *niveisPassados, MAPA *mapa, JOGADOR *jogador)
{
    printf("\nSalvando o jogo...");

    // Cria um arquivo para salvar o jogo
    FILE *arq;
    arq = fopen("lastsave.bin","wb");

    if (arq == NULL)
    {
        printf("\nErro ao salvar o jogo.");
    }
    else
    {
        // Salva as informa��es...
        fwrite(niveisPassados, sizeof(int), 1, arq);
        fwrite(jogador, sizeof(JOGADOR), 1, arq);
        fwrite(mapa, sizeof(MAPA), 1, arq);
        fclose(arq);

        printf("\nSalvo com sucesso!");
    }
}

/* Carrega o jogo salvo de um arquivo bin�rio */
void carregarJogo(int *niveisPassados, MAPA *mapa, JOGADOR *jogador)
{
    printf("\nCarregando o jogo...");

    // L� o arquivo do jogo salvo
    FILE *arq;
    arq = fopen("lastsave.bin","rb");

    if (arq == NULL)
    {
        printf("\nErro ao carregar o jogo salvo.");
    }
    else
    {
        // Carrega as informa��es...
        fread(niveisPassados, sizeof(int), 1, arq);
        fread(jogador, sizeof(JOGADOR), 1, arq);
        fread(mapa, sizeof(MAPA), 1, arq);

        fclose(arq);

        printf("\nJogo carregado com sucesso!");
    }
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
            (*nMapas)++;
        }
    }
}
