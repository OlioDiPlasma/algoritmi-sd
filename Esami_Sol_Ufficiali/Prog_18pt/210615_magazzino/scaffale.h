#ifndef SCAFFALE_H_DEFINED
#define SCAFFALE_H_DEFINED

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "pacco.h"

#define N_SLOT 4

typedef struct scaffale *SCAFFALE;

SCAFFALE   SCAFFALEinit();
PACCO      SCAFFALEgetPACCO(SCAFFALE s, int slot, int estrai);
void       SCAFFALEstore(FILE *out, SCAFFALE s);
int        SCAFFALEslotOccupati(SCAFFALE s);
int        SCAFFALEplacePACCO(SCAFFALE s, PACCO p, int slot);
int        SCAFFALEcheck(SCAFFALE s, int slot);
int        SCAFFALEgetSlotLibero(SCAFFALE s);

#endif
