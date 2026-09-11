#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);

/* Insere o arco orientado u -> v (digrafo). */
void inserir_arco_lista(GrafoLista *g, int u, int v);

int grau_saida_lista(GrafoLista *g, int v);
int grau_entrada_lista(GrafoLista *g, int v);
void exibir_grafo_lista(GrafoLista *g);
void liberar_grafo_lista(GrafoLista *g);

#endif
