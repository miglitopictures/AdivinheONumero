// Esse arquivo vai servir para desenvolver a "interface" do terminal

#include <stdio.h>
#include <string.h>
#include <logic.h>
#include <time.h>

void startTerminalMode(GameState *game) {

    IniciarJogo(game);
    game->state = STATE_MENU;
    while (game->state == STATE_MENU) {
        char desejaJogar[7];
        printf("------------ADIVINHE O NUMERO-------------\n");
        printf("Digite 'jogar' para jogar: ");
        scanf("%s%*c", desejaJogar);

        if (strcmp(desejaJogar, "jogar") == 0) {
            printf("Gerando numeros.... beep bop bep...\n\n");
            game->state = STATE_PLAYING;
        } else {
            game->state = STATE_MENU;
        }
    }
    while (game->state == STATE_PLAYING) {
        int palpite;
        printf("Palpite: ");
        scanf("%d", &palpite);
        
        // Call the engine
        ProcessarTentativa(game, palpite);
        
        // React to the results
        if (game->state == STATE_GAMEOVER) {
            printf("Você acertou em %d tentativas!\n", game->tentativas);
        } else {
            printf("%s\n", game->mensagem);
            printf("%s\n", game->temperatura);
        }
    }

    while (game->state == STATE_GAMEOVER) {
        char desejaContinuar;
        printf("Jogar novamente (S/N)? ");
        scanf("%c%*c", &desejaContinuar);

        if (desejaContinuar == 'S' || desejaContinuar == 's') {
            game->state == STATE_PLAYING;
        } else {
            game->state == STATE_MENU;
        }
    }
}