#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include "st.h"

/* Tipo opaco per il Grafo */
typedef struct graph *Graph;

/* --- FUNZIONI --- */
Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
/* Legge il file, riempie la ST e costruisce il grafo */
Graph GRAPHload(FILE *f, ST st); 
int GRAPHgetIndex(Graph G, char *name); /* Wrapper per ST */
char* GRAPHgetName(Graph G, int index); /* Wrapper per ST */
int GRAPHgetV(Graph G);
/* Funzioni per la prova d'esame */
int GRAPHcheckEdge(Graph G, int u, int v); /* Ritorna peso se esiste arco, altrimenti 0 */
/* Ottimizzazione: Wrapper per lanciare la ricorsione */
void GRAPHpathBest(Graph G, int idS, int idD, int k, int p);

#endif