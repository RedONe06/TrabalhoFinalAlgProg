#include "structs.h"
#include "constants.h"
#include "mapa.h"
#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "filemanager.h"

void iniciarVariaveis(MAPA *mapa, JOGADOR *jogador) {
    printf("\nReiniciando variáveis de mapa e jogador");

    // Copia o valor retornado para as estruturas apontadas pelos ponteiros
    *mapa = iniciarMapa();
    *jogador = iniciarJogador();
}

/* Inicia o jogo do zero */
void novoJogo(MAPA *mapa, JOGADOR *jogador, bool *trocandoDeNivel, int *niveisPassados, int indexMapas[MAX_MAPAS])
{
    printf("\nIniciando um novo jogo!");
    iniciarVariaveis(mapa, jogador);
    *trocandoDeNivel = true;
    *niveisPassados = 0;
    lerMapaDoArquivo(indexMapas[0], mapa);
}

void controlarMenu(bool *menuEstaRodando, MAPA *mapa, JOGADOR *jogador,  bool *trocandoDeNivel, int *niveisPassados, int indexMapas[MAX_MAPAS])
{
    if(IsKeyPressed(KEY_N))
    {
        novoJogo(mapa, jogador, trocandoDeNivel, niveisPassados, indexMapas);
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_C))
    {
        carregarJogo(niveisPassados, mapa, jogador);
        printf("\nNiveisPassados: %d", *niveisPassados);
        lerMapaDoArquivo(indexMapas[*niveisPassados], mapa);
        *trocandoDeNivel = true;
        *menuEstaRodando = false;
    }
    else if(IsKeyPressed(KEY_S))
    {
        salvarJogo(niveisPassados, mapa, jogador);
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

void terminarJogo(){
    printf("\nterminarJogo() - Implementar");
}

void trocarDeNivel(MAPA *mapa, int indexMapas[MAX_MAPAS], int *niveisDisponiveis, int *niveisPassados) {
    // Incrementa um aos níveis passados
    *niveisPassados = *niveisPassados + 1;

    // Lê o próximo mapa baseado no novo nível passado
    lerMapaDoArquivo(indexMapas[*niveisPassados], mapa);
}

