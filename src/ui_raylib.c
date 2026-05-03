#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <tipos.h>
#include <stdio.h>

#define DEBUGFONT 20


// ___tipos_________________________________________________________________________________________

typedef struct {
    char text[16];
    float currentY[15];
    float targetY[15];
    int count;
} DigitInput;

enum MenuState {LOGO, MAIN, MODES, STATS};
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
} OptionPicker;

typedef struct{
    Rectangle rect;
    int divisions;
    float margin;
} Ruler;

typedef struct{
    Vector2 centro;
    float raio;
} CircleMark;



// ___globals_________________________________________________________________________________________

// janela aplicacao
int LARGURA = 800;
int ALTURA = 450;

// paleta de cores
Color PS_BLACK = BLACK;
Color PS_DARKGREY = {190, 190, 190, 255};
Color PS_GREY = {205, 205, 205, 255};
Color PS_WHITE = WHITE;
Color PS_BLUE = {55, 60, 218, 255};
Color PS_RED = {218, 55, 55, 255};
Color PS_GREEN = {28, 121, 0, 255};
Color PS_DEBUG = PURPLE;

int startingScore;

Font font;
Ruler basicRuler;
CircleMark circlemark;
DigitInput input = {0};

// ___math utils_______________________________________________________________________________________

float flerp(float start, float end, float amount){
    return start + (end - start) * amount;
}

float fmap(float value, float fromStart, float fromEnd, float toStart, float toEnd){
    float t = (value - fromStart) / (fromEnd - fromStart);
    return flerp(toStart, toEnd, t);
}

int ilerp(int start, int end, float amount){
    return start + (int) ((end - start) * amount);
}

int imap(int value, int fromStart, int fromEnd, int toStart, int toEnd){
    float t = (float)(value - fromStart) / (float)(fromEnd - fromStart);
    return ilerp(toStart, toEnd, t);
}


// ___number input____________________________________________________________________________________

void numberInputAdd(DigitInput *input, char key){
    input->text[input->count] = key;
    input->text[input->count + 1] = '\0';
    input->targetY[input->count] = 0;
    input->currentY[input->count] = 30; // starts offscreen, animates in
    input->count++;
}

void handleKeysNumberInput(DigitInput *input, int maxSize){
    int key = GetCharPressed();
    while (key > 0) {
        // NOTE: para numeros seria (48 até 57)
        if ((key >= 48) && (key <= 57) && input->count < maxSize) {
            numberInputAdd(input, (char)key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        input->targetY[input->count - 1] = -30;
    }

}

void updateNumberInput(DigitInput *input, int maxSize){
    handleKeysNumberInput(input, maxSize);
    
    for (int i = 0; i < input->count; i++) {
        
        input->currentY[i] = flerp(input->currentY[i], input->targetY[i], 0.3f);

        if (input->currentY[i] <= -20) {
            input->count--;
            if (input->count < 0) input->count = 0;
            input->text[i] = '\0';
        }
    }
    

    if (input->currentY[input->count - 1] <= -20) {
        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[input->count] = '\0';
    }
}

void clearNumberInput(DigitInput *input){
    for (int i = 0; i < input->count; i++){
        input->targetY[i] = -30;
    }
}


void drawAnimatedNumberInput(DigitInput input, int posX, int posY, int fontSize, int spacing, Color color, Font font){
    if (input.count <= 0) return; // Nao precisa desenhar nada

    Vector2 totalSize = MeasureTextEx(font, input.text, fontSize, spacing);

    float currentAdvance = 0.0f;

    for (int i = 0; i < input.count; i++){
        char buf[2] = { input.text[i], '\0' };

        DrawTextEx(font, buf,
                 (Vector2){(posX - totalSize.x / 2.0f) + currentAdvance,
                 posY + input.currentY[i] - totalSize.y / 2.0f},
                 fontSize, 0,  color);
                 
        Vector2 charSize = MeasureTextEx(font, buf, fontSize, 0);
        currentAdvance += charSize.x + spacing;
    }
}

// ___score bar____________________________________________________________________________________________

void drawScoreBar(int currentScore, int max, Color bodyColor){
    int c = imap(currentScore, 0, max, 0 , LARGURA);
    DrawRectangle(0,0,c, 15, PS_BLUE);
}


// ___ruler_______________________________________________________________________________________________

Ruler createRuler(int divisions, float bodyHeight){
    Rectangle rect = {0,ALTURA - bodyHeight, LARGURA, bodyHeight};
    Ruler ruler = {rect, divisions, 10};
    return ruler;
}

void drawRuler(Ruler ruler, Color bodyColor, Color divisionColor) {
    
    DrawRectangleRec(ruler.rect,bodyColor);
    int spaces = ruler.divisions - 1;
    float startX = ruler.rect.x + ruler.margin;
    float distBetween = (ruler.rect.width - (2 * ruler.margin)) / spaces;
    
    for (int i = 0; i < ruler.divisions; i++){
        float x = startX + i * distBetween;
        DrawLineEx( (Vector2){ x, ruler.rect.y },
                    (Vector2){ x, ruler.rect.y + ruler.rect.height * 0.46 },
                    1.0f,
                    divisionColor);
    }
}

// ___circle mark_________________________________________________________________________________________

void drawCircleMark(CircleMark circlemark, Color bodyColor, float y){
    DrawCircleV(circlemark.centro, circlemark.raio, bodyColor);
    DrawLineEx( circlemark.centro, 
                (Vector2) {circlemark.centro.x, y},
                1.0f,
                bodyColor);
}

// ___button______________________________________________________________________________________________


int updateButton(Button *btn, Vector2 mouse){

    if(CheckCollisionPointRec(mouse, btn->rect)){
        btn->state = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? BT_PRESSED : BT_HOVER;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return 1;
    } else {
        btn->state = BT_IDLE;
    }
    return 0;
}

void drawButton(Button *btn){
    Color colors[3] = { PS_WHITE, PS_GREY, PS_DARKGREY };
    DrawRectangleRec(btn->rect, colors[btn->state]);
    DrawRectangleLinesEx(btn->rect, 1, PS_BLACK);
    
    Vector2 textSize = MeasureTextEx(font, btn->label, 20, 1);
    DrawTextEx(font,
               btn->label,
               (Vector2) {
                   btn->rect.x + btn->rect.width/2  - textSize.x/2,
                   btn->rect.y + btn->rect.height/2 - textSize.y/2 
               },
               20, 1,
               PS_BLACK);
}

// ___option picker_______________________________________________________________________________________

void updatePicker(OptionPicker *picker, Vector2 mouse){
    // falta interacao com mouse prra selecionar
    if (IsKeyPressed(KEY_RIGHT))
        picker->current = (picker->current + 1) % picker->count;
    if (IsKeyPressed(KEY_LEFT))
        picker->current = (picker->current - 1 + picker->count) % picker->count;
}

void drawPicker(OptionPicker *picker, int posX, int posY){
    // pessimo desenho, vou melhorar, mas deixando aqui pra representar.
    // label
    DrawText(picker->label, posX, posY, 20, PS_BLACK);
    
    // < current option >
    DrawText("<", posX,        posY + 30, 20, PS_BLACK);
    DrawText(picker->options[picker->current].label, posX + 20, posY + 30, 20, PS_BLUE);
    DrawText(">", posX + 100,  posY + 30, 20, PS_BLACK);
}

// ___state menu__________________________________________________________________________________________


Button btnPlay, btnExit; // MAIN MENU

OptionPicker modePicker, difficultyPicker;
Button btnStart;

enum MenuState menuState = MAIN;

void initMenu(Session *game){
    // Main Menu
    btnPlay = (Button){{LARGURA/2 - 75, ALTURA/2, 150, 40}, "JOGAR", BT_IDLE};
    btnExit = (Button){{LARGURA/2 + 75, ALTURA/2, 150, 40}, "SAIR", BT_IDLE};

    // Modes Menu
    modePicker = (OptionPicker){
    "MODO",
    {
        {"NORMAL",   MODO_NORMAL},
        {"ARCADE",   MODO_ARCADE}
    },
    2, 0 };

    difficultyPicker = (OptionPicker){
    "DIFICULDADE",
    {
        {"FACIL",   EASY},
        {"MEDIO",   MEDIUM},
        {"DIFICIL", HARD}
    },
    3, 0};

    btnStart = (Button){{LARGURA/2 - 75, ALTURA/2 + 60, 150, 40}, "COMEÇAR", BT_IDLE};

}

void updateMenu(Session *game){
    Vector2 mousePosition = GetMousePosition();
    switch (menuState)
    {
    case LOGO:
        break;
    case MAIN:
        if (updateButton(&btnPlay, mousePosition)) menuState = MODES;
        if (updateButton(&btnExit, mousePosition)) game->state = STATE_EXIT;
        break;
    case MODES:
        updatePicker(&modePicker, mousePosition);
        if (updateButton(&btnStart, mousePosition)) game->state = STATE_PLAYING;
        break;
    case STATS:
        break;
    default:
        break;
    }    
}

void drawMenu(Session *game){
    switch (menuState)
    {
    case LOGO:
        break;
    case MAIN:
        drawButton(&btnPlay);
        drawButton(&btnExit);
        break;
    case MODES:
        drawButton(&btnStart);
        drawPicker(&modePicker, LARGURA /2, ALTURA /2);
        break;
    case STATS:
        break;
    default:
        break;
    }   
    
}

// ___state playing______________________________________________________________________________________


void drawPlaying(Session *game){
    drawScoreBar(game->score, startingScore, PS_BLUE);

    drawRuler(basicRuler, PS_WHITE, PS_BLACK);
    drawCircleMark(circlemark, PS_BLUE, ALTURA - basicRuler.rect.height + (basicRuler.rect.height * 0.46));

    drawAnimatedNumberInput(input, LARGURA / 2, ALTURA / 2, 200, 10, PS_RED, font);

    DrawText(TextFormat("Numero randomizado = %d",game->target), 20, ALTURA-140, DEBUGFONT, PS_DEBUG); // apenas pro debug
    DrawText(TextFormat("Pontuação atual = %d",game->score), 20, ALTURA-120, DEBUGFONT, PS_DEBUG); // apenas pro debug
    DrawText(game->trivia, 20, ALTURA-100, DEBUGFONT, PS_DEBUG); // apenas pro debug
    
    if (game->guessCount > 0){
        float currentAdvance = 0.0f;
        for (int i =0; i < game->guessCount; i++){
            Vector2 itemSize = MeasureTextEx(font, TextFormat("%d", game->guessHistory[i]), 20, 2);
            DrawTextEx(font, TextFormat("%d", game->guessHistory[i]), (Vector2){50 + currentAdvance,50}, 20, 2, PS_DEBUG);
            currentAdvance += itemSize.x + 10;
        }
    }

    DrawText(game->message, 50, 100, 20, PS_DEBUG);
    DrawText(game->temperature, LARGURA - 100, ALTURA - 100, 20, PS_DEBUG);
}


void updatePlaying(Session *game){

    float dt = GetFrameTime(); 
    atualizarTempoRealScore(game, dt);

    
    // CIRCLE MARK POSTIONING
    if(input.count == 0) {
        circlemark.centro.x = -10;
    } else {
        circlemark.centro.x = atof(input.text) * ((basicRuler.rect.width - basicRuler.margin*2) / (basicRuler.divisions - 1)) + basicRuler.margin;
    }
    if(atoi(input.text) > 100){
        circlemark.centro.x = -10;
    }

    // KEYBOARD INPUT
    if (IsKeyPressed(KEY_ENTER)) {
        ProcessarTentativa(game, atoi(input.text)); //ACSII to INTEGER //
        clearNumberInput(&input);
    }
    updateNumberInput(&input, 3);
}

// ___state gameover___________________________________________________________________________________________


void updateGameover(Session *game){
    if (IsKeyPressed(KEY_R)) {
        IniciarJogo(game);
        game->state = STATE_PLAYING;
    }
}

void drawGameover(Session *game){
    int winTextWidth = MeasureText("YOU WIN!", 40);
    DrawText("YOU WIN!", LARGURA / 2 - winTextWidth / 2, ALTURA / 2, 40, PS_GREEN);    
}



// ___main loop__________________________________________________________________________________________________

void init(Session *game){
    IniciarJogo(game);

    initMenu(game);

    startingScore = game->score;
    input.text[0] = '\0';
    basicRuler = createRuler(101, 70);
    circlemark.centro = (Vector2) {-10 , ALTURA-90};
    circlemark.raio = 10;

    InitWindow(LARGURA, ALTURA, "Pablo Software's Numbers");
    SetTargetFPS(30);
    font = GetFontDefault();

}

void update(Session *game){
    switch (game->state) {
        case STATE_MENU:     updateMenu(game);     break;
        case STATE_PLAYING:  updatePlaying(game);  break;
        case STATE_GAMEOVER: updateGameover(game); break;
        case STATE_EXIT:     CloseWindow();        break;
    }
}

void draw(Session *game){
    BeginDrawing();

        ClearBackground(PS_GREY);

        switch (game->state) {
            case STATE_MENU:     drawMenu(game);     break;
            case STATE_PLAYING:  drawPlaying(game);  break;
            case STATE_GAMEOVER: drawGameover(game); break;
            default: break;
        }

    EndDrawing();   
}

//___raylib entrypoint____

void startRaylibMode(Session *game){

    init(game);

    while (!WindowShouldClose()) {
        update(game);
        draw(game);
    }
}