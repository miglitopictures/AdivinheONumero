#include <raylib.h>

int main(){

    // Init
    InitWindow(500,500, "Adivinhe O Número");

    while (!WindowShouldClose())
    {
        // Update



        // Draw
        BeginDrawing(); // ---------- //


        ClearBackground(RAYWHITE);
        DrawText("100", 250, 250, 100, RED);

        

        EndDrawing();   // ---------- //
    }
    
    CloseWindow();

    return 0;
}