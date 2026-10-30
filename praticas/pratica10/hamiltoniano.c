#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

static void *alocar(size_t tamanho) {
    if (tamanho == 0) return NULL;
    void *memoria = malloc(tamanho);
    if (memoria == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return memoria;
}

Grafo *criar_grafo(int n) {
    Grafo *g = alocar(sizeof(*g));
    g->n = n;
    size_t celulas = (size_t)n * n;
    g->adj = alocar(celulas * sizeof(*g->adj));
    for (size_t i = 0; i < celulas; i++) g->adj[i] = 0;
    return g;
}

void inserir_aresta(Grafo *g, int u, int v) {
    if (u == v) return;
    g->adj[(size_t)u * g->n + v] = 1;
    g->adj[(size_t)v * g->n + u] = 1;
}

void liberar_grafo(Grafo *g) {
    if (g == NULL) return;
    free(g->adj);
    free(g);
}

static int permutar(const Grafo *g, int *ordem, unsigned char *usado,
                    int posicao, int fechar_ciclo) {
    if (posicao == g->n) {
        return !fechar_ciclo || g->adj[(size_t)ordem[g->n - 1] * g->n + ordem[0]];
    }
    int anterior = ordem[posicao - 1];
    for (int v = 0; v < g->n; v++) {
        if (!usado[v] && g->adj[(size_t)anterior * g->n + v]) {
            ordem[posicao] = v;
            usado[v] = 1;
            if (permutar(g, ordem, usado, posicao + 1, fechar_ciclo)) return 1;
            usado[v] = 0;
        }
    }
    return 0;
}

static int buscar(const Grafo *g, int *ordem, int ciclo) {
    if (g->n <= 0) {
        if (ciclo) ordem[0] = -1;
        return 0;
    }
    size_t tamanho = (size_t)g->n + (ciclo ? 1 : 0);
    for (size_t i = 0; i < tamanho; i++) ordem[i] = -1;
    if (ciclo && g->n < 3) return 0;

    unsigned char *usado = alocar((size_t)g->n * sizeof(*usado));
    for (int v = 0; v < g->n; v++) usado[v] = 0;
    /* Fixar 0 em um ciclo nao perde solucoes: basta rotacionar o ciclo.
       Em um caminho essa restricao nao vale, entao tentamos cada origem. */
    int origens = ciclo ? 1 : g->n;
    for (int origem = 0; origem < origens; origem++) {
        ordem[0] = origem;
        usado[origem] = 1;
        if (permutar(g, ordem, usado, 1, ciclo)) {
            if (ciclo) ordem[g->n] = ordem[0];
            free(usado);
            return 1;
        }
        usado[origem] = 0;
    }
    for (size_t i = 0; i < tamanho; i++) ordem[i] = -1;
    free(usado);
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho) {
    return buscar(g, caminho, 0);
}

int encontrar_ciclo_hamiltoniano_bt(const Grafo *g, int *ciclo) {
    return buscar(g, ciclo, 1);
}
