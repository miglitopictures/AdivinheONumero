#include <stdio.h>
#include <math.h>


int somaMedRec(int tentativas[], int total) {
    if (total <= 0) {
        return 0;
    }
    return tentativas[total - 1] + somaMedRec(tentativas, total - 1);
}

double somaDesvRec(int tentativas[], int total, double media) {
    if (total <= 0) {
        return 0;
    }
    return pow((double) tentativas[total - 1] - media, 2) + somaDesvRec(tentativas, total - 1, media);
}



int main() {
    int testArray[] = {0, 1, 2, 3, 4, 5};
    int numeros = 6;
    
    int total = somaMedRec(testArray,numeros);

    double somaDesv = somaDesvRec(testArray, numeros, total/(float)numeros);
    printf("%f\n", somaDesv);
    double desvio = sqrt(somaDesv/numeros);
    printf("%f\n", desvio);


    printf("%f\n", total/(float)numeros);
}