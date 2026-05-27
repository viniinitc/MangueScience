#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdbool.h>

typedef enum GameScreen {
    SCREEN_MENU,
    SCREEN_CHARACTER_SELECT,
    SCREEN_SONG_SELECT, 
    SCREEN_GAMEPLAY,
    SCREEN_PAUSED,
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
    Texture2D backgrounds[9];
    Font fonte;
    int currentFrame;
    int frameCounter;
} GameState;


typedef struct Partida {
    char nomeMusica[100]; 
    int pontuacao;
} Partida;

#define BG_SKIN_0 5
#define BG_SKIN_1 6
#define BG_SKIN_2 7
#define BG_SCORE  8

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