// Esse arquivo vai servir para desenvolver a "interface" do terminal

#include <stdio.h>
#include <string.h>
#include <logic.h>
#include <time.h>

void startTerminalMode(GameState *game) {

    game->state = STATE_MENU;
    int sair = 0;

    while (1) {

        IniciarJogo(game); // seta todos os valores de game
        
        while (game->state == STATE_MENU) {
            char desejaJogar[7];
            printf("------------ADIVINHE O NUMERO-------------\n");
            printf("Digite 'jogar' para jogar ou 'sair' para sair: ");
            scanf("%s%*c", desejaJogar);

            if (strcmp(desejaJogar, "jogar") == 0) {
                printf("Lendo as cartas do seu futuro...\n\n");
                game->state = STATE_PLAYING;
            } else if (strcmp(desejaJogar, "sair") == 0) {
                game->state = STATE_EXIT;
            } else {
                game->state = STATE_MENU;
            }
        }

        while (game->state == STATE_PLAYING) {
            int palpite;
            printf("Palpite: ");
            scanf("%d", &palpite);
            
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
            char desejaContinuar[2];
            printf("Jogar novamente (S/N)? ");
            scanf("%s%*c", desejaContinuar);

            if (strcmp(desejaContinuar, "S") == 0) {
                // printf("\nsim\n"); // degub
                game->state = STATE_PLAYING;
            } else {
                // printf("\nnao\n"); // degub
                game->state = STATE_MENU;
            }
        }

        if (game->state == STATE_EXIT) break;
    }
}