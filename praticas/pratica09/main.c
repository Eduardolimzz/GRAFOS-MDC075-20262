#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

static void *alocar(size_t tamanho) {
    if (tamanho == 0) return NULL;
    void *memoria = malloc(tamanho);
    if (memoria == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return memoria;
}

static void imprimir(const char *nome, int n, const int *cobertura, int tamanho) {
    printf("%s: {", nome);
    int primeiro = 1;
    for (int v = 0; v < n; v++) {
        if (cobertura[v]) {
            printf("%s%d", primeiro ? "" : ", ", v);
            primeiro = 0;
        }
    }
    printf("} | tamanho = %d\n", tamanho);
}

static void verificar(const Grafo *g, const int *cobertura, int tamanho) {
    int contagem = 0;
    for (int v = 0; v < g->n; v++) contagem += cobertura[v] != 0;
    if (contagem != tamanho || !eh_cobertura(g, cobertura)) {
        fprintf(stderr, "Erro: cobertura invalida.\n");
        exit(EXIT_FAILURE);
    }
}

static void analisar(const char *nome, Grafo g, int minimo_esperado, int gulosa_esperada) {
    int *bt = alocar((size_t)g.n * sizeof(*bt));
    int *gulosa = alocar((size_t)g.n * sizeof(*gulosa));
    int exato = cobertura_minima_bt(&g, bt);
    int heuristica = cobertura_gulosa(&g, gulosa);
    verificar(&g, bt, exato);
    verificar(&g, gulosa, heuristica);
    printf("\n===== %s =====\n", nome);
    imprimir("Backtracking", g.n, bt, exato);
    imprimir("Gulosa      ", g.n, gulosa, heuristica);
    if (exato != minimo_esperado || heuristica != gulosa_esperada) {
        fprintf(stderr, "Erro: resultado diferente do esperado em %s.\n", nome);
        exit(EXIT_FAILURE);
    }
    printf("Verificacao: coberturas validas e tamanhos esperados confirmados.\n");
    free(bt);
    free(gulosa);
}

/* Gerador deterministico: o mesmo n produz o mesmo grafo em cada execucao.
   Considera cada par u < v, com probabilidade aproximada de 35%%. */
static int gerar_arestas(int n, Aresta *arestas) {
    uint32_t estado = 2026u + (uint32_t)n;
    int m = 0;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            estado = 1664525u * estado + 1013904223u;
            if (estado % 100u < 35u) arestas[m++] = (Aresta){u, v};
        }
    }
    return m;
}

typedef int (*Algoritmo)(const Grafo *, int *);

/* Mede tempo de CPU medio por chamada. Aumenta o lote ate acumular
   pelo menos 20 ms, reduzindo o efeito da resolucao de clock().
   A medicao inclui as alocacoes internas, mas nao geracao nem impressao. */
static double medir(const Grafo *g, int *cobertura, Algoritmo algoritmo,
                    int *tamanho, int *repeticoes) {
    int lote = 64;
    while (1) {
        clock_t inicio = clock();
        for (int i = 0; i < lote; i++) *tamanho = algoritmo(g, cobertura);
        clock_t fim = clock();
        if (inicio == (clock_t)-1 || fim == (clock_t)-1) {
            fprintf(stderr, "Erro: relogio de CPU indisponivel.\n");
            exit(EXIT_FAILURE);
        }
        double segundos = (double)(fim - inicio) / CLOCKS_PER_SEC;
        if (segundos >= 0.02 || lote >= 1048576) {
            *repeticoes = lote;
            return segundos * 1000000.0 / lote;
        }
        lote *= 2;
    }
}

static void experimentar(void) {
    const int tamanhos[] = {5, 8, 10, 12, 15};
    printf("\n===== Experimento: tempo de CPU medio por chamada =====\n");
    printf("Mesmo grafo para os dois algoritmos; densidade aproximada de 35%%.\n");
    printf("us = microssegundos. BT inclui a gulosa usada como limite inicial.\n");
    printf(" n |  m | BT tam | Gulosa tam | Excesso |      BT us |  Gulosa us | Reps BT / Gulosa\n");
    for (int i = 0; i < 5; i++) {
        int n = tamanhos[i];
        Aresta *arestas = alocar((size_t)(n * (n - 1) / 2) * sizeof(*arestas));
        Grafo g = {n, gerar_arestas(n, arestas), arestas};
        int *bt = alocar((size_t)n * sizeof(*bt));
        int *gulosa = alocar((size_t)n * sizeof(*gulosa));
        int exato, heuristica, reps_bt, reps_gulosa;
        double tempo_bt = medir(&g, bt, cobertura_minima_bt, &exato, &reps_bt);
        double tempo_gulosa = medir(&g, gulosa, cobertura_gulosa, &heuristica, &reps_gulosa);
        verificar(&g, bt, exato);
        verificar(&g, gulosa, heuristica);
        if (heuristica < exato) {
            fprintf(stderr, "Erro: heuristica menor que o otimo informado.\n");
            exit(EXIT_FAILURE);
        }
        printf("%2d | %2d | %6d | %10d | %7d | %10.3f | %10.3f | %d / %d\n",
               n, g.m, exato, heuristica, heuristica - exato,
               tempo_bt, tempo_gulosa, reps_bt, reps_gulosa);
        free(arestas);
        free(bt);
        free(gulosa);
    }
    printf("A gulosa pode usar mais vertices; o BT garante o minimo.\n");
    printf("Os tempos variam com a maquina e a estrutura do grafo.\n");
}

int main(void) {
    const Aresta triangulo[] = {{0, 1}, {1, 2}, {2, 0}};
    analisar("Triangulo", (Grafo){3, 3, triangulo}, 2, 2);
    const Aresta estrela[] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}};
    analisar("Estrela", (Grafo){6, 5, estrela}, 1, 1);

    /* Caminho 1--2--0--3--4: o desempate da gulosa escolhe 0 primeiro.
       Ela usa tres vertices, mas {2, 3} cobre todas as arestas com dois. */
    const Aresta caminho[] = {{1, 2}, {2, 0}, {0, 3}, {3, 4}};
    analisar("Gulosa diferente do otimo", (Grafo){5, 4, caminho}, 2, 3);
    const Aresta desconectado[] = {{0, 1}, {1, 2}, {3, 4}};
    analisar("Componentes desconectadas e vertice isolado", (Grafo){6, 3, desconectado}, 2, 2);
    const Aresta lacos[] = {{0, 0}, {0, 1}, {1, 2}, {1, 2}, {2, 2}};
    analisar("Lacos e arestas paralelas", (Grafo){4, 5, lacos}, 2, 3);
    analisar("Vertices sem arestas", (Grafo){4, 0, NULL}, 0, 0);
    analisar("Grafo vazio", (Grafo){0, 0, NULL}, 0, 0);

    experimentar();
    return 0;
}
