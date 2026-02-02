#ifndef ST_H
#define ST_H

#define MAXL 40

typedef struct symboltable *ST;
typedef struct {
  int dd, mm, yy;
} Date;
typedef struct {
  char name[MAXL];
  Date bd;  
} Key;

int KeyRead(FILE *f, Key *k);

ST    STinit(int maxN);
void  STfree(ST st);
int   STsize(ST st);
void  STinsert(ST st, Key k, int i);
int   STsearch(ST st, Key k);
Key STsearchByIndex(ST st, int i);

#endif

