#ifndef BST_H_DEFINED
#define BST_H_DEFINED

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "pacco.h"

typedef struct bst *BST;

int           BSTempty(BST bst);
PACCO         BSTsearch(BST bst, char *cod);
void          BSTremove(BST bst, char *cod);
void          BSTinsert(BST bst, PACCO p);
void          BSTstore(BST bst, FILE *fp);
BST           BSTinit();

#endif
