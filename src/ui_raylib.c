#include <raylib.h>
#include <logic.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 3

void startRaylibMode(GameState *game){

    // INIT //
    const int larguraJanela = 800;
    const int alturaJanela = 450;
    char input[MAX_INPUT_CHARS + 1];

    input[MAX_INPUT_CHARS + 1]= '\0';
    int charCount = 0;


    Rectangle textBox = { larguraJanela/2.0f - 100, 180, 225, 50 };

    InitWindow(larguraJanela, alturaJanela, "Adivinhe O Número");
    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input)); 
        }

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) )
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

            DrawText(TextFormat("Numero randomizado = %d",game->numeroSecreto), 40, alturaJanela-40, 20, BLACK);

            if (game->state == STATE_GAMEOVER) {
                DrawText("YOU WIN!", 100, 100, 20, GREEN);
            } else {
                DrawText(game->mensagem, 100, 100, 20, RED);
                DrawText(game->temperatura, larguraJanela - 100, alturaJanela - 100, 20, RED);
            }
        EndDrawing();
    }
}