#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "corridoio.h"

struct corridoio {
  int nSC;
  SCAFFALE *vSC;
};

void CORRIDOIOstore(FILE *out, CORRIDOIO c) {
  if(!c) return;
  fprintf(out, " * N. Scaffali %d\n", c->nSC);
  for(int i=0;i<c->nSC;i++) {
    fprintf(out, "   * Scaffale #%d\n", i);
    SCAFFALEstore(out, c->vSC[i]);
  }
}

CORRIDOIO CORRIDOIOinit(int nSC) {
  CORRIDOIO c = calloc(1, sizeof(*c));
  if (!c) return NULL;

  c->nSC = nSC;
  c->vSC = calloc(c->nSC, sizeof(SCAFFALE));
  for(int i=0;i<c->nSC;i++)
    c->vSC[i] = SCAFFALEinit();

  return c;
}

int CORRIDOIOgetNSC(CORRIDOIO c) {
  if(!c) return -1;
  return c->nSC;
}

int CORRIDOIOcheck(CORRIDOIO c, int sc, int slot) {
  if (!c) return errore;
  if (sc < 0 || c->nSC <= sc) return errore;
  if (!c->vSC || !c->vSC[sc]) return errore;
  return SCAFFALEcheck(c->vSC[sc], slot);
}

int CORRIDOIOplacePACCO(CORRIDOIO c, PACCO p, int sc, int slot) {
  int status = CORRIDOIOcheck(c, sc, slot);
  if (status == libero) {
    status = SCAFFALEplacePACCO(c->vSC[sc], p, slot);
    if (status == libero) PACCOsetScaffale(p, sc);
  }
  return status;
}

PACCO CORRIDOIOgetPACCO(CORRIDOIO c, int sc, int slot, int estrai) {
  int status = CORRIDOIOcheck(c, sc, slot);
  PACCO p = NULL;
  if (status == occupato) {
    p = SCAFFALEgetPACCO(c->vSC[sc], slot, estrai);
    if (p && estrai) PACCOsetScaffale(p, -1);
  }
  return p;
}

SCAFFALE CORRIDOIOgetSCAFFALE(CORRIDOIO c, int sc) {
  if (!c) return NULL;
  if (sc < 0 || c->nSC <= sc) return NULL;
  return c->vSC[sc];
}
