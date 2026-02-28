#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "Item.h"
#include "BST.h"


int main (void) {
  Item items[] = {{"milano"},{"cagliari"},{"roma"},{"bologna"},
      {"firenze"},{"palermo"},{"napoli"},{"ravenna"},{"torino"},
      {"genova"},{"cesena"},{""}};
  Key k1="cagliari", k2="ravenna";
  int i;

  BST b = NULL, b2;
  
  b = BSTinit();

  for (i=0; !ITEMcheckNull(items[i]); i++) {
    BSTinsert_leafR(b, items[i]);
  }
  printf("BST originale:\n");
  BSTvisit(b,INORDER);
  b2 = BSTdup(b);
  printf("BST duplicato:\n");
  BSTvisit(b2,INORDER);

  BSTfree(b);
  BSTfree(b2);
}

