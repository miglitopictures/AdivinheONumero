#include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <stdio.h>
#include <logic.h>

/**
 * @brief Calcula a proximidade (Temperatura) entre o palpite e o número secreto.
 * * @param game Ponteiro para o GameState.
 */
void ProcessarTemperatura(Session *game) {
    // Pegamos a distancia entre o palpite do jogador e o numero secreto da rodada
    int distancia = abs(game->guess - game->target);

    // Mudamos a mensagem se temperatura a partir dessa "distância"
    if (distancia >= 15){
        game->temperature = COLD; 
        //game->temperatura = "Frio"
    } else if (distancia > 5 && distancia < 15){
        game->temperature = WARM; 
    } else {
        game->temperature = HOT;
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
        pontosPorSegundo = 10;
        break;
    case HARD:
        pontosPorSegundo = 30;
        break;   
    }

    // Aplica o decremento usando o dt (Delta Time) que foi injetado na função
    game->score -= pontosPorSegundo * dt;
        
}

//========================================================================================================================================================================
// Para atualizar o score em relação ao palpite
int calcularPalpiteScore(Session *game){
    if (game == NULL) return 0;

    switch (game -> temperature)
    {
    case COLD:
        return 30;
    
    case WARM:
        return 15;
    
    case HOT:
        return 5;
    
    default:
        return 10000;
    }
	
    // int distancia = abs(game->guess - game->target);
	
	// if (distancia == 0) { 
    //     return 0;   // Verifica primeiro se acertou exatamente (distância 0). Sem penalidade.
    // } else if (distancia >= 15){
    //     return 30;  // Frio
    // } else if (distancia > 5){
    //     return 15;  // Morno
	// } else {
    //     return 5;   // Quente (inclui distância 1 a 5)
    // }
}