#include <stdlib.h>
#include <stdio.h>
#include <logic.h>

// Estrutura de espelhamento para leitura de partidas no arquivo de partidas.
typedef struct{
    // Mode modo;
    int dificuldade;
    int target;                       // O número secreto que foi adivinhado naquela partida.
    int numTentativas;                // Numero total de tentativas na rodada.
    int historico[16];                // Histórico com os palpites realizados na partida.
    int score;                        // Pontuação final obtida na partida.
} DadosPartida;

typedef struct {
  float media;
  float desvio;
  DadosPartida pior;
  DadosPartida melhor;
  int numPartidas;
  int error;    // 0 = ok, 1 = something went wrong
} Stats;

Stats coletarEstatisticas(const char *path) {
    

    Stats stats = {0};
    DadosPartida atual = {0};

    FILE *f = fopen(path, "r");

    if (!f) {
        stats.error = 1;
        return stats;
    }

    int count = 0; int guessAccumulator = 0;

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

        // captura o historico da patida
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

        count++;
        guessAccumulator += atual.numTentativas;

    }

    fclose(f);

    stats.media = (float) guessAccumulator / (float) count;
    stats.numPartidas = count;
    return stats;
}

int main(void){
    Stats estatisticas;
    estatisticas = coletarEstatisticas("./data/partidas.txt");
    
    printf("MEDIA (tentativas/partida): %f\n", estatisticas.media);   
    printf("TOTAL de partidas:          %d\n\n", estatisticas.numPartidas);   
    printf("score da MELHOR partida: %d\nnum de tentativas: %d\n\n", estatisticas.melhor.score, estatisticas.melhor.numTentativas);
    printf("score da PIOR partida: %d\nnum de tentativas: %d\n\n", estatisticas.pior.score, estatisticas.pior.numTentativas);
    return 0;
}