#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define DISTANCIA_INFINITA INT_MAX

typedef struct NoPonderado {
    int destino;
    int peso;
    struct NoPonderado *prox;
} NoPonderado;

typedef struct {
    int n;
    NoPonderado **adj;
    int vertice_ciclo; /* Ultimo vertice relaxado na n-esima passagem. */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_lista_ponderado(int n);
int inserir_arco_ponderado(GrafoListaPonderado *g, int u, int v, int peso);
void exibir_grafo_lista_ponderado(const GrafoListaPonderado *g);
void liberar_grafo_lista_ponderado(GrafoListaPonderado *g);

/* Requer grafo valido, origem em [0, n) e vetores com n posicoes.
   Retorna 1 se nao houver ciclo negativo ALCANCAVEL a partir da origem,
   ou 0 se houver. pred[v] = -1 indica ausencia de predecessor.
   Distancias inalcançaveis recebem DISTANCIA_INFINITA. O calculo interno
   usa long long; resultados fora da faixa int sao limitados a INT_MIN
   ou INT_MAX - 1, preservando o sentinela de infinito.
   Na presenca de ciclo negativo, dist[] nao representa menores caminhos;
   pred[] e vertice_ciclo permitem reconstruir um ciclo. */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Apos bellman_ford retornar 0, retrocede n vezes por pred[] para entrar
   no ciclo e o imprime na direcao dos arcos. Retorna 1 se imprimiu, ou 0
   quando nao ha ciclo registrado ou os predecessores sao invalidos. */
int reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *pred);

#endif
