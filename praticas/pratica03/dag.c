#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

/* Cores usadas pela DFS: cinza indica vertice ainda na pilha de recursao,
   logo um arco para um vertice cinza e um arco de retorno (ciclo). */
#define BRANCO 0
#define CINZA 1
#define PRETO 2

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

void calcular_graus_entrada(GrafoLista *g, int *grau_entrada) {
    for (int v = 0; v < g->n; v++) {
        grau_entrada[v] = 0;
    }
    for (int u = 0; u < g->n; u++) {
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            grau_entrada[atual->destino]++;
        }
    }
}

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int *grau_entrada = malloc(g->n * sizeof(int));
    calcular_graus_entrada(g, grau_entrada);

    Fila *f = criar_fila(g->n);
    for (int v = 0; v < g->n; v++) {
        if (grau_entrada[v] == 0) {
            enfileirar(f, v);
        }
    }

    int *ordem = malloc(g->n * sizeof(int));
    int visitados = 0;

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        ordem[visitados++] = u;

        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            int v = atual->destino;
            if (--grau_entrada[v] == 0) {
                enfileirar(f, v);
            }
        }
    }

    liberar_fila(f);
    free(grau_entrada);

    /* Sobrou vertice sem entrar na ordem: os arcos restantes formam um ciclo. */
    if (visitados != g->n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = visitados;
    return ordem;
}

/* Visita u em profundidade e grava os vertices em ordem->pos ao sair.
   Devolve 0 assim que encontra um arco de retorno. */
static int visitar_dfs(GrafoLista *g, int u, int *cor, int *ordem, int *pos) {
    cor[u] = CINZA;

    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;
        if (cor[v] == CINZA) {
            return 0;
        }
        if (cor[v] == BRANCO && !visitar_dfs(g, v, cor, ordem, pos)) {
            return 0;
        }
    }

    cor[u] = PRETO;
    ordem[(*pos)--] = u;
    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int *cor = calloc(g->n, sizeof(int));
    int *ordem = malloc(g->n * sizeof(int));
    int pos = g->n - 1;
    int aciclico = 1;

    for (int v = 0; v < g->n && aciclico; v++) {
        if (cor[v] == BRANCO) {
            aciclico = visitar_dfs(g, v, cor, ordem, &pos);
        }
    }

    free(cor);

    if (!aciclico) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = g->n;
    return ordem;
}

int eh_dag(GrafoLista *g) {
    int tamanho = 0;
    int *ordem = ordenacao_topologica_dfs(g, &tamanho);

    if (ordem == NULL) {
        return 0;
    }

    free(ordem);
    return 1;
}

int validar_ordenacao(GrafoLista *g, int *ordem, int tamanho) {
    if (ordem == NULL || tamanho != g->n) {
        return 0;
    }

    int *posicao = malloc(g->n * sizeof(int));
    for (int i = 0; i < tamanho; i++) {
        posicao[ordem[i]] = i;
    }

    int valida = 1;
    for (int u = 0; u < g->n && valida; u++) {
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            if (posicao[u] > posicao[atual->destino]) {
                valida = 0;
                break;
            }
        }
    }

    free(posicao);
    return valida;
}
