#include <stdio.h>
#include <string.h> 
#include "raylib.h"
#include "types.h"

extern GameState gs;
extern GameScreen currentScreen;

static Partida ranking[100];
static int totalPartidas = 0;
static int maiorRecorde = 0;
static bool processado = false;


void LoadRanking(void) {
    FILE *file = fopen("ranking.txt", "r");
    if (file == NULL) return; 

    totalPartidas = 0;
    while (fscanf(file, "%d|%[^\n]\n", &ranking[totalPartidas].pontuacao, ranking[totalPartidas].nomeMusica) == 2) {
        totalPartidas++;
        if (totalPartidas >= 100) break;
    }
    fclose(file);
}


void SaveRanking(void) {
    FILE *file = fopen("ranking.txt", "w");
    if (file == NULL) return;

    for (int i = 0; i < totalPartidas; i++) {
        fprintf(file, "%d|%s\n", ranking[i].pontuacao, ranking[i].nomeMusica);
    }
    fclose(file);
}

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

int UpdateScoreSystem(int pontuacaoAtual, const char* nomeDaMusica){

    if (!processado) {
        Partida novaPartida;
        
        strncpy(novaPartida.nomeMusica, nomeDaMusica, 99);
        novaPartida.nomeMusica[99] = '\0'; 
        
        novaPartida.pontuacao = pontuacaoAtual;

        ranking[totalPartidas++] = novaPartida;
        insertionSort(ranking, totalPartidas);

        SaveRanking();

        if (pontuacaoAtual > maiorRecorde)
            maiorRecorde = pontuacaoAtual;

        processado = true;
    }

    
    Rectangle btnVoltar = { GetScreenWidth()/2 - 120, 690, 240, 50 };
    
    if (CheckCollisionPointRec(GetMousePosition(), btnVoltar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        processado = false;
        return 1;
    }

    return 0;
}

void DrawScoreSystem(int score, float accuracy, int maxCombo){
    
    Texture2D background = gs.backgrounds[BG_SCORE];
    DrawTexturePro(
        background,
        (Rectangle){0,0,background.width,background.height},
        (Rectangle){0,0,GetScreenWidth(),GetScreenHeight()},
        (Vector2){0,0}, 0, WHITE
    );

    Rectangle tvScreen = { 350, 195, 468, 358 };

    DrawRectangleRec(tvScreen, Fade((Color){0,255,120,255}, 0.06f));
    for (int i = 0; i < tvScreen.height; i += 3)
        DrawLine(tvScreen.x, tvScreen.y + i,
                 tvScreen.x + tvScreen.width, tvScreen.y + i,
                 Fade(BLACK, 0.18f));

    Color verde = (Color){0,255,200,255};
    float cx = tvScreen.x + tvScreen.width / 2.0f;

    const char *titulo = "TRANSMISSAO ENCERRADA";
    Vector2 tSize = MeasureTextEx(gs.fonte, titulo, 16, 2);
    DrawTextEx(gs.fonte, titulo,
        (Vector2){ cx - tSize.x/2, tvScreen.y + 16 },
        16, 2, verde);


    const char *sub = "O MANGUE LEMBRA.";
    Vector2 subSize = MeasureTextEx(gs.fonte, sub, 8, 1);
    DrawTextEx(gs.fonte, sub,
        (Vector2){ cx - subSize.x/2, tvScreen.y + 46 },
        8, 1, Fade(verde, 0.7f));


    DrawLine(tvScreen.x + 20, tvScreen.y + 64,
             tvScreen.x + tvScreen.width - 20, tvScreen.y + 64,
             Fade(verde, 0.3f));


    int statY = tvScreen.y + 80;
    int statStep = 36;

    const char *s1 = TextFormat("PONTUACAO: %06i", score);
    const char *s2 = TextFormat("SINTONIA: %.1f%%", accuracy);
    const char *s3 = TextFormat("SEQUENCIA: %i", maxCombo);

    Vector2 s1Size = MeasureTextEx(gs.fonte, s1, 12, 2);
    Vector2 s2Size = MeasureTextEx(gs.fonte, s2, 12, 2);
    Vector2 s3Size = MeasureTextEx(gs.fonte, s3, 12, 2);

    DrawTextEx(gs.fonte, s1, (Vector2){ cx - s1Size.x/2, statY              }, 12, 2, verde);
    DrawTextEx(gs.fonte, s2, (Vector2){ cx - s2Size.x/2, statY + statStep   }, 12, 2, verde);
    DrawTextEx(gs.fonte, s3, (Vector2){ cx - s3Size.x/2, statY + statStep*2 }, 12, 2, verde);

    DrawLine(tvScreen.x + 20, statY + statStep*3 + 8,
             tvScreen.x + tvScreen.width - 20, statY + statStep*3 + 8,
             Fade(verde, 0.3f));


    const char *hist = "MEMORIA DO MANGUE";
    Vector2 hSize = MeasureTextEx(gs.fonte, hist, 12, 2);
    DrawTextEx(gs.fonte, hist,
        (Vector2){ cx - hSize.x/2, statY + statStep*3 + 22 },
        12, 2, verde);

    for(int i = 0; i < 5; i++) {
        int y = statY + statStep*3 + 48 + (i * 22);

        const char *linha = (i < totalPartidas)
            ? TextFormat("%d. %06d pts", i+1, ranking[i].pontuacao)
            : TextFormat("%d. ------ pts", i+1);

        Vector2 lSize = MeasureTextEx(gs.fonte, linha, 10, 2);
        DrawTextEx(gs.fonte, linha,
            (Vector2){ cx - lSize.x/2, y },
            10, 2,
            i < totalPartidas ? verde : Fade(verde, 0.4f));
    }


    Rectangle btnVoltar = { GetScreenWidth()/2 - 120, 690, 240, 50 };
    bool hover = CheckCollisionPointRec(GetMousePosition(), btnVoltar);

    DrawRectangleRounded(btnVoltar, 0.2f, 6,
        hover ? (Color){180,140,20,255} : (Color){120,90,10,255});
    DrawRectangleRoundedLinesEx(btnVoltar, 0.2f, 6, 2, (Color){255,220,120,255});

    Vector2 txt = MeasureTextEx(gs.fonte, "JOGAR DE NOVO", 12, 2);
    DrawTextEx(gs.fonte, "JOGAR DE NOVO",
        (Vector2){ btnVoltar.x + btnVoltar.width/2 - txt.x/2, btnVoltar.y + 19 },
        12, 2, WHITE);
}