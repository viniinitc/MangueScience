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

// Lê o arquivo TXT
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

// Salva no arquivo TXT
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

    Rectangle btnVoltar = { GetScreenWidth()/2 - 110, 610, 220, 50 };
    
    if (CheckCollisionPointRec(GetMousePosition(), btnVoltar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        processado = false;
        return 1;
    }

    return 0;
}

void DrawScoreSystem( int score, float accuracy, int maxCombo){
    DrawBackground(gs.backgrounds[SCREEN_SCORE]);

    // SEU DESIGN ORIGINAL DE VOLTA: Largura original de 540
    Rectangle painel = { GetScreenWidth()/2 - 270, 120, 540, 560 };

    DrawRectangleRounded(painel, 0.04f, 8, (Color){0,0,0,150});
    DrawRectangleRoundedLinesEx(painel, 0.04f, 8, 3, (Color){0,220,255,180});

    const char *titulo = "FIM DE JOGO!";
    Vector2 tituloSize = MeasureTextEx(gs.fonte, titulo, 34, 2);

    DrawTextEx(gs.fonte, titulo, (Vector2){ GetScreenWidth()/2 - tituloSize.x/2 + 3, 143 }, 34, 2, (Color){0,120,255,255});
    DrawTextEx(gs.fonte, titulo, (Vector2){ GetScreenWidth()/2 - tituloSize.x/2, 140 }, 34, 2, (Color){0,255,220,255});

    DrawTextEx(gs.fonte, TextFormat("SCORE: %06i", score), (Vector2){ GetScreenWidth()/2 - 145, 220 }, 24, 2, (Color){255,220,100,255});
    DrawTextEx(gs.fonte, TextFormat("ACCURACY: %.1f%%", accuracy), (Vector2){ GetScreenWidth()/2 - 145, 270 }, 16, 2, (Color){120,220,255,255});
    DrawTextEx(gs.fonte, TextFormat("MAX COMBO: %i", maxCombo), (Vector2){ GetScreenWidth()/2 - 145, 310 }, 16, 2, (Color){100,255,180,255});

    Vector2 histSize = MeasureTextEx(gs.fonte, "HISTORICO", 18, 2);
    DrawTextEx(gs.fonte, "HISTORICO", (Vector2){ GetScreenWidth()/2 - histSize.x/2, 360 }, 18, 2, (Color){0,255,180,255});

    for(int i = 0; i < 5; i++)
    {
        int y = 400 + (i * 34);

        // SEU DESIGN ORIGINAL DE VOLTA: Posição X e Largura originais (-150 e 300)
        DrawRectangleRounded(
            (Rectangle){ GetScreenWidth()/2 - 150, y - 4, 300, 28 },
            0.18f, 6, (Color){20,30,50,170}
        );

        if (i < totalPartidas) {
            // SEU DESIGN ORIGINAL DE VOLTA: Apenas os pontos e fonte tamanho 14
            DrawTextEx(
                gs.fonte,
                TextFormat("%d. %06d pts", i + 1, ranking[i].pontuacao),
                (Vector2){ GetScreenWidth()/2 - 120, y },
                14, 2, (Color){0,255,180,255}
            );
        } else {
            // SEU DESIGN ORIGINAL DE VOLTA: Traços originais e fonte tamanho 14
            DrawTextEx(
                gs.fonte,
                TextFormat("%d. ------ pts", i + 1),
                (Vector2){ GetScreenWidth()/2 - 120, y },
                14, 2, (Color){0,150,100,100} 
            );
        }
    }

    Rectangle btnVoltar = { GetScreenWidth()/2 - 110, 610, 220, 50 };
    bool hover = CheckCollisionPointRec(GetMousePosition(), btnVoltar);

    DrawRectangleRounded(btnVoltar, 0.3f, 8, hover ? (Color){0,180,255,255} : (Color){0,120,200,255});
    DrawRectangleRoundedLinesEx(btnVoltar, 0.3f, 8, 3, (Color){180,255,255,255});

    Vector2 txt = MeasureTextEx(gs.fonte, "JOGAR DE NOVO", 14, 2);
    DrawTextEx(gs.fonte, "JOGAR DE NOVO", (Vector2){ btnVoltar.x + btnVoltar.width/2 - txt.x/2, btnVoltar.y + 14 }, 14, 2, WHITE);
}