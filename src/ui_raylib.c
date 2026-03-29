#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <gamestate.h>

#define MAX_INPUT_CHARS 3 // limitar o input do usuario

void startRaylibMode(GameState *game){

    // INIT //
    const int LARGURA = 800;
    const int ALTURA = 450;

    char input[MAX_INPUT_CHARS + 1];
    input[MAX_INPUT_CHARS + 1]= '\0';
    int charCount = 0;

    Rectangle textBox = { LARGURA/2.0f - 100, 180, 225, 50 }; // x, y, largura, altura

    InitWindow(LARGURA, ALTURA, "Adivinhe O Número");
    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input)); //ACSII to INTEGER //
        }

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && charCount < MAX_INPUT_CHARS)
            {
                input[charCount] = (char)key;
                input[charCount+1] = '\0'; // Add null terminator at the end of the string
                charCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            charCount--;
            if (charCount < 0) charCount = 0;
            input[charCount] = '\0';
        }


        // DRAW //
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawText(input, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("Numero randomizado = %d",game->numeroSecreto), 40, ALTURA-40, 20, PURPLE); // apenas pro debug
            DrawText(TextFormat("Limite de tentativas = %d",game->limiteTentativas), 40, ALTURA-80, 20, BLACK);

            DrawText("Historico: ", 40, 10, 20, BLACK);
            for (int i = 0; i < MAX_HISTORICO; i++) {
                DrawText(TextFormat("%d",game->historicoTentativas[i]), 160 + i * 15, 10, 20, BLACK);
            }
            

            if (game->state == STATE_GAMEOVER) {
                DrawText("YOU WIN!", 100, 100, 20, GREEN);
            } else {
                DrawText(game->mensagem, 100, 100, 20, RED);
                DrawText(game->temperatura, LARGURA - 100, ALTURA - 100, 20, RED);
            }
        EndDrawing();
    }
}