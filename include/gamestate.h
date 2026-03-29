#ifndef GAMESTATE_H
#define GAMESTATE_H

#define MAX_HISTORICO 5


typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER} State;

typedef struct {
    State state;
    int max;
    int numeroSecreto;

    int palpite;

    int limiteTentativas;
    int tentativas;
    int historicoTentativas[MAX_HISTORICO];
    
    
    char mensagem[64]; // "Muito Alto", "Muito Baixo", etc.
    char temperatura[32]; // "Quente", "Morno", "Frio"
} GameState;

#endif