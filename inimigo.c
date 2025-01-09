#include "mapa.h"
#include "structs.h"
#include "gamemanager.h"

void moverInimigo(MAPA *mapa, INIMIGO *inimigo, JOGADOR *jogador)
{
    // Array para armazenar direções válidas
    int direcoesValidas[4];
    int quantidadeDirecoes = 0;

    // Verificar todas as direções possíveis
    for (int direcao = 0; direcao < 4; direcao++)
    {
        POSICAO proximaPosicao = acharProximaPosicao(inimigo->posicao, direcao);
        char conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];

        if (direcaoEstaLivre(mapa, inimigo->posicao, direcao) || conteudo == 'J')
        {
            // Armazenar direções válidas
            direcoesValidas[quantidadeDirecoes++] = direcao;
        }
    }

    // Selecionar uma direção válida aleatoriamente
    srand(time(NULL));
    int direcaoEscolhida = direcoesValidas[rand() % quantidadeDirecoes];

    POSICAO proximaPosicao;
    char conteudo;

    // Atualizar o mapa com base na direção escolhida
    switch (direcaoEscolhida)
    {
    case 3: // Direita
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';

        acharProximaPosicao(inimigo->posicao, direcaoEscolhida);
        conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];
        if(conteudo == 'J') jogador->nVidas--;

        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col + 1] = 'E';
        inimigo->posicao.col++;
        break;
    case 2: // Esquerda
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';

        proximaPosicao = acharProximaPosicao(inimigo->posicao, direcaoEscolhida);
        conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];
        if(conteudo == 'J') jogador->nVidas--;

        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col - 1] = 'E';
        inimigo->posicao.col--;
        break;
    case 1: // Cima
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';

        proximaPosicao = acharProximaPosicao(inimigo->posicao, direcaoEscolhida);
        conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];
        if(conteudo == 'J') jogador->nVidas--;

        mapa->matriz[inimigo->posicao.lin - 1][inimigo->posicao.col] = 'E';
        inimigo->posicao.lin--;
        break;
    case 0: // Baixo
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';

        proximaPosicao = acharProximaPosicao(inimigo->posicao, direcaoEscolhida);
        conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];
        if(conteudo == 'J') jogador->nVidas--;

        mapa->matriz[inimigo->posicao.lin + 1][inimigo->posicao.col] = 'E';
        inimigo->posicao.lin++;
        break;
    }
}

void moverInimigos(MAPA *mapa, JOGADOR *jogador)
{
    for(int i = 0; i < mapa->nInimigos; i++)
    {
        if (mapa->inimigos[i].vivo == false) continue;

        moverInimigo(mapa, &mapa->inimigos[i], jogador);
    }
}

void verificaTimerDosInimigos(MAPA *mapa, JOGADOR *jogador)
{
    if (jaPassouSegundos(mapa->tempoInicio, 1))
    {
        moverInimigos(mapa, jogador);
        mapa->tempoInicio = time(NULL);
    }
}

void matarInimigo(MAPA *mapa, POSICAO posicao)
{
    for(int i = 0; i < mapa->nInimigos; i++)
    {
        if(mapa->inimigos[i].posicao.lin == posicao.lin && mapa->inimigos[i].posicao.lin == posicao.col)
        {
            mapa->inimigos[i].vivo = false;
            break;
        }
    }

    mapa->nInimigos--;
}


