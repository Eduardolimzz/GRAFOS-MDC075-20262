#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include "conectividade.h"

/* Conta o numero de arestas (nao orientadas) do grafo. */
int contar_arestas(GrafoLista *g);

/* Condicao necessaria de Euler para planaridade: m <= 3n - 6 (n >= 3). */
int eh_planar_euler(GrafoLista *g);

/* Para n <= 10, busca por forca bruta um subgrafo completo K5 ou um
   subgrafo bipartido completo K3,3 -- se existir, o grafo nao e planar
   (heuristica de Kuratowski). Retorna 1 se encontrou algum dos dois. */
int contem_k5_ou_k33(GrafoLista *g);

/* Combina o teste de Euler com a heuristica de Kuratowski (quando n <= 10)
   para decidir se o grafo e planar. */
int eh_planar(GrafoLista *g);

#endif
