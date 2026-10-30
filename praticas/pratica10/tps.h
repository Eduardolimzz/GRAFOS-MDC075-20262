#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

typedef struct {
    int n;
    int *pesos; /* Matriz simetrica de custos de um grafo completo. */
} TSP;

/* Requer n >= 0, pesos nao negativos e indices em [0, n).
   Inicializa custos entre vertices diferentes com 1 e diagonal com 0.
   definir_peso_tsp atualiza os dois sentidos e preserva diagonal zero. */
TSP *criar_tsp(int n);
void definir_peso_tsp(TSP *t, int u, int v, int peso);
void liberar_tsp(TSP *t);

/* O chamador fornece rota[n + 1]. Retornam o custo em long long e
   preenchem uma permutacao com a origem repetida no fim. Nao alteram t.
   Para n == 0 retornam 0, nao escrevem a rota e ignoram a origem.
   Para n == 1 retornam a rota origem -> origem, com custo zero. */

/* Origem fixa: examina todas as (n - 1)! permutacoes, sem poda por custo.
   Custos parciais acumulados evitam recalcular a soma de cada rota. */
long long tsp_forca_bruta(const TSP *t, int origem, int *rota);

/* Proximo vertice nao visitado de menor custo; desempate pelo indice.
   Custo O(n^2). */
long long tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota);

/* Escolhe o vertice mais proximo de algum vertice do ciclo atual;
   insere na posicao com menor aumento do custo. Desempata por menor
   vertice e primeira posicao. Mantem distancias ao ciclo: O(n^2). */
long long tsp_insercao(const TSP *t, int origem, int *rota);

/* Reducao para TSP completo: aresta existente custa 1; inexistente, 2.
   Diagonal zero. Para n >= 3, ha ciclo Hamiltoniano se e somente se o
   custo otimo do TSP e n. O chamador libera com liberar_tsp(). */
TSP *grafo_para_tsp(const Grafo *g);

#endif
