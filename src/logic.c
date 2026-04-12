#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gamestate.h>


// Gerar numero aleatorio RNG
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

    game->tentativas = 0; // contador de tentativas
    game->limiteTentativas = 10;
    //game->state = STATE_MENU;
    game->mensagem[0] = '\0';
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
        //game->temperatura = "Frio"
    } else if (distancia > 5 && distancia < 15){
        strcpy(game->temperatura, "Morno"); 
    } else {
        strcpy(game->temperatura, "Quente"); 
    }
}

// Essa função, no momento, executa todos os passos necessários
// para atualizar estado do jogo (GameState) a partir do novo palpite (int) do usuário.
// * provavelmente separar em mais funcoes
void ProcessarTentativa(GameState *game, int palpite) {
    game->tentativas++; // incrementa tentativas

    game->palpite = palpite; // atribui o palpite do user
                             // ao GameState game 

    ProcessarTemperatura(game); // "Quente", "Frio" ...

    if (palpite == game->numeroSecreto) { // acertou?
        game->state = STATE_GAMEOVER;
    } else if (game->tentativas >= game->limiteTentativas) {
        game->state = STATE_GAMEOVER;
    } else if (palpite < game->numeroSecreto) {
        strcpy(game->mensagem, "Sonhe mais alto!"); 
    } else {
        strcpy(game->mensagem, "Abaixe essa bola!");
    }
}