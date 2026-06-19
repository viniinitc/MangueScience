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
    SCREEN_SCORE,
    SCREEN_TUTORIAL,
    SCREEN_NAME_INPUT,
    SCREEN_RANKING 
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
    Texture2D backgrounds[10];
    Font fonte;
    int currentFrame;
    int frameCounter;
} GameState;


typedef struct Partida {
    char nomeMusica[100];
    char nomeJogador[50];
    int pontuacao;
} Partida;

#define BG_SKIN_0 5
#define BG_SKIN_1 6
#define BG_SKIN_2 7
#define BG_SCORE  8
#define BG_TUTORIAL 9 

void DrawBackground(Texture2D texture);

int DrawMenu(Sound somConfirm);

void DrawSongSelect(songs playlist[], int totalSongs, int selectedSong);
int UpdateSongSelect(int totalSongs, int* selectedSong, Sound somSelecao, Sound somConfirm);

int UpdateCharacterSelect(Sound somSelecao, Sound somConfirm);
void DrawCharacterSelect(void);

void LoadRanking(void);
void SaveRanking(void);
int UpdateScoreSystem(int pontuacaoAtual, const char* nomeDaMusica, const char* nomeJogador);
bool NomeExisteNoRanking(const char* nome);
void DrawRankingScreen(songs playlist[], int totalSongs, Sound somConfirm);

void DrawScoreSystem(
    int score,
    float accuracy,
    int maxCombo
);

#endif