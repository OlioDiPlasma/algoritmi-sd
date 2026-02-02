#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ST.h"

struct symboltable {Key *a; int maxN; int N;};

int DateCmp(Date d1, Date d2) {
  if (d1.yy!=d2.yy) return d1.yy-d2.yy;
  if (d1.mm!=d2.mm) return d1.mm-d2.mm;
  return d1.dd-d2.dd;
}

int KeyCmp(Key k1, Key k2) {
  int r = strcmp(k1.name, k2.name);
  if (r==0) return r;
  return DateCmp(k1.bd,k2.bd);  
}

int KeyRead(FILE *f, Key *k) {
  if (fscanf(f, "%s %d/%d/%d",
             k->name, &k->bd.dd, &k->bd.mm, &k->bd.yy)>0)
    return 1;
  else return 0;
}

ST STinit(int maxN) {
  ST st;
  st = malloc(sizeof (*st));
  if (st == NULL) {
    printf("Memory allocation error\n");
    return NULL;
  }
  st->a = calloc(maxN, sizeof(Key));
  if (st->a == NULL) {
    printf("Memory allocation error\n");
    free(st);
    return NULL;
  }
  st->maxN = maxN;
  st->N = 0;
  return st;
}

void STfree(ST st) {
  int i;
  if (st==NULL)
    return;
  free(st->a);
  free(st);
}

int STsize(ST st) {
  return st->N;
}

void STinsert(ST st, Key k, int i) {
  if (i >= st->maxN) {
    st->a = realloc(st->a, (2*st->maxN)*sizeof(char *));
    if (st->a == NULL)
    st->maxN = 2*st->maxN;
  }
  st->a[i] = k;
  st->N++;
}

int STsearch(ST st, Key k) {
  int i;
  for (i = 0; i  < st->N; i++)
    if (KeyCmp(k, st->a[i])==0)
      return i;
  return -1;
}

Key STsearchByIndex(ST st, int i){
  Key voidKey = {"",{0,0,0}};
  if (i < 0 || i >= st->N)
    return voidKey;
  return (st->a[i]);
}

