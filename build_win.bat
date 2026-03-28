@echo off
:: Compile for Windows
gcc src/*.c -o game.exe -Iinclude -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm
game.exe