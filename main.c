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
#include <stdbool.h>
#include "raylib.h"
#include "canvas.h"
#include "structs.h"
#include "filemanager.h"

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

/************ Prot�tipos *************/
void criarInimigo(POSICAO posInimigos[], int lin, int col, int num);
void criarChave(POSICAO posChaves[], int lin, int col, int num);
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); // menu de pausa
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[]);
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA bomba, JOGADOR *jogador);
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); //cria um novo jogo
void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);
void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[], JOGADOR *jogador);
bool direcaoEstaLivre(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao);
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS],int contadores[]);
void mudarInimigos(POSICAO posInimigos[], clock_t *timer, int contadores[]);

/************* Main **************/
int main()
{
    clock_t timer_inimigos = 1 + clock();
    clock_t timer_aleatorio = clock();


    bool rodando = true; // Enquanto rodando == True o jogo ir� rodar

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
        for (int j = 0; j < COLUNAS; j++)
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
    //criarMapa(mapa, &posJogador, posInimigos, posChaves, contadores);

    // While principal do jogo
    while (rodando)
    {
        /****** Desenhar ******/
        BeginDrawing();
        ClearBackground(RAYWHITE); // Desenha o fundo branco

        DrawText(TextFormat("Vidas: %d\tBombas: %d\tPontos: %d\tChaves: %d", jogador.nVidas, jogador.nBombas, jogador.pontuacao, jogador.nChaves), 0, 520, 20, BLACK);//escreve vidas, bombas, pontos e chaves.
        desenharMapa(mapa, &jogador);

        /**** MOVER O PERSONAGEM + MENU ****/
        //----------------------------------------------------------------------------------
        controlarMovimentacao(mapa, &jogador);
        if (IsKeyPressed(KEY_B)) colocarBomba(&jogador, mapa, bombas);
        if (IsKeyPressed(KEY_TAB)) menu(&rodando, mapa, &posJogador, posInimigos, posChaves, &jogador, bombas, contadores); //abrir menu
        if (IsKeyPressed(KEY_ESCAPE)) rodando = False;//sair do jogo (apenas para testar)
        //----------------------------------------------------------------------------------

        verificaTimerDasBombas(mapa, bombas, &jogador);
        EndDrawing();

        //----------------------------------------------------------------------------------

        if (clock() >= timer_aleatorio) mudarInimigos(posInimigos, &timer_aleatorio, contadores);
        if (clock() >= timer_inimigos) moverInimigos(posInimigos, &timer_inimigos, mapa, contadores);
        printf("\nTimer Aleatorio: %ld, Timer Inimigos: %ld, Clock Atual: %ld", timer_aleatorio, timer_inimigos, clock());
    }



    return 0;
}

/*********** Fun��es ************/

/* A cada 3 segundos escolhe uma direcao nova aleatoria para cada inimigo*/
void mudarInimigos(POSICAO posInimigos[], clock_t *timer, int contadores[])
{
    int i, num;

    for (i = 0; i < MAX_INIMIGOS; i++)
    {
        num = rand() % 4 + 1;
        posInimigos[i].direcao = num;
    }

    *timer = 3 * CLOCKS_PER_SEC + clock();
}

/* A cada segundo, se poss�vel, move o inimigo na dire��o determinada*/
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS],int contadores[])
{
    int i;
    bool moveu;
    for (i=0; i < MAX_INIMIGOS; i++)
    {
        int tentativas = 0; //Contador de tentativas para o c�digo n�o ficar em um loop infinito caso tenha paredes nas 4 dire��es
        moveu = False;
        while(!moveu && tentativas < 4)
        {
            switch (posInimigos[i].direcao)
            {
            case 1:

                if (mapa[posInimigos[i].lin + 1][posInimigos[i].col] == ' ') //Verifica se h� uma parede no caminho
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' '; //limpa a regi�o onde estava
                    posInimigos[i].lin++; // muda a posi��o
                    moveu = True; //muda a booleana
                }

                break;
            case 2:

                if (mapa[posInimigos[i].lin][posInimigos[i].col + 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col++;
                    moveu = True;
                }

                break;
            case 3:

                if (mapa[posInimigos[i].lin - 1][posInimigos[i].col] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].lin--;
                    moveu = True;
                }

                break;
            case 4:

                if (mapa[posInimigos[i].lin][posInimigos[i].col - 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col--;
                    moveu = True;
                }

                break;
            }
            if (!moveu) //se encontrou uma parede ele muda de dire��o e aumenta as tentativas
            {
                posInimigos[i].direcao = (posInimigos[i].direcao % 4) + 1;
                tentativas++;
            }
        }
        mapa[posInimigos[i].lin][posInimigos[i].col] = 'E'; //coloca um 'E' na nova posi��o
    }


    *timer = clock() + CLOCKS_PER_SEC; // atualiza o timer
}

/* Dado um mapa, uma posicao atual e uma dire��o para movimento, checa se a posicao seguinte � poss�vel (est� vazia) ou n�o */
bool direcaoEstaLivre(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao)
{
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return true;
        break;
    case 1: // Cima
        if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return true;
        break;
    case 2: // Esquerda
        if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return true;
        break;
    default: // Direita
        if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return true;
    }

    return false;
}

POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao)
{
    POSICAO novaPosicao;

    switch(direcao)
    {
    case 0: // Baixo
        novaPosicao.col = posicaoAtual.col;
        novaPosicao.lin = posicaoAtual.lin++;
        break;
    case 1: // Cima
        novaPosicao.col = posicaoAtual.col;
        novaPosicao.lin = posicaoAtual.lin--;
        break;
    case 2: // Esquerda
        novaPosicao.col = posicaoAtual.col--;
        novaPosicao.lin = posicaoAtual.lin;
        break;
    default: // Direita
        novaPosicao.col = posicaoAtual.col++;
        novaPosicao.lin = posicaoAtual.lin;
    }

    return novaPosicao;
}

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[])
{
    // Se n�o tiver bombas ou espa�o para colocar bomba, n�o colocar bomba
    if (jogador->nBombas <= 0 || !direcaoEstaLivre(mapa, jogador->posicao, jogador->direcao)) return;

    // Define a posi��o que a bomba vai ficar
    POSICAO posicaoBomba = acharProximaPosicao(jogador->posicao, jogador->direcao);
    // Pega o index da bomba baseado na quantidade dispon�vel no arsenal
    int ultimaBomba = jogador->nBombas - 1;

    bombas[ultimaBomba].tempoInicio = time(NULL); // Armazena o tempo atual
    bombas[ultimaBomba].posicao.lin = posicaoBomba.lin;
    bombas[ultimaBomba].posicao.col = posicaoBomba.col;

    mapa[posicaoBomba.lin][posicaoBomba.col] = 'X'; // Coloca um B para marcar a bomba no mapa
    jogador->nBombas--; // Decrementa o n�mero de bombas do arsenal
}

void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[], JOGADOR *jogador)
{
    int bombasAtivas = MAX_BOMBAS - jogador->nBombas;

    for (int i = 0; i < bombasAtivas; i++)
    {
        double intervaloEmSegundos = difftime(time(NULL), bombas[i].tempoInicio);
        if (intervaloEmSegundos < 3) continue;

        // Se j� se passaram 3 segundos do tempo de in�cio da bomba, explodir ela
        explodirBomba(mapa, bombas[i], jogador);
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA bomba, JOGADOR *jogador)
{
    // Limpa o espa�o da bomba e marca como um espa�o vazio
    mapa[bomba.posicao.lin][bomba.posicao.col] = ' ';
    jogador->nBombas++;

    DrawRectangle(bomba.posicao.col * TAM_BLOCO, bomba.posicao.lin * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, WHITE);
}

/* Roda o menu de pausa */
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[])
{
    printf("\nEntrou no menu.");
    *rodando = false;
    int keyPressed = 0;
    char opcao = 'v';
    bool i = true;

    do
    {
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
            *rodando = false;
            i = false;
            printf("  Sucesso!\n");
            break;

        case 'V': // Retornar ao jogo
            printf("\nRetornou ao jogo!");
            i = false;
            *rodando = true;
            break;

        default:
            printf("\nComando incompreendido");
        }
    }
    while (i == true);
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

    // L� novamente o arquivo e recria do 0
    lerMapaDoArquivo("bombermap.txt", mapa);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}




void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    if (IsKeyPressed(KEY_RIGHT) && direcaoEstaLivre(mapa, jogador->posicao, 3))
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
        jogador->direcao = 3; // Direita
    }
    else if (IsKeyPressed(KEY_LEFT) && direcaoEstaLivre(mapa, jogador->posicao, 2))
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
        jogador->direcao = 2; // Esquerda
    }
    else if (IsKeyPressed(KEY_UP) && direcaoEstaLivre(mapa, jogador->posicao, 1))
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
        jogador->direcao = 1; // Cima
    }
    else if (IsKeyPressed(KEY_DOWN) && direcaoEstaLivre(mapa, jogador->posicao, 0))
    {
        mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
        jogador->direcao = 0; // Baixo
    }
}

