// constantes e estruturas compartilhadas.

#ifndef TIPOS_H
#define TIPOS_H


#define MAX_GUESSES 5

typedef struct {
  char timestamp[32];
  int target;
  int attempts_count;
  int guesses[MAX_GUESSES];
  int low_count;
  int high_count;
} Session;

typedef struct {
  int total_sessions;
  double average_attempts;
  int best_attempts;
  int worst_attempts;
  double stddev_attempts;
  double avg_low_bias;
  double avg_high_bias;
} Stats;

#endif
