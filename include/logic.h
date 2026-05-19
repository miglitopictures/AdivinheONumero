#ifndef LOGIC_H
#define LOGIC_H

#define MAX_HIGHSCORES 10   // Quantidade máxima de registros mantidos no placar de recordes.

//__tipos (enums e structs)__________________________________________________________________________________________________________

//__ENUMS___

// Estado geral do fluxo de execução do jogo.
typedef enum {
    STATE_MENU,     // O jogo está na tela de menus (MAIN, MODES, STATS, etc.).
    STATE_PLAYING,  // Rodada em execução ativa pelo jogador.
    STATE_GAMEOVER, // Fim de jogo por esgotamento de score/pontos.
    STATE_WIN,      // Vitória na rodada (jogador acertou o número secreto).
    STATE_EXIT      // Sinalização para encerrar o loop e fechar a janela do jogo.
} State;

// Modo de jogo selecionado.
typedef enum {
    MODO_NORMAL,    // Modo normal: partida tradicional de adivinhação simples com escolha de dificuldade.
    MODO_ARCADE     // Modo arcade: infinito por rodadas acumulativas com bônus de tempo/acerto.
} Mode;

// Nível de dificuldade (aplicado principalmente ao MODO_NORMAL).
typedef enum {
    EASY,           // Fácil.
    MEDIUM,         // Médio.
    HARD            // Difícil.
} Difficulty;

// Sistema numérico.
typedef enum {
    DECIMAL,        // Base 10.
    BINARY,         // Base 2.
    HEXADECIMAL,    // Base 16.
    OCTAL           // Base 8.
} NumericalSystem;

// Proximidade térmica do último palpite em relação ao número secreto.
typedef enum {
    COLD,           // Palpite distante do alvo.
    WARM,           // Palpite se aproximando do alvo.
    HOT             // Palpite muito próximo do alvo.
} Temperature;

//__STRUCTS___

// Estrutura principal (Session *game) que armazena todo o estado e progresso da sessão atual do jogo.
typedef struct {
    State state;                      // Estado atual do ciclo de vida do jogo.
    Mode mode;                        // Modo de jogo ativo (Normal ou Arcade).
    Difficulty difficulty;            // Dificuldade selecionada para a sessão.
    NumericalSystem numericalSystem;  // Sistema numérico ativo.

    int round;                        // Contador de rodadas (relevante para o MODO_ARCADE).    
    int roundBonus;                   // Pontuação extra concedida ao avançar de rodada.
    int totalGuesses;                 // Acumulador total de palpites enviados durante toda a sessão.

    int max;                          // Limite superior do intervalo de sorteio (ex: 100).
    int target;                       // O número secreto gerado pelo RNG que deve ser adivinhado.

    int guess;                        // Valor numérico do palpite atual.

    int guessCount;                   // Contador de tentativas feitas na rodada atual.
    int guessHistory[10];             // Histórico com os últimos palpites realizados na rodada.
    
    char player[32];                  // Nome ou iniciais do jogador (ex: "AAA").
    Temperature temperature;          // Temperatura atual do parlpite atual.

    char trivia[256];                 // Buffer de caracteres para curiosidade sobre o numero sorteado.

    int score;                        // Pontuação atual do jogador (reduz com o tempo/erros, causa GameOver se chegar a 0).
} Session;

// Estrutura de espelhamento para leitura e escrita de recordes no arquivo de highscores.
struct DadosPartida {
    char nome[50];                    // Nome do jogador associado ao recorde.
    int score;                        // Pontuação final obtida na partida.
    int target;                       // O número secreto que foi adivinhado naquela partida.
};

//__funções________________________________________________________________________________________________________________________

// ---     logic.c     --- //
void iniciarJogo(Session *game);                          // Inicializa e redefine as variáveis da struct Session para os valores padrão.
void processarTentativa(Session *game, int guess);        // Computa um palpite do usuário, avalia proximidade, atualiza histórico, deduz score e checa vitória.
void processarGameover(Session *game);                    // Verifica se os pontos (score) se esgotaram e altera o estado do jogo para STATE_GAMEOVER.

// ---     score.c     --- //
void processarTemperatura(Session *game);                 // Calcula a distância do palpite para o alvo e define o estado como COLD, WARM ou HOT.
void atualizarTempoRealScore(Session *game, double dt);   // Reduz progressivamente o score em tempo real com base no tempo decorrido por frame.
int calcularPalpiteScore(Session *game);                  // Retorna a penalidade de pontos a ser deduzida do score com base na gravidade do erro.

// ---   gamedata.c   --- //
void salvarFinalDePartida(Session *game);                 // Grava os dados detalhados e o histórico de palpites da rodada finalizada em "partidas.txt".
// highscores
void atualizarHighscore(Session *game);                   // Insere a pontuação atual no arquivo "highscores.txt", ordenando o ranking dos maiores para os menores.
int checarHighscore(Session *game);                       // Avalia se o score atual da sessão é alto o suficiente para entrar no top de recordes.
// curosidades
char* buscarCuriosidade(int target);               // Retorna a curiosidade atrelada ao target. Procura e lê no arquivo "curiosidades.txt".
void configurarCuriosidade(Session *game);                // Atualiza o buffer `game->trivia` buscando o texto correspondente ao número secreto atual.

// ---      rng.c     --- //
void resetarRandomSeed();                                 // Alimenta o seed (semente) do gerador com o tempo atual do sistema (srand) para garantir aleatoriedade.
int numeroAleatorio(int min, int max);                    // Retorna um valor inteiro pseudo-aleatório distribuído uniformemente dentro do intervalo [min, max].

#endif