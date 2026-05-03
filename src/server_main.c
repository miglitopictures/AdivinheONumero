// Este arquivo será usado apenas quando for rodar o backend de forma 100% independente, sem  usar a Raylib.
// usa a função padrão do C11 (timespec_get), que funciona em Windows e Linux.

#include <stdio.h>
#include <time.h> // biblioteca de tempo padrão do C
#include "logic.h"
#include "tipos.h"

// Função portátil (C11) para obter tempo em segundos em qualquer sistema operacional
double obterTempoC11() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

int main(void) {
    resetarRandomSeed();
    
    Session game;
    IniciarJogo(&game);
    printf("Servidor do Jogo Iniciado. Numero secreto gerado!\n");

    // Simulando configurações
    double dificuldadePontos = 1.0; // perde 1 ponto a cada 1 segundo
    double tempoAnterior = obterTempoC11();

    // O "Game Loop" do Servidor (Roda sem parar no terminal)
    while (game.state == STATE_PLAYING) {
        // 1. Calcula o Delta Time no servidor
        double tempoAtual = obterTempoC11();
        double dt = tempoAtual - tempoAnterior;
        tempoAnterior = tempoAtual;

        // 2. Alimenta a lógica do jogo
        atualizarTempoRealScore(&game, dt, dificuldadePontos);

        // 3. Imprime no terminal apenas para monitoramento (opcional)
        // Isso evita que o terminal spamme mensagens freneticamente, 
        // mas na prática o servidor rodará esse loop o mais rápido possível.
        static double tempoPrint = 0;
        tempoPrint += dt;
        if (tempoPrint >= 1.0) { // Imprime o log a cada 1 segundo
            printf("Score atual: %.0f\n", game.score); // O %.0f arredonda visualmente para inteiro
            tempoPrint = 0;
        }

        // Se o score zerar, a própria função atualizarTempoRealScore 
        // mudará o estado para STATE_GAMEOVER e o loop se encerrará.
    }

    printf("Fim da partida no servidor! Score final: %.1f\n", game.score);
    return 0;
}