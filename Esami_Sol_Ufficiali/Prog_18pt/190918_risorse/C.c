#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "Queue.h"

#define DBG 1
#define file_mappa "mappa.txt"
#define file_prop "proposta.txt"
#define MOVES 4

int mossaValida(int r, int c, int R, int C, int **m) {
  // Se è dentro i bordi, ritorna il valore della cella destinazione (sarà la BFS a decidere cosa fare)
  if ((r >= 0) && (r < R) &&  (c >= 0) && (c < C)) {
    return m[r][c];
  }
  // Se si finirebbe fuori dalla mappa, tratta il caso come l'aver incontrato un muro
  return -1;
}

// SU, SX, DX, GIU'
int rShift[MOVES] = {-1, 0, 0, 1};
int cShift[MOVES] = {0, -1, 1, 0};

void BFS(int **m, int R, int C, int K, P src, int *ok) {
  int i, r, c;
  Q q = QUEUEinit();
  QUEUEput(q, src);
  m[src.r][src.c] = src.id;

  while (!QUEUEempty(q)) {
    P p = QUEUEget(q);
    for (i = 0; i < MOVES; i++) {
      r = p.r + rShift[i];
      c = p.c + cShift[i];
      int mossa = mossaValida(r, c, R, C, m);
      if (mossa == 0) { // andiamo verso una cella libera
        m[r][c] = src.id;
        if (p.dist +1 < K) {
          P step = {r, c, src.id, p.dist+1};
          QUEUEput(q, step);
        }
      } else {
        // andiamo verso una cella non-muro occupata da una risorsa diversa
        if (mossa != -1 && mossa != src.id) *ok = 0;
        // non trattiamo esplicitamente l'andare verso un muro. Semplicemente evita di accodare una cella destinazione non valida in q
        // ...
      }
    }
  }
  QUEUEfree(q);
}


int **alloca2d(int R, int C) {
  int i, **m = calloc(R, sizeof(int**));
  for(i=0;i<R;i++)
    m[i] = calloc(C, sizeof(int));
  return m;
}

void copia2d(int R, int C, int **src, int **dst) {
  int i, j;
  for(i=0;i<R;i++) {
    for(j=0;j<C;j++)
      dst[i][j] = src[i][j];
  }
}

void stampa2d(int **m, int R, int C) {
  int i, j;
  for(i=0;i<R;i++) {
    for(j=0;j<C;j++)
      printf("%d\t", m[i][j]);
    printf("\n");
  }
}

void libera2d(int **m, int R) {
  if (!m) return;
  int i;
  for(i=0;i<R;i++) {
    if (m[i]) free(m[i]);
  }
  free(m);
}

void pulisci2d(int **m, int R, int C, int target) {
  int i, j;
  for(i=0;i<R;i++)
    for(j=0;j<C;j++)
      if(m[i][j] == target) m[i][j] = 0;
}

int contaCopertura(int **m, int R, int C) {
  int i, j, cnt = 0;
  for(i=0;i<R;i++)
    for(j=0;j<C;j++)
      if(m[i][j] > 0) cnt++;
  return cnt;
}

int** leggiMappa(int *R, int *C, int *K) {
  FILE *in = fopen(file_mappa, "r");
  int x, y, **mappa;
  fscanf(in, "%d %d %d", R, C, K);
  mappa = alloca2d(*R, *C);
  while(fscanf(in, "%d %d", &x, &y) == 2) {
    mappa[x][y] = -1;
  }
  fclose(in);
  return mappa;
}

void leggiProposta(int *Z, int R, int C, int **mappa, int K) {
  FILE *in = fopen(file_prop, "r");
  int i, j, z, ok = 1, **proposta, **proposta2;
  proposta = alloca2d(R, C);
  proposta2 = alloca2d(R, C);
  copia2d(R, C, mappa, proposta2);
#if DBG
      printf("Mappa originale\n");
      stampa2d(mappa, R, C);
#endif // DBG
  fscanf(in, "%d", Z);
  for(z=1;z<=*Z && ok;z++) {
    fscanf(in, "%d %d", &i, &j);
    if (proposta2[i][j] != 0) {
      ok = 0;
    } else {
      P src = {i, j, z, 0};
      BFS(proposta2, R, C, K, src, &ok);
#if DBG
      printf(" + BFS da %d %d [id=%d] {stato=%sOK}:\n", i, j, z, ok ? "" : "NON ");
      stampa2d(proposta2, R, C);
#endif // DBG
    }
  }
  if (ok) {
    for(i=0;i<R;i++) {
      for(j=0;j<C;j++) {
        fscanf(in, "%d", &proposta[i][j]);
        if (mappa[i][j] == -1 && proposta[i][j] > 0) ok = 0;
        else continue;
        if (proposta[i][j] != proposta2[i][j]) ok = 0;
        else continue;
      }
    }
#if DBG
      printf("Proposta letta da file\n");
      stampa2d(proposta, R, C);
#endif // DBG
  }

  printf("Proposta%svalida\n", ok ? " " : " NON ");
  fclose(in);
  libera2d(proposta, R);
  libera2d(proposta2, R);
}

void disp_simp(int R, int C, int K, int Z, int z, int pos, int **sol, int **best, int *bestCover) {
  if (pos >= R*C || z > Z) {
    // Se il target non è raggiunto, ritorna direttamente
    if (z <= Z)
      return;
#if DBG > 1
    printf("Possibile configurazione finale\n");
    stampa2d(sol, R, C);
#endif
    int cover = contaCopertura(sol, R, C);
    if (cover > *bestCover) {
      copia2d(R, C, sol, best);
      *bestCover = cover;
    }
    return;
  }

  int r = pos / C;
  int c = pos % C;

  /*
   * Se la cella è libera, prova sia a posizionare una risorsa ed espandere il vicinato (con check di ammissibilità)
   * sia a saltare deliberatamente la cella.
   * Se c'è un muro, salta la cella.
   * Se c'è un'altra risorsa o l'area di influenza di un'altra risorsa, salta la cella
   */

  if (!sol[r][c]) {
    int ok = 1;
    P src = {r, c, z, 0};
    BFS(sol, R, C, K, src, &ok);
    if (ok) {
      // Se la copertura mediante BFS non porta a conflitti, posiziona la risorsa e ricorri...
      disp_simp(R, C, K, Z, z+1, pos+1, sol, best, bestCover);
    }
    // Backtrack
    pulisci2d(sol, R, C, z);
  }
  disp_simp(R, C, K, Z, z, pos+1, sol, best, bestCover);
}

/*
 * NOTA: z (numero di risorse usate, è nell'intervallo [1..Z] anziché [0..Z[ per comodità di
 * nell'assegnare un id alla z-esima risorsa posizionata (le celle libere hanno id 0)
 */

void sol_1(int **m, int R, int C, int K, int Z) {
  int **sol = alloca2d(R, C), **best = alloca2d(R, C);
  copia2d(R, C, m, sol);
  int bestCover = -1;
  disp_simp(R, C, K, Z, 1, 0, sol, best, &bestCover);
  printf("Soluzione a copertura %d\n", bestCover);
  stampa2d(best, R, C);
  libera2d(sol, R);
  libera2d(best, R);
}

void sol_2(int **m, int R, int C, int K) {
  int **sol = alloca2d(R, C), **best = alloca2d(R, C);
  int z, bestCover = -1, round = 1, knownBest = -1, knownZ = -1;
  copia2d(R, C, m, sol);
  for(z=1;z<=R*C && round;z++) {
    round = 0;
    disp_simp(R, C, K, z, 1, 0, sol, best, &bestCover);
    if (bestCover > knownBest) {
      round = 1;
      knownBest = bestCover;
      knownZ = z;
#if DBG > 1
  printf("Soluzione (temporanea) a copertura %d con %d risorse\n", knownBest, knownZ);
  stampa2d(best, R, C);
#endif // DBG
    }
  }
  printf("Soluzione a copertura %d con %d risorse\n", knownBest, knownZ);
  stampa2d(best, R, C);
  libera2d(sol, R);
  libera2d(best, R);
}

int main(int argc, char **argv) {
  int R, C, K, Z, **mappa;
  mappa = leggiMappa(&R, &C, &K);

  // La funzione legge la proposta di soluzione e internamente verifica la validità
  printf("*** FASE 1 - Verifica ***\n");
  leggiProposta(&Z, R, C, mappa, K);

  // Genera soluzione a Z noto
  printf("*** FASE 2 - Generazione a Z fisso ***\n");
  sol_1(mappa, R, C, K, Z);

  // Genera soluzione a numero di risorse libero
  printf("*** FASE 3 - Generazione a Z variabile ***\n");
  sol_2(mappa, R, C, K);

  libera2d(mappa, R);
  return 0;
}
