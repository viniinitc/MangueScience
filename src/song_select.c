#include "raylib.h"
#include <stdbool.h>
#include "types.h"

extern GameScreen currentScreen;
extern GameState gs;

int UpdateSongSelect(int totalSongs, int* selectedSong, Sound somSelecao){

    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) ||
       IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_DOWN)) {
        PlaySound(somSelecao);
    }

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
    DrawBackground(gs.backgrounds[SCREEN_SONG_SELECT]);

    int tvX = 360;
    int tvY = 225;
    int tvW = 510;
    int tvH = 400;
    int margem = 25;

    const char* titulo = "PLAYLIST CHICO SCIENCE";
    const char* subtitulo = "& NACAO ZUMBI";
    Vector2 tituloSize    = MeasureTextEx(gs.fonte, titulo, 16, 2);
    Vector2 subtituloSize = MeasureTextEx(gs.fonte, subtitulo, 16, 2);

    DrawTextEx(gs.fonte, titulo,
        (Vector2){ tvX + tvW/2 - tituloSize.x/2, tvY + 8 }, 16, 2, RAYWHITE);
    DrawTextEx(gs.fonte, subtitulo,
        (Vector2){ tvX + tvW/2 - subtituloSize.x/2, tvY + 28 }, 16, 2, RAYWHITE);

    DrawLineEx(
        (Vector2){ tvX + margem, tvY + 60 },
        (Vector2){ tvX + tvW - margem, tvY + 60 },
        1.5f, (Color){80, 80, 80, 200}
    );

    int startY = tvY + 85;
    int espacamento = 46;

    for (int i = 0; i < totalSongs; i++) {
        int posY = startY + (i * espacamento);

        if (i == selectedSong) {
     
            DrawText(playlist[i].title, tvX + margem + 18 - 2, posY, 24, Fade(GREEN, 0.3f));
            DrawText(playlist[i].title, tvX + margem + 18 + 2, posY, 24, Fade(GREEN, 0.3f));
            DrawText(playlist[i].title, tvX + margem + 18, posY - 2, 24, Fade(GREEN, 0.3f));
            DrawText(playlist[i].title, tvX + margem + 18, posY + 2, 24, Fade(GREEN, 0.3f));

            DrawText(playlist[i].title, tvX + margem + 18, posY, 24, GREEN);

            DrawText(">", tvX + margem, posY, 24, GREEN);

        } else {
            DrawText(playlist[i].title, tvX + margem + 18, posY, 20, (Color){180, 180, 180, 180});
        }
    }

    const char* instrucao = "SETAS navegar | ENTER confirmar";
    Vector2 instrSize = MeasureTextEx(gs.fonte, instrucao, 11, 2);
    DrawTextEx(gs.fonte, instrucao,
        (Vector2){ tvX + tvW/2 - instrSize.x/2, tvY + tvH - 60 }, 11, 2, GRAY);
}