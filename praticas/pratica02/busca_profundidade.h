#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "grafo_lista.h"

/* Pilha (LIFO) para DFS iterativa */
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

Pilha *criar_pilha(int capacidade);
int pilha_vazia(Pilha *p);
void empilhar(Pilha *p, int valor);
int desempilhar(Pilha *p);
void liberar_pilha(Pilha *p);

/* DFS recursiva a partir de u, registrando tempos de entrada e saida.
   relogio e um contador compartilhado entre as chamadas. */
void dfs_recursiva(GrafoLista *g, int u, int *visitado,
                   int *entrada, int *saida, int *relogio);

/* DFS iterativa com pilha, marcando a ordem de visita em visitado. */
void dfs_iterativa(GrafoLista *g, int origem, int *visitado);

/* Numero de componentes conexos do grafo. */
int contar_componentes(GrafoLista *g);

/* Retorna 1 se o grafo nao orientado possui ao menos um ciclo. */
int tem_ciclo(GrafoLista *g);

#endif
