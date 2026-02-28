#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>

#define MAXC 11

typedef struct {
  char name[MAXC];
} Item;

typedef char *Key;

Item    ITEMscan();
Item    ITEMsetNull();
int     ITEMcheckNull(Item val);
void    ITEMstore(Item val);
int     KEYcmp(Key k1, Key k2);
Key     KEYget(Item *pval);
void    KEYscan(Key k);

#endif

