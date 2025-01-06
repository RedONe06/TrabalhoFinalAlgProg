/* Este Trabalho Final busca consolidar o aprendizado em relação ao conteúdo teórico e prático abordado e exercitar a
habilidade de trabalho em equipe, consistindo na implementação de um jogo na linguagem de programação C.
O jogo a implementado é uma versão simplificada do jogo BomberMan, utilizando a biblioteca gráfica RayLib*/

/*Eu consegui adicionar uma lógica para andomizar o movimento dos inimigos mas por algum motivo a função de movimentação não está funcionando.
eu não consigo ver nenhum erro na minha lógica e preciso de uma juda para resolver isso

o menu está completamente quebrado. Todas as funções funcionam mas qualquer tentativa de dar scanf ou gets faz o jogo crashar então
por enquanto deixei somente a função de retornar ao jogo*/

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

// Número máximo de conteúdos
#define MAX_BOMBAS 3
#define MAX_CHAVES 3
#define MAX_INIMIGOS 5

// Dimensões máximas da tela do jogo em px
#define ALTURA 600
#define LARGURA 1200

// Resoluções em px
#define TAM_BLOCO 20 // Tamanho de cada bloco
#define TAMANHO_MAPA 1500 // Total de blocos 20pxX20px no mapa (25 * 60)

// Quantidade de linhas e colunas no mapa
#define LINHAS 25
#define COLUNAS 60

// Outras definições
#define ALCANCE_BLOCOS_BOMBA 3

/************ Protótipos *************/
void criarInimigo(POSICAO posInimigos[], int lin, int col, int num);
void criarChave(POSICAO posChaves[], int lin, int col, int num);
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); // menu de pausa
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[]);
void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[], JOGADOR *jogador);
void timerBombas(BOMBA bombas[], char mapa[LINHAS][COLUNAS], JOGADOR *jogador); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA bomba, JOGADOR *jogador);
void novoJogo(char mapa[LINHAS][COLUNAS], POSICAO *posJogador, POSICAO posInimigos[], POSICAO posChaves[], JOGADOR *jogador, BOMBA bombas[], int contadores[]); //cria um novo jogo
void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
POSICAO acharProximaPosicao(POSICAO posicaoAtual, int direcao);
bool direcaoEstaLivre(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao);
int objetoNaDirecao(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao);
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS], JOGADOR *jogador);
void mudarDirecaoInimigos(POSICAO posInimigos[], clock_t *timer);
void perderVida(JOGADOR *jogador);

/************* Main **************/
int main()
{
    clock_t timer_inimigos = 1 + clock();
    clock_t timer_direcao = clock();

    bool rodando = true; // Enquanto rodando == True o jogo irá rodar

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

    // Inicia a tela do jogo no tamanho especificado pelas variáveis
    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window");

    // Jogo formatado para 60 fps
    SetTargetFPS(60);

    // Posições dos inimigos, chaves e jogador
    POSICAO posInimigos[MAX_INIMIGOS];
    POSICAO posChaves[MAX_CHAVES];
    POSICAO posJogador;

    BOMBA bombas[MAX_BOMBAS] = { 0 }; // Inicia as bombas como inativas

    // Informações sobre o jogador
    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;

    // Criar o mapa adicionando as posições dos objetos às respectivas structs
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

        //verificaTimerDasBombas(mapa, bombas, &jogador);
        EndDrawing();

        //----------------------------------------------------------------------------------

        if (clock() >= timer_direcao) mudarDirecaoInimigos(posInimigos, &timer_direcao);
        //if (clock() >= timer_inimigos) moverInimigos(posInimigos, &timer_inimigos, mapa, &jogador);

        // ***** Evitar colocar printf() para rodar com o jogo pq ele diminui a performance **** descomenta só quando for usar pra debugar
        // printf("\nTimer Aleatorio: %ld, Timer Inimigos: %ld, Clock Atual: %ld", timer_direcao, timer_inimigos, clock());
    }



    return 0;
}

/*********** Funções ************/

/* A cada 3 segundos escolhe uma direcao nova aleatoria para cada inimigo*/
void mudarDirecaoInimigos(POSICAO posInimigos[], clock_t *timer)
{
    int i, num;

    for (i = 0; i < MAX_INIMIGOS; i++)
    {
        num = rand() % 4 + 1;
        posInimigos[i].direcao = num;
    }

    *timer = 3 * CLOCKS_PER_SEC + clock();
}

/* A cada segundo, se possível, move o inimigo na direção determinada*/
void moverInimigos(POSICAO posInimigos[], clock_t *timer, char mapa[LINHAS][COLUNAS],JOGADOR *jogador)
{
    int i;
    bool moveu;
    for (i=0; i < MAX_INIMIGOS; i++)
    {
        int tentativas = 0; //Contador de tentativas para o código não ficar em um loop infinito caso tenha paredes nas 4 direções
        moveu = False;
        while(!moveu && tentativas < 4)
        {
            switch (posInimigos[i].direcao)
            {
            case 1:

                if (mapa[posInimigos[i].lin + 1][posInimigos[i].col] == ' ') //Verifica se há uma parede no caminho
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' '; //limpa a região onde estava
                    posInimigos[i].lin++; // muda a posição
                    moveu = True; //muda a booleana
                }
                else if (mapa[posInimigos[i].lin + 1][posInimigos[i].col] == 'J')  // se encontrar o jogador...
                {
                    perderVida(jogador); // ...tira uma vida dele
                }

                break;
            case 2:

                if (mapa[posInimigos[i].lin][posInimigos[i].col + 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col++;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin][posInimigos[i].col + 1] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            case 3:

                if (mapa[posInimigos[i].lin - 1][posInimigos[i].col] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].lin--;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin - 1][posInimigos[i].col] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            case 4:

                if (mapa[posInimigos[i].lin][posInimigos[i].col - 1] == ' ')
                {
                    mapa[posInimigos[i].lin][posInimigos[i].col] = ' ';
                    posInimigos[i].col--;
                    moveu = True;
                }
                else if (mapa[posInimigos[i].lin ][posInimigos[i].col - 1] == 'J')
                {
                    perderVida(jogador);
                }

                break;
            }
            if (!moveu) //se encontrou uma parede ele muda de direção e aumenta as tentativas
            {
                posInimigos[i].direcao = (posInimigos[i].direcao % 4) + 1;
                tentativas++;
            }
        }
        mapa[posInimigos[i].lin][posInimigos[i].col] = 'E'; //coloca um 'E' na nova posição
    }


    *timer = clock() + CLOCKS_PER_SEC; // atualiza o timer
}

/* Dado um mapa, uma posicao atual e uma direção para movimento, checa se a posicao seguinte é possível (está vazia) ou não */
int objetoNaDirecao(char mapa[LINHAS][COLUNAS], POSICAO posicaoAtual, int direcao)
{
    // 0 == Parede, 1 == Livre, 2 == Inimigo, 3 == Chave
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return 1;
        else if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == 'E') return 2;
        else if (mapa[posicaoAtual.lin + 1][posicaoAtual.col] == 'K') return 3;
        break;
    case 1: // Cima
        if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return 1;
        else if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == 'E') return 2;
        else if (mapa[posicaoAtual.lin - 1][posicaoAtual.col] == 'K') return 3;
        break;
    case 2: // Esquerda
        if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return 1;
        else if (mapa[posicaoAtual.lin][posicaoAtual.col - 1] == 'E') return 2;
        else if (mapa[posicaoAtual.lin ][posicaoAtual.col - 1] == 'K') return 3;
        break;
    default: // Direita
        if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return 1;
        else if (mapa[posicaoAtual.lin][posicaoAtual.col + 1] == 'E') return 2;
        else if (mapa[posicaoAtual.lin ][posicaoAtual.col + 1] == 'K') return 3;
    }

    return 0;
}

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
        novaPosicao.lin = posicaoAtual.lin + 1;
        break;
    case 1: // Cima
        novaPosicao.col = posicaoAtual.col;
        novaPosicao.lin = posicaoAtual.lin - 1;
        break;
    case 2: // Esquerda
        novaPosicao.col = posicaoAtual.col - 1;
        novaPosicao.lin = posicaoAtual.lin;
        break;
    default: // Direita
        novaPosicao.col = posicaoAtual.col + 1;
        novaPosicao.lin = posicaoAtual.lin;
    }

    return novaPosicao;
}

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, char mapa[LINHAS][COLUNAS], BOMBA bombas[])
{
    // Se não tiver bombas ou espaço para colocar bomba, não colocar bomba
    if (jogador->nBombas <= 0 || !direcaoEstaLivre(mapa, jogador->posicao, jogador->direcao)) return;

    // Define a posição que a bomba vai ficar
    POSICAO posicaoBomba = acharProximaPosicao(jogador->posicao, jogador->direcao);
    // Pega o index da bomba baseado na quantidade disponível no arsenal
    int ultimaBomba = jogador->nBombas - 1;

    bombas[ultimaBomba].tempoInicio = time(NULL); // Armazena o tempo atual
    bombas[ultimaBomba].posicao.lin = posicaoBomba.lin;
    bombas[ultimaBomba].posicao.col = posicaoBomba.col;

    mapa[posicaoBomba.lin][posicaoBomba.col] = 'X'; // Coloca um B para marcar a bomba no mapa
    jogador->nBombas--; // Decrementa o número de bombas do arsenal
}

void verificaTimerDasBombas(char mapa[LINHAS][COLUNAS], BOMBA bombas[], JOGADOR *jogador)
{
    int bombasAtivas = MAX_BOMBAS - jogador->nBombas;

    for (int i = 0; i < bombasAtivas; i++)
    {
        double intervaloEmSegundos = difftime(time(NULL), bombas[i].tempoInicio);
        if (intervaloEmSegundos < 3) continue;

        // Se já se passaram 3 segundos do tempo de início da bomba, explodir ela
        explodirBomba(mapa, bombas[i], jogador);
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], BOMBA bomba, JOGADOR *jogador)
{
    // Limpa o espaço da bomba e marca como um espaço vazio
    // mapa[bomba.posicao.lin][bomba.posicao.col] = ' ';
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
        opcao = toupper(opcao); // Normalização da opção

        switch (opcao)
        {
        case 'N': // Iniciar um novo jogo do zero
            novoJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'C': // Carregar um arquivo binario de um jogo salvo
            carregarJogo(mapa, posJogador, posInimigos, posChaves, jogador, bombas, contadores);
            break;

        case 'S': // Salvar o jogo (PRECISA SER EM UM ARQUIVO BINÁRIO)
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

    // Lê novamente o arquivo e recria do 0
    lerMapaDoArquivo("bombermap.txt", mapa);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

void controlarMovimentacao(char mapa[LINHAS][COLUNAS], JOGADOR *jogador)
{
    if (IsKeyPressed(KEY_RIGHT))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 3))
        {

        case 1: // se encontrar um espaço livre
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
            jogador->direcao = 3; // Direita
            break;
        case 2: // se encontrar um inimigo
            perderVida(jogador);
            break;
        case 3: // se encontrar uma chave
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin ][jogador->posicao.col +1] = 'J';
            jogador->nChaves++;
            jogador->direcao = 3;
            break;
        }
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        switch (direcaoEstaLivre(mapa, jogador->posicao, 2))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin ][jogador->posicao.col - 1] = 'J';
            jogador->direcao = 2; // Esquerda
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
            jogador->nChaves++;
            jogador->direcao = 2;
            break;
        }
    }
    else if (IsKeyPressed(KEY_UP))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 1))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
            jogador->direcao = 1; // Cima
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
            jogador->nChaves++;
            jogador->direcao = 1;
            break;
        }
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        switch (objetoNaDirecao(mapa, jogador->posicao, 0))
        {

        case 1:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
            jogador->direcao = 0; // Baixo
            break;
        case 2:
            perderVida(jogador);
            break;
        case 3:
            mapa[jogador->posicao.lin][jogador->posicao.col] = ' ';
            mapa[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
            jogador->nChaves++;
            jogador->direcao = 0;
            break;
        }
    }

}

void perderVida(JOGADOR *jogador)
{
    if (jogador->nVidas > 1)
    {
        jogador->nVidas--;
        jogador->pontuacao -= 100;
    }
    else exit(0);

}
