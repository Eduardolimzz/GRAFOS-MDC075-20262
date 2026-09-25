#ifndef COLORACAO_H
#define COLORACAO_H

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
int grau_lista(GrafoLista *g, int v);
void exibir_grafo_lista(GrafoLista *g);
void liberar_grafo_lista(GrafoLista *g);

/* Algoritmo guloso: percorre os vertices na ordem 0..n-1 e atribui a cada
   um a menor cor que nenhum vizinho ja visitado esteja usando. Devolve um
   vetor alocado (tamanho g->n) com a cor de cada vertice e escreve em
   *num_cores a quantidade de cores usadas. */
int *coloracao_gulosa(GrafoLista *g, int *num_cores);

/* Heuristica de Welsh-Powell: ordena os vertices por grau decrescente antes
   de aplicar o algoritmo guloso, tendendo a usar menos cores. Mesmo
   contrato de retorno de coloracao_gulosa. */
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);

/* Confere se a coloracao dada e valida (nenhum par de vertices adjacentes
   com a mesma cor). */
int validar_coloracao(GrafoLista *g, int *cores);

/* Verifica se o grafo e bipartido via BFS de 2-coloracao. */
int eh_bipartido(GrafoLista *g);

#endif
