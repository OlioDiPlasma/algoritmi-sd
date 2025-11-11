#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void s1();
void s2();
void s3();

int main() {

  // 1
  // s1();

  // 2
  // s2();

  // 3
  s3();

  return 0;
}

/** S1 **/

/**

{{0, 1, 1, 0, 0, 0},
 {1, 0, 0, 1, 1, 1},
 {1, 0, 0, 0, 1, 0},
 {0, 1, 0, 0, 1, 1},
 {0, 1, 1, 1, 0, 1},
 {0, 1, 0, 1, 1, 0}}

**/
int **alloca2d(int r, int c, int init) {
  int i, **m = (int**) calloc (r, sizeof(int*));
  for(i=0;i<r;i++) {
    m[i] = (int*) calloc (c, sizeof(int));
  }

  if (init) {
      // Inizializza anche con dei valori...
	  m[0][1] = m[1][0] = 1;
	  m[0][2] = m[2][0] = 1;
	  m[1][3] = m[3][1] = 1;
	  m[1][4] = m[4][1] = 1;
	  m[1][5] = m[5][1] = 1;
	  m[2][4] = m[4][2] = 1;
	  m[3][4] = m[4][3] = 1;
	  m[3][5] = m[5][3] = 1;
	  m[4][5] = m[5][4] = 1;
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

int s1_check(int val, int k) {
  // Vers. 1
  // return (val >= k);
  // Vers. 2
  return (val <= k);
  // Vers. 3
  // return (val == k);
}

void s1_solve(int **m, int n, int k) {

  // per ogni coppia (i, j) conta il numero di amici comuni
  int i, j, x, cnt;
  for(i=0;i<n-1;i++) {
    for(j=i+1;j<n;j++) {
      cnt = 0;
      for(x=0;x<n;x++) {
        if ((x == i) || (x == j))
          continue;
        if (m[i][x] && m[x][j]) cnt++;
      }
      if (s1_check(cnt, k)) {
        printf("(%d, %d)\n", i, j);
      }
    }
  }
}

void s1() {
  fprintf(stdout, "\t *** S1 *** \n");
  int n = 6, k = 1;
  int **m = alloca2d(n, n, 1);
  stampa2d(m, n, n);
  s1_solve(m, n, k);
}

/** S2 **/

// NOTA: Sto trascurando la divisione esplicita in moduli...

// In LIST.h
typedef struct lista *LIST;

// In LIST.c
typedef struct node_ {
  struct node_ *next;
  char *val;
} node;
typedef node* link;

struct lista {
  link head;
  link tail;
};

link newNode(char *val, link next) {
  link l = calloc(1, sizeof(*l));
  l->val = (char*) strdup(val);
  l->next = next;
  return l;
}

LIST LISTinit() {
  fprintf(stdout, "\n\t *** S2 *** \n");
  LIST l = calloc(1, sizeof(*l));
  return l;
}

void LISTinsert(LIST l, link n) {
  if (!l) return;
  if (!l->head) {
    l->head = l->tail = n;
  } else {
    l->tail->next = n;
    l->tail = l->tail->next;
  }
}

void LISTprint(LIST l) {
  if (!l) return;
  link iter;
  for(iter=l->head;iter;iter=iter->next) {
    fprintf(stdout, "%s ", iter->val);
  }
  printf("\n");
}

void s2_solve(LIST l, char *str, char sep) {
  if (!l || !str) return;
  int i, n = strlen(str);
  for(i=0;i<n;) {
    int base = 0, iter = i;
    char tmp[n+1];

    // Vers. 1
#if 1
    while(iter < n) {
      tmp[base++] = str[iter];
      if (str[iter++] == sep) break;
    }
    tmp[base] = '\0';
    LISTinsert(l, newNode(tmp, NULL));
    i += (iter-i);
#else
    // Vers. 2
    while(iter < n) {
      if (str[iter] == sep && base) break;
      tmp[base++] = str[iter++];
    }
    tmp[base] = '\0';
    LISTinsert(l, newNode(tmp, NULL));
    i += (iter-i);
#endif
  }
}

void s2() {
  LIST l = LISTinit();
  s2_solve(l, "ramarro", 'r');
  LISTprint(l);
}

/** S3 **/

typedef struct {
	char *s;
	int pos;
	int costo;
} sub;

void s3_solve_r(int pos, int *sol, int val, int *mark, int *solBest, int *lenBest, int *valBest, char *s, char *e, sub *S, int nSubs) {
  int i;
  if (!strcmp(s, e)) {
    // Individuata la stringa target
    if (*valBest == -1 || val < *valBest || (val == *valBest && pos < *lenBest)) { // Vers. 1 (predilige sequenze corte)
    // if (*valBest == -1 || val < *valBest || (val == *valBest && pos > *lenBest)) { // Vers. 2 (predilige sequenze lunghe)
      *valBest = val;
      *lenBest = pos;
      for(i=0;i<pos;i++) solBest[i] = sol[i];
    }
    return;
  }

  if (pos >= nSubs) {
    // Non ci sono altre sostituzioni da provare...
    return;
  }

  char *safe = (char*) strdup(s); // Mantieni una copia safe della stringa di partenza per questo livello della ricorsione
  for(i=0;i<nSubs;i++) {
    if (mark[i])
      continue;

    mark[i] = 1;
    sol[pos] = i;
    strncpy(s + S[i].pos, S[i].s, strlen(S[i].s));
    s3_solve_r(pos+1, sol, val + S[i].costo, mark, solBest, lenBest, valBest, s, e, S, nSubs);
    strcpy(s, safe);
    mark[i] = 0;
  }
  free(safe);

  return;
}


void s3_solve(char *s, char *e, sub *S, int nSubs) {

  // Al peggio useremo tutte le sostituzioni
  int *sol = (int*) calloc(nSubs, sizeof(int));
  int *solBest = (int*) calloc(nSubs, sizeof(int));
  int *mark = (int*) calloc(nSubs, sizeof(int));
  int i, valBest = -1, lenBest = -1;

  fprintf(stdout, "S3 - V1\n");
  s3_solve_r(0, sol, 0, mark, solBest, &lenBest, &valBest, s, e, S, nSubs);
  if (lenBest != -1) {
    printf("%s\n", s);
    for(i=0;i<lenBest;i++) {
      printf("\t+ %s in posizione %d a costo %d\n", S[solBest[i]].s, S[solBest[i]].pos, S[solBest[i]].costo);
    }
    printf("%s\n", e);
    printf("COSTO: %d\n", valBest);
  }
  free(sol);
  free(solBest);
  free(mark);
}

void s3() {
  fprintf(stdout, "\n\t *** S3 *** \n");
  char *s = strdup("passato"), *e = strdup("persona");
  int nSubs = 8;
  sub *S = calloc(nSubs, sizeof(sub));
  S[0] = (sub) {"er", 1, 4};
  S[1] = (sub) {"ers", 1, 5};
  S[2] = (sub) {"sa", 3, 1};
  S[3] = (sub) {"so", 3, 2};
  S[4] = (sub) {"ato", 0, 1};
  S[5] = (sub) {"on", 4, 2};
  S[6] = (sub) {"ona", 4, 3};
  S[7] = (sub) {"a", 6, 1};
  s3_solve(s, e, S, nSubs);
}
