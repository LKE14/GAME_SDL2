#!/bin/bash

# 1. Kompilieren mit SDL2
echo "Compiling the game..."
g++ main.cpp -o game `sdl2-config --cflags --libs`

# 2. Prüfen ob Kompilieren erfolgreich war
if [ $? -ne 0 ]; then
    echo "X Compilation failed!"
    exit 1
fi


# 3. Spiel ausführen
echo "Starting the game..."
./game
