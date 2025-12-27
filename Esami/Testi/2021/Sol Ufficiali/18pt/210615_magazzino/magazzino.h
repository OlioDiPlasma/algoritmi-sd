#ifndef MAGAZZINO_H_DEFINED
#define MAGAZZINO_H_DEFINED

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "corridoio.h"

typedef struct magazzino *MAGAZZINO;

MAGAZZINO  MAGAZZINOinit(int nCO);
MAGAZZINO  MAGAZZINOread(FILE *in);
PACCO      MAGAZZINOsearchByCod(MAGAZZINO m, char *cod);
PACCO      MAGAZZINOgetPACCO(MAGAZZINO m, int co, int sc, int slot, int estrai);
void       MAGAZZINOmerge(MAGAZZINO m, int co1, int co2, int sc1, int sc2);
void       MAGAZZINOinsertPACCO(MAGAZZINO m, PACCO p);
void       MAGAZZINOsetup(MAGAZZINO m, int *vCO);
void       MAGAZZINOstore(FILE *out, MAGAZZINO m);
int        MAGAZZINOplacePACCO(MAGAZZINO m, PACCO p, int co, int sc, int slot);
int        MAGAZZINOmovePACCO(MAGAZZINO m, PACCO p, int co, int sc, int slot);
int        MAGAZZINOcheck(MAGAZZINO m, int co, int sc, int slot);

#endif
