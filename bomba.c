#include "constants.h"
#include "structs.h"
#include "mapa.h"

void explodirBomba(MAPA *mapa, BOMBA *bomba, JOGADOR *jogador)
{
    // Limpa o espa�o da bomba e marca como um espa�o vazio
    mapa->matriz[bomba->posicao.lin][bomba->posicao.col] = ' ';
    bomba->ativa = false;
    jogador->nBombas++;
    /*
    int i,j;

    for (i=-2; i<3; i++)
    {
        for (j=-2; j<3; j++)
        {
        switch(mapa->matriz[bomba->posicao.lin + i][bomba->posicao.col + j])
        {
        case 'D':
            mapa->matriz[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 10;
            break;
        case 'B':
            mapa->mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 10;
            break;
        case 'K':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = 'C';
            contadores[2]--;
            jogador->pontuacao += 10;
            break;
        case 'E':
            mapa[bomba->posicao.lin + i][bomba->posicao.col + j] = ' ';
            jogador->pontuacao += 20;
            contadores[4]--;
            break;
        case 'J':
            perderVida(jogador);
            break;
        case 'X':
            //explodirBomba();
            break;
        default:
            break;

        }
        }
    }

    //desenharExplosao(mapa, jogador);*/
}

void verificaTimerDasBombas(MAPA *mapa, JOGADOR *jogador)
{
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (jogador->bombas[i].ativa == false) continue;

        double intervaloEmSegundos = difftime(time(NULL), jogador->bombas[i].tempoInicio)/10;
        intervaloEmSegundos = (intervaloEmSegundos - floor(intervaloEmSegundos)) * 10;
        if ((int)intervaloEmSegundos > 3)
        {
            // Se j� se passaram 3 segundos do tempo de in�cio da bomba, explodir ela
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
