#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "Graph.h"
#define DBG 0

int main(int argc, char **argv) {

  Graph G = NULL;
  FILE *in;

  if (argc != 3) {
    printf("Use: %s <file_graph> <file_cycles>\n", argv[0]);
    exit(-1);
  }

  in = fopen(argv[1], "r");
  if (in == NULL)
    exit(-1);

  // Load graph
  G = GRAPHload(in);
  if (G == NULL)
    exit(-1);
  fclose(in);

  // Load cycles
  in = fopen(argv[2], "r");
  GRAPHloadCycles(G, in);

  #if DBG
  GRAPHstore(stdout, G);
  #endif // DBG

  // I won't implement a GRAPHcycleCheck function...

  // Compute vertex cycle cover
  GRAPHcycleCover(G);

  return 0;
}
