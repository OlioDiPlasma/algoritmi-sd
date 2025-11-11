#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

typedef struct G *Graph;

Graph GRAPHinit(int nV, int nE);
Graph GRAPHload(FILE *);
void  GRAPHstore(FILE *, Graph g);
void  GRAPHloadCycles(Graph g, FILE *);
void  GRAPHfree(Graph g);
void  GRAPHcycleCover(Graph G);

#endif /* GRAPH_H_INCLUDED */
