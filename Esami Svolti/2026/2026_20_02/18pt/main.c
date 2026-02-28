#include <stdio.h>
#include <stdlib.h>
#include "cards.h"

int main(void) {
  FILE *fin = fopen("carte_verif1.txt", "r");
  CardLIST S1 = CardLISTread(fin);
  fclose(fin);
  fin = fopen("carte_verif2.txt", "r");
  CardLIST S2 = CardLISTread(fin);
  fclose(fin);
  printf("Insieme di verifica 1:\n");
  CardLISTstore(S1,stdout);
  printf("Insieme di verifica 2:\n");
  CardLISTstore(S2,stdout);
  printf("Gli insiemi %srispettano il vincolo\n\n", checkCardLists(S1,S2)?"":"NON ");
  CardLISTfree(S1);
  CardLISTfree(S2);

  fin = fopen("carte.txt", "r");
  CardLIST S = CardLISTread(fin);
  fclose(fin);  
  printf("\nLe carte per il problema di ottimizzazione sono:\n");
  CardLISTstore(S,stdout);


  SOL sol = bestSequences(S);
  S1 = SOLgetList(sol,0);
  S2 = SOLgetList(sol,1);
  printf("La prima sequenza ottima e':\n");
  CardLISTstore(S1,stdout);
  printf("La seconda sequenza ottima e':\n");
  CardLISTstore(S2,stdout);

  CardLISTfree(S);
  SOLfree(sol);
  return 0;
}
