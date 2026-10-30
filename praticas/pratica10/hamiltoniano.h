#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

typedef struct {
    int n;
    unsigned char *adj; /* Matriz de adjacencia, em ordem de linhas. */
} Grafo;

/* Grafo simples nao orientado. Requer n >= 0 e indices em [0, n).
   Insercoes repetidas nao duplicam arestas; lacos sao ignorados. */
Grafo *criar_grafo(int n);
void inserir_aresta(Grafo *g, int u, int v);
void liberar_grafo(Grafo *g);

/* Retorna 1 e preenche caminho[n] se existir uma permutacao dos vertices
   com arestas consecutivas. Tenta todas as origens. Retorna 0 se nao
   existir, deixando o vetor com -1. Grafo vazio retorna 0. */
int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho);

/* Retorna 1 e preenche ciclo[n + 1], com origem 0 repetida no fim.
   Em grafo simples, um ciclo exige pelo menos tres vertices.
   Retorna 0 e preenche -1 se nao existir. Ambos usam backtracking
   com permutacoes, descartando prefixos sem arestas. */
int encontrar_ciclo_hamiltoniano_bt(const Grafo *g, int *ciclo);

#endif
