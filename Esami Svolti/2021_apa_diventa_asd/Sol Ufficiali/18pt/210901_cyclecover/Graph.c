#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"

typedef struct cycle_ {
  int* v; // arrays of traversed vertices
  int len; // len of cycle
} cycle;

struct G {
  int V, E;
  int **madj;
  // Use a dynamic array to store cycles, in order to exploit indexing later
  // We could have used a list as well and manage direct references to cycle items
  cycle **C;
  int nC, maxC;
};

Graph GRAPHinit(int nV, int nE) {
  Graph g = malloc(sizeof(*g));
  if (g == NULL)
    return NULL;

  g->V = nV;
  g->E = nE;

  g->madj = (int**) calloc (nV, sizeof(int*));
  if (g->madj == NULL)
    return NULL;

  for(int i=0;i<nV;i++) {
    g->madj[i] = (int*) calloc (nV, sizeof(int));
    if (g->madj[i] == NULL) return NULL;
  }

  g->maxC = 1;
  g->nC = 0;
  g->C = calloc(g->maxC, sizeof(cycle*));

  return g;
}

Graph GRAPHload(FILE *in) {
  int nV, nE, src, dst;
  Graph g;

  fscanf(in, "%d %d\n", &nV, &nE);
  g = GRAPHinit(nV, nE);
  if (g == NULL)
    return NULL;

  while(fscanf(in, "%d %d\n", &src, &dst) == 2) {
    if (src != dst && src >= 0 && src >= 0 && src < nV && dst < nV) {
      g->madj[src][dst] = 1;
    }
  }
  return g;
}

void GRAPHloadCycles(Graph g, FILE *in) {
  if (in == NULL) return;
  int len;
  while(fscanf(in, "%d", &len) == 1) {
    cycle *c = calloc(1, sizeof(*c));
    c->len = len;
    c->v = calloc(c->len, sizeof(int));
    for(int i=0;i<len;i++) fscanf(in, "%d", &c->v[i]);
    if (g->nC == g->maxC) {
      g->maxC *= 2;
      g->C = realloc(g->C, g->maxC*sizeof(cycle*));
    }
    g->C[g->nC++] = c;
  }
}

void GRAPHstore(FILE *out, Graph g) {
  if(g == NULL || g->madj == NULL)
    return;
  fprintf(out, "%d %d\n", g->V, g->E);
  for(int i=0;i<g->V;i++) {
    for(int j=0;j<g->V;j++) {
      if (g->madj[i][j]) fprintf(out, "(%d, %d)\n", i, j);
    }
  }

  if(g->nC) {
    for(int i=0;i<g->nC;i++) {
      for(int l=0;l<g->C[i]->len;l++) {
        fprintf(out, "%d ", g->C[i]->v[l]);
      }
      fprintf(out, "\n");
    }
  }
}

int check(Graph G, int *sol, int k) {
  int *cover = calloc(G->V, sizeof(int));
  int ok = 1, size = 0;

  // Count the number of vertices (even repeated ones) in the "cover" and mark covered ones
  for(int i=0;i<k;i++) {
    for(int j=0;j<G->C[sol[i]]->len;j++) {
      size++;
      cover[G->C[sol[i]]->v[j]] = 1;
    }
  }

  // If at least one vertex is not covered, it is not a valid cover
  for(int i=0;i<G->V;i++) {
    if (cover[i] == 0) {
      ok = 0;
      break;
    }
  }

  free(cover);

  // Return the size of the cover, making it null in case of an invalid cover
  return ok * size;
}

static int cyclecover(Graph G, int pos, int start, int k, int *sol, int *bestSol, int *bestCover) {
  if (pos >= k) {
    int currentCover = check(G, sol, k);
    if (currentCover > 0) {
      if (*bestCover == 0 || *bestCover > currentCover) {
        *bestCover = currentCover;
        memcpy(bestSol, sol, G->nC*sizeof(int));
      }
    }
    return *bestCover;
  }

  for(int i=start;i<G->nC;i++) {
    sol[pos] = i;
    cyclecover(G, pos+1, i+1, k, sol, bestSol, bestCover);
  }
  return *bestCover;
}

void GRAPHcycleCover(Graph G) {
  int *sol = calloc(G->nC, sizeof(int));
  int *bestSol = calloc(G->nC, sizeof(int));
  int bestCover = 0;

  // Search for covers of increasing cardinality, the upper bound could be tighter (one cycle for vertex)
  for(int i=1;i<G->nC;i++) {
    printf("Looking for cover of card = %d\n", i);
    if(cyclecover(G, 0, 0, i, sol, bestSol, &bestCover)) {
      // Found a cover of minimum cardinality
      for(int j=0;j<i;j++) {
        printf("{");
        for(int k=0;k<G->C[bestSol[j]]->len;k++) printf("%s%d", k ? ", " : "", G->C[bestSol[j]]->v[k]);
        printf("}\n");
      }
      break;
    }
  }

}

void GRAPHfree(Graph g) {
  for (int v=0; v<g->V; v++) {
    free(g->madj[v]);
  }
  free(g->madj);
  free(g);
}
