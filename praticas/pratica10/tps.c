#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tps.h"

static void *alocar(size_t tamanho) {
    if (tamanho == 0) return NULL;
    void *memoria = malloc(tamanho);
    if (memoria == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return memoria;
}

static int custo(const TSP *t, int u, int v) {
    return t->pesos[(size_t)u * t->n + v];
}

TSP *criar_tsp(int n) {
    TSP *t = alocar(sizeof(*t));
    t->n = n;
    t->pesos = alocar((size_t)n * n * sizeof(*t->pesos));
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) t->pesos[(size_t)u * n + v] = u == v ? 0 : 1;
    }
    return t;
}

void definir_peso_tsp(TSP *t, int u, int v, int peso) {
    if (u == v) return;
    t->pesos[(size_t)u * t->n + v] = peso;
    t->pesos[(size_t)v * t->n + u] = peso;
}

void liberar_tsp(TSP *t) {
    if (t == NULL) return;
    free(t->pesos);
    free(t);
}

typedef struct {
    const TSP *t;
    int *ordem;
    int *rota;
    long long melhor;
} BuscaTSP;

static void trocar(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

static void permutar(BuscaTSP *b, int posicao, long long parcial) {
    if (posicao == b->t->n) {
        long long total = parcial + custo(b->t, b->ordem[posicao - 1], b->ordem[0]);
        if (total < b->melhor) {
            b->melhor = total;
            memcpy(b->rota, b->ordem, (size_t)b->t->n * sizeof(*b->rota));
            b->rota[b->t->n] = b->ordem[0];
        }
        return;
    }
    for (int i = posicao; i < b->t->n; i++) {
        trocar(&b->ordem[posicao], &b->ordem[i]);
        long long proximo = parcial + custo(b->t, b->ordem[posicao - 1], b->ordem[posicao]);
        permutar(b, posicao + 1, proximo);
        trocar(&b->ordem[posicao], &b->ordem[i]);
    }
}

long long tsp_forca_bruta(const TSP *t, int origem, int *rota) {
    if (t->n == 0) return 0;
    BuscaTSP b = {t, alocar((size_t)t->n * sizeof(*b.ordem)), rota, LLONG_MAX};
    b.ordem[0] = origem;
    int posicao = 1;
    for (int v = 0; v < t->n; v++) {
        if (v != origem) b.ordem[posicao++] = v;
    }
    permutar(&b, 1, 0);
    free(b.ordem);
    return b.melhor;
}

long long tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota) {
    if (t->n == 0) return 0;
    unsigned char *usado = alocar((size_t)t->n * sizeof(*usado));
    for (int v = 0; v < t->n; v++) usado[v] = 0;
    rota[0] = origem;
    usado[origem] = 1;
    long long total = 0;
    for (int i = 1; i < t->n; i++) {
        int melhor = -1;
        for (int v = 0; v < t->n; v++) {
            if (!usado[v] && (melhor == -1 || custo(t, rota[i - 1], v) < custo(t, rota[i - 1], melhor))) {
                melhor = v;
            }
        }
        rota[i] = melhor;
        usado[melhor] = 1;
        total += custo(t, rota[i - 1], melhor);
    }
    rota[t->n] = origem;
    total += custo(t, rota[t->n - 1], origem);
    free(usado);
    return total;
}

long long tsp_insercao(const TSP *t, int origem, int *rota) {
    if (t->n == 0) return 0;
    unsigned char *usado = alocar((size_t)t->n * sizeof(*usado));
    int *proximidade = alocar((size_t)t->n * sizeof(*proximidade));
    for (int v = 0; v < t->n; v++) {
        usado[v] = 0;
        proximidade[v] = custo(t, v, origem);
    }
    rota[0] = rota[1] = origem;
    usado[origem] = 1;
    long long total = 0;
    int tamanho = 1;
    while (tamanho < t->n) {
        int escolhido = -1;
        for (int v = 0; v < t->n; v++) {
            if (!usado[v] && (escolhido == -1 || proximidade[v] < proximidade[escolhido])) escolhido = v;
        }
        int posicao = 0;
        long long menor_aumento = LLONG_MAX;
        for (int i = 0; i < tamanho; i++) {
            long long aumento = (long long)custo(t, rota[i], escolhido)
                + custo(t, escolhido, rota[i + 1]) - custo(t, rota[i], rota[i + 1]);
            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                posicao = i;
            }
        }
        for (int i = tamanho + 1; i > posicao + 1; i--) rota[i] = rota[i - 1];
        rota[posicao + 1] = escolhido;
        usado[escolhido] = 1;
        tamanho++;
        total += menor_aumento;
        for (int v = 0; v < t->n; v++) {
            int distancia = custo(t, v, escolhido);
            if (!usado[v] && distancia < proximidade[v]) proximidade[v] = distancia;
        }
    }
    free(usado);
    free(proximidade);
    return total;
}

TSP *grafo_para_tsp(const Grafo *g) {
    TSP *t = criar_tsp(g->n);
    for (int u = 0; u < g->n; u++) {
        for (int v = u + 1; v < g->n; v++) {
            definir_peso_tsp(t, u, v, g->adj[(size_t)u * g->n + v] ? 1 : 2);
        }
    }
    return t;
}
