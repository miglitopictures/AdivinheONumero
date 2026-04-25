#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tipos.h>


// Gerar numero aleatorio RNG
void resetarRandomSeed(){
    srand(time(NULL)); // gerando novo seed para randomizacao do numero
}
int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

void configurarCuriosidade(Session *game){
    strcpy(game->trivia, "Uma curiosidade sobre o numero sorteado");
}

/**
 * @brief Inicializa o estado do jogo para uma nova rodada.
 * * @param game Ponteiro para a estrutura GameState que será inicializada.
 */
void IniciarJogo(Session *game) {
    game->dificulty = EASY;
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);

    configurarCuriosidade(game);

    game->guessCount = 0; // contador de tentativas
    game->message[0] = '\0';
    game->temperature[0] = '\0';
    game->score = 0;

}

/**
 * @brief Calcula a proximidade (Temperatura) entre o palpite e o número secreto.
 * * @param game Ponteiro para o GameState.
 */
void ProcessarTemperatura(Session *game) {
    // Pegamos a distancia entre o palpite do jogador e o numero secreto da rodada
    int distancia = abs(game->guess - game->target);

    // Mudamos a mensagem se temperatura a partir dessa "distância"
    if (distancia >= 15){
        strcpy(game->temperature, "Frio"); 
        //game->temperatura = "Frio"
    } else if (distancia > 5 && distancia < 15){
        strcpy(game->temperature, "Morno"); 
    } else {
        strcpy(game->temperature, "Quente"); 
    }
}

int calcularScore(Session *game){
    return 10;
}

// Essa função, no momento, executa todos os passos necessários
// para atualizar estado do jogo (GameState) a partir do novo palpite (int) do usuário.
// * provavelmente separar em mais funcoes
void ProcessarTentativa(Session *game, int palpite) {
    game->guessCount++; // incrementa tentativas

    game->guess = palpite; // atribui o palpite do user
                             // ao GameState game 

    ProcessarTemperatura(game); // "Quente", "Frio" ...

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada

    game->score += calcularScore(game); // por enquanto sempre adiciona 10, mas deve ser dinamico

    if (palpite == game->target) { // acertou?
        game->state = STATE_GAMEOVER;
        
    } 
    
    if (palpite < game->target) {
        strcpy(game->message, "Sonhe mais alto!"); 
    } else {
        strcpy(game->message, "Abaixe essa bola!");
    }
}