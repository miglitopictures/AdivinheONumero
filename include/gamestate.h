#ifndef GAMESTATE_H
#define GAMESTATE_H

#define MAX_HISTORICO 5


typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER, STATE_EXIT } State;
typedef enum {MODO_NORMAL, MODO_ARCADE} Modo;

typedef struct {
    State state;
    Modo modo;
    int max;
    int numeroSecreto;

    int palpite;

    int limiteTentativas;
    int tentativas; // contador de tentativas
    int historicoTentativas[MAX_HISTORICO]; // 5
    
    
    char mensagem[64]; // "Muito Alto", "Muito Baixo", etc.
    char temperatura[32]; // "Quente", "Morno", "Frio"
} GameState;

#endif