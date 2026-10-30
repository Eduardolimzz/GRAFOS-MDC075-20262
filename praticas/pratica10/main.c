#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static void exigir(int condicao, const char *mensagem) {
    if (!condicao) {
        fprintf(stderr, "Erro: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}

static void *alocar(size_t tamanho) {
    if (tamanho == 0) return NULL;
    void *memoria = malloc(tamanho);
    exigir(memoria != NULL, "memoria insuficiente.");
    return memoria;
}

static void imprimir_ordem(const int *ordem, int tamanho) {
    for (int i = 0; i < tamanho; i++) printf("%d%s", ordem[i], i + 1 < tamanho ? " -> " : "\n");
}

static void verificar_permutacao(int n, const int *ordem) {
    unsigned char *usado = alocar((size_t)n * sizeof(*usado));
    for (int v = 0; v < n; v++) usado[v] = 0;
    for (int i = 0; i < n; i++) {
        int v = ordem[i];
        exigir(v >= 0 && v < n, "vertice fora do grafo.");
        exigir(!usado[v], "vertice repetido na permutacao.");
        usado[v] = 1;
    }
    free(usado);
}

static void verificar_hamiltoniano(const Grafo *g, const int *ordem, int ciclo) {
    verificar_permutacao(g->n, ordem);
    if (ciclo) exigir(ordem[g->n] == ordem[0], "ciclo sem fechamento.");
    int tamanho = g->n + ciclo;
    for (int i = 1; i < tamanho; i++) {
        exigir(g->adj[(size_t)ordem[i - 1] * g->n + ordem[i]], "aresta inexistente no percurso.");
    }
}

static void verificar_tsp(const TSP *t, int origem, const int *rota, long long total) {
    if (t->n == 0) {
        exigir(total == 0, "grafo vazio com custo nao zero.");
        return;
    }
    verificar_permutacao(t->n, rota);
    exigir(rota[0] == origem && rota[t->n] == origem, "origem incorreta na rota.");
    long long soma = 0;
    for (int i = 0; i < t->n; i++) soma += t->pesos[(size_t)rota[i] * t->n + rota[i + 1]];
    exigir(soma == total, "custo informado diferente da soma da rota.");
}

static void analisar_grafo(const char *nome, Grafo *g, int caminho_esperado, int ciclo_esperado) {
    int *ordem = alocar(((size_t)g->n + 1) * sizeof(*ordem));
    printf("\n===== %s =====\n", nome);
    int caminho = encontrar_caminho_hamiltoniano_bt(g, ordem);
    exigir(caminho == caminho_esperado, "resultado inesperado para caminho Hamiltoniano.");
    printf("Caminho Hamiltoniano: ");
    if (caminho) {
        verificar_hamiltoniano(g, ordem, 0);
        imprimir_ordem(ordem, g->n);
    } else printf("nao existe.\n");

    int ciclo = encontrar_ciclo_hamiltoniano_bt(g, ordem);
    exigir(ciclo == ciclo_esperado, "resultado inesperado para ciclo Hamiltoniano.");
    printf("Ciclo Hamiltoniano: ");
    if (ciclo) {
        verificar_hamiltoniano(g, ordem, 1);
        imprimir_ordem(ordem, g->n + 1);
    } else printf("nao existe.\n");

    if (g->n >= 3) {
        TSP *t = grafo_para_tsp(g);
        long long otimo = tsp_forca_bruta(t, 0, ordem);
        verificar_tsp(t, 0, ordem, otimo);
        exigir((otimo == g->n) == ciclo, "reducao Hamiltoniano -> TSP inconsistente.");
        printf("Reducao 1/2: custo otimo = %lld; custo igual a n indica ciclo Hamiltoniano.\n", otimo);
        liberar_tsp(t);
    }
    free(ordem);
    liberar_grafo(g);
}

typedef long long (*AlgoritmoTSP)(const TSP *, int, int *);

static void analisar_tsp(const char *nome, TSP *t, long long esperado) {
    const char *nomes[] = {"Forca bruta", "Vizinho mais proximo", "Insercao do mais proximo"};
    AlgoritmoTSP algoritmos[] = {tsp_forca_bruta, tsp_vizinho_mais_proximo, tsp_insercao};
    int *rota = alocar(((size_t)t->n + 1) * sizeof(*rota));
    printf("\n===== %s =====\n", nome);
    for (int i = 0; i < 3; i++) {
        long long total = algoritmos[i](t, 0, rota);
        verificar_tsp(t, 0, rota, total);
        exigir(i == 0 ? total == esperado : total >= esperado, "custo TSP inesperado.");
        printf("%s | custo %lld | ", nomes[i], total);
        if (t->n > 0) imprimir_ordem(rota, t->n + 1);
        else printf("rota vazia.\n");
    }
    free(rota);
    liberar_tsp(t);
}

/* Lotes de chamadas acumulam ao menos 20 ms de CPU para reduzir o efeito
   da resolucao do relogio nos casos pequenos. Casos caros usam uma chamada.
   Inclui alocacoes dos algoritmos; exclui geracao, validacao e impressao. */
static double medir(const TSP *t, int *rota, AlgoritmoTSP algoritmo, long long *total) {
    int repeticoes = 1;
    while (1) {
        clock_t inicio = clock();
        for (int i = 0; i < repeticoes; i++) *total = algoritmo(t, 0, rota);
        clock_t fim = clock();
        exigir(inicio != (clock_t)-1 && fim != (clock_t)-1, "relogio de CPU indisponivel.");
        double segundos = (double)(fim - inicio) / CLOCKS_PER_SEC;
        if (segundos >= 0.02 || repeticoes >= 1048576) return segundos * 1000.0 / repeticoes;
        repeticoes *= 2;
    }
}

static void experimentar(void) {
    const int tamanhos[] = {5, 8, 10, 12};
    printf("\n===== Experimento TSP: custo e tempo medio de CPU =====\n");
    printf("Grafos completos simetricos, custos de 1 a 99 e semente fixa por n.\n");
    printf("Mesma matriz e origem 0 nos tres metodos; tempos em milissegundos por chamada.\n");
    printf(" n | Permutacoes | Exato custo / ms     | Vizinho custo / ms   | Insercao custo / ms\n");
    for (int i = 0; i < 4; i++) {
        int n = tamanhos[i];
        TSP *t = criar_tsp(n);
        uint32_t estado = 2026u + (uint32_t)n;
        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                estado = 1664525u * estado + 1013904223u;
                definir_peso_tsp(t, u, v, 1 + (int)(estado % 99u));
            }
        }
        int *rota = alocar(((size_t)n + 1) * sizeof(*rota));
        long long exato, vizinho, insercao;
        double tb = medir(t, rota, tsp_forca_bruta, &exato);
        verificar_tsp(t, 0, rota, exato);
        double tv = medir(t, rota, tsp_vizinho_mais_proximo, &vizinho);
        verificar_tsp(t, 0, rota, vizinho);
        double ti = medir(t, rota, tsp_insercao, &insercao);
        verificar_tsp(t, 0, rota, insercao);
        exigir(vizinho >= exato && insercao >= exato, "heuristica com custo menor que o otimo.");
        unsigned long long permutacoes = 1;
        for (int k = 2; k < n; k++) permutacoes *= (unsigned)k;
        printf("%2d | %11llu | %5lld / %11.6f | %7lld / %9.6f | %8lld / %9.6f\n",
               n, permutacoes, exato, tb, vizinho, tv, insercao, ti);
        fflush(stdout);
        free(rota);
        liberar_tsp(t);
    }
    printf("A busca exata e fatorial; as heuristicas sao quadraticas e podem custar mais.\n");
    printf("Os tempos variam com a maquina, as opcoes de compilacao e a matriz de custos.\n");
}

int main(void) {
    Grafo *g = criar_grafo(5);
    for (int v = 0; v < 5; v++) inserir_aresta(g, v, (v + 1) % 5);
    analisar_grafo("Ciclo com cinco vertices", g, 1, 1);

    g = criar_grafo(5);
    inserir_aresta(g, 1, 2);
    inserir_aresta(g, 2, 0);
    inserir_aresta(g, 0, 3);
    inserir_aresta(g, 3, 4);
    analisar_grafo("Caminho que nao pode comecar em 0", g, 1, 0);

    g = criar_grafo(5);
    for (int v = 1; v < 5; v++) inserir_aresta(g, 0, v);
    analisar_grafo("Estrela sem caminho Hamiltoniano", g, 0, 0);
    g = criar_grafo(4);
    inserir_aresta(g, 0, 1);
    inserir_aresta(g, 2, 3);
    analisar_grafo("Grafo desconectado", g, 0, 0);
    analisar_grafo("Um vertice", criar_grafo(1), 1, 0);
    analisar_grafo("Grafo vazio", criar_grafo(0), 0, 0);

    TSP *t = criar_tsp(4);
    definir_peso_tsp(t, 0, 1, 10);
    definir_peso_tsp(t, 0, 2, 15);
    definir_peso_tsp(t, 0, 3, 20);
    definir_peso_tsp(t, 1, 2, 35);
    definir_peso_tsp(t, 1, 3, 25);
    definir_peso_tsp(t, 2, 3, 30);
    analisar_tsp("TSP com otimo conhecido de 80", t, 80);
    t = criar_tsp(2);
    definir_peso_tsp(t, 0, 1, INT_MAX);
    analisar_tsp("Custo total que ultrapassa int", t, 2LL * INT_MAX);
    analisar_tsp("TSP com um vertice", criar_tsp(1), 0);
    analisar_tsp("TSP vazio", criar_tsp(0), 0);
    experimentar();
    return 0;
}
