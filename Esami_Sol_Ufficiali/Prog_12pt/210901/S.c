#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void s1();
void s2();
void s3();

int main() {

  // 1
  s1();

  // 2
  s2();

  // 3
  s3();

  return 0;
}

/** S1 **/

int **alloca2d(int r, int c) {
  int i, **m = (int**) calloc (r, sizeof(int*));
  for(i=0;i<r;i++) {
    m[i] = (int*) calloc (c, sizeof(int));
  }
  return m;
}

void stampa2d(int **m, int r, int c) {
  int i, j;
  for(i=0;i<r;i++) {
    for(j=0;j<c;j++) fprintf(stdout, "%d ", m[i][j]);
    fprintf(stdout, "\n");
  }
}

void s1_solve(int *v1, int *v2, int d1, int d2, int ***m) {

  // alloca matrice di comodo, interna alla funzione
  int **M = alloca2d(d1, d2);

  int i, j;
  for(i=0;i<d1;i++) {
    for(j=0;j<d2;j++) {
      M[i][j] = v1[i] + v2[j]; // Vers. 1
      // M[i][j] = v1[i] - v2[j]; // Vers. 2
      // M[i][j] = v1[i] * v2[j]; // Vers. 3
      // M[i][j] = v1[i] / v2[j]; // Vers. 4
    }
  }
  *m = M;
}

#define s1_d1 3
#define s1_d2 4

void s1() {
  fprintf(stdout, "\t *** S1 *** \n");
  int v1[s1_d1] = {2,4,6};
  int v2[s1_d2] = {1,3,5,7};
  int **m;
  s1_solve(v1, v2, s1_d1, s1_d2, &m);
  stampa2d(m, s1_d1, s1_d2);
}

/** S2 **/

// NOTA: Sto trascurando la divisione esplicita in moduli...

// In LIST.h
typedef struct lista *LIST;

// In LIST.c
typedef struct node_ {
  struct node_ *next;
  struct node_ *prev;
  int val;
} node;
typedef node* link;

struct lista {
  link head;
};

link newNode(int val, link prev, link next) {
  link l = calloc(1, sizeof(*l));
  l->val = val;
  l->next = next;
  l->prev = prev;
  return l;
}

LIST LISTinit() {
  LIST l = calloc(1, sizeof(*l));
  return l;
}

void LISTinsert(LIST l, int val) {
  if (!l) return;
  if (!l->head) {
    l->head = newNode(val, NULL, NULL);
  } else {
    link x = newNode(val, NULL, l->head);
    l->head->prev = x;
    l->head = x;
  }
}

void LISTprint(LIST l) {
  if (!l) return;
  link iter;
  for(iter=l->head;iter;iter=iter->next) {
    fprintf(stdout, "%d ", iter->val);
  }
  printf("\n");
}

int toDelete(int val, int div) {
  return !(val % div); // Vers. 1
  return val % div; // Vers. 2
}

void purge(LIST l, int div) {
  if (!l || !l->head) return;
  while(l->head && toDelete(l->head->val, div)) {
    link d = l->head;
    l->head = l->head->next;
    if (l->head) l->head->prev = NULL;
    free(d);
  }

  for(link iter=l->head;iter!=NULL;) {
    if (toDelete(iter->val, div)) {
      if(iter->next) iter->next->prev = iter->prev;
      if(iter->prev) iter->prev->next = iter->next;
      link d = iter;
      iter = iter->next;
      free(d);
    } else {
      iter = iter->next;
    }
  }
}

void s2_solve(LIST l, int div) {
  if (!l) return;
  purge(l, div);
}

void s2() {
  fprintf(stdout, "\n\t *** S2 *** \n");
  LIST l = LISTinit();
  for(int i=0;i<10;i++) LISTinsert(l, i);
  LISTprint(l);
  s2_solve(l, 3);
  LISTprint(l);
}

/** S3 **/

typedef struct {
	char *s;
	int pos;
	int costo;
} part;

void s3_solve_r(int pos, int *sol, int val, int *mark, int *solBest, int *lenBest, int *valBest, char *target, char *current, part *P, int nPart) {
  int i;

  if (!strcmp(target, current)) {
    // Individuata la stringa target
    if (*valBest == -1 || val < *valBest || (val == *valBest && pos < *lenBest)) { // Vers. 1 (predilige sequenze corte)
    // if (*valBest == -1 || val < *valBest || (val == *valBest && pos > *lenBest)) { // Vers. 2 (predilige sequenze lunghe)
      *valBest = val;
      *lenBest = pos;
      for(i=0;i<pos;i++) solBest[i] = sol[i];
    }
    return;
  }

  if (pos >= nPart) {
    // Non ci sono altri elementi da cui scegliere...
    return;
  }

  char *safe = (char*) strdup(current); // Mantieni una copia safe della stringa "corrente" di partenza per questo livello della ricorsione
  for(i=0;i<nPart;i++) {
    if (mark[i])
      continue;

    mark[i] = 1;
    sol[pos] = i;
    strncpy(current + P[i].pos, P[i].s, strlen(P[i].s));
    s3_solve_r(pos+1, sol, val + P[i].costo, mark, solBest, lenBest, valBest, target, current, P, nPart);
    strcpy(current, safe);
    mark[i] = 0;
  }
  free(safe);

  return;
}


void s3_solve(char *target, part *P, int nPart) {

  // Al peggio useremo tutte le parti, poiché alcune posizioni potrebbero sovrapporsi occorre tenere conto dell'ordine di costruzione
  int *sol = (int*) calloc(nPart, sizeof(int));
  int *solBest = (int*) calloc(nPart, sizeof(int));
  int *mark = (int*) calloc(nPart, sizeof(int));
  int i, valBest = -1, lenBest = -1;
  char *current = (char *) calloc(strlen(target)+1, sizeof(char));
  memset(current, '-', strlen(target)+1);
  current[strlen(target)] = '\0';

  fprintf(stdout, "S3\n");
  s3_solve_r(0, sol, 0, mark, solBest, &lenBest, &valBest, target, current, P, nPart);
  if (lenBest != -1) {
    printf("%s\n", target);
    for(i=0;i<lenBest;i++) {
      printf("\t+ %s in posizione %d a costo %d\n", P[solBest[i]].s, P[solBest[i]].pos, P[solBest[i]].costo);
    }
    printf("COSTO: %d\n", valBest);
  }
  free(sol);
  free(solBest);
  free(mark);
}

void s3() {
  fprintf(stdout, "\n\t *** S3 *** \n");
  char *target = (char*) strdup("persona");
  int nPart = 10;
  part *P = calloc(nPart, sizeof(part));
  P[0] = (part) {"p", 0, 1};
  P[1] = (part) {"pers", 0, 5};
  P[2] = (part) {"er", 1, 4};
  P[3] = (part) {"ers", 1, 4};
  P[4] = (part) {"sa", 3, 1};
  P[5] = (part) {"so", 3, 1};
  P[6] = (part) {"ato", 0, 1};
  P[7] = (part) {"on", 4, 2};
  P[8] = (part) {"ona", 4, 3};
  P[9] = (part) {"a", 6, 1};
  s3_solve(target, P, nPart);
}
