#ifndef TIPOS_H
#define TIPOS_H

typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER, STATE_EXIT } State;

typedef enum {MODO_NORMAL, MODO_ARCADE} Mode;

typedef enum {EASY, MEDIUM, HARD} Dificulty;

typedef enum {DECIMAL, BINARY, HEXADECIMAL} NumericalSystem;

// Session
typedef struct {
    State state;
    Mode mode;
    Dificulty dificulty;
    NumericalSystem numericalSystem;

    int max; // 100
    int target; // numero secreto

    int guess;

    int guessCount; // contador de tentativas
    int guessHistory[64];
    
    
    char message[64]; // "Muito Alto", "Muito Baixo", etc.
    char temperature[32]; // "Quente", "Morno", "Frio"

    char trivia[256]; // curiosidade sobre o numero sorteado

    int score;
} Session;

// Ui Digits
typedef struct {
    char text[16];
    char offsets[15];
    int count;
} DigitInput;

#endif