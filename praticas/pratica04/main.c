#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"
#include "planaridade.h"

static void analisar(const char *nome, GrafoLista *g) {
    printf("\n===== %s =====\n", nome);
    printf("Lista de adjacencia:\n");
    exibir_grafo_lista(g);

    int *articulacao = detectar_articulacoes(g);
    printf("Vertices de articulacao:");
    int nenhuma = 1;
    for (int v = 0; v < g->n; v++) {
        if (articulacao[v]) {
            printf(" %d", v);
            nenhuma = 0;
        }
    }
    if (nenhuma) {
        printf(" nenhum");
    }
    printf("\n");
    free(articulacao);

    printf("Pontes:\n");
    int total_pontes = detectar_pontes(g);
    if (total_pontes == 0) {
        printf("  nenhuma\n");
    }

    printf("Arestas: %d, Vertices: %d\n", contar_arestas(g), g->n);
    printf("Planar pela formula de Euler (m <= 3n-6)? %s\n",
           eh_planar_euler(g) ? "sim" : "nao");
    printf("Contem K5 ou K3,3 (heuristica de Kuratowski)? %s\n",
           contem_k5_ou_k33(g) ? "sim" : "nao");
    printf("Eh planar? %s\n", eh_planar(g) ? "sim" : "nao");
}

int main(void) {
    /* Grafo 1: dois triangulos ligados por uma ponte (2-3), com o vertice 2
       sendo uma articulacao. */
    GrafoLista *g1 = criar_grafo_lista(6);
    inserir_aresta_lista(g1, 0, 1);
    inserir_aresta_lista(g1, 1, 2);
    inserir_aresta_lista(g1, 0, 2);
    inserir_aresta_lista(g1, 2, 3);
    inserir_aresta_lista(g1, 3, 4);
    inserir_aresta_lista(g1, 4, 5);
    inserir_aresta_lista(g1, 3, 5);

    analisar("Grafo 1 (dois triangulos unidos por uma ponte)", g1);

    /* Grafo 2: K5, nao planar. */
    GrafoLista *g2 = criar_grafo_lista(5);
    for (int u = 0; u < 5; u++) {
        for (int v = u + 1; v < 5; v++) {
            inserir_aresta_lista(g2, u, v);
        }
    }

    analisar("Grafo 2 (K5)", g2);

    /* Grafo 3: K3,3, nao planar. */
    GrafoLista *g3 = criar_grafo_lista(6);
    for (int a = 0; a < 3; a++) {
        for (int b = 3; b < 6; b++) {
            inserir_aresta_lista(g3, a, b);
        }
    }

    analisar("Grafo 3 (K3,3)", g3);

    liberar_grafo_lista(g1);
    liberar_grafo_lista(g2);
    liberar_grafo_lista(g3);

    return 0;
}
