#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logic.h"


// Gerar numero aleatorio
void resetarRandomSeed(){
    srand(time(NULL)); // gerando novo seed para randomizacao do numero
}

int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

// Inicializar Jogo
void IniciarJogo(GameState *game) {
    game->max = 100;
    game->numeroSecreto = numeroAleatorio(0, game->max);
    game->tentativas = 0;
    game->state = STATE_PLAYING;
    game->mensagem[0] = '\0';
}

void ProcessarTemperatura(GameState *game, int guess) {

    int distancia = abs(guess - game->numeroSecreto);

    if (distancia >= 15){
        strcpy(game->temperatura, "Frio"); 
    } else if (distancia > 5 && distancia < 15){
        strcpy(game->temperatura, "Morno"); 
    } else {
        strcpy(game->temperatura, "Quente"); 
    }
}

void ProcessarTentativa(GameState *game, int guess) {
    game->tentativas++;
    ProcessarTemperatura(game, guess);
    if (guess == game->numeroSecreto) {
        game->state = STATE_GAMEOVER;
    } else if (guess < game->numeroSecreto) {
        // Here is the "Logic" (UH3) written only once!
        strcpy(game->mensagem, "Higher!"); 
    } else {
        strcpy(game->mensagem, "Lower!");
    }
}