#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct NoPonderado {
    int destino;
    int peso;
    struct NoPonderado *prox;
} NoPonderado;

typedef struct {
    int n;
    NoPonderado **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_ponderado(int n);

/* Insere o arco orientado u -> v com o peso dado. */
void inserir_arco_ponderado(GrafoListaPonderado *g, int u, int v, int peso);

void exibir_grafo_ponderado(GrafoListaPonderado *g);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
int heap_vazio(MinHeap *h);
void inserir_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);

/* Diminui a distancia de um vertice que ja esta no heap e o sobe ate a
   posicao correta, em O(log V) gracas ao vetor posicao[]. */
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);

/* Retorna 1 se o vertice ainda estiver no heap. */
int esta_no_heap(MinHeap *h, int vertice);

void liberar_min_heap(MinHeap *h);

/* Calcula em dist[] a menor distancia da origem ate cada vertice (INFINITO
   se inalcancavel) e em pred[] o predecessor de cada vertice no caminho
   minimo (-1 para a origem e para vertices inalcancaveis). Pesos devem ser
   nao negativos. */
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Imprime o caminho minimo origem -> destino seguindo pred[]. */
void reconstruir_caminho(int origem, int destino, int *pred);

#endif
