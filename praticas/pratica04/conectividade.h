#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);
void inserir_aresta_lista(GrafoLista *g, int u, int v);
void exibir_grafo_lista(GrafoLista *g);
void liberar_grafo_lista(GrafoLista *g);

/* Algoritmo de Tarjan: preenche descoberta[]/low[] e marca em articulacao[]
   os vertices que sao pontos de articulacao a partir da raiz u. */
void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo, int *articulacao);

/* Roda dfs_articulacoes em todos os componentes e devolve um vetor alocado
   (tamanho g->n) com 1 nas posicoes que sao vertices de articulacao. */
int *detectar_articulacoes(GrafoLista *g);

/* Roda a mesma DFS de Tarjan, mas marca como ponte toda aresta (u,v) em que
   low[v] > descoberta[u]. Imprime cada ponte encontrada e devolve a
   quantidade total. */
int detectar_pontes(GrafoLista *g);

#endif
