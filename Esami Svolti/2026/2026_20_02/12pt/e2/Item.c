#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Item.h"

Item ITEMscan() {
  Item val;
  printf("name and value: ");
  scanf("%s", val.name);
  return val;
}

void ITEMstore(Item val) {
  printf("name: %s\n", val.name);
}

int ITEMcheckNull(Item val) {
  if (strcmp(val.name, "")==0)
    return 1;
  return 0;
}

Item ITEMsetNull() {
  Item val = {""};
  return val;
}

void KEYscan(Key k) {
  scanf("%s", k);
}

Key KEYget(Item *pval) {
  return pval->name;
}

int KEYcmp(Key k1, Key k2){
  if (strcmp(k1,k2)<0)
    return -1;
  else if (strcmp(k1,k2)>0)
    return 1;
  else
    return 0;
}

