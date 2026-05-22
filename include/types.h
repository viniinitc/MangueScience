#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

typedef enum GameScreen {
    SCREEN_MENU,
    SCREEN_SONG_SELECT, 
    SCREEN_GAMEPLAY,
    SCREEN_SCORE 
} GameScreen;

typedef struct songs{
    Music musica;
    int qntbeats;
    const char *title;
}songs;

//notas
typedef struct balls{

    int type;
    //1 - up; 2 - down; 3 - right; 4 - left
    //quando vcs acharam que precisar mudar algo na lista nao esquecam de mudar na funcao de inicializacao
    //ORDEM DE DIRECOES UP DOWN RIGHT LEFT
    int dir;
    Rectangle rect;
    Rectangle outsiderect;
	int check;
    Texture2D sprite;
    Vector2 vect;
    struct balls* next;
    struct balls* prev;

}balls;

#endif