#include <raylib.h>

int main(){

    // Init
    InitWindow(500,500, "Adivinhe O Número");

    Texture2D alzaratest = LoadTexture("../assets/alzara-img-noedit.png");
    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        // Update



        // Draw
        BeginDrawing(); // ---------- //


        ClearBackground(WHITE);
        DrawTexture(alzaratest, 0, 0, WHITE);
        DrawText("testando imagem", 300, 300, 15, RED);
        

        

        EndDrawing();   // ---------- //
    }
    UnloadTexture(alzaratest);
    
    CloseWindow();

    return 0;
}