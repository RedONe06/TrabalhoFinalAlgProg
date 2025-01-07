#include "structs.h"

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Resoluções em px
#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Número máximo de conteúdos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

/* Lê o arquivo *de texto* do mapa e transforma em um mapa matriz para a utilização no jogo */
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
                } else if (mapa[i][j] == 'E') contadores[4]++;
            }

            getc(arq); // Pega o caractere \n após a leitura de cada linha
        }
        fclose(arq);
    }
}

/* Salva o jogo em um arquivo binario */
void salvarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    int i;
    int lin, col;

    printf("\nSalvando o jogo...");

    // Atualiza a posição dos inimigos no mapa
    for (i = 0; i < contadores[4]; i++)
    {
        lin = posInimigos[i].lin / TAM_BLOCO;
        col = posInimigos[i].col / TAM_BLOCO;
        mapa[lin][col] = 'E';
    }

    // Atualiza a posiçao do jogador no mapa
    lin = posJogador->lin / TAM_BLOCO;
    col = posJogador->col / TAM_BLOCO;
    mapa[lin][col] = 'J';

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
        fwrite(jogador, sizeof(int), 4, save); // ... do jogador
        fwrite(bombas, sizeof(BOMBA), MAX_BOMBAS, save); // ... das bombas

        // ... do mapa
        for (i = 0; i < LINHAS; i++)
        {
            fwrite(mapa, sizeof(char), 60, save);
            fwrite("\n", sizeof(char), 1, save);
        }

        fclose(save);

        printf("  Sucesso!");
    }
}

/* Carrega o jogo salvo de um arquivo binário */
void carregarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
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
        fread(bombas, sizeof(BOMBA), MAX_BOMBAS, arq); // ... das bombas
        fread(mapa, sizeof(char), TAMANHO_MAPA, arq); // ... do mapa

        fclose(arq);

        printf("  Jogo carregado com sucesso!");
    }

    // Cria o mapa de novo
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);
}
