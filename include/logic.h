#ifndef LOGIC_H
#define LOGIC_H

#define MAX_HIGHSCORES 10

typedef enum {STATE_MENU, STATE_PLAYING, STATE_GAMEOVER, STATE_WIN, STATE_EXIT } State;

typedef enum {MODO_NORMAL, MODO_ARCADE} Mode;

typedef enum {EASY, MEDIUM, HARD} Difficulty;

typedef enum {DECIMAL, BINARY, HEXADECIMAL, OCTAL} NumericalSystem;

typedef enum {COLD, WARM, HOT} Temperature;
// Session
typedef struct {
    State state;
    Mode mode;
    Difficulty difficulty;
    NumericalSystem numericalSystem;

    int round;
    int roundBonus;
    int totalGuesses;

    int max; // 100
    int target; // numero secreto

    int guess;

    int guessCount; // contador de tentativas
    int guessHistory[10];
    
    char player[32];
    char message[64]; // "Muito Alto", "Muito Baixo", etc.
    Temperature temperature;

    char trivia[256]; // curiosidade sobre o numero sorteado

    int score;
} Session;

struct DadosPartida {
    char nome[50];
    int score;
    int target;
};

// ---     logic.c     --- //
void IniciarJogo(Session *game);
void ProcessarTentativa(Session *game, int guess);
void ProcessarGameover(Session *game);

// ---     score.c     --- //
void ProcessarTemperatura(Session *game);
void atualizarTempoRealScore(Session *game, double dt);
int calcularPalpiteScore(Session *game);

// ---   gamedata.c   --- //
void salvarFinalDePartida(Session *game);
// highscores
void atualizarHighscore(Session *game);
int checarHighscore(Session *game);
// curosidades
char* buscarCuriosidadeArquivo(int target);
void configurarCuriosidade(Session *game);

// ---      rng.c     --- //
void resetarRandomSeed();
int numeroAleatorio(int min, int max); 


#endif