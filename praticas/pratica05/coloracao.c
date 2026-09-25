#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

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

int grau_lista(GrafoLista *grafo, int v) {
    int grau = 0;
    for (No *atual = grafo->adj[v]; atual != NULL; atual = atual->prox) {
        grau++;
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

/* Colore os vertices na ordem dada por "ordem" (ou 0..n-1, se ordem for
   NULL), atribuindo a cada um a menor cor nao usada por seus vizinhos. */
static int *colorir_na_ordem(GrafoLista *g, int *ordem, int *num_cores) {
    int *cor = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        cor[i] = -1;
    }

    int *cor_usada = malloc(g->n * sizeof(int));
    int maior_cor = -1;

    for (int i = 0; i < g->n; i++) {
        int u = ordem ? ordem[i] : i;

        for (int c = 0; c < g->n; c++) {
            cor_usada[c] = 0;
        }
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            if (cor[atual->destino] != -1) {
                cor_usada[cor[atual->destino]] = 1;
            }
        }

        int c = 0;
        while (cor_usada[c]) {
            c++;
        }
        cor[u] = c;
        if (c > maior_cor) {
            maior_cor = c;
        }
    }

    free(cor_usada);
    *num_cores = maior_cor + 1;
    return cor;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    return colorir_na_ordem(g, NULL, num_cores);
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int *ordem = malloc(g->n * sizeof(int));
    int *grau = malloc(g->n * sizeof(int));
    for (int v = 0; v < g->n; v++) {
        ordem[v] = v;
        grau[v] = grau_lista(g, v);
    }

    /* ordenacao por selecao, decrescente pelo grau */
    for (int i = 0; i < g->n - 1; i++) {
        int maior = i;
        for (int j = i + 1; j < g->n; j++) {
            if (grau[ordem[j]] > grau[ordem[maior]]) {
                maior = j;
            }
        }
        int tmp = ordem[i];
        ordem[i] = ordem[maior];
        ordem[maior] = tmp;
    }

    int *cor = colorir_na_ordem(g, ordem, num_cores);
    free(ordem);
    free(grau);
    return cor;
}

int validar_coloracao(GrafoLista *g, int *cor) {
    for (int u = 0; u < g->n; u++) {
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            if (cor[u] == cor[atual->destino]) {
                return 0;
            }
        }
    }
    return 1;
}

int eh_bipartido(GrafoLista *g) {
    int *cor = malloc(g->n * sizeof(int));
    int *fila = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        cor[i] = -1;
    }

    int bipartido = 1;

    for (int inicio = 0; inicio < g->n && bipartido; inicio++) {
        if (cor[inicio] != -1) {
            continue;
        }

        cor[inicio] = 0;
        int frente = 0, fim = 0;
        fila[fim++] = inicio;

        while (frente < fim && bipartido) {
            int u = fila[frente++];
            for (No *atual = g->adj[u]; atual != NULL && bipartido; atual = atual->prox) {
                int v = atual->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[fim++] = v;
                } else if (cor[v] == cor[u]) {
                    bipartido = 0;
                }
            }
        }
    }

    free(cor);
    free(fila);
    return bipartido;
}
