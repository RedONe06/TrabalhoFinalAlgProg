#include <stdio.h>
#include "canvas.h"
#include "raylib.h"
#include "constants.h"
#include "bau.h"
#include "gamemanager.h"
#include "mapa.h"
#include "jogador.h"
#include "inimigo.h"

void desenharMapa(MAPA *mapa, JOGADOR *jogador)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            switch (mapa->matriz[i][j])
            {
            case 'J': // Jogador
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, GREEN);
                jogador->posicao.lin = i;
                jogador->posicao.col = j;
                break;
            case 'W': // Parede indestrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, DARKGRAY);
                break;
            case 'D': // Parede destrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, GRAY);
                break;
            case 'B': // Baú
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;
            case 'K': // Baú com chave
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;
            case 'C': // Chave
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, GOLD);
                break;
            case 'E': // Inimigo
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                break;
            case 'X': // Bomba
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLACK);
                break;
            case '1': // Contador de explosão
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, ORANGE);
                mapa->matriz[i][j] = '2';
                break;
            case '2':
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, ORANGE);
                mapa->matriz[i][j] = '3';
                break;
            case '3':
                mapa->matriz[i][j] = ' ';
                break;
            default:
                continue;
            }
        }
    }
}

void desenharBarraStatus(JOGADOR *jogador)
{
    DrawText(TextFormat("Vidas: %d\tBombas: %d", jogador->nVidas, jogador->nBombas), 50, 540, 20, BLACK);
    DrawText(TextFormat("Chaves: %d", jogador->nChaves), LARGURA / 2.2, 540, 20, BLACK);
    DrawText(TextFormat("Pontuacao: %d", jogador->pontuacao), LARGURA - 200, 540, 20, BLACK);
}

void desenharMenu()
{
    int yTitulo = ALTURA / 4;
    int xTitulo = LARGURA / 2;
    int xTexto = LARGURA / 3;

    DrawText("Menu", xTitulo, yTitulo, 40, BLACK);
    DrawText("[N] Novo jogo", xTexto, yTitulo + 30, 20, BLACK);
    DrawText("[C] Carregar jogo", xTexto, yTitulo + 55, 20, BLACK);
    DrawText("[S] Salvar jogo", xTexto, yTitulo + 80, 20, BLACK);
    DrawText("[Q] Sair do jogo", xTexto, yTitulo + 105, 20, BLACK);
    DrawText("[V] Voltar", xTexto, yTitulo + 130, 20, BLACK);
}

void desenharProxNivel()
{
}

void desenharExplosao(MAPA *mapa, JOGADOR *jogador, POSICAO posBomba)
{
    desenharExplosaoDirecao(mapa, jogador, posBomba, 0); // Cima
    desenharExplosaoDirecao(mapa, jogador, posBomba, 1); // Baixo
    desenharExplosaoDirecao(mapa, jogador, posBomba, 2); // Esquerda
    desenharExplosaoDirecao(mapa, jogador, posBomba, 3); // Direita
}

void desenharExplosaoDirecao(MAPA *mapa, JOGADOR *jogador, POSICAO posBomba, int direcao)
{
    for (int i = 0; i < ALCANCE_BLOCOS_BOMBA; i++) // Para cima
    {
        POSICAO proximaPosicao = acharProximaPosicao(posBomba, direcao);
        char conteudo = mapa->matriz[proximaPosicao.lin][proximaPosicao.col];
        if (conteudo == 'J') // Jogador
        {
            matarJogador();
            // Remove o jogador do mapa e mostra a explosão
            // mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
            break;
        }
        else if (conteudo == 'W')   // Parede indestrutível
        {
            break;
        }
        else if (conteudo == 'D')   // Parede destrutivel
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
            jogador->pontuacao += 10;
        }
        else if (conteudo == 'B')   // Baú
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
            jogador->pontuacao += 10;
            break;
        }
        else if (conteudo == 'K')
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = 'C';
            jogador->pontuacao += 10;
            break;
        }
        else if (conteudo == 'C')   // Chave
        {
            continue;
        }
        else if (conteudo == 'E')   // Inimigo
        {
            matarInimigo(mapa, proximaPosicao);
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
            jogador->pontuacao += 20;
        }
        else if (conteudo == 'X')   // Bomba
        {
            break;
        }
        else
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
        }
    }
}
