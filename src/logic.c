#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tipos.h>
#include <stdio.h>


// declaração da função
char* buscarCuriosidadeArquivo(int target);

// Gerar numero aleatorio RNG
void resetarRandomSeed(){
    srand(time(NULL)); // gerando novo seed para randomizacao do numero
}
int numeroAleatorio(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

// mantém compatibilidade com o que já existia, mas adiciona busca real
void configurarCuriosidade(Session *game){
    char* curiosidade = buscarCuriosidadeArquivo(game->target);

    strcpy(game->trivia, curiosidade);

    // só pra testar se está funcionando
    printf("DEBUG curiosidade: %s\n", game->trivia);
}

/**
 * @brief Inicializa o estado do jogo para uma nova rodada.
 * * @param game Ponteiro para a estrutura GameState que será inicializada.
 */
void IniciarJogo(Session *game) {
    game->mode = MODO_NORMAL;
    game->max = 100;
    game->target = numeroAleatorio(0, game->max);
	game->state = STATE_MENU;
	
    // configurarCuriosidade(game); // carregava curiosidade no início, movido para o momento do acerto

    game->round = 0;
    game->totalGuesses = 0;

    strcpy(game->player, "AAA");
    game->guessCount = 0; // contador de tentativas
    game->message[0] = '\0';
    game->temperature = COLD;
    game->score = 600;

}

/**
 * @brief Calcula a proximidade (Temperatura) entre o palpite e o número secreto.
 * * @param game Ponteiro para o GameState.
 */
void ProcessarTemperatura(Session *game) {
    // Pegamos a distancia entre o palpite do jogador e o numero secreto da rodada
    int distancia = abs(game->guess - game->target);

    // Mudamos a mensagem se temperatura a partir dessa "distância"
    if (distancia >= 15){
        game->temperature = COLD; 
        //game->temperatura = "Frio"
    } else if (distancia > 5 && distancia < 15){
        game->temperature = WARM; 
    } else {
        game->temperature = HOT;
    }
}

//=======================================================================================================================================================================
// Para atualizar o score em relação ao tempo
// Será chamada em todos os frames do jogo

void atualizarTempoRealScore(Session *game, double dt)    // Verifica  Estado
{ 
 
    if (game == NULL || game->state != STATE_PLAYING) { 
        return;
    }

    double pontosPorSegundo;  // Define quantos pontos são perdidos por segundo de jogo ativo
    switch (game->difficulty)
    {
    case EASY:
        pontosPorSegundo = 2;
        break;
    case MEDIUM:
        pontosPorSegundo = 10;
        break;
    case HARD:
        pontosPorSegundo = 30;
        break;   
    }

    // Aplica o decremento usando o dt (Delta Time) que foi injetado na função
    game->score -= pontosPorSegundo * dt;
        
}

//========================================================================================================================================================================
// Para atualizar o score em relação ao palpite
int calcularPalpiteScore(Session *game){
    if (game == NULL) return 0;
	
    int distancia = abs(game->guess - game->target);
	
	if (distancia == 0) { 
        return 0;   // Verifica primeiro se acertou exatamente (distância 0). Sem penalidade.
    } else if (distancia >= 15){
        return 30;  // Frio
    } else if (distancia > 5){
        return 15;  // Morno
	} else {
        return 5;   // Quente (inclui distância 1 a 5)
    }
}

struct DadosPartida {
    char nome[50];
    int score;
    int target;
};

#define MAX_HIGHSCORES 10

void atualizarHighscore(Session *game) {

    struct DadosPartida lista[MAX_HIGHSCORES + 1];

    int totalEntradas = 0;

    FILE *highscoreFile = fopen("./data/highscores.txt", "r");

    if (highscoreFile != NULL) {

        // Lê os highscores válidos do arquivo
        while (
            totalEntradas < MAX_HIGHSCORES &&
            fscanf(
                highscoreFile,
                "%49s %d %d",
                lista[totalEntradas].nome,
                &lista[totalEntradas].score,
                &lista[totalEntradas].target
            ) == 3 // Se colocar outra entrada, mudar o número.
        ) {

            totalEntradas++;
        }

        fclose(highscoreFile);
    }

    // Adiciona a partida atual na lista
    strcpy(lista[totalEntradas].nome, game->player);
    lista[totalEntradas].score = game->score;
    lista[totalEntradas].target = game->target;

    totalEntradas++;

    // Ordena os scores do maior para o menor
    for (int i = 0; i < totalEntradas - 1; i++) {

        for (int j = 0; j < totalEntradas - i - 1; j++) {

            if (lista[j].score < lista[j + 1].score) {

                struct DadosPartida temp = lista[j];

                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    FILE *arquivo = fopen("./data/highscores.txt", "w");

    if (arquivo == NULL) {

        printf("Erro ao salvar ranking!\n");
        return;
    }

    int limite = totalEntradas;

    if (limite > MAX_HIGHSCORES) {
        limite = MAX_HIGHSCORES;
    }

    // Salva apenas o TOP 10
    for (int i = 0; i < limite; i++) {

        fprintf(
            arquivo,
            "%s %d %d\n",
            lista[i].nome,
            lista[i].score,
            lista[i].target
        );
    }

    fclose(arquivo);
}

int checarHighscore(Session *game) {

    struct DadosPartida lista[MAX_HIGHSCORES];

    int totalEntradas = 0;

    FILE *highscoreFile = fopen("./data/highscores.txt", "r");

    if (highscoreFile != NULL) {

        // Lê os highscores válidos do arquivo
        while (
            totalEntradas < MAX_HIGHSCORES &&
            fscanf(
                highscoreFile,
                "%49s %d %d",
                lista[totalEntradas].nome,
                &lista[totalEntradas].score,
                &lista[totalEntradas].target
            ) == 3
        ) {

            totalEntradas++;
        }

        fclose(highscoreFile);
    }

    // Se houver menos de 10 scores,
    // o jogador entra automaticamente
    if (totalEntradas < MAX_HIGHSCORES) {
        return 1;
    }

    // Verifica se o score atual supera algum score existente
    for (int i = 0; i < totalEntradas; i++) {

        if (game->score > lista[i].score) {
            return 1;
        }
    }

    return 0;
}

// Salvar estado final da partida   Lucas e rodrigo????
void salvarFinalDePartida(Session *game){
    FILE *partidasFile = fopen("./data/partidas.txt", "a");
    if (partidasFile == NULL) return;

    //Pegar data e hora
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    
    fprintf(partidasFile, "%d;%d;%s;%d;%d;", game->mode, game->score, timestamp, game->target, game->guessCount);
    for (int i = 0; i < game->guessCount; i++) {
        if (i == game->guessCount - 1) {
            fprintf(partidasFile, "%d", game->guessHistory[i]);
        } else {
            fprintf(partidasFile, "%d,", game->guessHistory[i]);
        }
        
    }
    fprintf(partidasFile, ";\n");

    fclose(partidasFile);
}

// Buscar curiosidade a partir do valor acertado
char* buscarCuriosidade(int target){
    static char curiosidade[128] = "Curiosidade";
    return curiosidade;
}

void avancarRodadaArcade(Session *game){
    int bonus = 200; // devemos calcular dinamicamente

    game->target = numeroAleatorio(0, game->max);
    game->round++;
    game->totalGuesses += game->guessCount;
    game->guessCount = 0;
    game->score+=bonus; // bonus por acertar
    
}

void ProcessarGameover(Session *game){
    if (game->score <= 0) { // Impede que o score fique negativo devido aos palpites.
        game->score = 0;
        game->state = STATE_GAMEOVER; // Opcional: declarar Game Over se zerar os pontos.
    }
}

//=====================================================================================================================================================================
// Essa função, no momento, executa todos os passos necessários
// para atualizar estado do jogo (GameState) a partir do novo palpite (int) do usuário.
// * provavelmente separar em mais funcoes
void ProcessarTentativa(Session *game, int palpite) {
	if (game->state != STATE_PLAYING) return;
	
    game->guessCount++; // incrementa tentativas
    game->guess = palpite; // atribui o palpite do user ao GameState game.
 
    ProcessarTemperatura(game); // Atualizar a temperatura com base na distância.

    game->guessHistory[game->guessCount - 1] = game->guess; // salvar palpite no historico de palpites dessa rodada.

    game->score -= calcularPalpiteScore(game); // Deduzir os pontos dinamicamente baseado na temperatura calculada em ProcessarTemperatura(game).
	
	ProcessarGameover(game);
	
	
	// Verificar condições de vitória ou dicas
	
    if (palpite == game->target) { // acertou?
        if (game->mode == MODO_ARCADE){
            avancarRodadaArcade(game);
        } else{
            printf("Acertou");
            configurarCuriosidade(game); // <-- adicionei aqui pra atualizar a curiosidade no momento do acerto
            strcpy(game->message, "Voce acertou!");
            salvarFinalDePartida(game);
            atualizarHighscore(game); 
            
            game->state = STATE_WIN;
        }
        
    }

        else if (palpite < game->target) {
        strcpy(game->message, "Sonhe mais alto!"); 
     } else {
        strcpy(game->message, "Abaixe essa bola!");
    }
}

// implementação de busca de curiosidade baseada no arquivo
// não mexi na função original pra não correr o risco de quebrar algo

char* buscarCuriosidadeArquivo(int target){
    static char resultado[256];

    FILE *file = fopen("./data/curiosidades.txt", "r");

    if (!file) {
        strcpy(resultado, "Erro ao carregar curiosidades");
        return resultado;
    }

    char linha[512];

    while (fgets(linha, sizeof(linha), file)) {

        int num;
        char tema[50];
        char texto[300];
        int exibida;

        sscanf(linha, "%d|%[^|]|%[^|]|%d", &num, tema, texto, &exibida);

        if (num == target) {
            fclose(file);

            strcpy(resultado, texto);
            return resultado;
        }
    }

    fclose(file);

    strcpy(resultado, "Nenhuma curiosidade encontrada.");
    return resultado;
}