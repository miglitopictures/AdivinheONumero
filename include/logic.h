#ifndef LOGIC_H
#define LOGIC_H

#include <gamestate.h>

// The "Engine" functions
void IniciarJogo(GameState *state);
void ProcessarTentativa(GameState *state, int guess);


void resetarRandomSeed();

#endif