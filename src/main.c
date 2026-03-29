#include <string.h>
#include <ui.h>
#include <logic.h>
#include <gamestate.h>

// argc - argument count
// argv - argument vector
// por exemplo, se eu rodar o jogo com "./game -term", temos dois argumentos "game" e "-term"
int main(int argc, char *argv[]){

    resetarRandomSeed(); // chamamos essa função apenas uma vez, para mudar o valor do seed

    GameState game;
    
    if (argc > 1 && strcmp(argv[1], "-term") == 0){
        startTerminalMode(&game);
    } else {
        startRaylibMode(&game);
    }

    return 1;
}