#include "raylib.h"
#include "types.h"

extern GameState gs;

static int currentFrame = 0;
static int frameCounter = 0;
static int frameSpeed = 4;

static int maxFrames = 16;

int UpdateCharacterSelect(Sound somSelecao, Sound somConfirm){

    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
        PlaySound(somSelecao);
    }

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
        PlaySound(somConfirm);
        return 1;
    }

    return 0;
}

void DrawCharacterSelect(void){

    const char* names[3] = {
        "Chico Caranguejo",
        "CrocoScience",
        "Pitu Atomico"
    };

    Color nameColors[3] = { RED, GREEN, ORANGE };

    ClearBackground(BLACK);
    DrawBackground(gs.backgrounds[SCREEN_CHARACTER_SELECT]);

    DrawTextEx(gs.fonte, "ESCOLHA SEU PERSONAGEM",
        (Vector2){ GetScreenWidth()/2 - MeasureTextEx(gs.fonte, "ESCOLHA SEU PERSONAGEM", 24, 2).x/2, 80 },
        24, 2, WHITE);

    int prevSkin = (gs.selectedSkin - 1 + 3) % 3;
    int nextSkin = (gs.selectedSkin + 1) % 3;

    {
        Texture2D skin = gs.skinsSelect[prevSkin];
        int frameWidth  = skin.width / maxFrames;
        int frameHeight = skin.height;
        Rectangle frameRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
        float scale = 3.0f;
        float charW = frameWidth  * scale;
        float charH = frameHeight * scale;
        float posX = GetScreenWidth()/2 - 350 - charW/2;
        float posY = GetScreenHeight()/2 - charH/2;

        DrawTexturePro(skin, frameRec,
            (Rectangle){ posX, posY, charW, charH },
            (Vector2){0,0}, 0.0f, Fade(WHITE, 0.5f));
    }

    
        Texture2D skin = gs.skinsSelect[nextSkin];
        int frameWidth  = skin.width / maxFrames;
        int frameHeight = skin.height;
        Rectangle frameRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
        float scale = 3.0f;
        float charW = frameWidth  * scale;
        float charH = frameHeight * scale;
        float posX = GetScreenWidth()/2 + 350 - charW/2;
        float posY = GetScreenHeight()/2 - charH/2;

        DrawTexturePro(skin, frameRec,
            (Rectangle){ posX, posY, charW, charH },
            (Vector2){0,0}, 0.0f, Fade(WHITE, 0.5f));

    {
        Texture2D skin = gs.skinsSelect[gs.selectedSkin];
        int frameWidth  = skin.width / maxFrames;
        int frameHeight = skin.height;
        Rectangle frameRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
        float scale = 6.0f;
        float charW = frameWidth  * scale;
        float charH = frameHeight * scale;
        float posX = GetScreenWidth()/2 - charW/2;
        float posY = GetScreenHeight()/2 - charH/2 - 20;

        DrawTexturePro(skin, frameRec,
            (Rectangle){ posX, posY, charW, charH },
            (Vector2){0,0}, 0.0f, WHITE);


        DrawTextEx(gs.fonte, names[gs.selectedSkin],
            (Vector2){ GetScreenWidth()/2 - MeasureTextEx(gs.fonte, names[gs.selectedSkin], 28, 2).x/2, 620 },
            28, 2, nameColors[gs.selectedSkin]);
    }

    DrawTextEx(gs.fonte, "Use <- e -> para trocar | ENTER para confirmar",
        (Vector2){ GetScreenWidth()/2 - MeasureTextEx(gs.fonte, "Use <- e -> para trocar | ENTER para confirmar", 12, 2).x/2, 710 },
        12, 2, GRAY);
}