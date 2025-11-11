#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "magazzino.h"
#include "bst.h"

struct magazzino {
  int nCO;
  BST bstPACCHI;
  CORRIDOIO *vCO;
};

MAGAZZINO MAGAZZINOinit(int nCO) {
  MAGAZZINO m = calloc(1, sizeof(*m));
  if (!m) return NULL;

  m->nCO = nCO;
  m->bstPACCHI = BSTinit();
  m->vCO = calloc(m->nCO, sizeof(CORRIDOIO));

  return m;
}

MAGAZZINO MAGAZZINOread(FILE *in) {
  int i, *vCO, nCO;
  fscanf(in, "%d\n", &nCO);
  MAGAZZINO m = MAGAZZINOinit(nCO);
  vCO = malloc(nCO * sizeof(int));
  for(i=0;i<nCO;i++)
    fscanf(in, "%d\n", &vCO[i]);

  MAGAZZINOsetup(m, vCO);
  free(vCO);

  return m;
}
void MAGAZZINOsetup(MAGAZZINO m, int *vCO) {
  if (!m) return;
  if (!vCO) return;
  for(int i=0;i<m->nCO;i++)
    m->vCO[i] = CORRIDOIOinit(vCO[i]);
}

void MAGAZZINOstore(FILE *out, MAGAZZINO m) {
  if(!m) return;
  fprintf(out, "N. Corridoi: %d\n", m->nCO);
  for(int i=0;i<m->nCO;i++) {
    fprintf(out, "Corridoio #%d\n", i);
    CORRIDOIOstore(out, m->vCO[i]);
  }
}

int MAGAZZINOcheck(MAGAZZINO m, int co, int sc, int slot) {
  if (!m) return errore;
  if (co < 0 || m->nCO <= co) return errore;
  if (!m->vCO || !m->vCO[co]) return errore;
  return CORRIDOIOcheck(m->vCO[co], sc, slot);
}

PACCO MAGAZZINOsearchByCod(MAGAZZINO m, char *cod) {
  return BSTsearch(m->bstPACCHI, cod);
}

PACCO MAGAZZINOgetPACCO(MAGAZZINO m, int co, int sc, int slot, int estrai) {
  int status = MAGAZZINOcheck(m, co, sc, slot);
  PACCO p = NULL;
  if (status == occupato) {
    p = CORRIDOIOgetPACCO(m->vCO[co], sc, slot, estrai);
    if (p && estrai) {
      BSTremove(m->bstPACCHI, PACCOgetCod(p));
      PACCOsetCorridoio(p, -1);
    }
  }
  return p;
}

int MAGAZZINOplacePACCO(MAGAZZINO m, PACCO p, int co, int sc, int slot) {
  int status = MAGAZZINOcheck(m, co, sc, slot);
  if (status == libero) {
    // Verifica se il pacco è già presente, se sì, trattalo come spostamento
    PACCO prev = MAGAZZINOsearchByCod(m, PACCOgetCod(p));
    if (prev != NULL) {
      return MAGAZZINOmovePACCO(m, p, co, sc, slot);
    } else {
      if (status == libero) {
        status = CORRIDOIOplacePACCO(m->vCO[co], p, sc, slot);
        if (status == libero) {
          PACCOsetCorridoio(p, co);
        }
      }
    }
    MAGAZZINOinsertPACCO(m, p);
    fprintf(stdout, "[INSERIMENTO]\n");
  } else if (status == occupato) {
    fprintf(stdout, "[POS. OCCUPATA]\n");
  } else {
    fprintf(stdout, "[ERRORE]\n");
  }
  return status;
}

void MAGAZZINOinsertPACCO(MAGAZZINO m, PACCO p) {
  if (!m) return;
  if (!p) return;
  if (!m->bstPACCHI) return;
  BSTinsert(m->bstPACCHI, p);
}

int MAGAZZINOmovePACCO(MAGAZZINO m, PACCO p, int co, int sc, int slot) {
  fprintf(stdout, "[SPOSTAMENTO]\n");
  PACCO prev = MAGAZZINOsearchByCod(m, PACCOgetCod(p));
  if (prev == NULL) {
    // Il pacco non esiste in magazzino
    return errore;
  }

  int status = MAGAZZINOcheck(m, co, sc, slot);
  PACCOstore(stdout, prev);
  if (status == libero) {
    // get con estrazione, per allineare i dati delle collezioni interne
    MAGAZZINOgetPACCO(m, PACCOgetCorridoio(prev), PACCOgetScaffale(prev), PACCOgetSlot(prev), 1);
    status = MAGAZZINOplacePACCO(m, p, co, sc, slot);
    MAGAZZINOinsertPACCO(m, p);
    fprintf(stdout, "[NUOVA POSIZIONE]\n");
    PACCOstore(stdout, p);
    fprintf(stdout, "[SPOSTAMENTO RIUSCITO]\n");
  } else {
    fprintf(stdout, "[DESTINAZIONE OCCUPATA]\n");
    // La posizione di destinazione è già occupata
  }
  return status;
}

void MAGAZZINOmerge(MAGAZZINO m, int co1, int co2, int sc1, int sc2) {
  if (!m || co1 < 0 || co2 < 0 || sc1 < 0 || sc2 < 0) return;
  if (m->nCO <= co1 || m->nCO <= co2) return;
  SCAFFALE s1 = CORRIDOIOgetSCAFFALE(m->vCO[co1], sc1);
  SCAFFALE s2 = CORRIDOIOgetSCAFFALE(m->vCO[co2], sc2);
  if (s1 && s2) {
    int no1 = SCAFFALEslotOccupati(s1);
    int no2 = SCAFFALEslotOccupati(s2);
    if (no1 + no2 <= N_SLOT) {
      fprintf(stdout, "[COMPATTAZIONE] (C %d, S %d) >> (C %d, S %d)\n", co2, sc2, co1, sc1);
      for(int i=0, moved=0;i < N_SLOT && moved < no2 ;i++) {
        PACCO p = MAGAZZINOgetPACCO(m, co2, sc2, i, 0);
        if (!p) continue;
        fprintf(stdout, "[SPOSTAMENTO %d]\n", moved++);
        MAGAZZINOmovePACCO(m, p, co1, sc1, SCAFFALEgetSlotLibero(s1));
      }
    } else {
      fprintf(stdout, "[SPAZIO INSUFFICIENTE]\n");
    }
  }
}
