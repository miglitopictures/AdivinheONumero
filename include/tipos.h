#ifndef TIPOS_H
#define TIPOS_H

typedef enum {STATE_MENU, STATE_PLAYING, STATE_GAMEOVER, STATE_EXIT } State;

typedef enum {MODO_NORMAL, MODO_ARCADE} Mode;

typedef enum {EASY, MEDIUM, HARD} Difficulty;

typedef enum {DECIMAL, BINARY, HEXADECIMAL, OCTAL} NumericalSystem;

// Session
typedef struct {
    State state;
    Mode mode;
    Difficulty difficulty;
    NumericalSystem numericalSystem;

    int max; // 100
    int target; // numero secreto

    int guess;

    int guessCount; // contador de tentativas
    int guessHistory[10];
    
    char player[32];
    char message[64]; // "Muito Alto", "Muito Baixo", etc.
    char temperature[32]; // "Quente", "Morno", "Frio"

    char trivia[256]; // curiosidade sobre o numero sorteado

    int score;
} Session;


#endif