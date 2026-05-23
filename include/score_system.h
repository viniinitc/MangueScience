#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "types.h"

#define HASH_SIZE 31

// Estrutura interna para as partidas salvas automaticamente
typedef struct Partida {
    char nome[16];
    int pontuacao;
} Partida;

// --- 1. BACKEND: ÁRVORE AVL ---
typedef struct NodeAVL {
    Partida partida;
    struct NodeAVL* esq;
    struct NodeAVL* dir;
    int altura;
} NodeAVL;

int obterAltura(NodeAVL* n) {
    if (n == NULL) return 0;
    return n->altura;
}

int maximo(int a, int b) {
    return (a > b) ? a : b;
}

NodeAVL* criarNo(Partida p) {
    NodeAVL* no = (NodeAVL*)malloc(sizeof(NodeAVL));
    no->partida = p;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 1;
    return no;
}

NodeAVL* rotacionarDireita(NodeAVL* y) {
    NodeAVL* x = y->esq;
    NodeAVL* T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = maximo(obterAltura(y->esq), obterAltura(y->dir)) + 1;
    x->altura = maximo(obterAltura(x->esq), obterAltura(x->dir)) + 1;
    return x;
}

NodeAVL* rotacionarEsquerda(NodeAVL* x) {
    NodeAVL* y = x->dir;
    NodeAVL* T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = maximo(obterAltura(x->esq), obterAltura(x->dir)) + 1;
    y->altura = maximo(obterAltura(y->esq), obterAltura(y->dir)) + 1;
    return y;
}

int obterBalanco(NodeAVL* n) {
    if (n == NULL) return 0;
    return obterAltura(n->esq) - obterAltura(n->dir);
}

NodeAVL* inserirAVL(NodeAVL* no, Partida p) {
    if (no == NULL) return criarNo(p);

    if (p.pontuacao < no->partida.pontuacao)
        no->esq = inserirAVL(no->esq, p);
    else
        no->dir = inserirAVL(no->dir, p);

    no->altura = 1 + maximo(obterAltura(no->esq), obterAltura(no->dir));
    int balanco = obterBalanco(no);

    if (balanco > 1 && p.pontuacao < no->esq->partida.pontuacao)
        return rotacionarDireita(no);

    if (balanco < -1 && p.pontuacao >= no->dir->partida.pontuacao)
        return rotacionarEsquerda(no);

    if (balanco > 1 && p.pontuacao >= no->esq->partida.pontuacao) {
        no->esq = rotacionarEsquerda(no->esq);
        return rotacionarDireita(no);
    }

    if (balanco < -1 && p.pontuacao < no->dir->partida.pontuacao) {
        no->dir = rotacionarDireita(no->dir);
        return rotacionarEsquerda(no);
    }

    return no;
}

void avlParaArray(NodeAVL* raiz, Partida arr[], int* index) {
    if (raiz != NULL) {
        avlParaArray(raiz->esq, arr, index);
        arr[*index] = raiz->partida;
        (*index)++;
        avlParaArray(raiz->dir, arr, index);
    }
}

// --- 2. BACKEND: TABELA HASH (Recorde Supremo do Jogo) ---
typedef struct ElementoHash {
    char chave[16];
    int recorde;
} ElementoHash;

ElementoHash tabelaHash[HASH_SIZE];
bool hashInicializada = false;

int funcaoHash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

void atualizarMaiorRecorde(int pontuacao) {
    if (!hashInicializada) {
        for (int i = 0; i < HASH_SIZE; i++) {
            strcpy(tabelaHash[i].chave, "");
            tabelaHash[i].recorde = 0;
        }
        hashInicializada = true;
    }
    
    int idx = funcaoHash("MAX_SCORE");
    strcpy(tabelaHash[idx].chave, "MAX_SCORE");
    if (pontuacao > tabelaHash[idx].recorde) {
        tabelaHash[idx].recorde = pontuacao;
    }
}

int obterMaiorRecorde() {
    if (!hashInicializada) return 0;
    int idx = funcaoHash("MAX_SCORE");
    return tabelaHash[idx].recorde;
}

// --- 3. BACKEND: ALGORITMO DE ORDENAÇÃO (Insertion Sort) ---
void insertionSortDecrescente(Partida arr[], int n) {
    int i, j;
    Partida chave;
    for (i = 1; i < n; i++) {
        chave = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j].pontuacao < chave.pontuacao) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = chave;
    }
}

// --- 4. FRONTEND AUTOMÁTICO DO PLACAR ---
void GerenciarTelaPontuacao(int pontuacaoAtual, NodeAVL** raizAVL, GameScreen* telaAtual) {
    static bool processadoEstaRodada = false;
    static Partida rankingGeral[100];
    static int totalPartidas = 0;
    static int numPartidaSequencial = 1;

    // Quando a tela muda para SCORE, salva e ordena tudo automaticamente UMA vez
    if (!processadoEstaRodada) {
        Partida novaPartida;
        sprintf(novaPartida.nome, "Partida %02d", numPartidaSequencial++);
        novaPartida.pontuacao = pontuacaoAtual;

        // 1. Grava na Árvore AVL balanceada
        *raizAVL = inserirAVL(*raizAVL, novaPartida);
        
        // 2. Compara e atualiza na Tabela Hash usando a função Hash
        atualizarMaiorRecorde(pontuacaoAtual);

        // 3. Copia a árvore para uma estrutura linear e aplica o Insertion Sort
        totalPartidas = 0;
        avlParaArray(*raizAVL, rankingGeral, &totalPartidas);
        insertionSortDecrescente(rankingGeral, totalPartidas);

        processadoEstaRodada = true;
    }

    ClearBackground(BLACK);

    // Design Visual da Tela de Placar
    DrawText("FIM DE JOGO!", GetScreenWidth()/2 - MeasureText("FIM DE JOGO!", 36)/2, 60, 36, MAROON);
    
    // Mostra a pontuação feita na partida atual
    DrawText(TextFormat("Sua Pontuacao: %d pts", pontuacaoAtual), GetScreenWidth()/2 - MeasureText(TextFormat("Sua Pontuacao: %d pts", pontuacaoAtual), 24)/2, 120, 24, GOLD);
    
    // Recorde Supremo resgatado dinamicamente da Tabela Hash
    DrawText(TextFormat("RECORDE MAXIMO DO JOGO: %d pts", obterMaiorRecorde()), GetScreenWidth()/2 - MeasureText(TextFormat("RECORDE MAXIMO DO JOGO: %d pts", obterMaiorRecorde()), 18)/2, 160, 18, SKYBLUE);
    
    DrawRectangle(GetScreenWidth()/2 - 250, 200, 500, 3, WHITE);
    DrawText("HISTORICO DE PONTUACOES (TOP 5)", GetScreenWidth()/2 - MeasureText("HISTORICO DE PONTUACOES (TOP 5)", 20)/2, 220, 20, GREEN);

    // Renderiza o Top 5 ordenado pelo Insertion Sort
    int exibirMax = (totalPartidas > 5) ? 5 : totalPartidas;
    for (int i = 0; i < exibirMax; i++) {
        Color corRank = (rankingGeral[i].pontuacao == pontuacaoAtual) ? GOLD : WHITE;
        
        DrawText(TextFormat("%d.", i + 1), GetScreenWidth()/2 - 180, 270 + (i * 40), 22, corRank);
        DrawText(rankingGeral[i].nome, GetScreenWidth()/2 - 130, 270 + (i * 40), 22, corRank);
        DrawText(TextFormat("%05d pts", rankingGeral[i].pontuacao), GetScreenWidth()/2 + 60, 270 + (i * 40), 22, corRank);
    }

    // Botão interativo para voltar ao Menu Principal
    Rectangle btnVoltar = { GetScreenWidth()/2 - 110, 530, 220, 45 };
    bool mouseSobreBotao = CheckCollisionPointRec(GetMousePosition(), btnVoltar);
    DrawRectangleRec(btnVoltar, mouseSobreBotao ? DARKGREEN : GREEN);
    DrawText("JOGAR DE NOVO", btnVoltar.x + 35, btnVoltar.y + 13, 18, WHITE);

    if (mouseSobreBotao && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        processadoEstaRodada = false; // Permite o salvamento da próxima música jogada
        *telaAtual = SCREEN_MENU;
    }
}

#endif