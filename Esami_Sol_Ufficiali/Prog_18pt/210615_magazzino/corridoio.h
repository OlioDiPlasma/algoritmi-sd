#ifndef CORRIDOIO_H_DEFINED
#define CORRIDOIO_H_DEFINED

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "scaffale.h"

typedef struct corridoio *CORRIDOIO;

CORRIDOIO  CORRIDOIOinit(int nSC);
SCAFFALE   CORRIDOIOgetSCAFFALE(CORRIDOIO c, int sc);
PACCO      CORRIDOIOgetPACCO(CORRIDOIO c, int sc, int slot, int estrai);
void       CORRIDOIOstore(FILE *out, CORRIDOIO c);
int        CORRIDOIOgetNSC(CORRIDOIO c);
int        CORRIDOIOplacePACCO(CORRIDOIO c, PACCO p, int sc, int slot);
int        CORRIDOIOcheck(CORRIDOIO c, int sc, int slot);
#endif
