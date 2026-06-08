#ifndef UI_H
#define UI_H
#include <logic.h>
#include <raylib.h>

#define DEBUGFONT 20    // Tamanho da fonte de debug.

//__tipos (enums e structs)__________________________________________________________________________________________________________

//__ENUMS___

// Estado atual do menu de navegação.
enum MenuState {
    LOGO,       // Tela de logo/splash inicial.
    MAIN,       // Menu principal com opções de jogar, analisar e sair.
    MODES,      // Tela de seleção de modo e dificuldade.
    STATS,       // Tela de estatísticas e histórico de partidas.
    RANKING
};

// Estado visual de um Button.
enum ButtonState {
    BT_IDLE,    // Botão em repouso, sem interação.
    BT_HOVER,   // Mouse sobre o botão.
    BT_PRESSED  // Botão sendo pressionado.
};

// Estado de uma CircleMark no Ruler.
enum CircleMarkState {
    CM_FREE,    // Marca ativa, pode ser movida pelo jogador.
    CM_LOCKED,  // Marca confirmada, posição travada no Ruler.
    CM_WAIT     // Marca aguardando animação ou evento.
};

//__STRUCTS___

// Input numérico animado digito-por-digito.
typedef struct {
    char  text[16];        // Buffer para string do input + '/0'.
    int   count;           // Quantidade de digitos no DigitInput.
    int   isClearing;      // Indica se atualmente estamos limpando o DigitInput. (0 = False, 1 = True).
    float currentY[15];    // Offsets atuais na posição y para cada dígito.
    float targetY[15];     // Offsets desejados na posição y para cada dígito.
} DigitInput;

// Botão clicável com label e estado visual.
typedef struct{
    Rectangle rect;         // Área de colisão e desenho do botão.
    char label[16];         // Texto exibido no centro do botão.
    enum ButtonState state; // Estado visual atual do botão.
} Button;

// Uma opção selecionável dentro de um OptionPicker.
typedef struct {
    char label[16];         // Texto exibido para essa opção.
    int  value;             // Valor inteiro associado a essa opção (ex: enum de modo ou dificuldade).
} Option;

// Seletor de opções com navegacao por botoes/setas < >.
typedef struct {
    char label[16];         // Label exibida acima do picker.
    Option options[4];      // Array de opções.
    int count;              // Quantidade de opções no array.
    int current;            // Índice da opção atualmente selecionada.

    int hoverLeft;          // 1 se o mouse está sobre a setinha esquerda.
    int hoverRight;         // 1 se o mouse está sobre a setinha direita.

    int isActive;           // 1 se o picker estiver ativado. 0 = desativado e visualmente esmaecido.
} OptionPicker;

// Régua horizontal com divisões uniformes na base da tela.
typedef struct{
    Rectangle rect;         // Área total da régua (posição e tamanho).
    int divisions;          // Número de divisões (pontos) na régua.
    float margin;           // Margem interna nas extremidades da régua.
} Ruler;

// Marcador circular posicionado no Ruler.
typedef struct{
    enum CircleMarkState state; // Estado atual da marca.
    Player owner;
    float currentX;             // Posição X atual (animada por flerp).
    float targetX;              // Posição X alvo (definida por input ou mouse).
    float y;                    // Posição Y da marca (calculada a partir do Ruler).
    float raio;                 // Raio do círculo desenhado.
} CircleMark;

// Seta de feedback direcional exibida após uma tentativa.
typedef struct{
    int dir;                    // Direção da seta: 1 = direita (maior), 0 = esquerda (menor).
    int shoudDraw;              // 1 se a seta deve ser desenhada neste frame.
    Vector2 pos;                // Posição atual da seta (animada com fletp).
    Vector2 target;             // Posição alvo da seta.
} FeedbackArrow;

typedef struct {
    int isActive;
    int maxInput;
    int lettercount;
    int framecounter;
    char text[32];
    Rectangle textBox;
} TextInput;

//__globals variales_______________________________________________________________________________________________________________

// sounds
extern Sound sfxChangeMark, sfxSelectSynth, sfxWin, sfxLose;
// debug info toggler
extern int debugMode;
// janela aplicacao
extern int LARGURA, ALTURA;
// paleta de cores da Pablo Software
extern Color PS_BLACK, PS_DARKGREY, PS_GREY, PS_WHITE, PS_BLUE, PS_RED, PS_GREEN, PS_DEBUG;
// tipografia do jogo
extern Font font;

extern float startingScore; // usado com drawScorebar()

extern Ruler basicRuler;
extern CircleMark circlemarks[128];
extern int activeMarkIndex;
extern DigitInput input;
extern FeedbackArrow arrow;

// menu
extern enum MenuState menuState;
extern Button btnSingleplayer, btnMultiplayer, btnExit, btnStats, btnRanking;

extern Button btnBack;

extern OptionPicker modePicker, difficultyPicker;
extern Button btnStart;

extern Button btnPlayAgain; 

extern TextInput playerNameInput;

//__funções________________________________________________________________________________________________________________________

//__zgame_loop.c____
void startRaylibMode(Session *game);                                                 // Inicializa o sistema, roda o loop principal e encerra a janela ao sair.

//__math_utils.c____
int ilerp(int start, int end, float amount);                                         // Interpolação linear entre dois inteiros.
float flerp(float start, float end, float amount);                                   // Interpolação linear entre dois floats.
int imap(int value, int fromStart, int fromEnd, int toStart, int toEnd);             // Mapeia um inteiro de um intervalo para outro.
float fmap(float value, float fromStart, float fromEnd, float toStart, float toEnd); // Mapeia um float de um intervalo para outro.

//___number_input.c____
// DigitInput
void numberInputAdd(DigitInput *input, char key);                                    // Adiciona um novo digito no DigitInput.
void handleKeysNumberInput(DigitInput *input, int maxSize);                          // Recebe input do teclado e utiliza numbetInputAdd para atualizar o DigitInput.
void updateNumberInput(DigitInput *input, int maxSize);                              // Atualiza e anima o DigitInput.
void clearAnimNumberInput(DigitInput *input);                                        // Limpa o number input com animação.
void clearInstantNumberInput(DigitInput *input);                                     // Limpa o number input instantaneamente.
void drawAnimatedNumberInput(DigitInput input, Vector2 pos, Color color);            // Desenha o DigitInput.

//___components.c____
// Ruler
Ruler createRuler(int divisions, float bodyHeight);                                  // Retorna um Ruler com as configuracoes especificadas.
void drawRuler(Ruler ruler, Color bodyColor, Color divisionColor);                   // Desenha um Ruler com as cores especificadas.
// CircleMark
float getXFromRulerPoint(Ruler ruler, int point);                                    // Retorna a posição (float x) referente ao ponto especificado no o Ruler.
int getRulerPointFromX(Ruler ruler, float x);                                        // Retorna o ponto no Ruler referente a posição (float x) especificada.
void spawnActiveMark(int index, int rulerPoint);                                     // Spawna uma nova CircleMark ativa no ponto especificado do Ruler.
void updateCircleMarks(Session *game);                                               // Atualiza todas as CircleMarks, incluindo drag do mouse e input de teclado.
void lockActiveCircleMark(Player owner);                                                     // Trava a CircleMark ativa na posição final e muda seu estado para CM_LOCKED.
void drawCircleMarks(Session *game);                                                 // Desenha todas as CircleMarks travadas e a ativa, se existir.
// barra de score
void drawTopBar(float value, float max, Color bodyColor);                       // Desenha a barra de score a partir do score atual usando o fmap().
// FeedbackArrow
void drawArrow(FeedbackArrow arrow, int length, int weight);                         // Desenha o FeedbackArrow com a largura e grossura indicada.

//___ui_menu.c____
// Button
int updateButton(Button *btn, Vector2 mouse);                                        // Atualiza o estado do Button com base na posição do mouse. Retorna 1 se clicado.
void drawButton(Button *btn);                                                        // Desenha o Button refletindo seu estado atual (idle, hover, pressed).
// OptionPicker
void updatePicker(OptionPicker *picker, Vector2 mouse, int posX, int posY);          // Atualiza o OptionPicker via mouse ou teclado. Ignorado se isActive == 0.
void drawPicker(OptionPicker *picker, int posX, int posY);                           // Desenha o OptionPicker com chevrons, label, valor atual e dots indicadores.
// STATE_MENU
void initMenu(Session *game);                                                        // Inicializa todos os botões e pickers do menu com posições e valores padrão.
void updateMenu(Session *game);                                                      // Atualiza o estado do menu de acordo com o MenuState atual.
void drawMenu(Session *game);                                                        // Desenha o menu de acordo com o MenuState atual.

//___ui_playing.c____
// STATE_PLAYING
void updatePlaying(Session *game);                                                   // Atualiza o estado do jogo em execução: score, marcadores, input e feedback.
void drawPlaying(Session *game);                                                     // Desenha a tela de jogo: régua, marcadores, input animado, seta e debug.

//___ui_end.c____
// STATE_HIGHSCORE
void updateHighscoreScreen(Session *game);                                                       // Atualiza a tela de vitória. Reinicia o jogo se R ou btnPlayAgain for acionado.
void drawHighscoreScreen(Session *game);     
// STATE_WIN
void updateWin(Session *game);                                                       // Atualiza a tela de vitória. Reinicia o jogo se R ou btnPlayAgain for acionado.
void drawWin(Session *game);                                                         // Desenha a tela de vitória com o número acertado, trivia e estatísticas da partida.
// STATE_GAMEOVER
void updateGameover(Session *game);                                                  // Atualiza a tela de derrota. Reinicia o jogo se R ou btnPlayAgain for acionado.
void drawGameover(Session *game);                                                    // Desenha a tela de derrota.

void drawCoopPlacar(Session *game);

void drawHitState(Session *game, Vector2 pos, Color color);
void drawHit(Session *game);

#endif
