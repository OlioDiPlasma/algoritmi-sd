#include "Queue.h"
#include <stdlib.h>

struct queue {
  int *q;
  int N, head, tail;
};

QUEUE QUEUEinit(int maxN) {
  QUEUE q = malloc(sizeof *q) ;
  q->q = malloc(maxN*sizeof(int));
  q->N=maxN+1;
  q->head = q->N;
  q->tail = 0;
  return q;
}

int QUEUEempty(QUEUE q) {
  return (q->head)%(q->N) == q->tail;
}

void QUEUEput(QUEUE q, int val) {
  q->q[q->tail++] = val;
  q->tail = q->tail%q->N;
}

int QUEUEget(QUEUE q) {
  q->head = q->head%q->N;
  return q->q[q->head++];
}

void QUEUEfree(QUEUE q) {
  free(q->q);
  free(q);
}
