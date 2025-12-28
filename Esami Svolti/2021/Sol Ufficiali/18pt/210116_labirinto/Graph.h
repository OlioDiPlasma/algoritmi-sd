#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "ST.h"
#define MAXL 31

typedef struct G *Graph;

Graph GRAPHinit(int nV);
Graph GRAPHload(FILE *);
void  GRAPHfree(Graph g);
char *GRAPHgetName(Graph g, int index);

void GRAPHtestPath(Graph g, int PF, int G, int M, FILE *path);
void GRAPHfindPath(Graph g, int PF, int G, int M);

#endif /* GRAPH_H_INCLUDED */
