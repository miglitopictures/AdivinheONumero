#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <logic.h>

// Gerar numero aleatorio RNG
void resetarRandomSeed(){
    srand(time(NULL)); // gerando novo seed para randomizacao do numero
}

int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}