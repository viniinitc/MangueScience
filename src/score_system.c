#include <stdio.h>
#include "raylib.h"
#include "types.h"

extern GameState gs;
extern GameScreen currentScreen;

static Partida ranking[100];
static int totalPartidas = 0;
static int maiorRecorde = 0;
static int numPartidaSequencial = 1;
static bool processado = false;

static void insertionSort(Partida arr[], int n) {
    for (int i = 1; i < n; i++) {
        Partida chave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].pontuacao < chave.pontuacao) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }
}

int UpdateScoreSystem(int pontuacaoAtual){

    if (!processado) {
        Partida novaPartida;
        sprintf(novaPartida.nome, "Partida %02d", numPartidaSequencial++);
        novaPartida.pontuacao = pontuacaoAtual;

        ranking[totalPartidas++] = novaPartida;
        insertionSort(ranking, totalPartidas);

        if (pontuacaoAtual > maiorRecorde)
            maiorRecorde = pontuacaoAtual;

        processado = true;
    }

    Rectangle btnVoltar = { GetScreenWidth()/2 - 110, 530, 220, 45 };
    if (CheckCollisionPointRec(GetMousePosition(), btnVoltar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        processado = false;
        return 1;
    }

    return 0;

}

void DrawScoreSystem( int score, float accuracy, int maxCombo){
    DrawBackground(gs.backgrounds[SCREEN_SCORE]);

    int scores[5] = {
        12345,
        11020,
        9870,
        8500,
        7200
    };

    int totalScores = 5;

    Rectangle painel = {
        GetScreenWidth()/2 - 270,
        120,
        540,
        560
    };

    DrawRectangleRounded(
        painel,
        0.04f,
        8,
        (Color){0,0,0,150}
    );

    DrawRectangleRoundedLinesEx(
        painel,
        0.04f,
        8,
        3,
        (Color){0,220,255,180}
    );

    const char *titulo = "FIM DE JOGO!";

    Vector2 tituloSize =
        MeasureTextEx(gs.fonte, titulo, 34, 2);

    DrawTextEx(
        gs.fonte,
        titulo,
        (Vector2){
            GetScreenWidth()/2 - tituloSize.x/2 + 3,
            143
        },
        34,
        2,
        (Color){0,120,255,255}
    );

    DrawTextEx(
        gs.fonte,
        titulo,
        (Vector2){
            GetScreenWidth()/2 - tituloSize.x/2,
            140
        },
        34,
        2,
        (Color){0,255,220,255}
    );

    DrawTextEx(
        gs.fonte,
        TextFormat("SCORE: %06i", score),
        (Vector2){
            GetScreenWidth()/2 - 145,
            220
        },
        24,
        2,
        (Color){255,220,100,255}
    );

    DrawTextEx(
        gs.fonte,
        TextFormat("ACCURACY: %.1f%%", accuracy),
        (Vector2){
            GetScreenWidth()/2 - 145,
            270
        },
        16,
        2,
        (Color){120,220,255,255}
    );

    DrawTextEx(
        gs.fonte,
        TextFormat("MAX COMBO: %i", maxCombo),
        (Vector2){
            GetScreenWidth()/2 - 145,
            310
        },
        16,
        2,
        (Color){100,255,180,255}
    );

    Vector2 histSize =
        MeasureTextEx(gs.fonte, "HISTORICO", 18, 2);

    DrawTextEx(
        gs.fonte,
        "HISTORICO",
        (Vector2){
            GetScreenWidth()/2 - histSize.x/2,
            360
        },
        18,
        2,
        (Color){0,255,180,255}
    );

    for(int i = 0; i < totalScores; i++)
    {
        int y = 400 + (i * 34);

        DrawRectangleRounded(
            (Rectangle){
                GetScreenWidth()/2 - 150,
                y - 4,
                300,
                28
            },
            0.18f,
            6,
            (Color){20,30,50,170}
        );

        DrawTextEx(
            gs.fonte,
            TextFormat("%d. %06d pts", i + 1, scores[i]),
            (Vector2){
                GetScreenWidth()/2 - 120,
                y
            },
            14,
            2,
            (Color){0,255,180,255}
        );
    }

    Rectangle btnVoltar = {
        GetScreenWidth()/2 - 110,
        610,
        220,
        50
    };

    bool hover =
        CheckCollisionPointRec(GetMousePosition(), btnVoltar);

    DrawRectangleRounded(
        btnVoltar,
        0.3f,
        8,
        hover
        ? (Color){0,180,255,255}
        : (Color){0,120,200,255}
    );

    DrawRectangleRoundedLinesEx(
        btnVoltar,
        0.3f,
        8,
        3,
        (Color){180,255,255,255}
    );

    Vector2 txt = MeasureTextEx(
        gs.fonte,
        "JOGAR DE NOVO",
        14,
        2
    );

    DrawTextEx(
        gs.fonte,
        "JOGAR DE NOVO",
        (Vector2){
            btnVoltar.x + btnVoltar.width/2 - txt.x/2,
            btnVoltar.y + 14
        },
        14,
        2,
        WHITE
    );
}