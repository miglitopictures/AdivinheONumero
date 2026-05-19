#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <logic.h>

/**
 * @brief Inicializa o estado do jogo para uma nova rodada.
 * * @param game Ponteiro para a estrutura GameState que será inicializada.
 */
void IniciarJogo(Session *game) {
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);
	game->state = STATE_MENU;
	
    // configurarCuriosidade(game); // carregava curiosidade no início, movido para o momento do acerto

    game->round = 0;
    game->totalGuesses = 0;

    strcpy(game->player, "AAA");
    game->guessCount = 0; // contador de tentativas
    game->message[0] = '\0';
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

void ProcessarGameover(Session *game){
    if (game->score <= 0) { // Impede que o score fique negativo devido aos palpites.
        game->score = 0;
        game->state = STATE_GAMEOVER; // Opcional: declarar Game Over se zerar os pontos.
    }
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
	
	ProcessarGameover(game);
	
	
	// Verificar condições de vitória ou dicas
	
    if (palpite == game->target) { // acertou?
        if (game->mode == MODO_ARCADE){
            avancarRodadaArcade(game);
        } else{
            printf("Acertou");
            configurarCuriosidade(game); // <-- adicionei aqui pra atualizar a curiosidade no momento do acerto
            strcpy(game->message, "Voce acertou!");
            salvarFinalDePartida(game);
            atualizarHighscore(game); 
            
            game->state = STATE_WIN;
        }
        
    }

        else if (palpite < game->target) {
        strcpy(game->message, "Sonhe mais alto!"); 
     } else {
        strcpy(game->message, "Abaixe essa bola!");
    }
}

