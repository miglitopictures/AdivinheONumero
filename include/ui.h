#ifndef UI_H
#define UI_H
#include <logic.h>
#include <raylib.h>

#define DEBUGFONT 20

//__structs e enums________________________________________________________________

enum MenuState {LOGO, MAIN, MODES, STATS};

typedef struct {
    char text[16];
    float currentY[15];
    float targetY[15];
    int count;
} DigitInput;

enum ButtonState {BT_IDLE, BT_HOVER, BT_PRESSED};

typedef struct{
    Rectangle rect;
    char label[16];
    enum ButtonState state;
} Button;

#define MAX_PICKER_OPTIONS 4

typedef struct {
    char label[16];
    int value;
} Option;

typedef struct {
    char label[16];
    Option options[MAX_PICKER_OPTIONS];
    int count;
    int current;

    int hoverLeft;  
    int hoverRight;

    int isActive;
} OptionPicker;

typedef struct{
    Rectangle rect;
    int divisions;
    float margin;
} Ruler;

enum CircleMarkState{ CM_FREE, CM_LOCKED, CM_WAIT };

typedef struct{
    enum CircleMarkState state;
    float currentX;
    float targetX;
    float y;
    float raio;
} CircleMark;

typedef struct{
    int dir;
    int shoudDraw;
    Vector2 pos;
    Vector2 target;
} FeedbackArrow;

// ___globals_________________________________________________________________________________________

extern int activeMarkIndex;
extern int inputClearing;

//sounds
extern Sound sfxChangeMark, sfxSelectSynth, sfxWin, sfxLose;

// debug info toggler
extern int debugMode;

// janela aplicacao
extern int LARGURA, ALTURA;

// paleta de cores
extern Color PS_BLACK, PS_DARKGREY, PS_GREY, PS_WHITE, PS_BLUE, PS_RED, PS_GREEN, PS_DEBUG;

extern int startingScore;

extern Font font;
extern Ruler basicRuler;
extern CircleMark circlemarks[100];
extern DigitInput input;

extern FeedbackArrow arrow;

// ___state menu__________________________________________________________________________________________

extern Button btnPlay, btnExit, btnStats; // MAIN MENU
extern Button btnPlayAgain; // END MENU

extern OptionPicker modePicker, difficultyPicker;
extern Button btnStart;

extern enum MenuState menuState;


//__funções________________________________________________________________________________

void startRaylibMode(Session *game);

float flerp(float start, float end, float amount);

float fmap(float value, float fromStart, float fromEnd, float toStart, float toEnd);

int ilerp(int start, int end, float amount);

int imap(int value, int fromStart, int fromEnd, int toStart, int toEnd);

void numberInputAdd(DigitInput *input, char key);

void handleKeysNumberInput(DigitInput *input, int maxSize);

void updateNumberInput(DigitInput *input, int maxSize);

// limpa o number input com animação
void clearAnimNumberInput(DigitInput *input);

// limpa o number input instantaneamente
void clearInstantNumberInput(DigitInput *input);

void drawAnimatedNumberInput(DigitInput input, int posX, int posY, int fontSize, int spacing, Color color, Font font);
// ___score bar____________________________________________________________________________________________

void drawScoreBar(int currentScore, int max, Color bodyColor);

// ___ruler_______________________________________________________________________________________________

Ruler createRuler(int divisions, float bodyHeight);

void drawRuler(Ruler ruler, Color bodyColor, Color divisionColor);
// ___circle mark_________________________________________________________________________________________

// Retorna posicao no eixo x que corresponde ao numero digitado.
float getXFromRulerPoint(Ruler ruler, int point);

// Retorna numero digitado que corresponde ponto no eixo X.
int getRulerPointFromX(Ruler ruler, float x);

void spawnActiveMark(int index, int rulerPoint);

void updateCircleMarks(Session *game);

void lockActiveCircleMark(void);

void drawCircleMarks(Session *game);

// ___arrow______________________________________________________________________________________________

void drawArrow(FeedbackArrow arrow, int length, int weight);

// ___button______________________________________________________________________________________________

int updateButton(Button *btn, Vector2 mouse);

void drawButton(Button *btn);

// ___option picker_______________________________________________________________________________________

void updatePicker(OptionPicker *picker, Vector2 mouse, int posX, int posY);

void drawPicker(OptionPicker *picker, int posX, int posY);

void initMenu(Session *game);

void updateMenu(Session *game);

void drawMenu(Session *game);

// ___state playing______________________________________________________________________________________

void updatePlaying(Session *game);

void drawPlaying(Session *game);


// ___state win___________________________________________________________________________________________

void updateWin(Session *game);

void drawWin(Session *game);

// ___state gameover______________________________________________________________________________________

void updateGameover(Session *game);

void drawGameover(Session *game);


#endif