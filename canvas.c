#include <stdio.h>
#include "canvas.h"
#include "raylib.h"
#include "constants.h"
#include "bau.h"
#include "gamemanager.h"
#include "mapa.h"
#include "jogador.h"

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
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, SKYBLUE);
                break;
            case 'D': // Parede destrutivel
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BROWN);
                break;
            case 'B': // Baú
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
                break;
            case 'K': // Baú com chave
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
                break;
            case 'E': // Inimigo
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                break;
            case 'X': // Bomba
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLACK);
                break;
            case '1': // Contador de explosão
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                mapa->matriz[i][j] = '2';
                break;
            case '2':
                DrawRectangle(j * TAM_BLOCO, i * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
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
    /*clock_t tempo = clock() + CLOCKS_PER_SEC * 4;
    while (tempo > clock()){
        DrawText("AVANCANDO PARA ", LARGURA / 5 , ALTURA / 2 - 85 , 80, BLACK);
        DrawText("A PRÓXIMA FASE", LARGURA / 5 , ALTURA / 2 , 80, BLACK);
    }*/
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
        else if(conteudo == 'W')   // Parede indestrutível
        {
            break;
        }
        else if(conteudo == 'D')   // Parede destrutivel
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
        }
        else if(conteudo == 'B')   // Baú
        {
            quebrarBau(mapa);
            break;
        }
        else if(conteudo == 'K')   // Chave
        {
            continue;
        }
        else if(conteudo == 'E')   // Inimigo
        {
            matarInimigo();
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
        }
        else if(conteudo == 'X')   // Bomba
        {
            break;
        }
        else
        {
            mapa->matriz[proximaPosicao.lin][proximaPosicao.col] = '1';
        }
    }
}
