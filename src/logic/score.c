#include <stdlib.h>
#include <logic.h>
#include <stdio.h>

void processarTemperatura(Session *game) {
    int distancia = abs(game->guess - game->target);

    if (distancia >= 15){
        game->temperature = COLD; 
    } else if (distancia > 5 && distancia < 15){
        game->temperature = WARM; 
    } else {
        game->temperature = HOT;
    }
}


void atualizarTempoRealScore(Session *game, double dt) { 
 
    if (game == NULL || game->state != STATE_PLAYING) { 
        return;
    }

    double pontosPorSegundo;  // Define quantos pontos são perdidos por segundo de jogo ativo

    switch (game->difficulty){
    case EASY:
        pontosPorSegundo = 30; // 20 segundos
        break;
    case MEDIUM:
        pontosPorSegundo = 60; // 10 segundos
        break;
    case HARD:
        pontosPorSegundo = 100; // +-6.5 segundos
        break;   
    }

    // Aplica o decremento usando o dt (Delta Time) que foi injetado na função
    game->score -= pontosPorSegundo * dt;
}


int calcularPalpiteScore(Session *game){
    if (game == NULL) return 0;

    switch (game -> temperature) {
    case COLD:
        return 30;
    
    case WARM:
        return 15;
    
    case HOT:
        return 5;
    
    default:
        return 10000; // retornando um valor alto para o erro ficar obvio
    }
}