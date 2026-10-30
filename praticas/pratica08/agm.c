#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agm.h"

static void *alocar(size_t tamanho) {
    if (tamanho == 0) return NULL;
    void *memoria = malloc(tamanho);
    if (memoria == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return memoria;
}

DSU *criar_dsu(int n) {
    DSU *d = alocar(sizeof(*d));
    d->n = n;
    d->pai = alocar((size_t)n * sizeof(*d->pai));
    d->rank = alocar((size_t)n * sizeof(*d->rank));
    for (int v = 0; v < n; v++) {
        d->pai[v] = v;
        d->rank[v] = 0;
    }
    return d;
}

void liberar_dsu(DSU *d) {
    if (d == NULL) return;
    free(d->pai);
    free(d->rank);
    free(d);
}

int find(DSU *d, int x) {
    int raiz = x;
    while (d->pai[raiz] != raiz) raiz = d->pai[raiz];

    /* Todos os vertices visitados passam a apontar direto para a raiz. */
    while (d->pai[x] != x) {
        int proximo = d->pai[x];
        d->pai[x] = raiz;
        x = proximo;
    }
    return raiz;
}

int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0;

    /* A raiz de menor rank fica abaixo da de maior rank. */
    if (d->rank[rx] < d->rank[ry]) {
        d->pai[rx] = ry;
    } else {
        d->pai[ry] = rx;
        if (d->rank[rx] == d->rank[ry]) d->rank[rx]++;
    }
    return 1;
}

static AGM criar_resultado(int n) {
    AGM r = {NULL, 0, n, 0};
    if (n > 1) r.arestas = alocar((size_t)(n - 1) * sizeof(*r.arestas));
    return r;
}

static void selecionar(AGM *r, Aresta aresta) {
    r->arestas[r->quantidade++] = aresta;
    r->peso_total += aresta.peso;
}

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a;
    const Aresta *y = b;
    /* Comparacoes, em vez de subtracao, evitam overflow com pesos extremos. */
    if (x->peso != y->peso) return (x->peso > y->peso) - (x->peso < y->peso);
    if (x->u != y->u) return (x->u > y->u) - (x->u < y->u);
    return (x->v > y->v) - (x->v < y->v);
}

AGM kruskal(int n, const Aresta *arestas, int m) {
    AGM r = criar_resultado(n);
    if (n == 0) return r;

    Aresta *ordenadas = alocar((size_t)m * sizeof(*ordenadas));
    if (m > 0) {
        memcpy(ordenadas, arestas, (size_t)m * sizeof(*ordenadas));
        qsort(ordenadas, (size_t)m, sizeof(*ordenadas), comparar_arestas);
    }
    DSU *d = criar_dsu(n);
    for (int i = 0; i < m && r.quantidade < n - 1; i++) {
        Aresta a = ordenadas[i];
        if (unir(d, a.u, a.v)) {
            selecionar(&r, a);
            r.componentes--;
        }
    }
    liberar_dsu(d);
    free(ordenadas);
    return r;
}

typedef struct Arco {
    int destino, peso;
    struct Arco *prox;
} Arco;

typedef struct {
    int *vertices;
    int *posicao; /* -1 indica que o vertice ja foi extraido. */
    long long *chave; /* LLONG_MAX permite usar INT_MAX como peso valido. */
    int tamanho;
} MinHeap;

static MinHeap criar_heap(int n) {
    MinHeap h;
    h.tamanho = n;
    h.vertices = alocar((size_t)n * sizeof(*h.vertices));
    h.posicao = alocar((size_t)n * sizeof(*h.posicao));
    h.chave = alocar((size_t)n * sizeof(*h.chave));
    for (int v = 0; v < n; v++) {
        h.vertices[v] = v;
        h.posicao[v] = v;
        h.chave[v] = LLONG_MAX;
    }
    /* Esta ordem inicial ja e um heap valido. */
    if (n > 0) h.chave[0] = 0;
    return h;
}

static int menor(const MinHeap *h, int i, int j) {
    int u = h->vertices[i], v = h->vertices[j];
    if (h->chave[u] != h->chave[v]) return h->chave[u] < h->chave[v];
    return u < v;
}

static void trocar(MinHeap *h, int i, int j) {
    int v = h->vertices[i];
    h->vertices[i] = h->vertices[j];
    h->vertices[j] = v;
    h->posicao[h->vertices[i]] = i;
    h->posicao[h->vertices[j]] = j;
}

static void diminuir_chave(MinHeap *h, int v, int peso) {
    int i = h->posicao[v];
    h->chave[v] = peso;
    while (i > 0 && menor(h, i, (i - 1) / 2)) {
        int pai = (i - 1) / 2;
        trocar(h, i, pai);
        i = pai;
    }
}

static int extrair_minimo(MinHeap *h) {
    int minimo = h->vertices[0];
    trocar(h, 0, h->tamanho - 1);
    h->tamanho--;
    h->posicao[minimo] = -1;

    int i = 0;
    /* Apenas nos internos possuem filhos; evita calcular 2*i em folhas. */
    while (i < h->tamanho / 2) {
        int filho = 2 * i + 1;
        if (filho + 1 < h->tamanho && menor(h, filho + 1, filho)) filho++;
        if (!menor(h, filho, i)) break;
        trocar(h, i, filho);
        i = filho;
    }
    return minimo;
}

AGM prim(int n, const Aresta *arestas, int m) {
    AGM r = criar_resultado(n);
    if (n == 0) return r;
    r.componentes = 0;

    Arco **adj = alocar((size_t)n * sizeof(*adj));
    Arco *arcos = alocar(2 * (size_t)m * sizeof(*arcos));
    int *pai = alocar((size_t)n * sizeof(*pai));
    for (int v = 0; v < n; v++) {
        adj[v] = NULL;
        pai[v] = -1;
    }
    /* Cada aresta gera dois arcos, pois o grafo nao e orientado. */
    for (int i = 0; i < m; i++) {
        Aresta a = arestas[i];
        Arco *ida = &arcos[2 * (size_t)i];
        Arco *volta = ida + 1;
        *ida = (Arco){a.v, a.peso, adj[a.u]};
        adj[a.u] = ida;
        *volta = (Arco){a.u, a.peso, adj[a.v]};
        adj[a.v] = volta;
    }

    MinHeap h = criar_heap(n);
    while (h.tamanho > 0) {
        int u = extrair_minimo(&h);
        if (pai[u] == -1) {
            /* Uma chave infinita inicia a proxima componente desconectada. */
            r.componentes++;
        } else {
            selecionar(&r, (Aresta){pai[u], u, (int)h.chave[u]});
        }
        for (Arco *a = adj[u]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (h.posicao[v] != -1 && a->peso < h.chave[v]) {
                pai[v] = u;
                diminuir_chave(&h, v, a->peso);
            }
        }
    }
    free(h.vertices);
    free(h.posicao);
    free(h.chave);
    free(adj);
    free(arcos);
    free(pai);
    return r;
}

void liberar_agm(AGM *resultado) {
    if (resultado == NULL) return;
    free(resultado->arestas);
    *resultado = (AGM){NULL, 0, 0, 0};
}
