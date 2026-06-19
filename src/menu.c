#include "raylib.h"
#include "types.h"

extern GameState gs;

int DrawMenu(Sound somConfirm) {

    ClearBackground(RAYWHITE);

    DrawBackground(gs.backgrounds[SCREEN_MENU]);

    Vector2 texto1 = MeasureTextEx(gs.fonte, "MANGUE", 40, 1);
    Vector2 texto2 = MeasureTextEx(gs.fonte, "SCIENCE", 40, 1);

    float x1 = GetScreenWidth()/2 - texto1.x/2;
    float x2 = GetScreenWidth()/2 - texto2.x/2;

    float y = 230;

    DrawTextEx(gs.fonte,
        "MANGUE",
        (Vector2){x1 + 3, y + 3},
        40,
        1,
        DARKBLUE);

    DrawTextEx(gs.fonte,
        "SCIENCE",
        (Vector2){x2 + 3, y + 63},
        40,
        1,
        DARKBLUE);

    DrawTextEx(gs.fonte,
        "MANGUE",
        (Vector2){x1, y},
        40,
        1,
        WHITE);

    DrawTextEx(gs.fonte,
        "SCIENCE",
        (Vector2){x2, y + 60},
        40,
        1,
        WHITE);


    Rectangle playButton = {
        GetScreenWidth()/2 - 110,
        GetScreenHeight()/2 + 50,
        220,
        55
    };

    Rectangle tutorialButton = {
        GetScreenWidth()/2 - 110,
        GetScreenHeight()/2 + 130,
        220,
        55
    };

    Rectangle rankingButton = {
        GetScreenWidth()/2 - 110,
        GetScreenHeight()/2 + 210,
        220,
        55
    };

    Vector2 mousePos = GetMousePosition();

    bool overPlay = CheckCollisionPointRec(mousePos, playButton);
    bool overTutorial = CheckCollisionPointRec(mousePos, tutorialButton);
    bool overRanking = CheckCollisionPointRec(mousePos, rankingButton);

    Color playColor = overPlay
        ? (Color){180, 220, 255, 255}
        : (Color){100, 110, 140, 255};

    Color tutorialColor = overTutorial
        ? (Color){180, 220, 255, 255}
        : (Color){100, 110, 140, 255};

    Color rankingColor = overRanking
        ? (Color){180, 220, 255, 255}
        : (Color){100, 110, 140, 255};


    DrawRectangleRounded(playButton, 0.2f, 6, playColor);
    DrawRectangleRoundedLinesEx(playButton, 0.2f, 6, 3, WHITE);

    Vector2 playText = MeasureTextEx(gs.fonte, "JOGAR", 20, 1);

    DrawTextEx(
        gs.fonte,
        "JOGAR",
        (Vector2){
            playButton.x + playButton.width/2 - playText.x/2,
            playButton.y + 16
        },
        20,
        1,
        WHITE
    );


    Vector2 tutorialText = MeasureTextEx(gs.fonte, "TUTORIAL", 20, 1);

    DrawRectangleRounded(tutorialButton, 0.2f, 6, tutorialColor);
    DrawRectangleRoundedLinesEx(tutorialButton, 0.2f, 6, 3, WHITE);

    DrawTextEx(
        gs.fonte,
        "TUTORIAL",
        (Vector2){
            tutorialButton.x + tutorialButton.width/2 - tutorialText.x/2,
            tutorialButton.y + 16
        },
        20,
        1,
        WHITE
    );


    Vector2 rankingText = MeasureTextEx(gs.fonte, "RANKING", 20, 1);

    DrawRectangleRounded(rankingButton, 0.2f, 6, rankingColor);
    DrawRectangleRoundedLinesEx(rankingButton, 0.2f, 6, 3, WHITE);

    DrawTextEx(
        gs.fonte,
        "RANKING",
        (Vector2){
            rankingButton.x + rankingButton.width/2 - rankingText.x/2,
            rankingButton.y + 16
        },
        20,
        1,
        WHITE
    );


    if(overPlay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        PlaySound(somConfirm);
        return 1;
    }

    if(overTutorial && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        PlaySound(somConfirm);
        return 2;
    }

    if(overRanking && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        PlaySound(somConfirm);
        return 3;
    }

    return 0;
}