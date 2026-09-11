#ifndef DAG_H
#define DAG_H

#include "grafo_lista.h"

/* Fila (FIFO) em array circular, usada pelo algoritmo de Kahn */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

Fila *criar_fila(int capacidade);
int fila_vazia(Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);
void liberar_fila(Fila *f);

/* Preenche grau_entrada com o numero de arcos que chegam em cada vertice. */
void calcular_graus_entrada(GrafoLista *g, int *grau_entrada);

/* Ordenacao topologica pelo algoritmo de Kahn (fila + grau de entrada).
   Devolve um vetor alocado com a ordem dos vertices e escreve seu tamanho
   em *tamanho. Devolve NULL e tamanho 0 se o digrafo tiver ciclo. */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);

/* Ordenacao topologica por DFS, empilhando cada vertice na saida.
   Mesmo contrato de retorno da versao de Kahn. */
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);

/* Retorna 1 se o digrafo for aciclico (DAG), 0 caso contrario. */
int eh_dag(GrafoLista *g);

/* Confere se a ordem dada respeita todos os arcos do digrafo. */
int validar_ordenacao(GrafoLista *g, int *ordem, int tamanho);

#endif
