#include <math.h>
#include <stdio.h>

// ___math utils_______________________________________________________________________________________

float flerp(float start, float end, float amount){
    return start + (end - start) * amount;
}

float fmap(float value, float fromStart, float fromEnd, float toStart, float toEnd){
    float t = (value - fromStart) / (fromEnd - fromStart);
    return flerp(toStart, toEnd, t);
}

int ilerp(int start, int end, float amount){
    return start + (int) ((end - start) * amount);
}

int imap(int value, int fromStart, int fromEnd, int toStart, int toEnd){
    float t = (float)(value - fromStart) / (float)(fromEnd - fromStart);
    return ilerp(toStart, toEnd, t);
}

int somaMedRec(int tentativas[], int total) {
    if (total <= 0) {
        return 0;
    }
    return tentativas[total - 1] + somaMedRec(tentativas, total - 1);
}

double somaDesvRec(int tentativas[], int total, float media) {
    if (total <= 0) {
        return 0;
    }
    return pow((double) tentativas[total - 1] - media, 2) + somaDesvRec(tentativas, total - 1, media);
}

