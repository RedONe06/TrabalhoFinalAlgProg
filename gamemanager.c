#include "structs.h"
#include "constants.h"
#include "mapa.h"
#include "raylib.h"

void proximoNivel(int nivel, MAPA *mapa, JOGADOR *jogador)
{
    nivel++;
    jogador->nChaves = 0;
    //desenharProxNivel();
    //lerMapaDoArquivo(nivel, mapa, contadores, baus);
}

/* Inicia o jogo do zero */
void novoJogo(int nivel, MAPA *mapa, JOGADOR *jogador)
{
    // Reinicia os contadores
    // contadores[0] = contadores[1] = contadores[2] = contadores[3] = contadores[4] = 0;
    nivel = 0;

    // Reinicia o jogador
    jogador->nVidas = 3;
    jogador->nBombas = 3;
    jogador->pontuacao = 0;
    jogador->nChaves = 0;

    // Reinicia as bombas
    jogador->bombas[0].ativa = false;
    jogador->bombas[1].ativa = false;
    jogador->bombas[2].ativa = false;

    // Lê novamente o arquivo e recria do 0
    //lerMapaDoArquivo(nivel, mapa);
    //criarMapa(mapa, posJogador, posInimigos, posChaves, contadores);

    printf("\n\n----- NOVO JOGO INICIADO! -----\n");
}


void controlarMenu(bool *menuEstaRodando, MAPA *mapa, JOGADOR *jogador)
{
    if(IsKeyPressed(KEY_N))
    {
        //novoJogo(nivel, mapa, jogador, contadores, baus);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_C))
    {
        printf("\nCarregar jogo (Implementar)");
        // carregarJogo(nivel, mapa, jogador, contadores, baus);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_S))
    {
        printf("\nSalvar jogo (Implementar)");
        // salvarJogo(nivel, mapa, jogador, contadores);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_Q))
    {
        printf("\nSaindo do jogo");
        CloseWindow();
    }
    else if (IsKeyPressed(KEY_V))
    {
        printf("Voltando ao jogo");
        *menuEstaRodando = false;
    }
}

bool jaPassouSegundos(time_t tempoInicial, int segundos)
{
    double intervaloEmSegundos = difftime(time(NULL), tempoInicial)/10;
    intervaloEmSegundos = (intervaloEmSegundos - floor(intervaloEmSegundos)) * 10;
    return (int)intervaloEmSegundos > segundos;
}
