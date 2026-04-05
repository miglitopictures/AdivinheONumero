// menu principal e fluxo da aplicacao
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "tipos.h"


int main(void) {

    srand(time(NULL));

    int menuSelection = 0;
    while (menuSelection != 3){
        printf("-----------------------------------------\n");
        printf("      JOGO DE ADIVINHAÇÃO OTIMIZADO      \n");
        printf("-----------------------------------------\n\n");

        printf("1. Jogar nova partida\n2. Analisar histórico\n3. Sair\n\n");

        printf("Escolha uma opção: ");
        scanf("%d%*c", &menuSelection);

        switch (menuSelection) {
        case 1:
            printf("\n-----------------------------------------\n");
            printf("Novo jogo iniciado, tente adivinhar entre 1 e 100.\n\n");

            Session session;

            session.target = randomRangeInt(1,100);
            session.attempts_count = 0;

            while (1) {
                int guess;
                printf("\nDigite deu palpite: ");
                scanf("%d%*c", &guess);

                session.attempts_count++;

                if (guess < 1 || guess > 100) {
                    printf("Palpite fora dos limite determinado, tente novamente.\n");
                    continue;
                }

                if (guess < session.target) {
                    printf("Muito baixo!\n");
                } else if (guess > session.target) {
                    printf("Muito alto!\n");
                } else {
                    printf("Acertou! Você encontrou o número em %d tentativas.\n\n", session.attempts_count);
                    break; // WIN
                }
            }

            break;
        case 2:
            printf("Falta implementação...\n\n");
            continue;
        default:
            printf("Encerrando... Até a próxima!\n\n");
            break;
        }
    }
    return 0;
}
