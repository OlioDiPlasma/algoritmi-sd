#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#define MAXC 11

int main(void) {
  Graph G;
  FILE *fin = fopen("albero.txt", "r");
  FILE *fcheck = fopen("parenti.txt", "r");
  char name1[MAXL], name2[MAXL];
  Date d1, d2;
  int k;

  /* si assume che i nomi non contengano spazi. Questo permette
  di semplificare leggermente le funzioni di lettura dei file
  */
  
  G = GRAPHload(fin);
  fclose(fin);
  FILE *fout = fopen("albero_out.txt", "w");

  GRAPHstore(G, fout);
  fclose(fout);

  while (fscanf(fcheck, "%d: %s %d/%d/%d %s %d/%d/%d ", &k,
                name1, &d1.dd,&d1.mm,&d1.yy,
                name2, &d2.dd,&d2.mm,&d2.yy) > 0) {              
    int check = relationshipK(G, name1, d1, name2, d2, k);
    printf("%s nato/a il %d/%d/%d e %s nato/a il %d/%d/%d ",
           name1, d1.dd,d1.mm,d1.yy,
           name2, d2.dd,d2.mm,d2.yy);
    if (!check) printf("non ");
    printf("sono parenti fino al grado %d\n", k);
  }

  for (int k=2; k<5; k++)
    largestLineageK(G, k);
    
  GRAPHfree(G);
  return 0;
}
