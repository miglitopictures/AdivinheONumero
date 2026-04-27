#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <tipos.h>
#include <stdio.h>

#define DEBUGFONT 20

int LARGURA = 800;
int ALTURA = 450;

// Paleta de cores // 
Color PS_BLACK = BLACK;
Color PS_DARKGREY = {190, 190, 190, 255};
Color PS_GREY = {205, 205, 205, 255};
Color PS_WHITE = WHITE;
Color PS_BLUE = {55, 60, 218, 255};
Color PS_RED = {218, 55, 55, 255};
Color PS_GREEN = {28, 121, 0, 255};
Color PS_DEBUG = PURPLE;

// Interpolação linear //
float lerp(float start, float end, float amount){
    return start + (end - start) * amount;
}

// Régua (Linha Numerica 1d) UH9 //
typedef struct{
    Rectangle rect;
    int divisions;
    float margin;
} Ruler;

typedef struct{
    Vector2 centro;
    float raio;
} CircleMark;

void drawCircleMark(CircleMark circlemark, Color bodyColor, float y){
    DrawCircleV(circlemark.centro, circlemark.raio, bodyColor);
    DrawLineEx( circlemark.centro, 
                (Vector2) {circlemark.centro.x, y},
                1.0f,
                bodyColor);
}


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

// User Input Digits //
typedef struct {
    char text[16];
    float currentY[15];
    float targetY[15];
    int count;
} DigitInput;

void drawAnimatedNumberInput(DigitInput input, int posX, int posY, int fontSize, int spacing, Color color, Font font){
    if (input.count <= 0) return; // Nothing to draw

    Vector2 totalSize = MeasureTextEx(font, input.text, fontSize, spacing);

    float currentAdvance = 0.0f;

    for (int i = 0; i < input.count; i++){
        char buf[2] = { input.text[i], '\0' };

        DrawTextEx(font, buf,
                 (Vector2){(posX - totalSize.x / 2.0f) + currentAdvance,
                 posY + input.currentY[i] - totalSize.y / 2.0f},
                 fontSize, 0,  color); // Spacing is 0 here since we handle it manually
                 
        Vector2 charSize = MeasureTextEx(font, buf, fontSize, 0);
        currentAdvance += charSize.x + spacing;
    }
}

void updateNumberInput(DigitInput *input, int maxSize){

    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();
    while (key > 0)
    {
        // NOTE: Only allow keys in range [32..125]
        // NOTE: para numeros seria (48 até 57)
        if ((key >= 48) && (key <= 57) && input->count < maxSize)
        {
            input->text[input->count] = (char)key;
            input->text[input->count + 1] = '\0';

            input->targetY[input->count] = 0;
            input->currentY[input->count] = 30;

            // printf("\n\n%d\n\n", key);
            input->count++;
        }

        key = GetCharPressed();
    }

    for (int i = 0; i < input->count; i++) {
        
        input->currentY[i] = lerp(input->currentY[i], input->targetY[i], 0.3f);

        if (input->currentY[i] <= -20) {
            input->count--;
            if (input->count < 0) input->count = 0;
            input->text[i] = '\0';
        }
    }


    if (IsKeyPressed(KEY_BACKSPACE))
    {
        input->targetY[input->count - 1] = -30;

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


void startRaylibMode(Session *game){
    
    DigitInput input = {0};
    input.text[0] = '\0';

    InitWindow(LARGURA, ALTURA, "Adivinhe O Número");

    //Font font = LoadFontEx("assets/font/ChonkyPixels.ttf", 32, 0, 250);
    Font font = GetFontDefault();

    Ruler basicRuler = createRuler(101, 70);

    CircleMark circlemark = {(Vector2) {-10 , ALTURA-90}, 10};

    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if(input.count == 0) {
            circlemark.centro.x = -10;
        } else {
            circlemark.centro.x = atof(input.text) * ((basicRuler.rect.width - basicRuler.margin*2) / (basicRuler.divisions - 1)) + basicRuler.margin;
        }
        if(atoi(input.text) > 100){
            circlemark.centro.x = -10;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input.text)); //ACSII to INTEGER //
            clearNumberInput(&input);
        }

        updateNumberInput(&input, 3);

        // DRAW //
        BeginDrawing();
            ClearBackground(PS_GREY);

            
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
            

            if (game->state == STATE_GAMEOVER) {
                int winTextWidth = MeasureText("YOU WIN!", 40);
                DrawText("YOU WIN!", LARGURA / 2 - winTextWidth / 2, ALTURA / 2, 40, PS_GREEN);
                if (IsKeyPressed(KEY_R)) {
                    game->state = STATE_PLAYING;
                    IniciarJogo(game);
                }
            } else {
                DrawText(game->message, 50, 100, 20, PS_DEBUG);
                DrawText(game->temperature, LARGURA - 100, ALTURA - 100, 20, PS_DEBUG);
            }
        EndDrawing();
    }
}