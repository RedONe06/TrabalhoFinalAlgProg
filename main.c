/* Este Trabalho Final busca consolidar o aprendizado em relação ao conteúdo teórico e prático abordado e exercitar a
habilidade de trabalho em equipe, consistindo na implementação de um jogo na linguagem de programação C.
O jogo a implementado é uma versão simplificada do jogo BomberMan, utilizando a biblioteca gráfica RayLib*/

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
#include "constants.h"
#include "bomba.h"
#include "mapa.h"
#include "jogador.h"
#include "gamemanager.h"
#include "inimigo.h"

int main()
{
    bool rodando = true; // Enquanto rodando == True o jogo irá rodar
    bool menuEstaRodando = false;
    bool trocandoDeNivel = false;
    bool terminou = false;
    MAPA mapa = iniciarMapa();
    JOGADOR jogador = iniciarJogador();

    int indexMapas[MAX_MAPAS];
    int niveisDisponiveis = 0;
    int niveisPassados = 0;
    acharMapasDisponiveis(indexMapas, &niveisDisponiveis);
    lerMapaDoArquivo(indexMapas[0], &mapa);

    // Printa a matriz para verificar se o mapa foi recebido corretamente
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%c", mapa.matriz[i][j]);
        }
        printf("\n");
    }

    // Inicia a tela do jogo no tamanho especificado pelas variáveis
    InitWindow(LARGURA, ALTURA, "raylib [core] example - basic window");

    // Jogo formatado para 60 fps
    SetTargetFPS(60);

    // While principal do jogo
    while (rodando)
    {
        /****** DESENHO ******/
        BeginDrawing();
        if(!terminou)
        {
            ClearBackground(RAYWHITE); // Desenha o fundo branco
            desenharBarraStatus(&jogador);

            if (!menuEstaRodando)
            {
                if (niveisPassados == niveisDisponiveis) // Se passou por todos os níveis
                {
                    desenharWin(&terminou);
                }
                else if (jogador.nChaves == 5) // Se pegou todas as chaves
                {
                    trocandoDeNivel = true;
                    iniciarVariaveis(&mapa, &jogador);
                    trocarDeNivel(&mapa, indexMapas, &niveisDisponiveis, &niveisPassados);
                }
                else if(jogador.nVidas == 0)
                {
                    desenharGameOver(&terminou);
                }
                else // Desenhar o nível
                {
                    desenharMapa(&mapa, &jogador);
                }
            }
            else
            {
                desenharMenu();
            }

            /**** PERSONAGEM ****/
            //----------------------------------------------------------------------------------
            if(!menuEstaRodando)
            {
                controlarMovimentacao(&mapa, &jogador);
                if (IsKeyPressed(KEY_B)) colocarBomba(&jogador, &mapa);
                if(!trocandoDeNivel)
                {
                    verificaTimerDasBombas(&mapa, &jogador);
                    verificaTimerDosInimigos(&mapa, &jogador);
                }
            }
            //----------------------------------------------------------------------------------
            /**** MENU ****/
            if (!menuEstaRodando && IsKeyPressed(KEY_TAB)) // Ativar menu
            {
                menuEstaRodando = true;
            }
            else if (menuEstaRodando)
            {
                controlarMenu(&menuEstaRodando, &mapa, &jogador, &trocandoDeNivel, &niveisPassados, indexMapas);
            }

            if (IsKeyPressed(KEY_ESCAPE)) rodando = false; // Sair do jogo
            //----------------------------------------------------------------------------------

            if(trocandoDeNivel) trocandoDeNivel = false;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}







