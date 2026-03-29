#include <raylib.h>

int main(){

    // Init
    InitWindow(500,500, "Adivinhe O Número");

    Image img_test = LoadImage("../assets/alzara-img-noedit.png");

    // Texture2D alzaratest = LoadTexture("../assets/alzara-img-noedit.png");
    SetTargetFPS(60);
    int numero = GetRandomValue(0, 100);
    char num[3];
    sprintf(num, "%d", numero);


    while (!WindowShouldClose())
    {
        // Update



        // Draw
        BeginDrawing(); // ---------- //


        ClearBackground(WHITE);
        // DrawTexture(alzaratest, 0, 0, WHITE);
        DrawText("testando imagem", 300, 300, 15, RED);
        DrawText(num, 100, 100, 30, RED);
        

        

        EndDrawing();   // ---------- //
    }
    // UnloadTexture(alzaratest);
    
    CloseWindow();

    return 0;
}