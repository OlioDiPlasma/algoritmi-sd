#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>

// Rappresenta un singolo triangolo {u, v, w}
typedef struct {
    int u, v, w;
} Triangle;

// Rappresenta un Packing (una soluzione)
typedef struct {
    int n_triangles; // Numero di triangoli nella soluzione
    Triangle *tris;  // Vettore dinamico di triangoli
} Packing;

// Rappresenta il Grafo
typedef struct {
    int V;          // Numero di vertici
    int E;          // Numero di archi (utile per statistiche)
    int **madj;     // Matrice di adiacenza (per check O(1))
} Graph;

// Funzioni di gestione memoria e I/O
Graph *loadGraph(const char *filename);
void freeGraph(Graph *G);
Packing *allocPacking(int max_size);
void freePacking(Packing *p);

// Funzioni richieste
int verifySolution(Graph *G, const char *sol_filename);
Packing *findMaxTrianglePacking(Graph *G);
void saveSolution(Packing *p, const char *filename);

#endif