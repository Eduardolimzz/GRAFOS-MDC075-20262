#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int u, v;
} Aresta;

typedef struct {
    int n, m;
    const Aresta *arestas;
} Grafo;

/* Grafo nao orientado: informe cada aresta uma vez. Requer n, m >= 0,
   extremos em [0, n) e vetor com m arestas (NULL permitido se m == 0).
   Aceita arestas paralelas e lacos. O grafo nao e alterado.

   O chamador fornece cobertura[] com n posicoes. Os algoritmos preenchem
   o vetor com 0/1 e retornam a quantidade de vertices selecionados.
   Para n == 0, cobertura pode ser NULL; o resultado e o conjunto vazio. */

/* Solucao exata por backtracking: ramifica nos extremos de uma aresta
   descoberta. Usa a gulosa como limite superior e um emparelhamento das
   arestas descobertas como limite inferior. Custo exponencial no pior caso. */
int cobertura_minima_bt(const Grafo *g, int *cobertura);

/* Seleciona o vertice com mais arestas ainda descobertas; em empate,
   escolhe o menor indice. Recalcula os graus a cada escolha.
   Um laco contribui uma vez e obriga a escolha do proprio vertice.
   Custo O(V * (V + E)); nao garante cardinalidade minima. */
int cobertura_gulosa(const Grafo *g, int *cobertura);

/* Retorna 1 se toda aresta tem ao menos um extremo selecionado, ou 0
   caso contrario. Neste verificador, qualquer valor nao zero seleciona. */
int eh_cobertura(const Grafo *g, const int *cobertura);

#endif
