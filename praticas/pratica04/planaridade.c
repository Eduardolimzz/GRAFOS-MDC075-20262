#include <stdio.h>
#include "planaridade.h"

static int adjacente(GrafoLista *g, int u, int v) {
    for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        if (atual->destino == v) {
            return 1;
        }
    }
    return 0;
}

int contar_arestas(GrafoLista *g) {
    int arestas = 0;
    for (int u = 0; u < g->n; u++) {
        for (No *atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            if (atual->destino > u) {
                arestas++;
            }
        }
    }
    return arestas;
}

int eh_planar_euler(GrafoLista *g) {
    if (g->n < 3) {
        return 1;
    }
    return contar_arestas(g) <= 3 * g->n - 6;
}

/* Testa, por forca bruta, se existem 5 vertices em que todos os pares sao
   adjacentes (subgrafo K5). */
static int existe_k5(GrafoLista *g) {
    int n = g->n;
    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            if (!adjacente(g, a, b)) continue;
            for (int c = b + 1; c < n; c++) {
                if (!adjacente(g, a, c) || !adjacente(g, b, c)) continue;
                for (int d = c + 1; d < n; d++) {
                    if (!adjacente(g, a, d) || !adjacente(g, b, d) || !adjacente(g, c, d)) continue;
                    for (int e = d + 1; e < n; e++) {
                        if (adjacente(g, a, e) && adjacente(g, b, e) &&
                            adjacente(g, c, e) && adjacente(g, d, e)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Testa, por forca bruta, se existem dois conjuntos disjuntos de 3 vertices
   com todas as arestas entre os conjuntos presentes (subgrafo K3,3). */
static int existe_k33(GrafoLista *g) {
    int n = g->n;
    for (int a0 = 0; a0 < n; a0++) {
        for (int a1 = a0 + 1; a1 < n; a1++) {
            for (int a2 = a1 + 1; a2 < n; a2++) {
                int lado_a[3] = {a0, a1, a2};

                for (int b0 = 0; b0 < n; b0++) {
                    if (b0 == a0 || b0 == a1 || b0 == a2) continue;
                    for (int b1 = b0 + 1; b1 < n; b1++) {
                        if (b1 == a0 || b1 == a1 || b1 == a2) continue;
                        for (int b2 = b1 + 1; b2 < n; b2++) {
                            if (b2 == a0 || b2 == a1 || b2 == a2) continue;
                            int lado_b[3] = {b0, b1, b2};

                            int completo = 1;
                            for (int i = 0; i < 3 && completo; i++) {
                                for (int j = 0; j < 3 && completo; j++) {
                                    if (!adjacente(g, lado_a[i], lado_b[j])) {
                                        completo = 0;
                                    }
                                }
                            }
                            if (completo) {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int contem_k5_ou_k33(GrafoLista *g) {
    if (g->n > 10) {
        return 0;
    }
    return existe_k5(g) || existe_k33(g);
}

int eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g)) {
        return 0;
    }
    if (g->n <= 10 && contem_k5_ou_k33(g)) {
        return 0;
    }
    return 1;
}
