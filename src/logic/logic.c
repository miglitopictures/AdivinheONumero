#include <string.h>
#include <stdio.h>
#include <logic.h>

void resetTimer(Timer *timer){
    timer->t = timer->max;
}

int atualizarTimer(Timer *timer, double dt){
    if (timer->t <= 0.0) return 1;
    timer->t -= dt;
    return 0;
}

void mudarPlayer(Session *game){
    resetTimer(&game->timer); 
    game->score = 600;
    game->currentPlayer = (game->currentPlayer + 1) % 2;
}

void atualizarTimerMultiplayer(Session *game, double dt){
    if (atualizarTimer(&game->timer, dt) != 0){
        mudarPlayer(game);
    }
}

void iniciarJogo(Session *game) {
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);
	game->state = STATE_MENU;

    
    game->round = 0;
    game->totalGuesses = 0;
    
    strcpy(game->playerName, "AAA");
    game->guessCount = 0; // contador de tentativas
    game->temperature = COLD;
    game->score = 600;
    game->isHighscore = 0;
    
    // modo multiplayer setup
    game->currentPlayer = PLAYER_1;
    game->timer.max = 5;
    game->timer.t = 5;
    game->placar[PLAYER_1] = 0;
    game->placar[PLAYER_2] = 0;
}

void avancarRodadaArcade(Session *game){
    int bonus = 200; // devemos calcular dinamicamente

    game->target = numeroAleatorio(0, game->max);
    game->round++;
    game->totalGuesses += game->guessCount;
    game->guessCount = 0;
    game->score+=bonus; // bonus por acertar
    
}

void processarGameover(Session *game){
    if (game->score <= 0) { // Impede que o score fique negativo devido aos palpites.
        game->score = 0;
        game->state = STATE_GAMEOVER; // Opcional: declarar Game Over se zerar os pontos.
    }
}

void processarAcerto(Session *game){
    switch (game->mode)
    {
    case MODO_ARCADE:
        printf("%d\n", game->target);
        avancarRodadaArcade(game);
        break;
    case MODO_NORMAL:
        printf("Acertou");
        configurarCuriosidade(game); // <-- adicionei aqui pra atualizar a curiosidade no momento do acerto
        salvarFinalDePartida(game);
        if (checarHighscore(game)) {
            game->isHighscore = 1;
            printf("\n\nhighscore!\n\n");
            game->state = STATE_HIGHSCORE;
        } else {

            game->state = STATE_WIN;
        }
        break;
    case MODO_COOP:
        game->placar[game->currentPlayer]++; // atualizar o placar
        
        // melhor de tres
        for (int i = 0; i < 2; i++){
            if (game->placar[i] >= 2){ 
               game->state = STATE_WIN; 
            }
        }

        game->target = numeroAleatorio(0, game->max);
        //game->currentPlayer *= -1; // troca player
    default:
        break;
    }
}


void processarTentativa(Session *game, int palpite) {
	if (game->state != STATE_PLAYING) return;
	
    game->guessCount++; // incrementa tentativas
    game->guess = palpite; // atribui o palpite do user ao GameState game.
 
    processarTemperatura(game); // Atualizar a temperatura com base na distância.

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada.

    game->score -= calcularPalpiteScore(game); // Deduzir os pontos dinamicamente baseado na temperatura calculada em ProcessarTemperatura(game).
	
	processarGameover(game);
	
    
    if (palpite == game->target) { // acertou?
        processarAcerto(game);
    }
    
    if (game->mode == MODO_COOP) {
        if (game-> state != STATE_WIN){
            mudarPlayer(game);
        }
    } // troca player
}