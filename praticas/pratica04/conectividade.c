#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *grafo = malloc(sizeof(GrafoLista));
    grafo->n = n;
    grafo->adj = malloc(n * sizeof(No *));
    for (int i = 0; i < n; i++) {
        grafo->adj[i] = NULL;
    }
    return grafo;
}

void inserir_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->destino = v;
    novo->prox = grafo->adj[u];
    grafo->adj[u] = novo;

    novo = malloc(sizeof(No));
    novo->destino = u;
    novo->prox = grafo->adj[v];
    grafo->adj[v] = novo;
}

void exibir_grafo_lista(GrafoLista *grafo) {
    for (int u = 0; u < grafo->n; u++) {
        printf("  %d ->", u);
        for (No *atual = grafo->adj[u]; atual != NULL; atual = atual->prox) {
            printf(" %d", atual->destino);
        }
        printf("\n");
    }
}

void liberar_grafo_lista(GrafoLista *grafo) {
    for (int i = 0; i < grafo->n; i++) {
        No *atual = grafo->adj[i];
        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(grafo->adj);
    free(grafo);
}

/* Tarjan: descoberta[u] marca a ordem de visita e low[u] a menor descoberta
   alcancavel a partir de u usando no maximo uma aresta de retorno. */
void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo, int *articulacao) {
    visitado[u] = 1;
    descoberta[u] = low[u] = (*tempo)++;
    int filhos = 0;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;

        if (v == pai) {
            continue;
        }

        if (visitado[v]) {
            if (descoberta[v] < low[u]) {
                low[u] = descoberta[v];
            }
            continue;
        }

        filhos++;
        dfs_articulacoes(g, v, u, visitado, descoberta, low, tempo, articulacao);

        if (low[v] < low[u]) {
            low[u] = low[v];
        }

        /* raiz da DFS e articulacao se tiver mais de um filho;
           demais vertices sao articulacao se o filho nao alcancar um
           ancestral estrito de u. */
        if (pai == -1 && filhos > 1) {
            articulacao[u] = 1;
        }
        if (pai != -1 && low[v] >= descoberta[u]) {
            articulacao[u] = 1;
        }
    }
}

int *detectar_articulacoes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *descoberta = malloc(g->n * sizeof(int));
    int *low = malloc(g->n * sizeof(int));
    int *articulacao = calloc(g->n, sizeof(int));
    int tempo = 0;

    for (int v = 0; v < g->n; v++) {
        if (!visitado[v]) {
            dfs_articulacoes(g, v, -1, visitado, descoberta, low, &tempo, articulacao);
        }
    }

    free(visitado);
    free(descoberta);
    free(low);
    return articulacao;
}

static void dfs_pontes(GrafoLista *g, int u, int pai, int *visitado,
                        int *descoberta, int *low, int *tempo, int *total) {
    visitado[u] = 1;
    descoberta[u] = low[u] = (*tempo)++;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;

        if (v == pai) {
            continue;
        }

        if (visitado[v]) {
            if (descoberta[v] < low[u]) {
                low[u] = descoberta[v];
            }
            continue;
        }

        dfs_pontes(g, v, u, visitado, descoberta, low, tempo, total);

        if (low[v] < low[u]) {
            low[u] = low[v];
        }

        if (low[v] > descoberta[u]) {
            printf("  ponte: %d - %d\n", u, v);
            (*total)++;
        }
    }
}

int detectar_pontes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *descoberta = malloc(g->n * sizeof(int));
    int *low = malloc(g->n * sizeof(int));
    int tempo = 0;
    int total = 0;

    for (int v = 0; v < g->n; v++) {
        if (!visitado[v]) {
            dfs_pontes(g, v, -1, visitado, descoberta, low, &tempo, &total);
        }
    }

    free(visitado);
    free(descoberta);
    free(low);
    return total;
}
