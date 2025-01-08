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

/*
TODO ==========================
- Explosão de bombas
- Inimigo andar
- Pontuação
- Níveis
- Novo jogo, salvar, carregar
- A bomba não explode outras bombas
*/

int main()
{
    int nivel = 1;
    bool rodando = true; // Enquanto rodando == True o jogo irá rodar
    bool menuEstaRodando = false;

    MAPA mapa = iniciarMapa();
    JOGADOR jogador = iniciarJogador();

    lerMapaDoArquivo(nivel, &mapa);

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
        ClearBackground(RAYWHITE); // Desenha o fundo branco

        desenharBarraStatus(&jogador);
        if (!menuEstaRodando) desenharMapa(&mapa, &jogador);
        else desenharMenu();

        /**** PERSONAGEM ****/
        //----------------------------------------------------------------------------------
        if(!menuEstaRodando)
        {
            controlarMovimentacao(&mapa, &jogador);
            if (IsKeyPressed(KEY_B)) colocarBomba(&jogador, &mapa);
            verificaTimerDasBombas(&mapa, &jogador);
        }
        //----------------------------------------------------------------------------------
        /**** MENU ****/
        if (!menuEstaRodando && IsKeyPressed(KEY_TAB)) // Ativar menu
        {
            menuEstaRodando = true;
        }
        else if (menuEstaRodando)
        {
            controlarMenu(&menuEstaRodando, nivel, &mapa, &jogador);
        }

        if (IsKeyPressed(KEY_ESCAPE)) rodando = false; // Sair do jogo
        //----------------------------------------------------------------------------------

        /**** MUDAR DE FASE ****/
        if (jogador.nChaves >= 5) proximoNivel(nivel, &mapa, &jogador);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    return 0;
}







