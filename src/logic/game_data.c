#include <string.h>
#include <time.h>
#include <stdio.h>
#include <logic.h>
#include <math.h>

// Coloca no *buffer a string para o caminho do arquivo de highscores correto a partir do modo de jogo e dificuldade selecionada.
void montarCaminhoHighscore(char *buffer, size_t size, Session *game) {
    const char *modo;
    const char *dificuldade;

    if (game->mode == MODO_NORMAL)       modo = "normal";
    else if (game->mode == MODO_ARCADE)  modo = "arcade";
    else                                 modo = "coop";

    if (game->difficulty == EASY)        dificuldade = "easy";
    else if (game->difficulty == MEDIUM) dificuldade = "medium";
    else                                 dificuldade = "hard";

    snprintf(buffer, size, "./data/highscores/%s_%s.txt", modo, dificuldade);
}

void atualizarHighscore(Session *game) {

    char path[128];
    montarCaminhoHighscore(path, sizeof(path), game);

    struct DadosHighscore lista[MAX_HIGHSCORES + 1];
    int totalEntradas = 0;
    FILE *highscoreFile = fopen(path, "r");

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
    strcpy(lista[totalEntradas].nome, game->playerName);
    lista[totalEntradas].score = (int) game->score;
    lista[totalEntradas].target = game->target;

    printf("\n%d score!\n\n", (int) game->score);
    totalEntradas++;

    // Ordena os scores do maior para o menor
    for (int i = 0; i < totalEntradas - 1; i++) {

        for (int j = 0; j < totalEntradas - i - 1; j++) {

            if (lista[j].score < lista[j + 1].score) {

                struct DadosHighscore temp = lista[j];

                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    FILE *arquivo = fopen(path, "w");

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

    char path[128];
    montarCaminhoHighscore(path, sizeof(path), game);

    struct DadosHighscore lista[MAX_HIGHSCORES];
    int totalEntradas = 0;
    FILE *highscoreFile = fopen(path, "r");

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



ListaHighscores coletarHighscores(Session *game){

    ListaHighscores highscores = {0};
    highscores.modo = game->mode;
    highscores.dificuldade = game->difficulty;

    char path[128];
    montarCaminhoHighscore(path, sizeof(path), game);

    FILE *arquivoHighscores = fopen(path, "r");

    if (arquivoHighscores != NULL){

        while (
            highscores.count < MAX_HIGHSCORES &&
            fscanf(arquivoHighscores,
                      "%49s %d %d",
                      highscores.lista[highscores.count].nome,
                      &highscores.lista[highscores.count].score,
                      &highscores.lista[highscores.count].target) == 3)
        {
            highscores.count++;
        }
        
    }
    
    fclose(arquivoHighscores);
    return highscores;
}

void salvarFinalDePartida(Session *game){
    FILE *partidasFile = fopen("./data/partidas.txt", "a");
    if (partidasFile == NULL) return;

    //Pegar data e hora
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    
    fprintf(partidasFile, "%d;%d;%.0f;%s;%d;%d;", game->mode, game->difficulty, game->score, timestamp, game->target, game->guessCount);
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


Stats coletarEstatisticas(const char *path) {
    

    Stats stats = {0};
    DadosPartida atual = {0};

    FILE *f = fopen(path, "r");

    if (!f) {
        stats.error = 1;
        return stats;
    }

    int count = 0;

    int tentativas[256];

    int matched = 0;
    while (matched != EOF) {

        int modeBuff;
        char timestampBuff[20];

        // campos fixos
        matched = fscanf(f, "%d;%d;%d;%19[^;];%d;%d;",
            &modeBuff,
            &atual.dificuldade,
            &atual.score,
            timestampBuff,
            &atual.target,
            &atual.numTentativas);

        if (matched != 6) break; // algo deu errado ou final do arquivo

        for (int i = 0; i < atual.numTentativas ; i++) {
            fscanf(f, "%d%*c", &atual.historico[i]);
        }

        if (count == 0) {
            stats.melhor = atual;
            stats.pior = atual;
        } else {
            if ((atual.score == stats.melhor.score && atual.numTentativas < stats.melhor.numTentativas ) || (atual.score > stats.melhor.score)){
                stats.melhor = atual;
            }

            if ((atual.score == stats.pior.score && atual.numTentativas > stats.pior.numTentativas ) || (atual.score < stats.pior.score)){
                stats.pior = atual;
            }
        }

        tentativas[count] = atual.numTentativas;
        count++;

    }

    fclose(f);

    int totalTentativas = somaMedRec(tentativas, count);
    stats.media = totalTentativas / (float) count;
    
    double somaDesv = somaDesvRec(tentativas, count, stats.media);
    double desvioPadrao = sqrt(somaDesv / count);

    stats.desvio = desvioPadrao;
    stats.numPartidas = count;
    return stats;
}

char* buscarCuriosidade(int target){
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

void configurarCuriosidade(Session *game){
    char* curiosidade = buscarCuriosidade(game->target);
    strcpy(game->trivia, curiosidade);
}
