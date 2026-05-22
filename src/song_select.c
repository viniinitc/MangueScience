#include "raylib.h"
#include <stdbool.h>
#include "types.h"

extern GameScreen currentScreen;

int UpdateSongSelect(int totalSongs, int* selectedSong){

    if (IsKeyPressed(KEY_DOWN)) {
        *selectedSong = (*selectedSong + 1) % totalSongs;
    }
    if (IsKeyPressed(KEY_UP)) {
        *selectedSong = (*selectedSong - 1 + totalSongs) % totalSongs;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        return *selectedSong;
    }
    return -1;

}

void DrawSongSelect(songs playlist[], int totalSongs, int selectedSong){

    ClearBackground(BLACK);
                
                
    DrawText("SELECIONE SUA MUSICA", GetScreenWidth()/2 - MeasureText("SELECIONE SUA MUSICA", 30)/2, 100, 30, RAYWHITE);

    for (int i = 0; i < totalSongs; i++) {
        int posY = 250 + (i * 60);
        if (i == selectedSong) {
            DrawText(TextFormat("> %s <", playlist[i].title), 200, posY, 24, GOLD);                    
        } else {
            DrawText(playlist[i].title, 220, posY, 24, LIGHTGRAY);
        }
    }
                        
    DrawText("Use as SETAS para navegar e ENTER para confirmar", 200, 650, 20, GRAY);

}