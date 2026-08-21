#include <stdio.h>
#include "grafo_matriz.h"

int main() {
    
    GrafoMatriz grafo;

    inicializer(&grafo, 8);

    inserir_aresta(&grafo, 0, 1);
    inserir_aresta(&grafo, 0, 2);
    inserir_aresta(&grafo, 0, 3);
    inserir_aresta(&grafo, 1, 4);
    inserir_aresta(&grafo, 1, 5);
    inserir_aresta(&grafo, 2, 3);
    inserir_aresta(&grafo, 2, 6);
    inserir_aresta(&grafo, 3, 6);
    inserir_aresta(&grafo, 7, 4);
    inserir_aresta(&grafo, 7, 5);
    inserir_aresta(&grafo, 7, 6);

    printf("Matriz de Adjacência - grafo nao orientado:\n");
    exibir_matriz(&grafo);

    inicializer(&grafo, 3);
    inserir_aresta(&grafo, 0, 1);
    inserir_aresta(&grafo, 1, 2);
    inserir_aresta(&grafo, 2, 0);

    printf("Matriz de Adjacência - grafo orientado:\n");
    exibir_matriz(&grafo);

    return 0;
}