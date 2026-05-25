#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdbool.h>

typedef enum GameScreen {
    SCREEN_MENU,
    SCREEN_CHARACTER_SELECT,
    SCREEN_SONG_SELECT, 
    SCREEN_GAMEPLAY,
    SCREEN_SCORE 
} GameScreen;

typedef struct songs {
    Music musica;
    int qntbeats;
    const char *title;
    float offset;
} songs;

typedef struct balls {
    int type;
    int dir;
    Rectangle rect;
    Rectangle outsiderect;
    int check;
    Texture2D sprite;
    Vector2 vect;
    struct balls* next;
    struct balls* prev;
} balls;

typedef struct GameState {
    Texture2D skins[3]; 
    Texture2D skinsSelect[3];
    int selectedSkin;
    Texture2D backgrounds[5];
    Font fonte;
    int currentFrame;
    int frameCounter;
} GameState;

// 1. Modificamos a struct Partida para caber o nome da música
typedef struct Partida {
    char nomeMusica[100]; 
    int pontuacao;
} Partida;

void DrawBackground(Texture2D texture);

bool DrawMenu(void);

int UpdateSongSelect(int totalSongs, int* selectedSong);
void DrawSongSelect(songs playlist[], int totalSongs, int selectedSong);

int UpdateCharacterSelect(void);
void DrawCharacterSelect(void);

// 2. Adicionamos as funções do TXT e modificamos o UpdateScoreSystem
void LoadRanking(void);
void SaveRanking(void);
int UpdateScoreSystem(int pontuacaoAtual, const char* nomeDaMusica);

void DrawScoreSystem(
    int score,
    float accuracy,
    int maxCombo
);

#endif