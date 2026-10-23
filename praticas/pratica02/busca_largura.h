#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "grafo_lista.h"

#define INFINITO -1
#define SEM_PREDECESSOR -1

/* Fila (FIFO) para BFS, implementada como array circular */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

Fila *criar_fila(int capacidade);
int fila_vazia(Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);
void liberar_fila(Fila *f);

/* Percorre o grafo a partir de origem preenchendo dist e pred.
   Vertices nao alcancados ficam com dist = INFINITO. */
void bfs(GrafoLista *g, int origem, int *dist, int *pred);

/* Imprime o caminho de origem ate destino usando o vetor pred da BFS. */
void imprimir_caminho(int origem, int destino, int *pred);

/* Retorna 1 se o grafo admite 2-coloracao (bipartido), 0 caso contrario. */
int eh_bipartido(GrafoLista *g);

#endif
