#ifndef GRAPH_H
#define GRAPH_H

#include "Queue.h"
#include "ST.h"

typedef struct edge { int v; int w; int wt; } Edge;

typedef struct graph *Graph;

Graph GRAPHinit(int V);
void  GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void  GRAPHstore(Graph G, FILE *fout);

void largestLineageK(Graph G, int k);
int relationshipK(Graph G, char *name1, Date d1, char *name2, Date d2, int k);


#endif
