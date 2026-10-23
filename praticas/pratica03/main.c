#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "dag.h"

static void imprimir_ordem(const char *rotulo, int *ordem, int tamanho) {
    printf("  %s: ", rotulo);
    if (ordem == NULL) {
        printf("impossivel, o digrafo possui ciclo\n");
        return;
    }
    for (int i = 0; i < tamanho; i++) {
        printf("%d%s", ordem[i], i + 1 < tamanho ? " " : "\n");
    }
}

static void analisar(const char *nome, GrafoLista *g) {
    printf("\n===== %s =====\n", nome);
    printf("Lista de adjacencia:\n");
    exibir_grafo_lista(g);

    printf("Eh DAG? %s\n", eh_dag(g) ? "sim" : "nao");

    int tam_kahn = 0;
    int *ordem_kahn = ordenacao_topologica_kahn(g, &tam_kahn);
    imprimir_ordem("Kahn", ordem_kahn, tam_kahn);
    if (ordem_kahn != NULL) {
        printf("  ordem valida? %s\n",
               validar_ordenacao(g, ordem_kahn, tam_kahn) ? "sim" : "nao");
    }

    int tam_dfs = 0;
    int *ordem_dfs = ordenacao_topologica_dfs(g, &tam_dfs);
    imprimir_ordem("DFS ", ordem_dfs, tam_dfs);
    if (ordem_dfs != NULL) {
        printf("  ordem valida? %s\n",
               validar_ordenacao(g, ordem_dfs, tam_dfs) ? "sim" : "nao");
    }

    free(ordem_kahn);
    free(ordem_dfs);
}

int main(void) {
    /* DAG 1: pre-requisitos de disciplinas.
       0 -> 1 -> 3 -> 5 e 0 -> 2 -> 4 -> 5 */
    GrafoLista *dag = criar_grafo_lista(6);
    inserir_arco_lista(dag, 0, 1);
    inserir_arco_lista(dag, 0, 2);
    inserir_arco_lista(dag, 1, 3);
    inserir_arco_lista(dag, 2, 4);
    inserir_arco_lista(dag, 3, 5);
    inserir_arco_lista(dag, 4, 5);

    analisar("Digrafo 1 (DAG de pre-requisitos)", dag);

    /* Digrafo 2: mesmo grafo com o arco 5 -> 0, fechando um ciclo. */
    GrafoLista *ciclico = criar_grafo_lista(6);
    inserir_arco_lista(ciclico, 0, 1);
    inserir_arco_lista(ciclico, 0, 2);
    inserir_arco_lista(ciclico, 1, 3);
    inserir_arco_lista(ciclico, 2, 4);
    inserir_arco_lista(ciclico, 3, 5);
    inserir_arco_lista(ciclico, 4, 5);
    inserir_arco_lista(ciclico, 5, 0);

    analisar("Digrafo 2 (com ciclo 0-1-3-5-0)", ciclico);

    /* DAG 3: desconexo, com dois vertices isolados. */
    GrafoLista *desconexo = criar_grafo_lista(5);
    inserir_arco_lista(desconexo, 0, 1);
    inserir_arco_lista(desconexo, 2, 3);

    analisar("Digrafo 3 (DAG desconexo)", desconexo);

    printf("\nGraus do vertice 5 no digrafo 1: entrada = %d, saida = %d\n",
           grau_entrada_lista(dag, 5), grau_saida_lista(dag, 5));

    liberar_grafo_lista(dag);
    liberar_grafo_lista(ciclico);
    liberar_grafo_lista(desconexo);

    return 0;
}
