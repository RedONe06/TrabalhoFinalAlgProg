#include "mapa.h"
#include "structs.h"
#include "gamemanager.h"

void moverInimigo(MAPA *mapa, INIMIGO *inimigo)
{
    // Array para armazenar dire��es v�lidas
    int direcoesValidas[4];
    int quantidadeDirecoes = 0;

    // Verificar todas as dire��es poss�veis
    for (int direcao = 0; direcao < 4; direcao++)
    {
        POSICAO proximaPosicao = acharProximaPosicao(inimigo->posicao, direcao);
        char conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];

        if (direcaoEstaLivre(mapa, inimigo->posicao, direcao) || conteudo == 'J')
        {
            // Armazenar dire��es v�lidas
            direcoesValidas[quantidadeDirecoes++] = direcao;
        }
    }

    // Selecionar uma dire��o v�lida aleatoriamente
    srand(time(NULL));
    int direcaoEscolhida = direcoesValidas[rand() % quantidadeDirecoes];

    // Atualizar o mapa com base na dire��o escolhida
    switch (direcaoEscolhida)
    {
    case 3: // Direita
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col + 1] = 'E';
        inimigo->posicao.col++;
        break;
    case 2: // Esquerda
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col - 1] = 'E';
        inimigo->posicao.col--;
        break;
    case 1: // Cima
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';
        mapa->matriz[inimigo->posicao.lin - 1][inimigo->posicao.col] = 'E';
        inimigo->posicao.lin--;
        break;
    case 0: // Baixo
        mapa->matriz[inimigo->posicao.lin][inimigo->posicao.col] = ' ';
        mapa->matriz[inimigo->posicao.lin + 1][inimigo->posicao.col] = 'E';
        inimigo->posicao.lin++;
        break;
    }
}


void moverInimigos(MAPA *mapa)
{
    for(int i = 0; i < mapa->nInimigos; i++)
    {
        if (mapa->inimigos[i].vivo == false) continue;

        moverInimigo(mapa, &mapa->inimigos[i]);
    }
}


void verificaTimerDosInimigos(MAPA *mapa)
{
    if (jaPassouSegundos(mapa->tempoInicio, 1))
    {
        moverInimigos(mapa);
        mapa->tempoInicio = time(NULL);
    }
}


