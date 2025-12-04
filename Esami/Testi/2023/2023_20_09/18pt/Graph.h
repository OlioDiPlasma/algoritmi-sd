#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>

/* STRUTTURA ARCO */
typedef struct {
    int u;      // Vertice sorgente
    int v;      // Vertice destinazione
    int w;      // Peso
} Edge;

/* STRUTTURA GRAFO (Spostata qui per visibilità in ottimizzazione.c) */
struct graph {
    int V;          // Numero vertici
    int E_count;    // Numero archi
    Edge *edges;    // Vettore degli archi
    int **adj;      // Matrice di adiacenza
};

/* Puntatore al grafo */
typedef struct graph *Graph;

/* FUNZIONI DI ACQUISIZIONE E GESTIONE */
Graph GRAPHload(const char *filename); // LISTA PROTOTIPI
void GRAPHfree(Graph G); //LISTA PROTOTIPI
int GRAPHgetV(Graph G); // NUOVA DA IMPLEMENTARE
int GRAPHcheckEdge(Graph G, int u, int v); // NUOVA Ritorna il peso se esiste, -1 se no
Edge* GRAPHgetEdges(Graph G, int *num_edges); // LISTA PROTOTIPI solo che ritorna direttamente Edge

#endif