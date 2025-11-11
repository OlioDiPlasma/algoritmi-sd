#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pacco.h"

struct pacco {
  char *cod;
  int co, sc, slot;
};

PACCO PACCOinit(char *cod) {
  PACCO p = calloc(1, sizeof(*p));
  if(!p) return NULL;
  p->cod = (char*) strdup(cod);
  p->co = -1;
  p->sc = -1;
  p->slot = -1;
  return p;
}

void PACCOstore(FILE *out, PACCO p) {
  if(!p) return;
  fprintf(out, "%s %d %d %d\n", p->cod, p->co, p->sc, p->slot);
}

char* PACCOgetCod(PACCO p) {
  if(!p) return NULL;
  return p->cod;
}

int PACCOgetSlot(PACCO p) {
  if(!p) return -1;
  return p->slot;
}

int PACCOgetScaffale(PACCO p) {
  if(!p) return -1;
  return p->sc;
}

int PACCOgetCorridoio(PACCO p) {
  if(!p) return -1;
  return p->co;
}

void PACCOsetSlot(PACCO p, int slot) {
  if(!p) return;
  p->slot = slot;
}

void PACCOsetScaffale(PACCO p, int sc) {
  if(!p) return;
  p->sc = sc;
}

void PACCOsetCorridoio(PACCO p, int co) {
  if(!p) return;
  p->co = co;
}
