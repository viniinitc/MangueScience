#include "raylib.h"
#include "types.h"

void DrawBackground(Texture2D texture) {
    DrawTexturePro(
        texture,
        (Rectangle){ 0, 0, texture.width, texture.height },
        (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}