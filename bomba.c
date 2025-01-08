#include "constants.h"
#include "structs.h"
#include "mapa.h"
#include "gamemanager.h"
#include "canvas.h"

void explodirBomba(MAPA *mapa, BOMBA *bomba, JOGADOR *jogador)
{
    // Limpa o espa�o da bomba e marca como um espa�o vazio
    mapa->matriz[bomba->posicao.lin][bomba->posicao.col] = ' ';
    bomba->ativa = false;
    jogador->nBombas++;

    desenharExplosao(mapa, jogador, bomba->posicao);
}

void verificaTimerDasBombas(MAPA *mapa, JOGADOR *jogador)
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (jogador->bombas[i].ativa == false) continue;

        if (jaPassouSegundos(jogador->bombas[i].tempoInicio, 3))
        {
            explodirBomba(mapa, &jogador->bombas[i], jogador);
        }
    }
}

/* Posiciona a bomba no mapa */
void colocarBomba(JOGADOR *jogador, MAPA *mapa)
{
    // Se n�o tiver bombas ou espa�o para colocar bomba, n�o colocar bomba
    if (jogador->nBombas <= 0 || !direcaoEstaLivre(mapa, jogador->posicao, jogador->direcao)) return;

    // Define a posi��o que a bomba vai ficar
    POSICAO posicaoBomba = acharProximaPosicao(jogador->posicao, jogador->direcao);
    // Pega o index da bomba baseado na quantidade dispon�vel no arsenal
    int ultimaBomba = jogador->nBombas - 1;

    jogador->bombas[ultimaBomba].tempoInicio = time(NULL); // Armazena o tempo atual
    jogador->bombas[ultimaBomba].posicao.lin = posicaoBomba.lin;
    jogador->bombas[ultimaBomba].posicao.col = posicaoBomba.col;
    jogador->bombas[ultimaBomba].ativa = true;

    mapa->matriz[posicaoBomba.lin][posicaoBomba.col] = 'X'; // Coloca um B para marcar a bomba no mapa
    jogador->nBombas--; // Decrementa o n�mero de bombas do arsenal
}
