#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gamestate.h>


// Gerar numero aleatorio
void resetarRandomSeed(){
    srand(time(NULL)); // gerando novo seed para randomizacao do numero
}

int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

/**
 * @brief Inicializa o estado do jogo para uma nova rodada.
 * * @param game Ponteiro para a estrutura GameState que será inicializada.
 */
void IniciarJogo(GameState *game) {
    game->max = 100;
    game->numeroSecreto = numeroAleatorio(0, game->max);
    game->tentativas = 0;
    game->limiteTentativas = 10;
    game->state = STATE_PLAYING;
    game->mensagem[0] = '\0';

    for (int i = 0; i < MAX_HISTORICO; i++){
        game->historicoTentativas[i] = 0;
    }
}

/**
 * @brief Calcula a proximidade (Temperatura) entre o palpite e o número secreto.
 * * @param game Ponteiro para o GameState.
 */
void ProcessarTemperatura(GameState *game) {
    // Pegamos a distancia entre o palpite do jogador e o numero secreto da rodada
    int distancia = abs(game->palpite - game->numeroSecreto);

    // Mudamos a mensagem se temperatura a partir dessa "distância"
    if (distancia >= 15){
        strcpy(game->temperatura, "Frio"); 
    } else if (distancia > 5 && distancia < 15){
        strcpy(game->temperatura, "Morno"); 
    } else {
        strcpy(game->temperatura, "Quente"); 
    }
}

void ProcessarTentativa(GameState *game, int palpite) {
    game->tentativas++;
    game->palpite = palpite;
    ProcessarTemperatura(game);

    if (palpite == game->numeroSecreto) {
        game->state = STATE_GAMEOVER;
    } else if (game->tentativas >= game->limiteTentativas) {
        game->state = STATE_GAMEOVER;
    } else if (palpite < game->numeroSecreto) {
        strcpy(game->mensagem, "Sonhe mais alto!"); 
    } else {
        strcpy(game->mensagem, "Abaixe essa bola!");
    }
}