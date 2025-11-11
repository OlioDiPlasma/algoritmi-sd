#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "Graph.h"

#define TEST 0

void generaSetE(Graph g);

int main(int argc, char **argv) {

  Graph dungeon = NULL;
  FILE *in;

  if (argc != 6) {
    printf("Use: %s <file_dungeon> <file_path> <PF> <G> <M>\n", argv[0]);
    exit(-1);
  }

  in = fopen(argv[1], "r");
  if (in == NULL)
    exit(-1);

  int PF = atoi(argv[3]);
  int G = atoi(argv[4]);
  int M = atoi(argv[5]);

  dungeon = GRAPHload(in);
  if (dungeon == NULL)
    exit(-1);
  fclose(in);

  // Note: testPath open doors without closing them and collect gold from rooms, so it alters the dungeon
  // DO NOT call both test and find without making a copy of the dungeon itself first
#if TEST
  in = fopen(argv[2], "r");
  if (in == NULL)
    exit(-1);
  GRAPHtestPath(dungeon, PF, G, M, in);
  fclose(in);
#else
  GRAPHfindPath(dungeon, PF, G, M);
#endif

  GRAPHfree(dungeon);

  return 0;
}
