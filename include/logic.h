#ifndef LOGIC_H
#define LOGIC_H

#include <gamestate.h>

void IniciarJogo(GameState *state);
void ProcessarTentativa(GameState *state, int guess);


void resetarRandomSeed(); // usado no inicio de main

#endif