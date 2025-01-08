#include "structs.h"
#include "constants.h"

bool direcaoEstaLivre(MAPA *mapa, POSICAO posicaoAtual, int direcao)
{
    switch(direcao)
    {
    case 0: // Baixo
        if (mapa->matriz[posicaoAtual.lin + 1][posicaoAtual.col] == ' ') return true;
        break;
    case 1: // Cima
        if (mapa->matriz[posicaoAtual.lin - 1][posicaoAtual.col] == ' ') return true;
        break;
    case 2: // Esquerda
        if (mapa->matriz[posicaoAtual.lin][posicaoAtual.col - 1] == ' ') return true;
        break;
    default: // Direita
        if (mapa->matriz[posicaoAtual.lin][posicaoAtual.col + 1] == ' ') return true;
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
