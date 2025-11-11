#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bst.h"

typedef struct nodo_ {
  struct nodo_ *l, *r;
  int cnt;
  PACCO p;
} nodo;

typedef nodo* link;

struct bst {
  link root;
};

BST BSTinit() {
  BST bst = calloc(1, sizeof(*bst));
  return bst;
}

int BSTempty(BST bst) {
  if (bst == NULL || bst->root == NULL)
    return 1;
  return 0;
}

static PACCO searchR(link h, char *cod) {
  int cmp;
  if (h == NULL)
    return NULL;
  cmp = strcmp(cod, PACCOgetCod(h->p));
  if (cmp == 0)
    return h->p;
  else if (cmp < 0)
  	return searchR(h->l, cod);
  else
	return searchR(h->r, cod);
}

link rotR(link h) {
  link x = h->l;
  h->l = x->r;
  x->r = h;
  x->cnt = h->cnt;
  h->cnt = 1;
  h->cnt += (h->l) ? h->l->cnt : 0;
  h->cnt += (h->r) ? h->r->cnt : 0;
  return x;
}

link rotL(link h) {
  link x = h->r;
  h->r = x->l;
  x->l = h;
  x->cnt = h->cnt;
  h->cnt = 1;
  h->cnt += (h->l) ? h->l->cnt : 0;
  h->cnt += (h->r) ? h->r->cnt : 0;
  return x;
}

static link partR(link h, int r) {
  if (h == NULL)
    return NULL;
  int t = (h->l) ? h->l->cnt : 0;
  if (t > r) {
    h->l = partR(h->l, r);
    h = rotR(h);
  }
  if (t < r) {
    h->r = partR(h->r, r-t-1);
    h = rotL(h);
  }
  return h;
}

static link joinLR(link a, link b) {
  if (b == NULL)
    return a;
  if (a == NULL)
    return b;
  b = partR(a, 0);
    b->l = a;
  return b;
}

static link deleteR(link h, char *cod) {
  link y;
  int cmp;
  if (h == NULL)
    return NULL;
  cmp = strcmp(cod, PACCOgetCod(h->p));
  if (cmp < 0)
  	h->l = deleteR(h->l, cod);
  if (cmp > 0)
    h->r = deleteR(h->r, cod);
  h->cnt--;
  if (cmp == 0) {
    y = h;
    h = joinLR(h->l, h->r);
    free(y);
  }
  return h;
}

PACCO BSTsearch(BST bst, char *cod) {
  return searchR(bst->root, cod);
}

void BSTremove(BST bst, char *cod) {
  bst->root = deleteR(bst->root, cod);
}

static link NEW(PACCO p, link left, link right) {
  link l = calloc(1, sizeof(*l));
  l->p = p;
  l->l = left;
  l->r = right;
  return l;
}

static link insertR(link h, PACCO p) {
  int cmp;
  link n;
  if (h == NULL) {
    n = NEW(p, NULL, NULL);
    return n;
  }
  cmp = strcmp(PACCOgetCod(p), PACCOgetCod(h->p));
  if (cmp < 0) {
    h->l = insertR(h->l, p);
  } else if (cmp > 0) {
    h->r = insertR(h->r, p);
  } else {
    // Duplicato. Non dovrebbe mai capitare!
  }
  h->cnt++;
  return h;
}

void BSTinsert(BST bst, PACCO p) {
  bst->root = insertR(bst->root, p);
  return;
}

static void treePrint(link h, FILE *fp) {
  if (h == NULL)
    return;

  treePrint(h->l, fp);
  PACCOstore(fp, h->p);
  treePrint(h->r, fp);
}

void BSTstore(BST bst, FILE *fp) {
  printf("\n TREE \n");
  if (BSTempty(bst))
    return;
  treePrint(bst->root, fp);
}
