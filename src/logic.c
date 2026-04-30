#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tipos.h>
#include <stdio.h>


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
    game->dificulty = EASY;
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);

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

// Para atualizar o score em relação ao tempo
// Será chamada em todos os frames do jogo
void atualizarTempoRealScore (Session *game){

}

// Para atualizar o score em relação ao palpite
int calcularPalpiteScore(Session *game){
    return 10;
}


struct Teste {
    char nome[50];
    int score;
    int tentativas;
};

void atualizarHighscore(Session *game) {
    struct Teste lista[11];
    int total = 0;

    FILE *salvarscore = fopen("highscore.txt", "r");

    if (salvarscore != NULL) {
        while (total < 10 && fscanf(salvarscore, "%s %d", lista[total].nome, &lista[total].score) == 2) {
            total++;
        }
        fclose(salvarscore);
    }

    //NÃO TENHO IDEIA DE COMO FAZER
    //strncpy(lista[total].nome, game->nome, 49);
    //lista[total].nome[49] = '\0';
    //lista[total].score = game->score;
    //total++;

    //bubble sort pra organizar 
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (lista[j].score < lista[j + 1].score) {
                struct Teste temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    //salva o top 10
    FILE *arquivo = fopen("highscore.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar ranking!\n");
        return;
    }

    int limite = (total > 10) ? 10 : total;
    for (int i = 0; i < limite; i++) {
        fprintf(arquivo, "%s %d\n", lista[i].nome, lista[i].score);
    }

    fclose(arquivo);
   


}

// Verifica se o score do jogo é um high score   Lucas e Rodrigo ????
int checarHighscore(Session *game){
    int a = 1;
    if (a == 1) {
        return 1;
    } else {
        return 0;
    }
}

// Salvar estado final da partida   Lucas e rodrigo????
void salvarFinalDePartida(Session *game){

}

// Buscar curiosidade a partir do valor acertado
char* buscarCuriosidade(int target){
    static char curiosidade[128] = "Curiosidade";
    return curiosidade;
}

// Essa função, no momento, executa todos os passos necessários
// para atualizar estado do jogo (GameState) a partir do novo palpite (int) do usuário.
// * provavelmente separar em mais funcoes
void ProcessarTentativa(Session *game, int palpite) {
    game->guessCount++; // incrementa tentativas

    game->guess = palpite; // atribui o palpite do user
                             // ao GameState game 

    ProcessarTemperatura(game); // "Quente", "Frio" ...

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada

    game->score -= calcularPalpiteScore(game); // por enquanto sempre adiciona 10, mas deve ser dinamico

    if (palpite == game->target) { // acertou?
        game->state = STATE_GAMEOVER;
        strcpy(game->message, "Voce acertou!"); 
    } else if (palpite < game->target) {
        strcpy(game->message, "Sonhe mais alto!"); 
    } else {
        strcpy(game->message, "Abaixe essa bola!");
    }
}