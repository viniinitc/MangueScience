#include <stdio.h>
#include "raylib.h"
#include "types.h"

extern GameState gs;

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

void DrawScoreSystem(int pontuacaoAtual){

    ClearBackground(BLACK);

    DrawText("FIM DE JOGO!", GetScreenWidth()/2 - MeasureText("FIM DE JOGO!", 36)/2, 60, 36, MAROON);

    DrawText(TextFormat("Sua Pontuacao: %d pts", pontuacaoAtual),
        GetScreenWidth()/2 - MeasureText(TextFormat("Sua Pontuacao: %d pts", pontuacaoAtual), 24)/2, 120, 24, GOLD);

    DrawText(TextFormat("RECORDE MAXIMO DO JOGO: %d pts", maiorRecorde),
        GetScreenWidth()/2 - MeasureText(TextFormat("RECORDE MAXIMO DO JOGO: %d pts", maiorRecorde), 18)/2, 160, 18, SKYBLUE);

    DrawRectangle(GetScreenWidth()/2 - 250, 200, 500, 3, WHITE);
    DrawText("HISTORICO DE PONTUACOES (TOP 5)",
        GetScreenWidth()/2 - MeasureText("HISTORICO DE PONTUACOES (TOP 5)", 20)/2, 220, 20, GREEN);

    int exibirMax = (totalPartidas > 5) ? 5 : totalPartidas;
    for (int i = 0; i < exibirMax; i++) {
        Color corRank = (ranking[i].pontuacao == pontuacaoAtual) ? GOLD : WHITE;
        DrawText(TextFormat("%d.", i + 1), GetScreenWidth()/2 - 180, 270 + (i * 40), 22, corRank);
        DrawText(ranking[i].nome, GetScreenWidth()/2 - 130, 270 + (i * 40), 22, corRank);
        DrawText(TextFormat("%05d pts", ranking[i].pontuacao), GetScreenWidth()/2 + 60, 270 + (i * 40), 22, corRank);
    }

    Rectangle btnVoltar = { GetScreenWidth()/2 - 110, 530, 220, 45 };
    bool hover = CheckCollisionPointRec(GetMousePosition(), btnVoltar);
    DrawRectangleRec(btnVoltar, hover ? DARKGREEN : GREEN);
    DrawText("JOGAR DE NOVO", btnVoltar.x + 35, btnVoltar.y + 13, 18, WHITE);

}