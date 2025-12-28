#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "UF.h"

/* Struttura Arco esposta (come da traccia) */
typedef struct {
    int v;
    int w;
    int wt; // weight (costo)
} Edge;

/* Struttura Componenti Connesse (ADT di prima classe richiesto) */
typedef struct components *CC;
struct components {
    int nCC;        // Numero componenti connesse
    int *vertexCC;  // Vettore ID componente per ogni vertice
};

typedef struct graph *Graph;

/* --- PROTOTIPI --- */

/* Caricamento e gestione base */
Graph GRAPHinit(int V);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHfree(Graph G);

/* Funzioni richieste specificamente dalla traccia */
void GRAPHedgeDisable(Graph g, Edge e);
void GRAPHedgeEnable(Graph g, Edge e);

/* Generazione Componenti Connesse */
CC CCgen(Graph g);
void CCfree(CC cc);

/* Funzione check richiesta (problema di verifica) */
int GRAPHcheckTreeEdges(Graph g, CC comp, Edge *ev, int en);

/* Funzione principale di OTTIMIZZAZIONE */
Edge *GRAPHgenOptTrees(Graph g, CC comp);

#endif