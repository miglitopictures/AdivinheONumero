#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <logic.h>

void atualizarHighscore(Session *game) {

    struct DadosPartida lista[MAX_HIGHSCORES + 1];

    int totalEntradas = 0;

    FILE *highscoreFile = fopen("./data/highscores.txt", "r");

    if (highscoreFile != NULL) {

        // Lê os highscores válidos do arquivo
        while (
            totalEntradas < MAX_HIGHSCORES &&
            fscanf(
                highscoreFile,
                "%49s %d %d",
                lista[totalEntradas].nome,
                &lista[totalEntradas].score,
                &lista[totalEntradas].target
            ) == 3 // Se colocar outra entrada, mudar o número.
        ) {

            totalEntradas++;
        }

        fclose(highscoreFile);
    }

    // Adiciona a partida atual na lista
    strcpy(lista[totalEntradas].nome, game->player);
    lista[totalEntradas].score = game->score;
    lista[totalEntradas].target = game->target;

    totalEntradas++;

    // Ordena os scores do maior para o menor
    for (int i = 0; i < totalEntradas - 1; i++) {

        for (int j = 0; j < totalEntradas - i - 1; j++) {

            if (lista[j].score < lista[j + 1].score) {

                struct DadosPartida temp = lista[j];

                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    FILE *arquivo = fopen("./data/highscores.txt", "w");

    if (arquivo == NULL) {

        printf("Erro ao salvar ranking!\n");
        return;
    }

    int limite = totalEntradas;

    if (limite > MAX_HIGHSCORES) {
        limite = MAX_HIGHSCORES;
    }

    // Salva apenas o TOP 10
    for (int i = 0; i < limite; i++) {

        fprintf(
            arquivo,
            "%s %d %d\n",
            lista[i].nome,
            lista[i].score,
            lista[i].target
        );
    }

    fclose(arquivo);
}

int checarHighscore(Session *game) {

    struct DadosPartida lista[MAX_HIGHSCORES];

    int totalEntradas = 0;

    FILE *highscoreFile = fopen("./data/highscores.txt", "r");

    if (highscoreFile != NULL) {

        // Lê os highscores válidos do arquivo
        while (
            totalEntradas < MAX_HIGHSCORES &&
            fscanf(
                highscoreFile,
                "%49s %d %d",
                lista[totalEntradas].nome,
                &lista[totalEntradas].score,
                &lista[totalEntradas].target
            ) == 3
        ) {

            totalEntradas++;
        }

        fclose(highscoreFile);
    }

    // Se houver menos de 10 scores,
    // o jogador entra automaticamente
    if (totalEntradas < MAX_HIGHSCORES) {
        return 1;
    }

    // Verifica se o score atual supera algum score existente
    for (int i = 0; i < totalEntradas; i++) {

        if (game->score > lista[i].score) {
            return 1;
        }
    }

    return 0;
}

// Salvar estado final da partida   Lucas e rodrigo????
void salvarFinalDePartida(Session *game){
    FILE *partidasFile = fopen("./data/partidas.txt", "a");
    if (partidasFile == NULL) return;

    //Pegar data e hora
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    
    fprintf(partidasFile, "%d;%d;%s;%d;%d;", game->mode, game->score, timestamp, game->target, game->guessCount);
    for (int i = 0; i < game->guessCount; i++) {
        if (i == game->guessCount - 1) {
            fprintf(partidasFile, "%d", game->guessHistory[i]);
        } else {
            fprintf(partidasFile, "%d,", game->guessHistory[i]);
        }
        
    }
    fprintf(partidasFile, ";\n");

    fclose(partidasFile);
}

// implementação de busca de curiosidade baseada no arquivo
// não mexi na função original pra não correr o risco de quebrar algo

char* buscarCuriosidadeArquivo(int target){
    static char resultado[256];

    FILE *file = fopen("./data/curiosidades.txt", "r");

    if (!file) {
        strcpy(resultado, "Erro ao carregar curiosidades");
        return resultado;
    }

    char linha[512];

    while (fgets(linha, sizeof(linha), file)) {

        int num;
        char tema[50];
        char texto[300];
        int exibida;

        sscanf(linha, "%d|%[^|]|%[^|]|%d", &num, tema, texto, &exibida);

        if (num == target) {
            fclose(file);

            strcpy(resultado, texto);
            return resultado;
        }
    }

    fclose(file);

    strcpy(resultado, "Nenhuma curiosidade encontrada.");
    return resultado;
}

// mantém compatibilidade com o que já existia, mas adiciona busca real
void configurarCuriosidade(Session *game){
    char* curiosidade = buscarCuriosidadeArquivo(game->target);

    strcpy(game->trivia, curiosidade);

    // só pra testar se está funcionando
    printf("DEBUG curiosidade: %s\n", game->trivia);
}
