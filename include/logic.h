#ifndef LOGIC_H
#define LOGIC_H

#include <tipos.h>

void IniciarJogo(Session *state);
void ProcessarTentativa(Session *state, int guess);
void atualizarTempoRealScore(Session *game, double dt);

void resetarRandomSeed(); // usado no inicio de main

#endif