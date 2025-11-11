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

int **alloca2d(int r, int c, int init) {
  int i, j, **m = (int**) calloc (r, sizeof(int*));
  for(i=0;i<r;i++) {
    m[i] = (int*) calloc (c, sizeof(int));
  }
  
  if (init)
	  for(i=0;i<r;i++) {
		for(j=0;j<c;j++) m[i][j] = i+j;
	  } 
   
  return m;
}

int s1_min(int **m, int r, int c) {
	int i, j;
	int min = m[0][0];
	for(i=0;i<r;i++) {
		for(j=0;j<c;j++) if (min > m[i][j]) min = m[i][j];
	}
	return min;
}

int s1_max(int **m, int r, int c) {
	int i, j;
	int max = m[0][0];
	for(i=0;i<r;i++) {
		for(j=0;j<c;j++) if (max < m[i][j]) max = m[i][j];
	}
	return max;
}

int s1_sum(int **m, int r, int c) {
	int i, j;
	int sum = 0;
	for(i=0;i<r;i++) {
		for(j=0;j<c;j++) sum += m[i][j];
	}
}

float s1_avg(int **m, int r, int c) {
	return s1_sum(m,r,c) / (float) (r*c);
}

void s1_find_print_quad(int **m, int r, int c, int val, int I, int J, int K) {
	int i, j;
	int sum = 0;
	float avg = 0.0;
	
	// Calcolo valore della sottomatrice
	// SUM
	for(i=0;i<K;i++) {
		for(j=0;j<K;j++) sum += m[I+i][J+j];
	}	
	// AVG
	avg = sum / (float) (K*K);
	
	if (sum < val)   // V1
	//if (sum > val) // V2
	//if (avg < val) // V3
	//if (avg > val) // V4
	{
	  for(i=0;i<K;i++) {
		for(j=0;j<K;j++) fprintf(stdout, "%d ", m[I+i][J+j]);
		fprintf(stdout, "\n");
	  }		
	}
}

void s1_find_print(int **m, int r, int c, int val) {
	int i, j, k;
	for(k=1;k<r && k<c;k++) {
		fprintf(stdout, "DIM: %d\n", k);
		for(i=0;i<=r-k;i++) {
			for(j=0;j<=c-k;j++) {
				s1_find_print_quad(m, r, c, val, i, j, k);
			}
		}
	}
}

void s1_solve(int **m, int r, int c) {
  int i, j, val;
  
  // V1 e V3 - Trova MIN
  val = s1_min(m,r,c);
  // V2 e V4 - Trova MAX
  // val = s1_max(m,r,c);

  s1_find_print(m, r, c, val);

}

void s1() {
  fprintf(stdout, "\t *** S1 *** \n");
  int r = 3, c = 4;
  int **m = alloca2d(r, c, 1);
  s1_solve(m, r, c);
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
  link tail;
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

int s2_check(int val1, int val2) {
	// V1
	return val1 > val2;
	// V2
	return val1 < val2;
	// V3
	return val1%2 != val2%2;
	// Fallback
	return 0;
}

LIST* split(LIST l, int *n){	
	link iter = l->head;
	link prev = NULL;
	int index = -1;
	LIST *lv = NULL;
	while(iter) {
		if (prev == NULL || s2_check(prev->val, iter->val)) {
			// Nuovo inserimento in testa
			(*n)++;
			index++;
			lv = realloc(lv, (*n)*sizeof(LIST));
			lv[index] = LISTinit();
			lv[index]->head = lv[index]->tail = newNode(iter->val, NULL);
		} else {
			lv[index]->tail->next = newNode(iter->val, NULL);
			lv[index]->tail = lv[index]->tail->next;
		}
		prev = iter;
		iter = iter->next;
	}
	
	return lv;
}

void s2_solve(LIST l) {
  if (!l || !l->head) return;
  int i, n = 0;
  LIST *lv = split(l, &n);
  for(i=0;i<n;i++) {
	  fprintf(stdout, "LISTA %d\n", i);
	  LISTprint(lv[i]);
  }
}

void s2() {
  LIST l = LISTinit();
  l->tail = newNode(4, NULL);
  l->head = newNode(3, newNode(10, newNode(6, newNode(3, newNode(1, l->tail)))));
  s2_solve(l);
}

/** S3 **/

// Ritorna un valore diverso da -1 solo se cambia esattamente una lettera tra le due parole
// L'indice della lettera che cambia è il valore ritornato
int dist(char *s1, char *s2) {
	int i, d = -1, len = strlen(s1);
	for(i=0;i<len;i++) {
		if (s1[i] != s2[i]) {
			if (d == -1) d = i;
			else
				return -1;
		}
	}
	return d;
}


static char **s3_sol = NULL;

int s3_solve_r1(char *curr, char *end, char **dict, int n, int k, int p, int *count_p, int *count_l, int pos) {
	
	int i, d;
	char pre;
		
	if (!strcmp(curr, end)) {
		// V2 - ... cambiando almeno k volte ogni lettera 
		int len = strlen(curr);
		for(i=0;i<len;i++) if (count_l[i] < k) return 0;
		
		// Solo per preparare gli esempi
		for(i=0;i<pos;i++) fprintf(stdout, "%s\n", s3_sol[i]);
		return 1;
	}
	
	if (pos >= n*p) {
		return 0;
	}
	
	for(i=0;i<n;i++) {
		if (count_p[i] >= p) continue;
		if ((d = dist(curr, dict[i])) == -1) continue;
		// V1 - ... cambiando al massimo k volte ogni lettera 
		// if (count_l[d] >= k) continue;
		// Solo per gli esempi
		s3_sol[pos] = dict[i];
		count_p[i]++;
		count_l[d]++;
		pre = curr[d];
		curr[d] = dict[i][d];
		if (s3_solve_r1(curr, end, dict, n, k, p, count_p, count_l, pos+1))
			return 1;
		curr[d] = pre;
		count_l[d]--;
		count_p[i]--;
	}
	return 0;
}

void s3_solve(char *start, char *end, char **dict, int n, int k, int p) {
  int *count_l = (int*) calloc(strlen(start), sizeof(int));
  int *count_p = (int*) calloc(n, sizeof(int));
  // Copia di start solo per non distruggerla
  char *curr = strdup(start);

  // Non serve per la soluzione dell'esercizio, è utile solo per preparare gli esempi
  s3_sol = (char**) calloc(n*p, sizeof(char*));

  // Vers. 1
  fprintf(stdout, "S3 - V1\n");
  int ok = s3_solve_r1(curr, end, dict, n, k, p, count_p, count_l, 0);
  fprintf(stdout, "Sequenza %sesiste", ok ? "" : "non ");
  free(count_p);
  free(count_l);
  free(curr);

}

void s3() {
  fprintf(stdout, "\n\t *** S3 *** \n");
  char *start = "male";
  char *end = "cosa";
  char **dict = calloc(11, sizeof(char*));
  dict[0] = strdup("male");
  dict[1] = strdup("mali");
  dict[2] = strdup("pali");
  dict[3] = strdup("palo");
  dict[4] = strdup("poli");
  dict[5] = strdup("voli");
  dict[6] = strdup("vola");
  dict[7] = strdup("cola");
  dict[8] = strdup("vile");
  dict[9] = strdup("pile");
  dict[10] = strdup("cosa");
  s3_solve(start, end, dict, 11, 1, 1);
}
