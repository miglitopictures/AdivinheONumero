#include <stdlib.h>
#include <time.h>
#include <logic.h>

void resetarRandomSeed(){
    srand(time(NULL));
}

int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}