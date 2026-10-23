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

void inserir_arco_lista(GrafoLista *grafo, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->destino = v;
    novo->prox = grafo->adj[u];
    grafo->adj[u] = novo;
}

int grau_saida_lista(GrafoLista *grafo, int v) {
    int grau = 0;
    for (No *atual = grafo->adj[v]; atual != NULL; atual = atual->prox) {
        grau++;
    }
    return grau;
}

int grau_entrada_lista(GrafoLista *grafo, int v) {
    int grau = 0;
    for (int u = 0; u < grafo->n; u++) {
        for (No *atual = grafo->adj[u]; atual != NULL; atual = atual->prox) {
            if (atual->destino == v) {
                grau++;
            }
        }
    }
    return grau;
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
