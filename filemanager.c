#include "structs.h"
#include "constants.h"

/* L� o arquivo *de texto* do mapa e transforma em um mapa matriz para a utiliza��o no jogo */
void lerMapaDoArquivo(int nivel, MAPA *mapa)
{
    int i, j;

    FILE *arq;
    arq = fopen("bombermap.txt", "r");

    /*
    if (nivel == 1) arq = fopen("Mapa01.txt", "r");
    else if (nivel == 2) arq = fopen("Mapa02.txt", "r");
    else if (nivel == 3) arq = fopen("Mapa03.txt", "r");*/

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
                mapa->matriz[i][j] = getc(arq); // Busca cada caractere do arquivo
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
void carregarJogo(int nivel, MAPA *mapa, JOGADOR *jogador)
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
    lerMapaDoArquivo(nivel, mapa);
}
