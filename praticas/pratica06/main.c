#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

static void testar_heap(void) {
    printf(" Teste do Min-Heap \n");
    MinHeap *h = criar_min_heap(5);
    inserir_heap(h, 0, 50);
    inserir_heap(h, 1, 30);
    inserir_heap(h, 2, 40);
    inserir_heap(h, 3, 10);
    inserir_heap(h, 4, 20);

    printf("Inseridos: v0=50 v1=30 v2=40 v3=10 v4=20\n");
    diminuir_chave(h, 0, 5);
    printf("diminuir_chave(v0, 5)\n");

    printf("Extraindo em ordem:");
    while (!heap_vazio(h)) {
        NoHeap no = extrair_minimo(h);
        printf(" v%d(%d)", no.vertice, no.distancia);
    }
    printf("\n");

    liberar_min_heap(h);
}

static void analisar(const char *nome, GrafoListaPonderado *g, int origem) {
    printf("\n %s \n", nome);
    printf("Lista de adjacencia (destino(peso)):\n");
    exibir_grafo_ponderado(g);

    int *dist = malloc(g->n * sizeof(int));
    int *pred = malloc(g->n * sizeof(int));
    dijkstra(g, origem, dist, pred);

    printf("Menores distancias a partir de %d:\n", origem);
    for (int v = 0; v < g->n; v++) {
        if (dist[v] == INFINITO) {
            printf("  %d: inf   caminho: ", v);
        } else {
            printf("  %d: %-4d  caminho: ", v, dist[v]);
        }
        reconstruir_caminho(origem, v, pred);
    }

    free(dist);
    free(pred);
}

int main(void) {
    testar_heap();

    /* Grafo 1: exemplo classico. O caminho direto 0 -> 5 custa 14, mas
       0 -> 2 -> 5 custa so 11, e Dijkstra deve encontrar isso. */
    GrafoListaPonderado *g1 = criar_grafo_ponderado(6);
    inserir_arco_ponderado(g1, 0, 1, 7);
    inserir_arco_ponderado(g1, 0, 2, 9);
    inserir_arco_ponderado(g1, 0, 5, 14);
    inserir_arco_ponderado(g1, 1, 2, 10);
    inserir_arco_ponderado(g1, 1, 3, 15);
    inserir_arco_ponderado(g1, 2, 3, 11);
    inserir_arco_ponderado(g1, 2, 5, 2);
    inserir_arco_ponderado(g1, 3, 4, 6);
    inserir_arco_ponderado(g1, 5, 4, 9);

    analisar("Grafo 1 (exemplo classico)", g1, 0);

    /* Grafo 2: o vertice 4 nao e alcancavel a partir da origem. */
    GrafoListaPonderado *g2 = criar_grafo_ponderado(5);
    inserir_arco_ponderado(g2, 0, 1, 4);
    inserir_arco_ponderado(g2, 0, 2, 1);
    inserir_arco_ponderado(g2, 2, 1, 2);
    inserir_arco_ponderado(g2, 1, 3, 1);
    inserir_arco_ponderado(g2, 4, 0, 3);

    analisar("Grafo 2 (vertice 4 inalcancavel)", g2, 0);

    liberar_grafo_ponderado(g1);
    liberar_grafo_ponderado(g2);

    return 0;
}
