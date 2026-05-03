@echo off
:: Compile for Windows
gcc src/server_main.c src/logic.c -Iinclude -Llib/windows -lm -std=c11 -o servidor_backend.exe -Wall