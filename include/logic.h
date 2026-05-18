#ifndef LOGIC_H
#define LOGIC_H

#include <tipos.h>

void IniciarJogo(Session *game);
void ProcessarTentativa(Session *game, int guess);
void atualizarTempoRealScore(Session *game, double dt);
void ProcessarGameover(Session *game);

void resetarRandomSeed(); // usado no inicio de main

#endif