#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

static void imprimir_cores(int *cor, int n) {
    for (int v = 0; v < n; v++) {
        printf("%d%s", cor[v], v + 1 < n ? " " : "\n");
    }
}

static void analisar(const char *nome, GrafoLista *g) {
    printf("\n===== %s =====\n", nome);
    printf("Lista de adjacencia:\n");
    exibir_grafo_lista(g);

    int num_cores_gulosa = 0;
    int *cor_gulosa = coloracao_gulosa(g, &num_cores_gulosa);
    printf("Coloracao gulosa (ordem 0..n-1): ");
    imprimir_cores(cor_gulosa, g->n);
    printf("  cores usadas: %d, valida? %s\n", num_cores_gulosa,
           validar_coloracao(g, cor_gulosa) ? "sim" : "nao");

    int num_cores_wp = 0;
    int *cor_wp = coloracao_welsh_powell(g, &num_cores_wp);
    printf("Coloracao Welsh-Powell: ");
    imprimir_cores(cor_wp, g->n);
    printf("  cores usadas: %d, valida? %s\n", num_cores_wp,
           validar_coloracao(g, cor_wp) ? "sim" : "nao");

    printf("Eh bipartido? %s\n", eh_bipartido(g) ? "sim" : "nao");

    free(cor_gulosa);
    free(cor_wp);
}

int main(void) {
    /* Grafo 1: ciclo par (bipartido, numero cromatico 2). */
    GrafoLista *ciclo_par = criar_grafo_lista(6);
    for (int v = 0; v < 6; v++) {
        inserir_aresta_lista(ciclo_par, v, (v + 1) % 6);
    }
    analisar("Grafo 1 (ciclo de 6 vertices)", ciclo_par);

    /* Grafo 2: ciclo impar (nao bipartido, numero cromatico 3). */
    GrafoLista *ciclo_impar = criar_grafo_lista(5);
    for (int v = 0; v < 5; v++) {
        inserir_aresta_lista(ciclo_impar, v, (v + 1) % 5);
    }
    analisar("Grafo 2 (ciclo de 5 vertices)", ciclo_impar);

    /* Grafo 3: grafo em que a ordem dos vertices importa -- a coloracao
       gulosa na ordem natural usa mais cores que a Welsh-Powell. */
    GrafoLista *estrela_dupla = criar_grafo_lista(6);
    inserir_aresta_lista(estrela_dupla, 0, 1);
    inserir_aresta_lista(estrela_dupla, 0, 2);
    inserir_aresta_lista(estrela_dupla, 0, 3);
    inserir_aresta_lista(estrela_dupla, 1, 4);
    inserir_aresta_lista(estrela_dupla, 1, 5);
    inserir_aresta_lista(estrela_dupla, 2, 4);
    inserir_aresta_lista(estrela_dupla, 3, 5);
    analisar("Grafo 3 (graus variados)", estrela_dupla);

    /* Grafo 4: K4 completo (numero cromatico 4). */
    GrafoLista *k4 = criar_grafo_lista(4);
    for (int u = 0; u < 4; u++) {
        for (int v = u + 1; v < 4; v++) {
            inserir_aresta_lista(k4, u, v);
        }
    }
    analisar("Grafo 4 (K4)", k4);

    liberar_grafo_lista(ciclo_par);
    liberar_grafo_lista(ciclo_impar);
    liberar_grafo_lista(estrela_dupla);
    liberar_grafo_lista(k4);

    return 0;
}
