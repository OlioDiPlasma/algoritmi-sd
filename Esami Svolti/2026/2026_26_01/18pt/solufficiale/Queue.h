#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue *QUEUE;
QUEUE QUEUEinit (int maxN);
int QUEUEempty (QUEUE q);
void QUEUEput (QUEUE q, int val);
int QUEUEget (QUEUE q);
void QUEUEfree(QUEUE q);

#endif
