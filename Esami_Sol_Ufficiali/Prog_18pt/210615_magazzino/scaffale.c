#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "scaffale.h"

struct scaffale {
  PACCO *vP;
  int nOCC;
};

SCAFFALE SCAFFALEinit() {
  SCAFFALE s = calloc(1, sizeof(*s));
  if (!s) return NULL;

  s->vP = calloc(N_SLOT, sizeof(PACCO));

  return s;
}

void SCAFFALEstore(FILE *out, SCAFFALE s) {
  if(!s) return;

  fprintf(out, "   * In uso [%d/%d]\n", s->nOCC, N_SLOT);
  for(int i=0;i<N_SLOT;i++) {
      fprintf(out, "   - ");
    if(s->vP[i]) PACCOstore(out, s->vP[i]);
    else {
      fprintf(out, "<VUOTO>\n");
    }
  }
}

int SCAFFALEcheck(SCAFFALE s, int slot) {
  if (!s) return errore;
  if (slot < 0 || N_SLOT <= slot) return errore;
  if (!s->vP) return errore;
  if (s->vP[slot]) return occupato;
  return libero;
}

int SCAFFALEplacePACCO(SCAFFALE s, PACCO p, int slot) {
  int status = SCAFFALEcheck(s, slot);
  if (status == libero) {
    s->nOCC++;
    s->vP[slot] = p;
    PACCOsetSlot(p, slot);
  }
  return status;
}

PACCO SCAFFALEgetPACCO(SCAFFALE s, int slot, int estrai) {
  int status = SCAFFALEcheck(s, slot);
  if (status == occupato) {
    PACCO p = s->vP[slot];
    if (estrai) {
      s->vP[slot] = NULL;
      s->nOCC--;
      PACCOsetSlot(p, -1);
    }
    return p;
  }
  return NULL;
}

int SCAFFALEslotOccupati(SCAFFALE s) {
  if (!s) return -1;
  return s->nOCC;
}

int SCAFFALEgetSlotLibero(SCAFFALE s) {
  if (!s) return -1;
  for(int i=0;i<N_SLOT;i++) {
    if (!s->vP[i]) return i;
  }
  return -1;
}
