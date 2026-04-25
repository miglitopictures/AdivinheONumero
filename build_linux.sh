# Compile for Linux
gcc src/*.c -o game -std=c11 -Iinclude -Llib/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall