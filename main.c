/* Este Trabalho Final busca consolidar o aprendizado em rela��o ao conte�do te�rico e pr�tico abordado e exercitar a
habilidade de trabalho em equipe, consistindo na implementa��o de um jogo na linguagem de programa��o C.
O jogo a implementado � uma vers�o simplificada do jogo BomberMan, utilizando a biblioteca gr�fica RayLib*/

/*********** Includes e Defines ***********/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "raylib.h"

// Defini��o de boolean
#define True 1
#define False 0

// N�mero m�ximo de conte�dos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

// Dimens�es m�ximas da tela do jogo em px
#define ALTURA 600
#define LARGURA 1200

// Resolu��es em px
#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Outras defini��es
#define ALCANCE_BLOCOS_BOMBA 3

/************ Structs ***************/

// Defini��o de posi��o gen�rica no mapa linha e coluna
typedef struct
{
    int lin;
    int col;
} POSICAO;

// Informa��es do jogador
typedef struct
{
    int nVidas; // N�mero de vidas dispon�veis
    int nBombas; // N�mero de bombas restantes
    int pontuacao; // Pontua��o atual do jogador
    int nChaves; // N�mero de chaves encontradas
} JOGADOR;

// Informa��es de uma bomba
typedef struct
{
    bool ativa; // Status atual da bomba
    float tempoInicio; // O tempo em que a bomba foi colocada
    POSICAO posicao;
} BOMBA;


/************ Prot�tipos *************/
void criarInimigo(POSICAO posInimigos[], int lin, int col, int num);
void criarChave(POSICAO posChaves[], int lin, int col, int num);
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); // menu de pausa
void lerMapaDoArquivo(char *nomeArquivo, char mapa[LINHAS][COLUNAS]); //transforma o mapa.txt em matriz
void criarMapa(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], int contadores[]); // Cria o mapa
void colocarBomba(JOGADOR *jogador, POSICAO *posJogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[]);//coloca as bombas no mapa
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa); //explode as bombas
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); //cria um novo jogo
void carregarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]);
void salvarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); //salva o jogo atual

/************* Main **************/
int main()
{
    bool rodando = True; // Enquanto rodando == True o jogo ir� rodar
    int j, k;

    char mapa[LINHAS][COLUNAS];

    // Vetor para contar o numero de  paredes indestrutiveis, destrutiveis, baus com chaves, baus sem chaves e inimigos.
    int contadores[5] = {0, 0, 0, 0, 0};
    /*[0] = paredes indestrutiveis
      [1] = paredes destrutiveis
      [2] = baus com chaves
      [3] = baus sem chaves
      [4] = inimigos */

    lerMapaDoArquivo("bombermap.txt", mapa);

    // Printa a matriz para verificar se o mapa foi recebido corretamente
    for (int i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }

    // Inicia a tela do jogo no tamanho especificado pelas vari�veis
    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window");

    // Jogo formatado para 60 fps
    SetTargetFPS(60);

    // Posi��es dos inimigos, chaves e jogador
    POSICAO posInimigos[MAX_INIMIGOS];
    POSICAO posChaves[MAX_CHAVES];
    POSICAO posJogador;

    BOMBA bombas[MAX_BOMBAS] = { 0 }; // Inicia as bombas como inativas

    // Informa��es sobre o jogador
    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;

    // Criar o mapa adicionando as posi��es dos objetos �s respectivas structs
    criarMapa(mapa, &posJogador, posInimigos, posChaves, contadores);

    // While principal do jogo
    while (rodando == True)
    {
        /**** MOVER O PERSONAGEM + MENU ****/
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) posJogador.col += 2; // Mover direita
        if (IsKeyDown(KEY_LEFT)) posJogador.col -= 2; // Mover esquerda
        if (IsKeyDown(KEY_UP)) posJogador.lin -= 2; // Mover cima
        if (IsKeyDown(KEY_DOWN)) posJogador.lin += 2; // Mover baixo
        if (IsKeyPressed(KEY_SPACE)) colocarBomba(&jogador, &posJogador, mapa, bombas); //colocar uma bomba
        if (IsKeyPressed(KEY_TAB)) menu(&rodando, mapa, &posJogador, posInimigos, posChaves, &jogador, bombas, contadores); //abrir menu
        if (IsKeyPressed(KEY_ESCAPE)) rodando = False;//sair do jogo (apenas para testar)
        //----------------------------------------------------------------------------------

        timerBombas(bombas, mapa, &jogador); //atualiza o timer e, se necess�rio, explode bombas

        /****** Desenhar ******/

        BeginDrawing();
        ClearBackground(RAYWHITE); // Desenha o fundo branco
        DrawText(TextFormat("Vidas: %d\tBombas: %d\tPontos: %d\tChaves: %d", jogador.nVidas, jogador.nBombas, jogador.pontuacao, jogador.nChaves), 0, 520, 20, BLACK);//escreve vidas, bombas, pontos e chaves.
        DrawRectangle(posJogador.col, posJogador.lin, TAM_BLOCO, TAM_BLOCO, GREEN); // Desenha o personagem

        // Desenha as bombas
        for (int i = 0; i < MAX_BOMBAS; i++)
        {
            if (bombas[i].ativa)   //caso a bomba esteja ativa desenha ela
            {
                DrawRectangle(bombas[i].posicao.col * TAM_BLOCO, bombas[i].posicao.lin * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
            }
        }

        // Desenha as chaves
        for (k = 0; k < MAX_CHAVES; k++)
        {
            DrawRectangle(posChaves[k].col, posChaves[k].lin, TAM_BLOCO, TAM_BLOCO, GOLD); // teste chave
        }

        // Desenha os inimigos
        for (k = 0; k < MAX_INIMIGOS; k++)
        {
            DrawRectangle(posInimigos[k].col, posInimigos[k].lin, TAM_BLOCO, TAM_BLOCO, RED);
        }

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    return 0;
}


/*********** Fun��es ************/

/* L� o arquivo *de texto* do mapa e transforma em um mapa matriz para a utiliza��o no jogo */
void lerMapaDoArquivo(char *nomeArquivo, char mapa[LINHAS][COLUNAS])
{
    int i, j;
    FILE *arq = fopen(nomeArquivo, "r");

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
            }

            getc(arq); // Pega o caractere \n ap�s a leitura de cada linha
        }

        fclose(arq);
    }
}

/* Usando o mapa dado, coloca a posi��o dos objetos nas structs correspondentes */
void criarMapa(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], int contadores[])
{
    int i, j;

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            switch (mapa[i][j])
            {
            case 'J':
                posJogador->lin = i * TAM_BLOCO;
                posJogador->col = j * TAM_BLOCO; //posi��o inicial do jogador
                mapa[i][j] = ' ';
                break;

            case 'W': // Parede indestrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, SKYBLUE);
                break;

            case 'D': // Parede destrutivel
                DrawRectangle(i * TAM_BLOCO, j * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;

            case 'K': // Posi��o chave
                if (contadores[2] < MAX_CHAVES) // Ter certeza que n�o tenha mais que 3 chaves
                {
                    criarChave(posChaves, i, j, contadores[2]); //
                    contadores[2]++;
                }
                break;

            case 'B': // Posi��o ba�s vazios
                break;

            case 'E': //inimigos
                if (contadores[4] < MAX_INIMIGOS)   //ter certeza que n�o tenha mais que 5 inimigos
                {
                    criarInimigo(posInimigos, i, j, contadores[4]); //
                    contadores[4]++;
                }
                break;

            case ' ': //espa�o vazio
                break;

            default:
                printf("Char desconhecido: %c na posi��o [%d, %d] do mapa.\n", mapa[i][j], i, j); //verificar se n�o tem nenhuma outra letra no mapa
                break;
            }
        }
    }
}

/* Inicia um inimigo na posi��o */
void criarInimigo(POSICAO posInimigos[], int lin, int col, int indexInimigo)
{
    posInimigos[indexInimigo].lin = lin * TAM_BLOCO;
    posInimigos[indexInimigo].col = col * TAM_BLOCO;
}

/* Inicia a chave na posicao */
void criarChave(POSICAO posChaves[], int lin, int col, int indexChave)
{
    posChaves[indexChave].lin = lin * TAM_BLOCO;
    posChaves[indexChave].col = col * TAM_BLOCO;
}

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, POSICAO *posJogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[])
{
    int lin = posJogador->lin / TAM_BLOCO;
    int col = posJogador->col / TAM_BLOCO;

    // Se n�o tiver bombas, n�o h� para colocar
    if (jogador->nBombas <= 0) return;

    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (bombas[i].ativa)
        {
            continue;
        }

        // Para cada bomba inativa
        if (mapa[lin][col] == ' ')  // Verifica se o espa�o do jogador est� vazio
        {
            lin = posJogador->lin / TAM_BLOCO;
            col = posJogador->col / TAM_BLOCO;

            bombas[i].ativa = True; // Ativa a bomba
            bombas[i].tempoInicio = 0; // Armazena o tempo atual
            bombas[i].posicao.lin = lin;
            bombas[i].posicao.col = col;

            mapa[lin][col] = 'B'; // Coloca um B para marcar a bomba no mapa
            jogador->nBombas--; // Decrementa o n�mero de bombas
            break;
        }

    }
}

/* Verifica se os 3 segundos da bomba se passaram */
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (bombas[i].ativa == True)
        {
            if (4 >= 3.0) /**PRECISO DE ALGO UE CONTE POR 3 SEGUNDOS SEM PARAR COMPLETAMENTE O JOGO**/
            {

                explodirBomba(mapa, bombas[i].posicao.lin, bombas[i].posicao.col, bombas[i].ativa);//explode a bomba

                jogador->nBombas++; //retorna a bomba pro inventario
                bombas[i].ativa = False; //bomba fica como inativa
            }
        }
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa)
{
    // Se n�o est� ativa, n�o pode explodir
    if (!ativa) return;

    // Limpa o espa�o da bomba e marca como um espa�o vazio
    mapa[lin][col] = ' ';

    /* Aplica os efeitos da explos�es em todas as dire��es */

    // Cima (exemplo de como todos os fors funcionam)*/
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaLinha = lin - i;
        char blocoAtual = mapa[novaLinha][col];

        // Se encontrar uma parede indestrutivel
        if (novaLinha >= 0 && blocoAtual == 'W')
        {
            break; //Para a explos�o (N�o destr�i nada atr�s da parede)
        }
        else
        {
            // Se !'W' desenha uma explos�o
            DrawRectangle(novaLinha, col, TAM_BLOCO, TAM_BLOCO, ORANGE);// 20x20 Cor: laranja
        }

        if (novaLinha < 0) continue;

        // Se a parede for destrut�vel, destroi ela
        if (blocoAtual == 'D')
        {
            mapa[lin - i][col] = ' ';
            break; //Para a explos�o (N�o destr�i nada atr�s da parede)
        }

        // Se encontrar outra bomba, explode ela imediatamente
        if (blocoAtual == 'B')
        {
            mapa[lin - i][col] = ' '; //marca como espa�o vazio
            explodirBomba(mapa, novaLinha, col, True); //explode aquela bomba
        }
    }

    // Baixo
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaLinha = lin + i;
        char blocoAtual = mapa[novaLinha][col];

        if (novaLinha < LINHAS && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaLinha, col, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaLinha < LINHAS && blocoAtual == 'D')
        {
            mapa[lin + i][col] = ' ';
            break;
        }
        if (novaLinha >= 0 && blocoAtual == 'B')
        {
            mapa[lin + i][col] = ' ';
            explodirBomba(mapa, novaLinha, col, True);
        }
    }

    // Esquerda
    for (int i = 1; i <= ALCANCE_BLOCOS_BOMBA; i++)
    {
        int novaColuna = col - i;
        char blocoAtual = mapa[lin][novaColuna];

        if (novaColuna >= 0 && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaColuna, lin, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaColuna < 0) continue;

        if (blocoAtual == 'D')
        {
            mapa[lin][col - i] = ' ';
            break;
        }
        if (blocoAtual == 'B')
        {
            mapa[lin][col - i] = ' ';
            explodirBomba(mapa, lin, novaColuna, True);
        }
    }

    // Direita
    for (int i = 1; i <= 3; i++)
    {
        int novaColuna = col + i;
        char blocoAtual = mapa[lin][novaColuna];

        if (novaColuna < COLUNAS && blocoAtual == 'W')
        {
            break;
        }
        else
        {
            DrawRectangle(novaColuna, lin, TAM_BLOCO, TAM_BLOCO, ORANGE);
        }

        if (novaColuna < COLUNAS && blocoAtual == 'D')
        {
            mapa[lin][col + i] = ' ';
            break;
        }
        if (novaColuna >= 0 && blocoAtual == 'B')
        {
            mapa[lin][col + i] = ' ';
            explodirBomba(mapa, lin, novaColuna, True);
        }
    }
}

/* Roda o menu de pausa */
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    printf("\nEntrou no menu.");
    *rodando = False;
    int keyPressed = 0;
    char opcao = ' ';
    bool i = True;

    do
    {
        keyPressed = GetCharPressed();
        opcao = (char)keyPressed;
        opcao = toupper(opcao); // Normaliza��o da op��o

        switch (opcao)
        {
        case 'N': // Iniciar um novo jogo do zero
            novoJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'C': // Carregar um arquivo binario de um jogo salvo
            carregarJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'S': // Salvar o jogo (PRECISA SER EM UM ARQUIVO BIN�RIO)
            salvarJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'Q':  // Sair do jogo sem salvar
            printf("\nFechando...");
            CloseWindow();
            *rodando = False;
            i = False;
            printf("  Sucesso!\n");
            break;

        case 'V': // Retornar ao jogo
            printf("\nRetornou ao jogo!");
            i = False;
            *rodando = True;
            break;

        default:
            printf("\nComando incompreendido");
        }
    }
    while (i == True);
}

/* Inicia o jogo do zero */
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    // Reinicia os contadores
    contadores[0] = contadores[1] = contadores[2] = contadores[3] = contadores[4] = 0;

    // Reinicia o jogador
    jogador->nVidas = 3;
    jogador->nBombas = 3;
    jogador->pontuacao = 0;
    jogador->nChaves = 0;

    // Reinicia as bombas
    for (int i = 0; i<MAX_BOMBAS; i++)
        bombas[i].ativa = False;

    // L� novamente o arquivo e recria do 0
    lerMapaDoArquivo("bombermap.txt", mapa);
    criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

/* Carrega o jogo salvo de um arquivo bin�rio */
void carregarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
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
        fread(bombas, sizeof(BOMBA), MAX_BOMBAS, arq); // ... das bombas
        fread(mapa, sizeof(char), TAMANHO_MAPA, arq); // ... do mapa

        fclose(arq);

        printf("  Jogo carregado com sucesso!");
    }

    // Cria o mapa de novo
    criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);
}

/* Salva o jogo em um arquivo binario */
void salvarJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    int i;
    int lin, col;

    printf("\nSalvando o jogo...");

    // Atualiza a posi��o dos inimigos no mapa
    for (i = 0; i < contadores[4]; i++)
    {
        lin = posInimigos[i].lin / TAM_BLOCO;
        col = posInimigos[i].col / TAM_BLOCO;
        mapa[lin][col] = 'E';
    }

    // Atualiza a posi�ao do jogador no mapa
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
        // Salva as informa��es...
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
