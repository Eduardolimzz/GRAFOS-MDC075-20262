#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"

Fila *criar_fila(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}

void enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) {
        return;
    }
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int desenfileirar(Fila *f) {
    if (fila_vazia(f)) {
        return -1;
    }
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = INFINITO;
        pred[i] = SEM_PREDECESSOR;
    }

    Fila *f = criar_fila(g->n);
    dist[origem] = 0;
    enfileirar(f, origem);

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            int v = atual->destino;
            if (dist[v] == INFINITO) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(f, v);
            }
        }
    }

    liberar_fila(f);
}

void imprimir_caminho(int origem, int destino, int *pred) {
    if (destino == origem) {
        printf("%d", origem);
    } else if (pred[destino] == SEM_PREDECESSOR) {
        printf("(sem caminho)");
    } else {
        imprimir_caminho(origem, pred[destino], pred);
        printf(" -> %d", destino);
    }
}

int eh_bipartido(GrafoLista *g) {
    int *cor = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        cor[i] = -1;
    }

    Fila *f = criar_fila(g->n);
    int bipartido = 1;

    for (int s = 0; s < g->n && bipartido; s++) {
        if (cor[s] != -1) {
            continue;
        }
        cor[s] = 0;
        enfileirar(f, s);

        while (!fila_vazia(f) && bipartido) {
            int u = desenfileirar(f);
            for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
                int v = atual->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    enfileirar(f, v);
                } else if (cor[v] == cor[u]) {
                    bipartido = 0;
                    break;
                }
            }
        }
    }

    liberar_fila(f);
    free(cor);
    return bipartido;
}
