#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define file_magazzino "magazzino.txt"
#define file_pacchi "pacchi.txt"

#include "magazzino.h"

MAGAZZINO leggiFileMagazzino(char *fn) {

  FILE *fp = fopen(fn, "r");
  if (fp == NULL)
    return NULL;

  MAGAZZINO m = MAGAZZINOread(fp);

  fclose(fp);

  return m;
}

void leggiFilePacchi(MAGAZZINO m, char *fn) {
  FILE *fp = fopen(fn, "r");
  if (fp == NULL)
    return;

  char cod[LEN];
  int co, sc, slot;
  PACCO p;
  while(fscanf(fp, "%s %d %d %d\n", cod, &co, &sc, &slot) == 4) {
    fprintf(stdout, "PACCO: %s CORR = %d SCAF = %d SLOT = %d\n", cod, co, sc, slot);
    p = PACCOinit(cod);
    MAGAZZINOplacePACCO(m, p, co, sc, slot);
    fprintf(stdout, "\n");
  }

  fclose(fp);
}

int main(int argc, char **argv) {

  MAGAZZINO m = leggiFileMagazzino(file_magazzino);
  leggiFilePacchi(m, file_pacchi);
  MAGAZZINOstore(stdout, m);
  MAGAZZINOmerge(m, 3, 0, 0, 1);
  MAGAZZINOstore(stdout, m);
  return 0;
}
