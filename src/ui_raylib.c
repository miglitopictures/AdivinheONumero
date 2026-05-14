#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <tipos.h>
#include <stdio.h>
#include <math.h>

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

enum CicleMarkState{ CM_FREE, CM_LOCKED, CM_WAIT };

typedef struct{
    enum CicleMarkState state;
    float currentX;
    float targetX;
    float y;
    float raio;
} CircleMark;

int activeMarkIndex = -1;
int inputClearing = 0;

// ___globals_________________________________________________________________________________________

// debug info toggler
int debugMode = 1;

// janela aplicacao
int LARGURA = 1280;
int ALTURA = 720;

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
CircleMark circlemarks[100];
DigitInput input = {0};

// arrow feedback
int shouldDrawArrow = 0, arrowDir = 1;
Vector2 arrowPos, arrowTarget;

//sounds
Sound sfxChangeMark, sfxSelectSynth;


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

    if (input->count == 0) inputClearing = false;  // ← done clearing

}

void clearAnimNumberInput(DigitInput *input){
    for (int i = 0; i < input->count; i++){
        input->targetY[i] = -30;
    }
    inputClearing = 1;
}

void clearInstantNumberInput(DigitInput *input){
    inputClearing = 0; 

    for (int i = 0; i < input->count; i++) {

        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[i] = '\0';

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

// Retorna posicao no eixo x que corresponde ao numero digitado.
float getXFromRulerPoint(Ruler ruler, int point){
    float spacing = (ruler.rect.width - ruler.margin * 2) / (ruler.divisions - 1);
    return point * spacing + ruler.margin;
}

// Retorna numero digitado que corresponde ponto no eixo X.
int getRulerPointFromX(Ruler ruler, float x) {
    float spacing = (ruler.rect.width - ruler.margin * 2) / (ruler.divisions - 1);
    int point = (int)roundf((x - ruler.margin) / spacing);
    if (point < 0) point = 0;
    if (point >= ruler.divisions) point = ruler.divisions - 1;
    return point;
}

void spawnActiveMark(int index, int rulerPoint) {
    float x = getXFromRulerPoint(basicRuler, rulerPoint);
    circlemarks[index].currentX = x;
    circlemarks[index].targetX  = x;
    circlemarks[index].raio  = 10;
    circlemarks[index].y = ALTURA - basicRuler.rect.height - circlemarks[index].raio;
    circlemarks[index].state = CM_FREE;
    activeMarkIndex = index;
}

void updateCircleMarks(Session *game) {
    static float soundCooldown = 0.0f;
    float dt = GetFrameTime();
    if (soundCooldown > 0.0f) soundCooldown -= dt;
    Vector2 mouse = GetMousePosition();

    // mouse overlapping detection
    int detectionPadding = 400;
    bool mouseOnRuler = CheckCollisionPointRec(mouse,
        (Rectangle){basicRuler.rect.x, basicRuler.rect.y-detectionPadding,
                    basicRuler.rect.width, basicRuler.rect.height+detectionPadding});

    // --- Spawn new marker ---
    // From mouse click on ruler
    if (activeMarkIndex < 0 && mouseOnRuler && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int point = getRulerPointFromX(basicRuler, mouse.x);
        // sync input text
        input.count = 0;
        char buf[4];
        snprintf(buf, sizeof(buf), "%d", point);
        for (int i = 0; buf[i] != '\0'; i++) numberInputAdd(&input, buf[i]);
        spawnActiveMark(game->guessCount, point);
    }
    // From first digit typed
    if (activeMarkIndex < 0 && !inputClearing && input.count > 0) {
        int point = atoi(input.text);
        spawnActiveMark(game->guessCount, point);
    }

    // --- Drive active marker ---
    if (activeMarkIndex >= 0) {
        CircleMark *m = &circlemarks[activeMarkIndex];
        int prevPoint = getRulerPointFromX(basicRuler, m->currentX);

        // Mouse drag overrides keyboard
        if (mouseOnRuler && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int point = getRulerPointFromX(basicRuler, mouse.x);
            
            // experimentando com som,isso aqui pode acabar saindo
            if (point != prevPoint && soundCooldown <= 0.0f) {
                float t = (float)point / (basicRuler.divisions - 1); // 0.0 to 1.0
                SetSoundPitch(sfxChangeMark, 0.8f + t * 0.8f); // 0.8 to 1.6
                PlaySound(sfxChangeMark);
                soundCooldown = 0.08f; // 80ms
            };

            m->targetX = getXFromRulerPoint(basicRuler, point);
            // sync input text to mouse position
            input.count = 0;
            char buf[4];
            snprintf(buf, sizeof(buf), "%d", point);
            for (int i = 0; buf[i] != '\0'; i++) numberInputAdd(&input, buf[i]);
        } else {
            // Keyboard drives target
            if (input.count > 0) {
                int point = atoi(input.text);
                if (point > basicRuler.divisions - 1) point = basicRuler.divisions - 1;
                m->targetX = getXFromRulerPoint(basicRuler, point);
            }
        }

        // Lerp currentX toward targetX
        m->currentX = flerp(m->currentX, m->targetX, 0.4f);
        m->y = ALTURA - basicRuler.rect.height - m->raio;
    }
}

void lockActiveCircleMark(void) {
    CircleMark *m = &circlemarks[activeMarkIndex];
    int point = atoi(input.text);
    m->currentX = getXFromRulerPoint(basicRuler, point);
    m->raio = 7;
    m->y = ALTURA - basicRuler.rect.height + (basicRuler.rect.height * 0.46f);
    m->state = CM_LOCKED;
}

void drawCircleMarks(Session *game) {
    // Draw all locked (confirmed) markers
    for (int i = 0; i < game->guessCount; i++) {
        CircleMark m = circlemarks[i];
        DrawCircleV((Vector2){m.currentX, m.y}, m.raio, PS_BLUE);
    }

    // Draw active marker only if one exists
    if (activeMarkIndex >= 0) {
        CircleMark m = circlemarks[activeMarkIndex];
        float lineBottom = m.y + basicRuler.rect.height * 0.46f + m.raio;
        DrawCircleV((Vector2){m.currentX, m.y}, m.raio, PS_BLUE);
        DrawLineEx((Vector2){m.currentX, m.y},
                   (Vector2){m.currentX, lineBottom},
                   2.0f, PS_BLUE);
    }
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
    // pessimo desenho, melhorar.

    // label
    DrawText(picker->label, posX, posY, 20, PS_BLACK);
    
    // < item atual >
    DrawText("<", posX,        posY + 30, 20, PS_BLACK);
    DrawText(picker->options[picker->current].label, posX + 20, posY + 30, 20, PS_BLUE);
    DrawText(">", posX + 100,  posY + 30, 20, PS_BLACK);
}

// ___state menu__________________________________________________________________________________________


Button btnPlay, btnExit, btnStats; // MAIN MENU

OptionPicker modePicker, difficultyPicker;
Button btnStart;

enum MenuState menuState = MAIN;

void initMenu(Session *game){
    // Main Menu
    btnPlay = (Button){{LARGURA/2 - 150 - 75, ALTURA/2, 140, 40}, "JOGAR", BT_IDLE};
    btnStats = (Button){{LARGURA/2 - 75, ALTURA/2, 140, 40}, "ANALISAR", BT_IDLE};
    btnExit = (Button){{LARGURA/2 + 150 - 75, ALTURA/2, 140, 40}, "SAIR", BT_IDLE};

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
        updateButton(&btnStats, mousePosition);
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
        drawButton(&btnStats);
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

void drawArrow(int x, int y, int length, int weight, int dir){
    float headOffsetX = dir ? length/2 : -length/2; // troca a posicao no eixo x dependendo da direcao
    float headAngleOffset = dir ? 0 : 180; // troca a direcao 
    // Ponta da seta, retangulo base para a ">" ponta
    Rectangle arrowHeadRect = {x + headOffsetX, y, weight, length / 2};
    
    // Base (sempre igual)
    DrawRectangle(x - length/2, y - weight/2, length, weight, PS_BLUE);
    // Metade superior
    DrawRectanglePro(arrowHeadRect,
                        (Vector2){weight / 2, weight / 2},
                        45 + headAngleOffset, PS_BLUE);
    // Metade inferior
    DrawRectanglePro(arrowHeadRect,
                        (Vector2){weight / 2, weight / 2},
                        45+90 + headAngleOffset, PS_BLUE);

}

// ___state playing______________________________________________________________________________________

void updatePlaying(Session *game){

    float dt = GetFrameTime(); 
    atualizarTempoRealScore(game, dt);

    updateCircleMarks(game); 

    // update debug mode
    if (IsKeyPressed(KEY_D)) debugMode *= -1;

    // Confirmar tentativa
    if (activeMarkIndex >= 0 && (IsKeyPressed(KEY_ENTER) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON))) {
        lockActiveCircleMark();
        ProcessarTentativa(game, atoi(input.text));
        clearAnimNumberInput(&input);
        activeMarkIndex = -1;
        arrowPos.y = ALTURA / 2 + 50;
        PlaySound(sfxSelectSynth);
    }

    // update arrow feedback
    if (game->guessCount != 0 && input.count == 0){
        shouldDrawArrow = 1;
        arrowPos.y = flerp(arrowPos.y, arrowTarget.y, 0.3);

    } else {
        shouldDrawArrow = 0;
    }

    // shouldDrawArrow = (game->guessCount != 0 && input.count == 0) ? 1 : 0;
    arrowDir = game->guess < game->target ? 1 : 0;

    updateNumberInput(&input, 3);
}

void drawPlaying(Session *game){
    drawScoreBar(game->score, startingScore, PS_BLUE);
    drawRuler(basicRuler, PS_WHITE, PS_BLACK);

    drawCircleMarks(game);   

    drawAnimatedNumberInput(input, LARGURA / 2, ALTURA / 2, 200, 10, PS_RED, font);

    if (shouldDrawArrow) drawArrow(arrowPos.x, arrowPos.y, 200, 20, arrowDir);

    // DEBUG DRAW // aperte "D" para ativar e desativar o desenho de debug.
    if (debugMode == 1){
        DrawText(TextFormat("shouldDrawArrow = %d", shouldDrawArrow), 20, ALTURA-200, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("guessCount = %d",game->guessCount), 20, ALTURA-180, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("inputCount = %d",input.count), 20, ALTURA-160, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("Numero randomizado = %d",game->target), 20, ALTURA-140, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("Pontuação atual = %d",game->score), 20, ALTURA-120, DEBUGFONT, PS_DEBUG); 
        DrawText(game->trivia, 20, ALTURA-100, DEBUGFONT, PS_DEBUG); 
        if (game->guessCount > 0){
            float currentAdvance = 0.0f;
            for (int i =0; i < game->guessCount; i++){
                Vector2 itemSize = MeasureTextEx(font, TextFormat("%d", game->guessHistory[i]), 20, 2);
                DrawTextEx(font, TextFormat("%d", game->guessHistory[i]), (Vector2){50 + currentAdvance,50}, 20, 2, PS_DEBUG);
                currentAdvance += itemSize.x + 10;
            }
        }
        DrawText(game->message, 50, 100, 20, PS_DEBUG);
    }
}

// ___state gameover___________________________________________________________________________________________


void updateGameover(Session *game){
    if (IsKeyPressed(KEY_R)) {
        IniciarJogo(game);
        clearInstantNumberInput(&input);
        for (int i = 0; i < 100; i++){
            circlemarks[i].currentX = -10;
            circlemarks[i].y = ALTURA - 90;
            circlemarks[i].raio = 10;
            circlemarks[i].state = CM_FREE; // will be ignored until spawned
        }
        activeMarkIndex = -1;
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

    arrowPos.x = LARGURA / 2;
    arrowPos.y = ALTURA / 2 + 50;
    arrowTarget.x = LARGURA / 2;
    arrowTarget.y = ALTURA / 2;
    
    InitWindow(LARGURA, ALTURA, "Pablo Software's Numbers");
    InitAudioDevice();      // Initialize audio device
    sfxChangeMark = LoadSound("assets/sfx/changeMarkPoint_Beep.wav");
    sfxSelectSynth = LoadSound("assets/sfx/select_synth.wav");

    SetTargetFPS(60);
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
        DrawFPS(20,20);
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