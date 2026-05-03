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
    game->difficulty = EASY;
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);
	game->state = STATE_PLAYING;
	
    configurarCuriosidade(game);

    game->guessCount = 0; // contador de tentativas
    game->message[0] = '\0';
    game->temperature[0] = '\0';
    game->score = 600;

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

//=======================================================================================================================================================================
// Para atualizar o score em relação ao tempo
// Será chamada em todos os frames do jogo

void atualizarTempoRealScore(Session *game, double dt)    // Verifica  Estado
{ 
 
    if (game == NULL || game->state != STATE_PLAYING) { 
        return;
    }

    double pontosPorSegundo;  // Define quantos pontos são perdidos por segundo de jogo ativo
    switch (game->difficulty)
    {
    case EASY:
        pontosPorSegundo = 2;
        break;
    case MEDIUM:
        pontosPorSegundo = 4;
        break;
    case HARD:
        pontosPorSegundo = 8;
        break;   
    }

    // Aplica o decremento usando o dt (Delta Time) que foi injetado na função
    game->score -= pontosPorSegundo * dt;
        
}

//========================================================================================================================================================================
// Para atualizar o score em relação ao palpite
int calcularPalpiteScore(Session *game){
    if (game == NULL) return 0;
	
    int distancia = abs(game->guess - game->target);
	
	if (distancia == 0) { 
        return 0;   // Verifica primeiro se acertou exatamente (distância 0). Sem penalidade.
    } else if (distancia >= 15){
        return 30;  // Frio
    } else if (distancia > 5){
        return 15;  // Morno
	} else {
        return 5;   // Quente (inclui distância 1 a 5)
    }
}

// Recebe dados de arquivo de highscore a partir dele e da ultima partida
void atualizarHighscore(Session *game){

}

// Verifica se o score do jogo é um high score
int checarHighscore(Session *game){
    int a = 1;
    if (a == 1) {
        return 1;
    } else {
        return 0;
    }
}

// Salvar estado final da partida
void salvarFinalDePartida(Session *game){

}

// Buscar curiosidade a partir do valor acertado
char* buscarCuriosidade(int target){
    static char curiosidade[128] = "Curiosidade";
    return curiosidade;
}

//=====================================================================================================================================================================
// Essa função, no momento, executa todos os passos necessários
// para atualizar estado do jogo (GameState) a partir do novo palpite (int) do usuário.
// * provavelmente separar em mais funcoes
void ProcessarTentativa(Session *game, int palpite) {
	if (game->state != STATE_PLAYING) return;
	
    game->guessCount++; // incrementa tentativas
    game->guess = palpite; // atribui o palpite do user ao GameState game.
 
    ProcessarTemperatura(game); // Atualizar a temperatura com base na distância.

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada.

    game->score -= calcularPalpiteScore(game); // Deduzir os pontos dinamicamente baseado na temperatura calculada em ProcessarTemperatura(game).
	
	if (game->score < 0) { // Impede que o score fique negativo devido aos palpites.
        game->score = 0;
        game->state = STATE_GAMEOVER; // Opcional: declarar Game Over se zerar os pontos.
    }
	
	
	// Verificar condições de vitória ou dicas
	
    if (palpite == game->target) { // acertou?
        game->state = STATE_GAMEOVER;
        
    } 
    
    if (palpite < game->target) {
        strcpy(game->message, "Sonhe mais alto!"); 
    } else {
        strcpy(game->message, "Abaixe essa bola!");
    }
}