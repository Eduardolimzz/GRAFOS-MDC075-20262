#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

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

void remover_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *atual = grafo->adj[u];
    No *anterior = NULL;
    while (atual != NULL && atual->destino != v) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (anterior == NULL) {
            grafo->adj[u] = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
    }

    atual = grafo->adj[v];
    anterior = NULL;
    while (atual != NULL && atual->destino != u) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (anterior == NULL) {
            grafo->adj[v] = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
    }
}

int grau_lista(GrafoLista *grafo, int v) {
    int grau = 0;
    No *atual = grafo->adj[v];
    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *grafo, int u, int v) {
    No *atual = grafo->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
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
