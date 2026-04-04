CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic src/*.c

all: programa

programa:
	$(CC) $(CFLAGS) -lm -o jogo_adivinhacao 

clean:
	rm -f jogo_adivinhacao
