#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

static void testar_bfs(GrafoLista *g, int origem) {
    int *dist = malloc(g->n * sizeof(int));
    int *pred = malloc(g->n * sizeof(int));

    bfs(g, origem, dist, pred);

    printf("BFS a partir do vertice %d:\n", origem);
    for (int v = 0; v < g->n; v++) {
        printf("  vertice %d | dist = ", v);
        if (dist[v] == INFINITO) {
            printf("inalcancavel\n");
        } else {
            printf("%d | caminho: ", dist[v]);
            imprimir_caminho(origem, v, pred);
            printf("\n");
        }
    }

    free(dist);
    free(pred);
}

static void testar_dfs(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = malloc(g->n * sizeof(int));
    int *saida = malloc(g->n * sizeof(int));
    int relogio = 0;

    for (int v = 0; v < g->n; v++) {
        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado, entrada, saida, &relogio);
        }
    }

    printf("DFS recursiva (tempos de entrada/saida):\n");
    for (int v = 0; v < g->n; v++) {
        printf("  vertice %d | entrada = %d | saida = %d\n", v, entrada[v], saida[v]);
    }

    free(visitado);
    free(entrada);
    free(saida);
}

static void analisar(const char *nome, GrafoLista *g) {
    printf("\n===== %s =====\n", nome);
    printf("Componentes conexos: %d\n", contar_componentes(g));
    printf("Possui ciclo? %s\n", tem_ciclo(g) ? "sim" : "nao");
    printf("Eh bipartido? %s\n", eh_bipartido(g) ? "sim" : "nao");
}

int main(void) {
    /* Grafo 1: dois componentes, com ciclo (0-1-2) e um triangulo impar */
    GrafoLista *g1 = criar_grafo_lista(7);
    inserir_aresta_lista(g1, 0, 1);
    inserir_aresta_lista(g1, 0, 2);
    inserir_aresta_lista(g1, 1, 2);
    inserir_aresta_lista(g1, 2, 3);
    inserir_aresta_lista(g1, 4, 5);
    /* vertice 6 isolado */

    analisar("Grafo 1 (com triangulo)", g1);
    testar_bfs(g1, 0);
    testar_dfs(g1);

    /* Grafo 2: ciclo par C4, conexo e bipartido */
    GrafoLista *g2 = criar_grafo_lista(4);
    inserir_aresta_lista(g2, 0, 1);
    inserir_aresta_lista(g2, 1, 2);
    inserir_aresta_lista(g2, 2, 3);
    inserir_aresta_lista(g2, 3, 0);

    analisar("Grafo 2 (ciclo par C4)", g2);
    testar_bfs(g2, 0);

    /* Grafo 3: arvore, sem ciclo e bipartida */
    GrafoLista *g3 = criar_grafo_lista(5);
    inserir_aresta_lista(g3, 0, 1);
    inserir_aresta_lista(g3, 0, 2);
    inserir_aresta_lista(g3, 1, 3);
    inserir_aresta_lista(g3, 1, 4);

    analisar("Grafo 3 (arvore)", g3);
    testar_dfs(g3);

    liberar_grafo_lista(g1);
    liberar_grafo_lista(g2);
    liberar_grafo_lista(g3);

    return 0;
}
