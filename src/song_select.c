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