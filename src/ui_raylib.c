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
            input->text[input->count + 1] = '\0'; // Add null terminator at the end of the string

            input->targetY[input->count] = 0;
            input->currentY[input->count] = 30;

            // printf("\n\n%d\n\n", key);
            input->count++;
        }

        key = GetCharPressed();  // Check next character in the queue
    }

    for (int i = 0; i < input->count; i++) {
        // Each character independently crawls back to 0
        input->currentY[i] = lerp(input->currentY[i], input->targetY[i], 0.3f);
    }


    if (IsKeyPressed(KEY_BACKSPACE))
    {
        input->targetY[input->count] = -30;
        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[input->count] = '\0';
    }

}

void startRaylibMode(Session *game){

    // INIT //
    const int LARGURA = 800;
    const int ALTURA = 450;

    DigitInput input = {0};
    input.text[0] = '\0';

    Rectangle textBox = { LARGURA/2.0f - 100, 180, 225, 50 }; // x, y, largura, altura

    InitWindow(LARGURA, ALTURA, "Adivinhe O Número");

    Font font = LoadFontEx("assets/font/ChonkyPixels.ttf", 32, 0, 250);

    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input.text)); //ACSII to INTEGER //
        }

        updateNumberInput(&input, 3);


        // DRAW //
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //DrawRectangleRec(textBox, LIGHTGRAY);
            //DrawText(input.text, (int)textBox.x + 5, (int)textBox.y + 8, 100, MAROON);
            drawAnimatedNumberInput(input, LARGURA / 2, ALTURA / 2, 200, 0, MAROON, font);

            DrawText(TextFormat("Numero randomizado = %d",game->target), 40, ALTURA-40, 0, PURPLE); // apenas pro debug            

            if (game->state == STATE_GAMEOVER) {
                DrawText("YOU WIN!", 100, 100, 20, GREEN);
            } else {
                DrawText(game->message, 0, 100, 20, RED);
                DrawText(game->temperature, LARGURA - 100, ALTURA - 100, 20, RED);
            }
        EndDrawing();
    }
}