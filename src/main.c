#include <string.h>
#include <ui.h>
#include <logic.h>
#include <tipos.h>


int main(void){

    resetarRandomSeed(); // chamamos essa função apenas uma vez, para mudar o valor do seed

    Session game; // CRIAR NOSSO GAMESTATE
    
    startRaylibMode(&game);

    return 1;
}