#include "structs.h"

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Resolu��es em px
#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// N�mero m�ximo de conte�dos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

/* L� o arquivo *de texto* do mapa e transforma em um mapa matriz para a utiliza��o no jogo */
void lerMapaDoArquivo(int nivel, char mapa[LINHAS][COLUNAS], int contadores[], BAU baus[50])
{
    int i, j;

    FILE *arq;

    if (nivel == 1) arq = fopen("Mapa01.txt", "r");
    else if (nivel == 2) arq = fopen("Mapa02.txt", "r");
    else if (nivel == 3) arq = fopen("Mapa03.txt", "r");

    if (arq == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return;
    }
    else
    {
        for (i = 0; i < LINHAS; i++)
        {
            for (j = 0; j < COLUNAS; j++)
            {
                mapa[i][j] = getc(arq); // Busca cada caractere do arquivo
                if (mapa[i][j] == 'K')
                {
                    baus[contadores[2]].posicao.lin = i;
                    baus[contadores[2]].posicao.col = j;
                    baus[contadores[2]].temChave = True;
                    contadores[2]++; // quantidade de baus no mapa
                }
                else if (mapa[i][j] == 'B')
                {
                    baus[contadores[2]].posicao.lin = i;
                    baus[contadores[2]].posicao.col = j;
                    baus[contadores[2]].temChave = False;
                    contadores[2]++; // quantidade de baus no mapa
                }
                else if (mapa[i][j] == 'E') contadores[4]++;
            }

            getc(arq); // Pega o caractere \n ap�s a leitura de cada linha
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
        // Salva as informa��es...
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

/* Carrega o jogo salvo de um arquivo bin�rio */
void carregarJogo(int nivel, char mapa[LINHAS][COLUNAS], JOGADOR *jogador, int contadores[], BAU baus[50])
{
    printf("\nCarregando o jogo...");

    // L� o arquivo do jogo salvo
    FILE *arq;
    arq = fopen("JogoSalvo.bomb","rb");

    if (arq == NULL)
    {
        printf("\nErro ao carregar o jogo salvo.");
    }
    else
    {
        // Carrega as informa��es...
        fread(jogador, sizeof(int), 4, arq); // ... do jogador
        fread(jogador->bombas, sizeof(BOMBA), MAX_BOMBAS, arq); // ... das bombas
        fread(mapa, sizeof(char), TAMANHO_MAPA, arq); // ... do mapa

        fclose(arq);

        printf("\n  Jogo carregado com sucesso!");
    }

    // Cria o mapa de novo
    lerMapaDoArquivo(nivel, mapa, contadores, baus);
}
