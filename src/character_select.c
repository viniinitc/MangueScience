#include "raylib.h"
#include "types.h"

extern GameState gs;

static int currentFrame = 0;
static int frameCounter = 0;
static int frameSpeed = 4;

static int maxFrames = 16;

int UpdateCharacterSelect(void){

    if(IsKeyPressed(KEY_RIGHT)){
        gs.selectedSkin++;

        if(gs.selectedSkin > 2){
            gs.selectedSkin = 0;
        }
    }

    if(IsKeyPressed(KEY_LEFT)){
        gs.selectedSkin--;

        if(gs.selectedSkin < 0){
            gs.selectedSkin = 1;
        }
    }
    frameCounter++;

    if(frameCounter >= (60/frameSpeed)){

        frameCounter = 0;

        currentFrame++;

        if(currentFrame >= maxFrames){
            currentFrame = 0;
        }
    }

    if(IsKeyPressed(KEY_ENTER)){
        return 1;
    }

    return 0;
}

void DrawCharacterSelect(void){

    const char* names[3] = {
        "Chico Caranguejo",
        "CrocoScience",
        "Pitú Atômico"
    };

    Color nameColors[3] = {
        RED,
        GREEN,
        ORANGE
    };

    ClearBackground(BLACK);

    DrawBackground(gs.backgrounds[SCREEN_CHARACTER_SELECT]);

    DrawText(
        "ESCOLHA SEU PERSONAGEM",
        GetScreenWidth()/2 - MeasureText("ESCOLHA SEU PERSONAGEM", 30)/2,
        100,
        30,
        WHITE
    );

    int spacing = 150;

    for(int i = 0; i < 3; i++){

        Texture2D skin = gs.skinsSelect[i];

        int frameWidth = skin.width / maxFrames;
        int frameHeight = skin.height;

        Rectangle frameRec = {
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight
        };

        float scale = 4.0f;

        float characterWidth = frameWidth * scale;
        float totalWidth = (characterWidth * 3) + (spacing * 2);
        float startX = GetScreenWidth()/2 - totalWidth/2;

        Vector2 position = {
            startX + (i * (characterWidth + spacing)),
            GetScreenHeight()/2 - 120
        };

        Rectangle destRec = {
            position.x,
            position.y,
            frameWidth * scale,
            frameHeight * scale
        };

        DrawTexturePro(
            skin,
            frameRec,
            destRec,
            (Vector2){0,0},
            0.0f,
            WHITE
        );

        if(i == gs.selectedSkin){

            DrawRectangleLinesEx(
                (Rectangle){
                    destRec.x - 10,
                    destRec.y - 10,
                    destRec.width + 20,
                    destRec.height + 20
                },
                5,
                WHITE
            );

            DrawText(
                names[i],
                GetScreenWidth()/2 - MeasureText(names[i], 40)/2,
                600,
                40,
                nameColors[i]
            );
        }

    }

    DrawText(
        "Use <- e -> para trocar e ENTER para confirmar",
        GetScreenWidth()/2 - MeasureText("Use <- e -> para trocar | ENTER para confirmar", 20)/2,
        700,
        20,
        GRAY
    );
}