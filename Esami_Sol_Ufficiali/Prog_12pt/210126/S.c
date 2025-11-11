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
  s2();

  // 3
  // s3();

  return 0;
}

/** S1 **/

int **alloca2d(int r, int c, int init) {
  int i, j, **m = (int**) calloc (r, sizeof(int*));
  for(i=0;i<r;i++) {
    m[i] = (int*) calloc (c, sizeof(int));
  }
  
  if (init) {
      // Inizializza anche con dei valori...
	  m[0][0] = 1;
	  m[0][1] = 1;
	  m[0][2] = 3;
	  m[0][3] = 0;
	  m[1][0] = 2;
	  m[1][1] = 4;
	  m[1][2] = 6;
	  m[1][3] = 8;
	  m[2][0] = 1;
	  m[2][1] = 2;
	  m[2][2] = 1;
	  m[2][3] = 2;
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

int s1_check(int val) {
  // Vers. 1
  // return val == 0;
  // Vers. 2
  // return val % 2;
  // Vers. 3
  //return !(val % 2);
  // Vers. 4
  return !(val % 3);
}

void s1_change(int **m, int x, int y, int r, int c) {
  int i;
  // Vers. 1
  for(i=0;i<c;i++) m[x][i] = 3;
  for(i=0;i<r;i++) m[i][y] = 3;

  // m[x][i] = 0 per Vers. 1
  // m[x][i] = m[x][y] per Vers. 2 e Vers. 3
  // m[x][i] = 3 per Vers. 4
}

void s1_solve(int **m, int r, int c) {
  // Matrice di mark per le posizioni che rispettano il vincolo
  int i, j, **mark = alloca2d(r, c, 0);
  for(i=0;i<r;i++) {
    for(j=0;j<c;j++) {
      if (s1_check(m[i][j])) mark[i][j] = 1;
    }
  }
  
  printf("MARK\n");
  stampa2d(mark, r, c);

  for(i=0;i<r;i++) {
    for(j=0;j<c;j++) {
      if (mark[i][j])
        s1_change(m, i, j, r, c);
    }
  }
}

void s1() {
  fprintf(stdout, "\t *** S1 *** \n");
  int r = 3, c = 4;
  int **m = alloca2d(r, c, 1);
  fprintf(stdout, "PRIMA\n");
  stampa2d(m, r, c);
  s1_solve(m, r, c);
  fprintf(stdout, "DOPO\n");
  stampa2d(m, r, c);
}

/** S2 **/

// NOTA: Sto trascurando la divisione esplicita in moduli...

// In LIST.h
typedef struct lista *LIST;

// In LIST.c
typedef struct node_ {
  struct node_ *next;
  int val;
} node;
typedef node* link;

struct lista {
  link head;
};

link newNode(int val, link next) {
  link l = calloc(1, sizeof(*l));
  l->val = val;
  l->next = next;
  return l;
}

LIST LISTinit() {
  fprintf(stdout, "\n\t *** S2 *** \n");
  LIST l = calloc(1, sizeof(*l));
  return l;
}

void LISTprint(LIST l) {
  if (!l) return;
  link iter;
  for(iter=l->head;iter;iter=iter->next) {
    fprintf(stdout, "%d ", iter->val);
  }
  printf("\n");
}

int s2_check(int val, int *v, int n) {
  int i;
  // Vers. 1
  /*
  for(i=0;i<n;i++) {
    if (v[i] == val) return 1;
  }
  return 0;
  */

  // Vers. 2
  /*
  for(i=0;i<n;i++) {
    if (v[i] == val) return 0;
  }
  return 1;
  */

  // Vers. 3
  /*
  int min = v[0];
  for(i=0;i<n;i++) {
    if (v[i] < min) min = v[i];
  }
  return val > min;
  */

  // Vers. 3
  int max = v[0];
  for(i=0;i<n;i++) {
    if (v[i] > max) max = v[i];
  }
  return val < max;
}

void s2_solve(LIST l, int *v, int n) {
  if (!l || !l->head || !v || n <= 0) return;
  link iter, prev = NULL, del;
  for(iter=l->head;iter;) {
    del = NULL;
    if(s2_check(iter->val, v, n)) {
      fprintf(stdout, "DELETE %d\n", iter->val);
      del = iter;
      if (prev == NULL) {
        l->head = iter->next;
      } else {
        prev->next = iter->next;
      }
    }
    if (del) {
      iter=iter->next;
      free(del);
    } else {
      prev = iter;
      iter=iter->next;
    }
  }
}

void s2() {
  LIST l = LISTinit();
  int v[3] = {1,2,3};
  int n = 3;

  l->head = newNode(3, newNode(10, newNode(6, newNode(3, newNode(1, newNode(4, NULL))))));

  fprintf(stdout, "PRIMA\n");
  LISTprint(l);
  s2_solve(l, v, n);
  fprintf(stdout, "DOPO\n");
  LISTprint(l);
}

/** S3 **/

int s3_solve_r1(char *s, char **dict, int n, int *sol, int pos, int posmax, int curr_len, int K, int *count) {
  int i;

  if (curr_len == strlen(s)) {
    char *t = calloc(curr_len+1, sizeof(char));
    int len = 0, ok = 1;
    for(i=0;i<pos;i++) {
      strcat(t+len, dict[sol[i]]);
      len += strlen(dict[sol[i]]);
    }
    ok = !strcmp(s, t);
    free(t);
    if (ok) {
      printf("TROVATO: ");
      for(i=0;i<pos;i++) {
        printf("%s ", dict[sol[i]]);
      }
      printf("\n");
      return 1;
    }
  }

  if (pos >= posmax) {
    return 0;
  }

  for(i=0;i<n;i++) {
    if (count[i] >= K) continue;
    sol[pos] = i;
    count[i]++;
    if (s3_solve_r1(s, dict, n, sol, pos+1, posmax, curr_len+strlen(dict[i]), K, count))
      return 1;
    count[i]--;
  }

  return 0;
}

int s3_solve_r2(char *s, char **dict, int n, int *sol, int pos, int posmax, int curr_len, int S, int *count, int diff) {
  int i;

  if (diff > S) return 0;

  if (curr_len == strlen(s)) {
    char *t = calloc(curr_len+1, sizeof(char));
    int len = 0, ok = 1;
    for(i=0;i<pos;i++) {
      strcat(t+len, dict[sol[i]]);
      len += strlen(dict[sol[i]]);
    }
    ok = !strcmp(s, t);
    free(t);
    if (ok) {
      printf("TROVATO: ");
      for(i=0;i<pos;i++) {
        printf("%s ", dict[sol[i]]);
      }
      printf("\n");
      return 1;
    }
  }

  if (pos >= posmax) {
    return 0;
  }

  for(i=0;i<n;i++) {
    sol[pos] = i;
    count[i]++;
    if (s3_solve_r2(s, dict, n, sol, pos+1, posmax, curr_len+strlen(dict[i]), S, count, diff + (count[i] == 1)))
      return 1;
    count[i]--;
  }

  return 0;
}

int s3_solve_r3(char *s, char **dict, int n, int *sol, int pos, int posmax, int curr_len, int S, int *count, int diff) {
  int i;

  if (curr_len == strlen(s)) {
    char *t = calloc(curr_len+1, sizeof(char));
    int len = 0, ok = 1;
    for(i=0;i<pos;i++) {
      strcat(t+len, dict[sol[i]]);
      len += strlen(dict[sol[i]]);
    }
    ok = !strcmp(s, t);
    free(t);
    if (ok && diff >= S) {
      printf("TROVATO: ");
      for(i=0;i<pos;i++) {
        printf("%s ", dict[sol[i]]);
      }
      printf("\n");
      return 1;
    }
  }

  if (pos >= posmax) {
    return 0;
  }

  for(i=0;i<n;i++) {
    sol[pos] = i;
    count[i]++;
    if (s3_solve_r3(s, dict, n, sol, pos+1, posmax, curr_len+strlen(dict[i]), S, count, diff + (count[i] == 1)))
      return 1;
    count[i]--;
  }

  return 0;
}

void s3_solve(char *s, char **dict, int n) {
  int min_str_len = 2; // Ho barato per non calcolarla. Si tratta della lunghezza della stringa più corta in dict
  int len = strlen(s);
  int max_n_str = len/min_str_len+1; // Dai due dati sopra, ricava il numero massimo di stringhe da scegliere
  int *sol = (int*) calloc(max_n_str, sizeof(int));

  int K = 2;
  int S = 3;
  int *count = NULL;

  // Vers. 1
  fprintf(stdout, "S3 - V1\n");
  count = (int*) calloc(n, sizeof(int));
  s3_solve_r1(s, dict, n, sol, 0, max_n_str, 0, K, count);
  free(count);

  // Vers. 2
  fprintf(stdout, "S3 - V2\n");
  count = (int*) calloc(n, sizeof(int));
  s3_solve_r2(s, dict, n, sol, 0, max_n_str, 0, S, count, 0);
  free(count);

  // Vers. 3
  fprintf(stdout, "S3 - V3\n");
  count = (int*) calloc(n, sizeof(int));
  s3_solve_r3(s, dict, n, sol, 0, max_n_str, 0, S, count, 0);
  free(count);
}

void s3() {
  fprintf(stdout, "\n\t *** S3 *** \n");
  char *s = "abracadabra";
  char **dict = calloc(8, sizeof(char*));
  dict[0] = strdup("a");
  dict[1] = strdup("ab");
  dict[2] = strdup("cada");
  dict[3] = strdup("abra");
  dict[4] = strdup("ra");
  dict[5] = strdup("da");
  dict[6] = strdup("ca");
  dict[7] = strdup("bra");
  s3_solve(s, dict, 8);
}
