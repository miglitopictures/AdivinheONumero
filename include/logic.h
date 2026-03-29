#ifndef LOGIC_H
#define LOGIC_H

typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER} State;

typedef struct {
    int max;
    int numeroSecreto;
    int tentativas;
    State state;
    char mensagem[50]; // "Muito Alto", "Muito Baixo", etc.
    char temperatura[50]; //
} GameState;

// The "Engine" functions
void IniciarJogo(GameState *state);
void ProcessarTentativa(GameState *state, int guess);


void resetarRandomSeed();

#endif