#ifndef AGM_H
#define AGM_H

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

typedef struct {
    Aresta *arestas;
    int quantidade;
    int componentes;
    long long peso_total;
} AGM;

/* Requer n >= 0 e indices em [0, n) em find/unir.
   Cada conjunto comeca com um unico vertice e rank zero. */
DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
int find(DSU *d, int x);
/* Retorna 1 se uniu conjuntos distintos, ou 0 se ja estavam unidos. */
int unir(DSU *d, int x, int y);

/* Grafo NAO orientado: cada aresta deve ser informada apenas uma vez.
   Requer n >= 0, m >= 0, extremos em [0, n) e vetor com m arestas
   (pode ser NULL quando m == 0). Aceita pesos negativos, lacos e paralelas.
   Nenhum dos algoritmos altera o vetor de entrada.

   Em grafos desconectados, retorna uma FLORESTA geradora minima,
   com n - componentes arestas. O grafo vazio tem zero componentes.
   O chamador deve liberar o resultado com liberar_agm(). */

/* Ordenacao das arestas + Union-Find: O(E log E), alem de O(V)
   para inicializar os conjuntos e armazenar o resultado. */
AGM kruskal(int n, const Aresta *arestas, int m);

/* Lista de adjacencia + min-heap indexado com decrease-key:
   O((V + E) log V), com armazenamento O(V + E). */
AGM prim(int n, const Aresta *arestas, int m);

void liberar_agm(AGM *resultado);

#endif
