# Compile for Linux
gcc src/*.c -o game -Iinclude -Llib/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./game