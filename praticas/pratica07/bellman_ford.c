#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void *alocar(size_t tamanho) {
    void *memoria = malloc(tamanho);
    if (memoria == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return memoria;
}

GrafoListaPonderado *criar_grafo_lista_ponderado(int n) {
    if (n <= 0) return NULL;

    GrafoListaPonderado *g = alocar(sizeof(*g));
    g->n = n;
    g->vertice_ciclo = -1;
    g->adj = alocar((size_t)n * sizeof(*g->adj));
    for (int v = 0; v < n; v++) g->adj[v] = NULL;
    return g;
}

int inserir_arco_ponderado(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL || u < 0 || u >= g->n || v < 0 || v >= g->n) return 0;

    NoPonderado *novo = alocar(sizeof(*novo));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
    return 1;
}

void exibir_grafo_lista_ponderado(const GrafoListaPonderado *g) {
    for (int u = 0; u < g->n; u++) {
        printf("  %d ->", u);
        for (NoPonderado *a = g->adj[u]; a != NULL; a = a->prox) {
            printf(" %d(peso=%d)", a->destino, a->peso);
        }
        printf("\n");
    }
}

void liberar_grafo_lista_ponderado(GrafoListaPonderado *g) {
    if (g == NULL) return;
    for (int u = 0; u < g->n; u++) {
        NoPonderado *a = g->adj[u];
        while (a != NULL) {
            NoPonderado *proximo = a->prox;
            free(a);
            a = proximo;
        }
    }
    free(g->adj);
    free(g);
}

/* Uma passagem examina todos os arcos. Guardar o ultimo destino alterado
   na passagem extra permite encontrar o ciclo mesmo se ele tiver saidas. */
static int relaxar_arestas(const GrafoListaPonderado *g, long long *d, int *pred) {
    int ultimo = -1;
    for (int u = 0; u < g->n; u++) {
        if (d[u] == LLONG_MAX) continue;
        for (NoPonderado *a = g->adj[u]; a != NULL; a = a->prox) {
            long long candidato = d[u] + a->peso;
            if (candidato < d[a->destino]) {
                d[a->destino] = candidato;
                pred[a->destino] = u;
                ultimo = a->destino;
            }
        }
    }
    return ultimo;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    long long *d = alocar((size_t)g->n * sizeof(*d));
    g->vertice_ciclo = -1;
    for (int v = 0; v < g->n; v++) {
        d[v] = LLONG_MAX;
        pred[v] = -1;
    }
    d[origem] = 0;

    /* n - 1 passagens de relaxacao para os menores caminhos simples. */
    for (int i = 0; i < g->n - 1; i++) {
        relaxar_arestas(g, d, pred);
    }

    /* Uma melhora na n-esima passagem prova um ciclo negativo alcancavel. */
    g->vertice_ciclo = relaxar_arestas(g, d, pred);
    for (int v = 0; v < g->n; v++) {
        if (d[v] == LLONG_MAX) dist[v] = DISTANCIA_INFINITA;
        else if (d[v] >= INT_MAX) dist[v] = INT_MAX - 1;
        else if (d[v] < INT_MIN) dist[v] = INT_MIN;
        else dist[v] = (int)d[v];
    }
    free(d);
    return g->vertice_ciclo == -1;
}

int reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *pred) {
    if (g == NULL || pred == NULL || g->vertice_ciclo < 0) return 0;
    int v = g->vertice_ciclo;

    /* O ultimo relaxado pode estar depois do ciclo: n passos garantem
       que entramos nele, pois o grafo possui apenas n vertices. */
    for (int i = 0; i < g->n; i++) {
        if (v < 0 || v >= g->n) return 0;
        v = pred[v];
    }
    if (v < 0 || v >= g->n) return 0;

    int inicio = v;
    int *ciclo = alocar(((size_t)g->n + 1) * sizeof(*ciclo));
    int tamanho = 0;
    do {
        if (v < 0 || v >= g->n || tamanho >= g->n) {
            free(ciclo);
            return 0;
        }
        ciclo[tamanho++] = v;
        v = pred[v];
    } while (v != inicio);
    ciclo[tamanho++] = inicio;

    /* pred[] segue os arcos ao contrario; inverter devolve a orientacao. */
    printf("Ciclo negativo: ");
    for (int i = tamanho - 1; i >= 0; i--) {
        printf("%d%s", ciclo[i], i == 0 ? "\n" : " -> ");
    }
    free(ciclo);
    return 1;
}
