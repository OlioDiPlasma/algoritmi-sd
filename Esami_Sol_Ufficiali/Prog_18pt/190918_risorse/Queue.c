#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

typedef struct QUEUEnode *link;

struct QUEUEnode{ P p; link next; };
struct queue { link head; link tail; int size; };

static link NEW(P p, link next);
static void LINKfree(link l);

static link NEW(P p, link next) {
  link x = malloc(sizeof *x) ;
  x->p = p;
  x->next = next;
  return x;
}

Q QUEUEinit() {
  Q q = malloc(sizeof *q) ;
  q->head = NULL;
  q->size = 0;
  return q;
}

int QUEUEempty(Q q) {
  return q->head == NULL;
}

int QUEUEsize(Q q) {
    return q->size;
}

void QUEUEput (Q q, P p) {
  if (q->head == NULL) {
    q->tail = NEW(p, q->head) ;
    q->head = q->tail;
    q->size += 1;
    return;
  }
  q->tail->next = NEW(p, q->tail->next) ;
  q->tail = q->tail->next;
  q->size += 1;
}

P QUEUEget(Q q){
  P p = q->head->p;
  link t = q->head->next;
  free(q->head);
  q->head = t;
  q->size--;
  return p;
}

static void LINKfree(link l) {
  if (l != NULL) {
    LINKfree(l->next);
    free(l);
  }
}

void QUEUEfree(Q q) {
  LINKfree(q->head);
  free(q);
}
