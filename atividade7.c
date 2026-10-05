#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define PONTUACAO_MAX 500
#define QUANTIDADE_PLACAR 50
#define LARGURA_TELA 800
#define ALTURA_TELA 600

typedef struct {
    char nome;
    int pontuacao;
} Placar;

Placar *criarPlacares(int quantidade) {
    Placar *placares = (Placar *) malloc(quantidade * sizeof(Placar));
    if (placares == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Placar *p = (placares + i);
        TextCopy(p->nome, TextFormat("J%02d", i + 1));
        p->pontuacao = GetRandomValue(10, PONTUACAO_MAX);
    }
    return placares;
}

void ordenarBubbleSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > vetor[j + 1].pontuacao) {
                Placar temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                (*trocas)++;
            }
        }
    }
}

void ordenarInsertionSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 1; i < n; i++) {
        Placar chave = vetor[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > chave.pontuacao) {
                vetor[j + 1] = vetor[j];
                (*trocas)++;
                j--;
            } else {
                break;
            }
        }
        vetor[j + 1] = chave;
    }
}

int buscaSequencial(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n; i++) {
        (*comparacoes)++;
        if (vetor[i].pontuacao == alvo) return i;
    }
    return -1;
}

int buscaBinaria(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        (*comparacoes)++;
        int meio = (inicio + fim) / 2;

        if (vetor[meio].pontuacao == alvo) return meio;
        if (vetor[meio].pontuacao < alvo) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

int main(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Atividade 7 - Algoritmos de Busca e Ordenacao");
    SetTargetFPS(60);

    int quantidade = QUANTIDADE_PLACAR;
    Placar *placares = criarPlacares(quantidade);

    long comparacoes = 0;
    long trocas = 0;
    double tempo_ms = 0.0;
    bool ordenado = false;
    
    char algoritmo_atual = "Nenhum";
    int resultado_busca = -2; 
    int alvo_busca = 0;

    while (!WindowShouldClose()) {
    
        if (IsKeyPressed(KEY_R)) {
            free(placares);
            placares = criarPlacares(quantidade);
            ordenado = false;
            comparacoes = 0;
            trocas = 0;
            tempo_ms = 0.0;
            resultado_busca = -2;
            TextCopy(algoritmo_atual, "Resetado");
        }
        if (IsKeyPressed(KEY_B)) {
            double inicio = GetTime();
            ordenarBubbleSort(placares, quantidade, &comparacoes, &trocas);
            double fim = GetTime();
            
            tempo_ms = (fim - inicio) * 1000.0;
            ordenado = true;
            resultado_busca = -2;
            TextCopy(algoritmo_atual, "Bubble Sort");
        }
        if (IsKeyPressed(KEY_I)) {
            double inicio = GetTime();
            ordenarInsertionSort(placares, quantidade, &comparacoes, &trocas);
            double fim = GetTime();
            
            tempo_ms = (fim - inicio) * 1000.0;
            ordenado = true;
            resultado_busca = -2;
            TextCopy(algoritmo_atual, "Insertion Sort");
        }
        if (IsKeyPressed(KEY_Q)) {
            alvo_busca = placares[GetRandomValue(0, quantidade - 1)].pontuacao;
            double inicio = GetTime();
            resultado_busca = buscaSequencial(placares, quantidade, alvo_busca, &comparacoes);
            double fim = GetTime();
            
            tempo_ms = (fim - inicio) * 1000.0;
            trocas = 0;
            TextCopy(algoritmo_atual, "Busca Sequencial");
        }
        if (IsKeyPressed(KEY_W)) {
            if (ordenado) {
                alvo_busca = placares[GetRandomValue(0, quantidade - 1)].pontuacao;
                double inicio = GetTime();
                resultado_busca = buscaBinaria(placares, quantidade, alvo_busca, &comparacoes);
                double fim = GetTime();
                
                tempo_ms = (fim - inicio) * 1000.0;
                trocas = 0;
                TextCopy(algoritmo_atual, "Busca Binaria");
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(10, 10, 780, 130, LIGHTGRAY);
        DrawText("Controles: [R] Resetar | [B] Bubble Sort | [I] Insertion Sort | [Q] Busca Sequencial | [W] Busca Binaria", 20, 20, 15, DARKGRAY);
        DrawText(TextFormat("Ultima Acao: %s", algoritmo_atual), 20, 45, 18, BLACK);
        DrawText(TextFormat("Comparacoes: %ld | Trocas: %ld | Tempo: %.4f ms | Ordenado: %s", 
                            comparacoes, trocas, tempo_ms, ordenado ? "SIM" : "NAO"), 20, 70, 18, BLUE);

        if (resultado_busca != -2) {
            if (resultado_busca >= 0) {
                DrawText(TextFormat("Busca por %d: ENCONTRADO no indice %d (%s)", alvo_busca, resultado_busca, placares[resultado_busca].nome), 20, 95, 18, DARKGREEN);
            } else {
                DrawText(TextFormat("Busca por %d: NAO ENCONTRADO", alvo_busca), 20, 95, 18, RED);
            }
        }

        int largura_barra = (LARGURA_TELA - 40) / quantidade;
        int inicio_x = 20;
        int base_y = 550;

        for (int i = 0; i < quantidade; i++) {
            int altura_barra = (int)(((float)placares[i].pontuacao / PONTUACAO_MAX) * 380);
            int x = inicio_x + (i * largura_barra);
            int y = base_y - altura_barra;

            Color cor_barra = SKYBLUE;
            if (resultado_busca == i) {
                cor_barra = GOLD;
            }

            DrawRectangle(x, y, largura_barra - 2, altura_barra, cor_barra);
            DrawRectangleLines(x, y, largura_barra - 2, altura_barra, BLUE);
        }

        EndDrawing();
    }

    free(placares);
    CloseWindow();

    return 0;
}
