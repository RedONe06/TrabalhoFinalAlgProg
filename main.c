// Jogo do Bomberman para o trabalho final de Algoritmos e Programação

/*********** Includes e Defines ***********/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "raylib.h"

#define MAXBOMBAS 3//número máximo de bombas
#define MAXCHAVE 3 //Numero máximo de chaves no mapa
#define MAXINIMIGO 5 //numero máximo de inimigos no mapa

//typedef int bool; //definindo booleano
#define True 1
#define False 0

#define ALTURA 600 //tamanho da tela do jogo
#define LARGURA 1200 //tamanho da tela do jogo

#define TAMBLOCO 20 //tamanho de cada bloco
#define TAMANHOMAPA 1500 //total de blocos 20x20 no mapa (25 * 60)

#define LINHAS 25 // linhas do jogo
#define COLUNAS 60 // colunas do jogo



/************ Structs ***************/

//struct da posição do jogador e dos inimigos;
struct posicao
{
    int lin;
    int col;

};

//informações do jogador
struct infJogador
{
    int vidas;
    int bombas;
    int pontos;
    int keys;
};

//informações das bombas
struct infBomba
{
    bool ativa;  // booleano se a bomba tá ativa
    float tempoInicio; // o tempo em que a bomba foi colocada
    int lin;   // Linha da bomba
    int col;// Coluna da bomba
};


/************ Protótipos *************/
void criarInimigo(struct posicao inimigos[], int lin, int col, int num);//cria os inimigos
void criarChave(struct posicao chaves[], int lin, int col, int num); //cria as chaves
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[]); // menu de pausa
void lerArquivoParaMatriz(char *nomeArquivo, char matriz[LINHAS][COLUNAS]);//transforma o mapa.txt em matriz
void criarMapa(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], int numero[]); // cria o mapa
void colocarBomba(struct infJogador *player, struct posicao *jog, char mapa[LINHAS][COLUNAS], struct infBomba bombas[]);//coloca as bombas no mapa
void timerBombas(struct infBomba bombas[], char mapa[LINHAS][COLUNAS], struct infJogador *player); //cuida do timer de 3 segundos das bombas
void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa); //explode as bombas
void novoJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[]); //cria um novo jogo
void carregarJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[]);
void salvarJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[]); //salva o jogo atual


/************* Main **************/
int main()  // a main()
{
    bool rodando = True; //enquanto rodando == True o jogo irá rodar
    int i,j,k; //contadores

    char mapa[LINHAS][COLUNAS];

    int numero[5] = {0,0,0,0,0}; /* vetor para contar o numero de  paredes indestrutiveis, destrutiveis, baus com chaves, baus sem chaves e inimigos.
    [0] = paredes indestrutiveis
    [1] = paredes destrutiveis
    [2] = baus com chaves
    [3] = baus sem chaves
    [4] = inimigos */

    lerArquivoParaMatriz("bombermap.txt", mapa);// chama a função passando o nome do mapa.txt

    for (int i = 0; i < LINHAS; i++)  // exibe a matriz para verificar
    {
        for (j=0;j<COLUNAS;j++){
        printf("%c", mapa[i][j]);
        }
        printf("\n");
    }

    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window"); //inicia a tela do jogo no tamanho especificado acima

    SetTargetFPS(60); //jogo formatado para 60 fps

    struct posicao inimigos[MAXINIMIGO]; //posição dos inimigos
    struct posicao chaves[MAXCHAVE]; //posição das chaves
    struct posicao jog; // posição do jogador (jog)

    struct infBomba bombas[MAXBOMBAS] = {0}; //inicia as bombas como inativas

    struct infJogador player; //informações sobre o jogador
    player.vidas = 3;
    player.bombas = 3;
    player.pontos = 0;
    player.keys = 0;

    criarMapa(mapa, &jog, inimigos, chaves, numero); //criar o mapa adicionando as posições dos objetos às respectivas structs





    while (rodando == True)  //While principal do jogo
    {
        /**** MOVER O PERSONAGEM + MENU ****/
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) jog.col += 2; //mover direita
        if (IsKeyDown(KEY_LEFT)) jog.col -= 2; //mover esquerda
        if (IsKeyDown(KEY_UP)) jog.lin -= 2; // mover cima
        if (IsKeyDown(KEY_DOWN)) jog.lin += 2; //mover baixo
        if (IsKeyPressed(KEY_SPACE)) colocarBomba(&player, &jog, mapa, bombas); //colocar uma bomba
        if (IsKeyPressed(KEY_TAB)) menu(&rodando, mapa, &jog, inimigos, chaves, &player, bombas, numero); //abrir menu
        if (IsKeyPressed(KEY_ESCAPE)) rodando = False;//sair do jogo (apenas para testar)
        //----------------------------------------------------------------------------------

        timerBombas(bombas, mapa, &player); //atualiza o timer e, se necessário, explode bombas

        /****** Desenhar ******/

        BeginDrawing();

        ClearBackground(RAYWHITE); // desenha o fundo branco


        DrawText("Congrats! You created your first window!\n\n", 190, 200, 30, LIGHTGRAY); //desenha texto na tela
        DrawText(TextFormat("Vidas: %d\tBombas: %d\tPontos: %d\tChaves: %d", player.vidas,player.bombas, player.pontos, player.keys), 0, 520, 20, BLACK);//escreve vidas, bombas, pontos e chaves.

        DrawRectangle(jog.col, jog.lin, TAMBLOCO, TAMBLOCO, GREEN); // desenha o pesonagem

        for (int i = 0; i < MAXBOMBAS; i++)
        {
            if (bombas[i].ativa)   //caso a bomba estja ativa desenha ela
            {
                DrawRectangle(bombas[i].col * TAMBLOCO, bombas[i].lin * TAMBLOCO, TAMBLOCO, TAMBLOCO, BLUE);
            }
        }

        for (k = 0; k< MAXCHAVE; k++)
        {
            DrawRectangle(chaves[k].col, chaves[k].lin, TAMBLOCO, TAMBLOCO, GOLD); // teste chave
        }

        for (k = 0; k< MAXINIMIGO; k++)
        {
            DrawRectangle(inimigos[k].col, inimigos[k].lin, TAMBLOCO, TAMBLOCO, RED); //desenha os inimigos
        }

        EndDrawing();

        //----------------------------------------------------------------------------------
    }



    return 0;
}


/*********** Outras Funções ************/

/* ESSA FUNÇÃO LÊ O ARQUIVO de texto do mapa E TRANSFORMA EM UMA MATRIZ PARA SER USADA NO JOGO */
void lerArquivoParaMatriz(char *nomeArquivo, char mapa[LINHAS][COLUNAS])
{
    int i,j;
    char temp;

    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");//fala se deu um erro
        return;
    }
    else
    {

        for (i = 0; i < LINHAS; i++)   // le as linhas do arquivo e armazena na matriz
        {
            for (j=0; j<COLUNAS; j++)
            {
                mapa[i][j] = getc(arq); //getc a matriz do mapa
            }
            temp = getc(arq);
        }
        fclose(arq); //fecha o arquivo
    }
}

/* ESSA FUNÇAO PEGA A MATRIZ E COLOCA A POSIÇÃO DOS OBJETOS NAS STRUCTS CORRESPONDENTES*/
void criarMapa(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], int numero[])
{

    int i,j;

    for (i=0; i<LINHAS; i++)
    {
        for (j=0; j<COLUNAS; j++)
        {
            switch (mapa[i][j])
            {
            case 'J':
                jog->lin = i * TAMBLOCO;
                jog->col = j * TAMBLOCO; //posição inicial do jogador
                mapa[i][j] = ' ';
                break;

            case 'W': //parede indestrutivel
                DrawRectangle(j * TAMBLOCO, i * TAMBLOCO, TAMBLOCO, TAMBLOCO, SKYBLUE);
                break;

            case 'D': //parede destrutivel
                DrawRectangle(i * TAMBLOCO, j * TAMBLOCO, TAMBLOCO, TAMBLOCO, BROWN);
                break;

            case 'K': //posição chave
                if (numero[2] < MAXCHAVE)   //ter certeza que não tenha mais que 3 chaves
                {
                    criarChave(chaves, i, j, numero[2]); //
                    numero[2]++;
                }
                break;

            case 'B'://posição baús vazios
                break;

            case 'E': //inimigos
                if (numero[4] < MAXINIMIGO)   //ter certeza que não tenha mais que 5 inimigos
                {
                    criarInimigo(inimigos, i, j, numero[4]); //
                    numero[4]++;
                }
                break;

            case ' ': //espaço vazio
                break;

            default:
                printf("Char desconhecido: %c na posição [%d, %d] do mapa.\n", mapa[i][j], i, j); //verificar se não tem nenhuma outra letra no mapa
                break;
            }
        }
    }
}


//inicia os inimigos
void criarInimigo(struct posicao inimigos[], int lin, int col, int num)
{
    inimigos[num].lin = lin * TAMBLOCO;
    inimigos[num].col = col * TAMBLOCO;
}

//inicia as chaves
void criarChave(struct posicao chaves[], int lin, int col, int num)
{
    chaves[num].lin = lin * TAMBLOCO;
    chaves[num].col = col * TAMBLOCO;
}

/*ESSA FUNÇAO POSICIONA AS BOMBAS NO MAPA*/
void colocarBomba(struct infJogador *player, struct posicao *jog, char mapa[LINHAS][COLUNAS], struct infBomba bombas[])
{
    int lin = jog->lin / TAMBLOCO;
    int col = jog->col / TAMBLOCO;
    if (player->bombas > 0)
    {
        for (int i = 0; i < MAXBOMBAS; i++)//verifica se tem bombas disponíveis
        {
            if (bombas[i].ativa == False)//pega uma bomba inativa
            {
                if (mapa[lin][col] == ' ')  // verifica se o espaço está vazio
                {
                    lin = jog->lin / TAMBLOCO;
                    col = jog->col / TAMBLOCO;

                    bombas[i].ativa = True; //ativa a bomba
                    bombas[i].tempoInicio = 0; //armazena o tempo atual
                    bombas[i].lin = lin; //armazena a linha
                    bombas[i].col = col; //armazena a coluna

                    mapa[lin][col] = 'B';//coloca um B para marcar a bomba no mapa
                    player->bombas--; //diminui uma das bombas
                    break; //sai do for
                }
            }
        }
    }
}

/*ESSA FUNÇÃO VERIFICA SE os 3 SEGUNDOS das bombas SE PASSARAM*/
void timerBombas(struct infBomba bombas[], char mapa[LINHAS][COLUNAS], struct infJogador *player)
{
    for (int i = 0; i < MAXBOMBAS; i++)
    {
        if (bombas[i].ativa == True)
        {
            if (4 >= 3.0) /**PRECISO DE ALGO UE CONTE POR 3 SEGUNDOS SEM PARAR COMPLETAMENTE O JOGO**/
            {

                explodirBomba(mapa, bombas[i].lin, bombas[i].col, bombas[i].ativa);//explode a bomba

                player->bombas++; //retorna a bomba pro inventario
                bombas[i].ativa = False; //bomba fica como inativa
            }
        }
    }
}

void explodirBomba(char mapa[LINHAS][COLUNAS], int lin, int col, bool ativa)
{

    if (ativa == True)  //só explode se a bomba estiver ativa
    {
        mapa[lin][col] = ' '; //limpa o espaço da bomba e marca como um espaço vazio

        // aplica os efeitos da explosãe em todas as direções

        /*cima (exemplo de como todos os fors funcionam)*/
        for (int i = 1; i <= 3; i++)   // Alcance de 3 blocos
        {
            if (lin - i >= 0 && mapa[lin - i][col] == 'W')   // se encontrar uma parede indestrutivel
            {
                break; //Para a explosão (Não destrói nada atrás da parede)
            }
            else         // Se !'W' desenha uma explosão
            {

                DrawRectangle(lin - i, col, TAMBLOCO, TAMBLOCO, ORANGE);// 20x20 Cor: laranja
            }
            if (lin - i >= 0 && mapa[lin - i][col] == 'D')
            {
                mapa[lin - i][col] = ' '; // Destrói se parede for destrutivel
                break; //Para a explosão (Não destrói nada atrás da parede)
            }
            if (lin - i >= 0 && mapa[lin - i][col] == 'B')   // se encontrar outra bomba explode ela imediatamente
            {
                mapa[lin - i][col] = ' '; //marca como espaço vazio
                explodirBomba(mapa, lin - i, col,True); //explode aquela bomba
            }
        }

        // baixo
        for (int i = 1; i <= 3; i++)
        {
            if (lin + i < LINHAS && mapa[lin + i][col] == 'W')
            {
                break;
            }
            else
            {
                DrawRectangle(lin + i, col, TAMBLOCO, TAMBLOCO, ORANGE);
            }
            if (lin + i < LINHAS && mapa[lin + i][col] == 'D')
            {
                mapa[lin + i][col] = ' ';
                break;
            }
            if (lin + i >= 0 && mapa[lin + i][col] == 'B')
            {
                mapa[lin + i][col] = ' ';
                explodirBomba(mapa, lin + i, col,True);

            }
        }
        // esquerda
        for (int i = 1; i <= 3; i++)
        {
            if (col - i >= 0 && mapa[lin][col - i] == 'W')
            {
                break;
            }
            else
            {
                DrawRectangle( col - i, lin, TAMBLOCO, TAMBLOCO, ORANGE);
            }
            if (col - i >= 0 && mapa[lin][col - i] == 'D')
            {
                mapa[lin][col - i] = ' ';
                break;
            }
            if (col - i >= 0 && mapa[lin][col - i] == 'B')
            {
                mapa[lin][col - i] = ' ';
                explodirBomba(mapa, lin, col - i, True);
            }
        }

        // direita
        for (int i = 1; i <= 3; i++)
        {
            if (col + i < COLUNAS && mapa[lin][col + i] == 'W')
            {
                break;
            }
            else
            {
                DrawRectangle(col + i, lin, TAMBLOCO, TAMBLOCO, ORANGE);
            }
            if (col + i < COLUNAS && mapa[lin][col + i] == 'D')
            {
                mapa[lin][col + i] = ' ';
                break;
            }
            if (col + i >= 0 && mapa[lin][col + i] == 'B')
            {
                mapa[lin][col + i] = ' ';
                explodirBomba(mapa, lin, col + i, True);
            }
        }
    }

}


/* ESSA É A FUNÇÃO DO MENU DE PAUSA */
void menu(bool *rodando, char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[])  // função do menu de pausa
{
    printf("\nEntrou no menu.");
    *rodando = False;
    int k = 0;
    char fazer = ' '; //dependendo de Qual tecla o usuario clicar o menu faz uma função diferente
    bool i = True;

    do{
    k = GetCharPressed();
    fazer = (char)k;
    fazer = toupper(fazer); //deixa o char MAIUSCULO para facilitar o switch
    switch (fazer)
    {
    case 'N': // inicia um novo jogo do zero
        novoJogo(mapa, jog, inimigos, chaves, player, bombas, numero);
        break;

    case 'C': // carregar um arquivo binario de um jogo salvo
        carregarJogo(mapa, jog, inimigos, chaves, player, bombas, numero);
        break;

    case 'S': // salvar o jogo (PRECISA SER EM UM ARQUIVO BINÁRIO)
        salvarJogo(mapa, jog, inimigos, chaves, player, bombas, numero);
        break;

    // Sair do jogo sem salvar
    case 'Q': // Sair do jogo
        printf("\nFechando...");
        CloseWindow();
        *rodando = False;
        i = False;
        printf("  Sucesso!\n");
        break;

    case 'V': //retornar ao jogo
        printf("\nRetornou ao jogo!");
        i = False;
        *rodando = True;
        break;
    default:
        printf("\nComando incompreendido");
    }
} while (i == True);
}
/*ESSA FUNÇÃO INICIA O JOGO DO ZERO*/
void novoJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[])
{

    /*reinicia os contadores*/
    numero[0] = numero[1] = numero[2] = numero[3] = numero[4] = 0;

    /* reinicia o jogador*/
    player->vidas = 3;
    player->bombas = 3;
    player->pontos = 0;
    player->keys = 0;

    /*reinicia as bombas*/
    for (int i = 0; i<MAXBOMBAS; i++)
    {
        bombas[i].ativa = False;
    }

    lerArquivoParaMatriz("bombermap.txt", mapa); //lê o arquivo denovo

    criarMapa(mapa, jog, inimigos, chaves, numero);// cria o mapa denovo
    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}

/*ESSA FUNÇÃO CARREGA UM JOGO SALVO EM UM ARQUIVO BINARIO*/
void carregarJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[])
{
    int i;
    int lin,col;

    printf("\nCarregando o jogo...");

    FILE *arq;
    arq = fopen("JogoSalvo.bomb","rb"); //lê o arquivo do jogo salvo
    if (arq == NULL) // verifica erros
    {
        printf("\nErro ao carregar o jogo salvo.");
    }
    else
    {
        /**carrega as informações do jogador**/
        fread(player,sizeof(int),4,arq);

        /**carrega as informações das bombas**/
        fread(bombas,sizeof(struct infBomba),MAXBOMBAS,arq);

        /**carrega o mapa**/
        fread(mapa,sizeof(char),TAMANHOMAPA,arq);
    }
    fclose(arq);//   fecha o arquivo

    criarMapa(mapa, jog, inimigos, chaves, numero);//cria o mapa denovo
    printf("  Jogo carregado com sucesso!");
}

/*ESSA FUNÇÃO SALVA O JOGO EM UM ARQUIVO BINARIO*/
void salvarJogo(char mapa[LINHAS][COLUNAS], struct posicao *jog, struct posicao inimigos[], struct posicao chaves[], struct infJogador *player, struct infBomba bombas[], int numero[])
{
    int i;
    int lin,col;
    char n = '\n';

    printf("\nSalvando o jogo...");

//atualiza a posição dos inimigos no mapa
    for (i=0; i<numero[4]; i++)
    {
        lin = inimigos[i].lin / TAMBLOCO;
        col = inimigos[i].col / TAMBLOCO;
        mapa[lin][col] = 'E';
    }
    // atualiza a posiçao do jogador no mapa
    lin = jog->lin / TAMBLOCO;
    col = jog->col / TAMBLOCO;
    mapa[lin][col] = 'J';


    FILE *save;
    save = fopen("JogoSalvo.bomb","wb"); //cria um arquivo para salvar o jogo
    if (save == NULL)
    {
        printf("\nErro ao salvar o jogo.");//verifica erros
    }
    else
    {
        /**salva as informações do jogador**/
        fwrite(player,sizeof(int),4,save);

        /**salva as informações das bombas**/
        fwrite(bombas,sizeof(struct infBomba),MAXBOMBAS,save);

        /**Salva o Mapa**/
        for (i=0; i<LINHAS; i++)
        {
            fwrite(mapa,sizeof(char),60,save);
            fwrite("\n",sizeof(char),1,save);
        }
    }
    fclose(save);/// fecha o arquivo
    printf("  Sucesso!");
}
