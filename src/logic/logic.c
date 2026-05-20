#include <string.h>
#include <stdio.h>
#include <logic.h>

void iniciarJogo(Session *game) {
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);
	game->state = STATE_MENU;

    game->currentPlayer = PLAYER_1;

    game->round = 0;
    game->totalGuesses = 0;

    strcpy(game->player, "AAA");
    game->guessCount = 0; // contador de tentativas
    game->temperature = COLD;
    game->score = 600;
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


void processarTentativa(Session *game, int palpite) {
	if (game->state != STATE_PLAYING) return;
	
    game->guessCount++; // incrementa tentativas
    game->guess = palpite; // atribui o palpite do user ao GameState game.
 
    processarTemperatura(game); // Atualizar a temperatura com base na distância.

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada.

    game->score -= calcularPalpiteScore(game); // Deduzir os pontos dinamicamente baseado na temperatura calculada em ProcessarTemperatura(game).
	
	processarGameover(game);
	
	// Verificar condições de vitória ou dicas

    // if (game->mode == MODO_COOP)
	game->currentPlayer *= -1; // troca player

    if (palpite == game->target) { // acertou?
        switch (game->mode)
        {
        case MODO_ARCADE:
            avancarRodadaArcade(game);
            break;
        case MODO_NORMAL:
            printf("Acertou");
            configurarCuriosidade(game); // <-- adicionei aqui pra atualizar a curiosidade no momento do acerto
            salvarFinalDePartida(game);
            atualizarHighscore(game); 
            game->state = STATE_WIN;
            break;
        case MODO_COOP:
            game->currentPlayer *= -1; // troca player
        default:
            break;
        }
    }
}

