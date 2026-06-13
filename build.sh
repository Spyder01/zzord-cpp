#!/bin/bash

# Source files to compile
SRC="src/main.cpp src/terminal/raw_mode.cpp src/editor/editor.cpp src/buffers/piece-table-buffer.cpp src/render-engine/render-engine.cpp src/key-system/zord-key-system.cpp"         
OUT="zord"              
CXXFLAGS="-Wall -Wextra -std=c++20" 

echo "Compiling $SRC with Clang++..."

clang++ $CXXFLAGS $SRC -o $OUT

if [ $? -eq 0 ]; then
    echo "Build succeeded. Executable: $OUT"
else
    echo "Build failed."
fi
