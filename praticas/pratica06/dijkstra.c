#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

GrafoListaPonderado *criar_grafo_ponderado(int n) {
    GrafoListaPonderado *grafo = malloc(sizeof(GrafoListaPonderado));
    grafo->n = n;
    grafo->adj = malloc(n * sizeof(NoPonderado *));
    for (int i = 0; i < n; i++) {
        grafo->adj[i] = NULL;
    }
    return grafo;
}

void inserir_arco_ponderado(GrafoListaPonderado *grafo, int u, int v, int peso) {
    NoPonderado *novo = malloc(sizeof(NoPonderado));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = grafo->adj[u];
    grafo->adj[u] = novo;
}

void exibir_grafo_ponderado(GrafoListaPonderado *grafo) {
    for (int u = 0; u < grafo->n; u++) {
        printf("  %d ->", u);
        for (NoPonderado *atual = grafo->adj[u]; atual != NULL; atual = atual->prox) {
            printf(" %d(%d)", atual->destino, atual->peso);
        }
        printf("\n");
    }
}

void liberar_grafo_ponderado(GrafoListaPonderado *grafo) {
    for (int i = 0; i < grafo->n; i++) {
        NoPonderado *atual = grafo->adj[i];
        while (atual != NULL) {
            NoPonderado *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(grafo->adj);
    free(grafo);
}

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int v = 0; v < capacidade; v++) {
        h->posicao[v] = -1;
    }
    return h;
}

int heap_vazio(MinHeap *h) {
    return h->tamanho == 0;
}

int esta_no_heap(MinHeap *h, int vertice) {
    return h->posicao[vertice] != -1;
}

/* Troca dois elementos do heap mantendo posicao[] atualizado. */
static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].distancia <= h->dados[i].distancia) {
            break;
        }
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    while (1) {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia) {
            menor = esq;
        }
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia) {
            menor = dir;
        }
        if (menor == i) {
            break;
        }
        trocar(h, i, menor);
        i = menor;
    }
}

void inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho == h->capacidade) {
        return;
    }
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap minimo = h->dados[0];
    h->tamanho--;

    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        descer(h, 0);
    }

    h->posicao[minimo.vertice] = -1;
    return minimo;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];
    if (i == -1 || nova_distancia >= h->dados[i].distancia) {
        return;
    }
    h->dados[i].distancia = nova_distancia;
    subir(h, i);
}

void liberar_min_heap(MinHeap *h) {
    free(h->dados);
    free(h->posicao);
    free(h);
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    MinHeap *h = criar_min_heap(g->n);

    for (int v = 0; v < g->n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;

    for (int v = 0; v < g->n; v++) {
        inserir_heap(h, v, dist[v]);
    }

    while (!heap_vazio(h)) {
        NoHeap atual = extrair_minimo(h);
        int u = atual.vertice;

        /* os vertices restantes sao inalcancaveis a partir da origem */
        if (dist[u] == INFINITO) {
            break;
        }

        for (NoPonderado *aresta = g->adj[u]; aresta != NULL; aresta = aresta->prox) {
            int v = aresta->destino;
            if (esta_no_heap(h, v) && dist[u] + aresta->peso < dist[v]) {
                dist[v] = dist[u] + aresta->peso;
                pred[v] = u;
                diminuir_chave(h, v, dist[v]);
            }
        }
    }

    liberar_min_heap(h);
}

static void imprimir_caminho(int origem, int destino, int *pred) {
    if (destino == origem) {
        printf("%d", origem);
        return;
    }
    imprimir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino != origem && pred[destino] == -1) {
        printf("sem caminho de %d ate %d\n", origem, destino);
        return;
    }
    imprimir_caminho(origem, destino, pred);
    printf("\n");
}
