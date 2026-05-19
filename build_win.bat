@echo off
:: Compile for Windows
gcc src/*/*.c src/main.c -o game.exe -std=c11 -lm -Iinclude -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm -Wall