#!/bin/bash
# Compile
clang src/*.c -o game -Iinclude -Llib/mac -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL