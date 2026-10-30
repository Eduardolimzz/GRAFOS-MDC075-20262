#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int eh_cobertura(const Grafo *g, const int *cobertura) {
    for (int i = 0; i < g->m; i++) {
        Aresta a = g->arestas[i];
        if (!cobertura[a.u] && !cobertura[a.v]) return 0;
    }
    return 1;
}

int cobertura_gulosa(const Grafo *g, int *cobertura) {
    int *grau = alocar((size_t)g->n * sizeof(*grau));
    for (int v = 0; v < g->n; v++) cobertura[v] = 0;
    int tamanho = 0;

    while (1) {
        for (int v = 0; v < g->n; v++) grau[v] = 0;
        for (int i = 0; i < g->m; i++) {
            Aresta a = g->arestas[i];
            if (!cobertura[a.u] && !cobertura[a.v]) {
                grau[a.u]++;
                if (a.v != a.u) grau[a.v]++;
            }
        }
        int escolhido = -1;
        for (int v = 0; v < g->n; v++) {
            if (grau[v] > 0 && (escolhido == -1 || grau[v] > grau[escolhido])) {
                escolhido = v;
            }
        }
        if (escolhido == -1) break;
        cobertura[escolhido] = 1;
        tamanho++;
    }
    free(grau);
    return tamanho;
}

typedef struct {
    const Grafo *g;
    int *atual;
    int *melhor;
    int *usado;
    int limite;
} Busca;

static void buscar(Busca *b, int tamanho) {
    /* Ja existe uma cobertura valida com limite vertices. */
    if (tamanho >= b->limite) return;

    int u = -1, v = -1, inferior = 0;
    for (int x = 0; x < b->g->n; x++) b->usado[x] = 0;
    for (int i = 0; i < b->g->m; i++) {
        Aresta a = b->g->arestas[i];
        if (b->atual[a.u] || b->atual[a.v]) continue;
        if (u == -1) {
            u = a.u;
            v = a.v;
        }
        /* Arestas sem extremos em comum exigem escolhas distintas.
           Cada laco tambem exige seu proprio vertice. */
        if (!b->usado[a.u] && !b->usado[a.v]) {
            b->usado[a.u] = b->usado[a.v] = 1;
            inferior++;
        }
    }

    if (u == -1) {
        b->limite = tamanho;
        memcpy(b->melhor, b->atual, (size_t)b->g->n * sizeof(*b->melhor));
        return;
    }
    if (tamanho + inferior >= b->limite) return;

    /* Toda cobertura deve conter u OU v para cobrir a aresta (u, v). */
    b->atual[u] = 1;
    buscar(b, tamanho + 1);
    b->atual[u] = 0;
    if (v != u) {
        b->atual[v] = 1;
        buscar(b, tamanho + 1);
        b->atual[v] = 0;
    }
}

int cobertura_minima_bt(const Grafo *g, int *cobertura) {
    int limite = cobertura_gulosa(g, cobertura);
    if (limite == 0) return 0;

    Busca b;
    b.g = g;
    b.melhor = cobertura;
    b.limite = limite;
    b.atual = alocar((size_t)g->n * sizeof(*b.atual));
    b.usado = alocar((size_t)g->n * sizeof(*b.usado));
    for (int v = 0; v < g->n; v++) b.atual[v] = 0;
    buscar(&b, 0);
    free(b.atual);
    free(b.usado);
    return b.limite;
}
