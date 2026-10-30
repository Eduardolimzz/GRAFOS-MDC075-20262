#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void analisar(const char *nome, GrafoListaPonderado *g, int origem) {
    int *dist = malloc((size_t)g->n * sizeof(*dist));
    int *pred = malloc((size_t)g->n * sizeof(*pred));
    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        free(dist);
        free(pred);
        liberar_grafo_lista_ponderado(g);
        exit(EXIT_FAILURE);
    }

    printf("\n===== %s =====\n", nome);
    exibir_grafo_lista_ponderado(g);
    printf("Origem: %d\n", origem);
    if (bellman_ford(g, origem, dist, pred)) {
        printf("Sem ciclo negativo alcancavel.\nVertice | Distancia | Predecessor\n");
        for (int v = 0; v < g->n; v++) {
            if (dist[v] == DISTANCIA_INFINITA) {
                printf("%7d |       INF | %d\n", v, pred[v]);
            } else {
                printf("%7d | %9d | %d\n", v, dist[v], pred[v]);
            }
        }
    } else {
        printf("Ha ciclo negativo alcancavel; nao existem menores caminhos finitos para os vertices afetados.\n");
        if (!reconstruir_ciclo_negativo(g, pred)) {
            fprintf(stderr, "Erro ao reconstruir o ciclo.\n");
            free(dist);
            free(pred);
            liberar_grafo_lista_ponderado(g);
            exit(EXIT_FAILURE);
        }
    }
    free(dist);
    free(pred);
    liberar_grafo_lista_ponderado(g);
}

int main(void) {
    /* Pesos negativos sem ciclo negativo. Distancias: 0, 2, 7, 4, -2. */
    GrafoListaPonderado *g = criar_grafo_lista_ponderado(5);
    inserir_arco_ponderado(g, 0, 1, 6);
    inserir_arco_ponderado(g, 0, 2, 7);
    inserir_arco_ponderado(g, 1, 2, 8);
    inserir_arco_ponderado(g, 1, 3, 5);
    inserir_arco_ponderado(g, 1, 4, -4);
    inserir_arco_ponderado(g, 2, 3, -3);
    inserir_arco_ponderado(g, 2, 4, 9);
    inserir_arco_ponderado(g, 3, 1, -2);
    inserir_arco_ponderado(g, 4, 0, 2);
    inserir_arco_ponderado(g, 4, 3, 7);
    analisar("Pesos negativos sem ciclo negativo", g, 0);

    /* Ciclo 1 -> 2 -> 3 -> 1, custo -2, com uma saida para 4. */
    g = criar_grafo_lista_ponderado(5);
    inserir_arco_ponderado(g, 0, 1, 1);
    inserir_arco_ponderado(g, 1, 2, 1);
    inserir_arco_ponderado(g, 2, 3, 1);
    inserir_arco_ponderado(g, 3, 1, -4);
    inserir_arco_ponderado(g, 3, 4, 2);
    analisar("Ciclo negativo alcancavel com saida", g, 0);

    /* O ciclo 2 -> 3 -> 2 nao e alcancavel a partir de 0. */
    g = criar_grafo_lista_ponderado(5);
    inserir_arco_ponderado(g, 0, 1, 3);
    inserir_arco_ponderado(g, 2, 3, -2);
    inserir_arco_ponderado(g, 3, 2, 1);
    analisar("Ciclo desconectado e vertices inalcançaveis", g, 0);

    /* Com n = 1, a propria primeira passagem ja e a passagem de deteccao. */
    g = criar_grafo_lista_ponderado(1);
    inserir_arco_ponderado(g, 0, 0, -1);
    analisar("Laco negativo em um unico vertice", g, 0);
    return 0;
}
