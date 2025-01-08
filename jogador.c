#include "structs.h"
#include "constants.h"
#include "raylib.h"
#include "mapa.h"

void controlarMovimentacao(MAPA *mapa, JOGADOR *jogador)
{
    if (IsKeyPressed(KEY_RIGHT) && direcaoEstaLivre(mapa, jogador->posicao, 3))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col + 1] = 'J';
        jogador->direcao = 3; // Direita
    }
    else if (IsKeyPressed(KEY_LEFT) && direcaoEstaLivre(mapa, jogador->posicao, 2))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col - 1] = 'J';
        jogador->direcao = 2; // Esquerda
    }
    else if (IsKeyPressed(KEY_UP) && direcaoEstaLivre(mapa, jogador->posicao, 1))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin - 1][jogador->posicao.col] = 'J';
        jogador->direcao = 1; // Cima
    }
    else if (IsKeyPressed(KEY_DOWN) && direcaoEstaLivre(mapa, jogador->posicao, 0))
    {
        mapa->matriz[jogador->posicao.lin][jogador->posicao.col] = ' ';
        mapa->matriz[jogador->posicao.lin + 1][jogador->posicao.col] = 'J';
        jogador->direcao = 0; // Baixo
    }
}

void perderVida(JOGADOR *jogador)
{
    if (jogador->nVidas > 1)
    {
        jogador->nVidas--;
        jogador->pontuacao -= 100;
        if (jogador->pontuacao < 0) jogador->pontuacao = 0;
    }
    else
    {
        CloseWindow();
        printf("\n\n\t\t- Game over -");
    }
}

JOGADOR iniciarJogador()
{
    JOGADOR jogador;
    jogador.nVidas = 3;
    jogador.nBombas = 3;
    jogador.pontuacao = 0;
    jogador.nChaves = 0;
    jogador.bombas[0].ativa = false;
    jogador.bombas[1].ativa = false;
    jogador.bombas[2].ativa = false;
    return jogador;
}

void matarJogador(){
    printf("\nmatarJogador() - Implementar");
}
