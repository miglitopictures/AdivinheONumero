#include <string.h>
#include <ui.h>
#include <logic.h>

// argc - argument count
// argv - argument vector
// por exemplo, se eu rodar o jogo com "./game -term", temos dois argumentos "game" e "-term"
int main(int argc, char *argv[]){

    resetarRandomSeed();
    
    if (argc > 1 && strcmp(argv[1], "-term") == 0){
        startTerminalMode();
    } else {
        startRaylibMode();
    }

    return 1;
}