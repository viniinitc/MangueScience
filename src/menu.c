#include "raylib.h"
#include "types.h"

extern GameState gs;

bool DrawMenu(void) {

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


    Rectangle buttonRect = {
        GetScreenWidth()/2 - 110,
        GetScreenHeight()/2 + 120,
        220,
        55
    };

    Vector2 mousePos = GetMousePosition();

    bool mouseOverButton = CheckCollisionPointRec(mousePos, buttonRect);

    bool clicou = false;

    Color buttonColor = mouseOverButton
        ? (Color){180, 220, 255, 255}
        : (Color){100, 110, 140, 255};


    DrawRectangleRounded(buttonRect, 0.2f, 6, buttonColor);


    DrawRectangleRoundedLinesEx(buttonRect, 0.2f, 6, 3, WHITE);


    Vector2 playText = MeasureTextEx(gs.fonte, "JOGAR", 20, 1);

    DrawTextEx(gs.fonte,
        "JOGAR",
        (Vector2){
            buttonRect.x + buttonRect.width/2 - playText.x/2,
            buttonRect.y + 16
        },
        20,
        1,
        WHITE);

    if(mouseOverButton && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        clicou = true;
    }

    return clicou;
}