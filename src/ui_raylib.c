#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <tipos.h>
#include <stdio.h>

// Interpolação linear
float lerp(float start, float end, float amount){
    return start + (end - start) * amount;
}

void drawAnimatedNumberInput(DigitInput input, int posX, int posY, int fontSize, int spacing, Color color, Font font){
    if (input.count <= 0) return; // Nothing to draw

    Vector2 totalSize = MeasureTextEx(font, input.text, fontSize, spacing);

    float currentAdvance = 0.0f;

    for (int i = 0; i < input.count; i++){
        char buf[2] = { input.text[i], '\0' };

        // Draw at the accumulated position
        DrawTextEx(font, buf,
                 (Vector2){(posX - totalSize.x / 2.0f) + currentAdvance,
                 posY + input.currentY[i] - totalSize.y / 2.0f},
                 fontSize, 0,  color); // Spacing is 0 here since we handle it manually
                 
        // Measure THIS character and add its width + spacing to the advance
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
    

    //input->count = 0;
    // input->text[0] = '\0';
}


void startRaylibMode(Session *game){

    // INIT //
    const int LARGURA = 800;
    const int ALTURA = 450;

    DigitInput input = {0};
    input.text[0] = '\0';

    Rectangle textBox = { LARGURA/2.0f - 100, 180, 225, 50 }; // x, y, largura, altura

    InitWindow(LARGURA, ALTURA, "Adivinhe O Número");

    //Font font = LoadFontEx("assets/font/ChonkyPixels.ttf", 32, 0, 250);
    Font font = GetFontDefault();


    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input.text)); //ACSII to INTEGER //
            clearNumberInput(&input);
        }

        updateNumberInput(&input, 4);


        // DRAW //
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //DrawRectangleRec(textBox, LIGHTGRAY);
            //DrawText(input.text, (int)textBox.x + 5, (int)textBox.y + 8, 100, MAROON);
            drawAnimatedNumberInput(input, LARGURA / 2, ALTURA / 2, 200, 10, MAROON, font);

            DrawText(TextFormat("Numero randomizado = %d",game->target), 40, ALTURA-40, 0, PURPLE); // apenas pro debug
            
            if (game->guessCount > 0){
                float currentAdvance = 0.0f;
                for (int i =0; i < game->guessCount; i++){
                    Vector2 itemSize = MeasureTextEx(font, TextFormat("%d", game->guessHistory[i]), 20, 2);
                    DrawTextEx(font, TextFormat("%d", game->guessHistory[i]), (Vector2){50 + currentAdvance,50}, 20, 2, MAROON);
                    currentAdvance += itemSize.x + 10;
                }
            }
            

            if (game->state == STATE_GAMEOVER) {
                int winTextWidth = MeasureText("YOU WIN!", 40);
                DrawText("YOU WIN!", LARGURA / 2 - winTextWidth / 2, ALTURA / 2, 40, ORANGE);
            } else {
                DrawText(game->message, 50, 100, 20, RED);
                DrawText(game->temperature, LARGURA - 100, ALTURA - 100, 20, RED);
            }
        EndDrawing();
    }
}