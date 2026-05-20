#include "raylib.h"
#include <stdbool.h>

typedef enum GameScreen { 
    SCREEN_MENU, 
    SCREEN_SONG_SELECT, 
    SCREEN_GAMEPLAY 
} GameScreen;

extern GameScreen currentScreen;

bool DrawMenu(void) {
    ClearBackground(RAYWHITE);

    DrawText("MangueScience", GetScreenWidth() / 2 - MeasureText("MangueScience", 40) / 2, 150, 40, MAROON);
    
    Rectangle buttonRect = { GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 25, 200, 50 };
    Vector2 mousePos = GetMousePosition();
    bool mouseOverButton = CheckCollisionPointRec(mousePos, buttonRect);
    bool clicou = false;

    Color buttonColor = mouseOverButton ? LIGHTGRAY : GRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawRectangleLinesEx(buttonRect, 2, DARKGRAY);

    DrawText("JOGAR", buttonRect.x + (buttonRect.width / 2) - (MeasureText("JOGAR", 20) / 2), buttonRect.y + 15, 20, BLACK);

    if (mouseOverButton && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        clicou = true;
    }

    return clicou;
}