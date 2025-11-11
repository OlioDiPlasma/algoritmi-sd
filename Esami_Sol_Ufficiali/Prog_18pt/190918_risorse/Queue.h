#ifndef _QUEUE_INCLUDED
#define _QUEUE_INCLUDED

typedef struct queue *Q;
typedef struct P_ {
  int r;
  int c;
  int id;
  int dist;
} P;

Q       QUEUEinit(void);
int     QUEUEempty(Q q);
void    QUEUEput (Q q, P p);
P       QUEUEget(Q q);
void    QUEUEfree(Q q);

#endif

