#include <raylib.h>
#include <logic.h>
#include <stdlib.h>
#include <tipos.h>

#define MAX_INPUT_CHARS 3 // limitar o input do usuario

typedef struct 
{
    char text[MAX_INPUT_CHARS + 1];
    int count;
} DigitInput;




void updateNumberInput(DigitInput *input, int maxSize){

    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();
    while (key > 0)
    {
        // NOTE: Only allow keys in range [32..125]
        if ((key >= 32) && (key <= 125) && input->count < maxSize)
        {
            input->text[input->count] = (char)key;
            input->text[input->count + 1] = '\0'; // Add null terminator at the end of the string
            input->count++;
        }

        key = GetCharPressed();  // Check next character in the queue
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[input->count] = '\0';
    }

}

void startRaylibMode(Session *game){

    // INIT //
    const int LARGURA = 800;
    const int ALTURA = 450;

    DigitInput input;
    input.text[0] = '\0';
    input.count = 0;

    Rectangle textBox = { LARGURA/2.0f - 100, 180, 225, 50 }; // x, y, largura, altura

    InitWindow(LARGURA, ALTURA, "Adivinhe O Número");

    SetTargetFPS(30);
    IniciarJogo(game);

    while (!WindowShouldClose()) {

        // UPDATE //

        // INPUTS DO TECLADO
        if (IsKeyPressed(KEY_ENTER)) {
            ProcessarTentativa(game, atoi(input.text)); //ACSII to INTEGER //
        }

        updateNumberInput(&input, MAX_INPUT_CHARS);


        // DRAW //
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //DrawRectangleRec(textBox, LIGHTGRAY);
            DrawText(input.text, (int)textBox.x + 5, (int)textBox.y + 8, 100, MAROON);

            DrawText(TextFormat("Numero randomizado = %d",game->target), 40, ALTURA-40, 20, PURPLE); // apenas pro debug            

            if (game->state == STATE_GAMEOVER) {
                DrawText("YOU WIN!", 100, 100, 20, GREEN);
            } else {
                DrawText(game->message, 100, 100, 20, RED);
                DrawText(game->temperature, LARGURA - 100, ALTURA - 100, 20, RED);
            }
        EndDrawing();
    }
}