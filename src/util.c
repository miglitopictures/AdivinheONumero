// leitura segura de inteiros, timestamps e utilitarios
#include <stdlib.h>
#include <time.h>

/* Gera um int aleatório dentro de um limite especificado pelo valor máximo.*/
int randomInt(int max) {
    return rand() % (max + 1);
}

/* Gera um int aleatório dentro de um alcançe especificado por mínimo e máximo.*/
int randomRangeInt(int min, int max){
    return randomInt(max - min) + min;
}
