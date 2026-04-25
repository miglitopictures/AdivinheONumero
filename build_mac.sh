#!/bin/bash
# Compile
clang src/*.c -o game -std=c11 -lm -Iinclude -Llib/mac -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL -Wall