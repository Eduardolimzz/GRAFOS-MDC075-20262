#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

static void imprimir(const char *algoritmo, const AGM *r) {
    printf("\n%s:\n", algoritmo);
    for (int i = 0; i < r->quantidade; i++) {
        Aresta a = r->arestas[i];
        printf("  %d -- %d (peso %d)\n", a.u, a.v, a.peso);
    }
    printf("Arestas selecionadas: %d\nComponentes: %d\nPeso total: %lld\n",
           r->quantidade, r->componentes, r->peso_total);
    if (r->componentes > 1) printf("Grafo desconectado: floresta geradora minima.\n");
}

static void analisar(const char *nome, int n, const Aresta *arestas, int m,
                     long long peso_esperado, int componentes_esperadas) {
    printf("\n===== %s =====\nVertices: %d | Arestas: %d\n", nome, n, m);
    AGM k = kruskal(n, arestas, m);
    AGM p = prim(n, arestas, m);
    imprimir("Kruskal", &k);
    imprimir("Prim", &p);

    int correto = k.peso_total == peso_esperado && p.peso_total == peso_esperado
        && k.componentes == componentes_esperadas && p.componentes == componentes_esperadas
        && k.quantidade == n - componentes_esperadas && p.quantidade == n - componentes_esperadas;
    liberar_agm(&k);
    liberar_agm(&p);
    if (!correto) {
        fprintf(stderr, "Erro: resultado diferente do esperado em %s.\n", nome);
        exit(EXIT_FAILURE);
    }
    printf("Verificacao: ambos atingiram o peso minimo esperado (%lld).\n", peso_esperado);
}

int main(void) {
    const Aresta classico[] = {
        {0, 1, 4}, {0, 7, 8}, {1, 2, 8}, {1, 7, 11},
        {2, 3, 7}, {2, 8, 2}, {2, 5, 4}, {3, 4, 9},
        {3, 5, 14}, {4, 5, 10}, {5, 6, 2}, {6, 7, 1},
        {6, 8, 6}, {7, 8, 7}
    };
    analisar("Grafo conexo classico", 9, classico, 14, 37, 1);

    const Aresta negativos[] = {
        {0, 1, 4}, {0, 1, -2}, {1, 2, 0}, {0, 2, 5},
        {2, 3, 3}, {1, 3, 8}, {3, 3, -100}
    };
    analisar("Pesos negativos, paralelas e laco", 4, negativos, 7, 1, 1);

    const Aresta desconectado[] = {{0, 1, 2}, {1, 2, 1}, {0, 2, 5}, {3, 4, -4}};
    analisar("Duas componentes com arestas e um vertice isolado", 6, desconectado, 4, -1, 3);

    const Aresta empates[] = {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 0, 1}, {0, 2, 1}};
    analisar("Empates: pode existir mais de uma AGM", 4, empates, 5, 3, 1);

    const Aresta extremos[] = {{0, 1, INT_MAX}, {1, 2, INT_MAX}};
    analisar("Peso total que ultrapassa int", 3, extremos, 2, 2LL * INT_MAX, 1);
    analisar("Um vertice sem arestas", 1, NULL, 0, 0, 1);
    analisar("Somente vertices isolados", 3, NULL, 0, 0, 3);
    analisar("Grafo vazio", 0, NULL, 0, 0, 0);
    return 0;
}
