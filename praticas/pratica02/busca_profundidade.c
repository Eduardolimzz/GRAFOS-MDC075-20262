#include <stdio.h>
#include <stdlib.h>
#include "busca_profundidade.h"

Pilha *criar_pilha(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    p->dados = malloc(capacidade * sizeof(int));
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

int pilha_vazia(Pilha *p) {
    return p->topo < 0;
}

void empilhar(Pilha *p, int valor) {
    if (p->topo == p->capacidade - 1) {
        p->capacidade *= 2;
        p->dados = realloc(p->dados, p->capacidade * sizeof(int));
    }
    p->dados[++p->topo] = valor;
}

int desempilhar(Pilha *p) {
    if (pilha_vazia(p)) {
        return -1;
    }
    return p->dados[p->topo--];
}

void liberar_pilha(Pilha *p) {
    free(p->dados);
    free(p);
}

void dfs_recursiva(GrafoLista *g, int u, int *visitado,
                   int *entrada, int *saida, int *relogio) {
    visitado[u] = 1;
    entrada[u] = (*relogio)++;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado, entrada, saida, relogio);
        }
    }

    saida[u] = (*relogio)++;
}

void dfs_iterativa(GrafoLista *g, int origem, int *visitado) {
    Pilha *p = criar_pilha(g->n);
    empilhar(p, origem);

    while (!pilha_vazia(p)) {
        int u = desempilhar(p);
        if (visitado[u]) {
            continue;
        }
        visitado[u] = 1;

        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            if (!visitado[atual->destino]) {
                empilhar(p, atual->destino);
            }
        }
    }

    liberar_pilha(p);
}

int contar_componentes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int componentes = 0;

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            componentes++;
            dfs_iterativa(g, i, visitado);
        }
    }

    free(visitado);
    return componentes;
}

/* Explora o componente de u procurando uma aresta de retorno.
   pai evita confundir a aresta de ida com um ciclo. */
static int busca_ciclo(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (!visitado[v]) {
            if (busca_ciclo(g, v, u, visitado)) {
                return 1;
            }
        } else if (v != pai) {
            return 1;
        }
    }

    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int ciclo = 0;

    for (int i = 0; i < g->n && !ciclo; i++) {
        if (!visitado[i]) {
            ciclo = busca_ciclo(g, i, -1, visitado);
        }
    }

    free(visitado);
    return ciclo;
}
