#include "raylib.h"
#include "types.h"

extern GameState gs;

int UpdateCharacterSelect(void){

    if(IsKeyPressed(KEY_RIGHT)){
        gs.selectedSkin++;

        if(gs.selectedSkin > 1){
            gs.selectedSkin = 0;
        }
    }

    if(IsKeyPressed(KEY_LEFT)){
        gs.selectedSkin--;

        if(gs.selectedSkin < 0){
            gs.selectedSkin = 1;
        }
    }

    if(IsKeyPressed(KEY_ENTER)){
        return 1;
    }

    return 0;
}

void DrawCharacterSelect(void){

    ClearBackground(BLACK);

    DrawText(
        "ESCOLHA SEU PERSONAGEM",
        GetScreenWidth()/2 - MeasureText("ESCOLHA SEU PERSONAGEM", 30)/2,
        80,
        30,
        WHITE
    );

    DrawTexture(
        gs.skins[gs.selectedSkin],
        GetScreenWidth()/2 - gs.skins[gs.selectedSkin].width/2,
        GetScreenHeight()/2 - gs.skins[gs.selectedSkin].height/2,
        WHITE
    );

    DrawText(
        "Use <- e -> para trocar | ENTER para confirmar",
        250,
        700,
        20,
        GRAY
    );
}