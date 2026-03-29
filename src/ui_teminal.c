// Esse arquivo vai servir para desenvolver a "interface" do terminal

#include <stdio.h>
#include <logic.h>

void startTerminalMode(GameState *game) {

    IniciarJogo(game);
    while (game->state == STATE_PLAYING) {
        int myGuess;
        printf("Guess: ");
        scanf("%d", &myGuess);
        
        // Call the engine
        ProcessarTentativa(game, myGuess);
        
        // React to the results
        if (game->state == STATE_GAMEOVER) {
            printf("You won in %d tries!\n", game->tentativas);
        } else {
            printf("%s\n", game->mensagem);
            printf("%s\n", game->temperatura);
        }
    }

}