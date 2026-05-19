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

void IniciarJogo(Session *game);
void ProcessarTentativa(Session *game, int guess);
void ProcessarGameover(Session *game);

void resetarRandomSeed(); // usado no inicio de main

void ProcessarTemperatura(Session *game);
//=======================================================================================================================================================================
// Para atualizar o score em relação ao tempo
// Será chamada em todos os frames do jogo

void atualizarTempoRealScore(Session *game, double dt);
//========================================================================================================================================================================
// Para atualizar o score em relação ao palpite
int calcularPalpiteScore(Session *game);

void atualizarHighscore(Session *game);

int checarHighscore(Session *game);

// Salvar estado final da partida   Lucas e rodrigo????
void salvarFinalDePartida(Session *game);

// Buscar curiosidade a partir do valor acertado
char* buscarCuriosidade(int target);

char* buscarCuriosidadeArquivo(int target);

// mantém compatibilidade com o que já existia, mas adiciona busca real
void configurarCuriosidade(Session *game);

// Gerar numero aleatorio RNG
void resetarRandomSeed(); // gerando novo seed para randomizacao do numero

int numeroAleatorio(int min, int max);



#endif