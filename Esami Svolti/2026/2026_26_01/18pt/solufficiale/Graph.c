#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "Graph.h"

#define MAXC 11
#define maxWT INT_MAX

typedef struct node *link;
struct node { int v; int wt; int en; link next; } ;
struct graph { int V; int E; link *ladj; ST tab; link z; } ;

static Edge  EDGEcreate(int v, int w, int wt);
static link  NEW(int v, int wt, link next);
static void  insertE(Graph G, Edge e);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);

/* INIZIO FUNZIONI DEL COMPITO DI ESAME 26/1/2026 */

typedef struct parents {
  int p1;
  int p2;
} par_t;

/* variante delle visita in ampiezza, in cui si possono percorrere
nella parte iniziale di un cammino anche gli archi trasposti, per
cui occorre immagazzinare questi archi (al maassimo due per nodo)
*/

static int bfsK(Graph G, par_t *pArray, int v1, int v2, int k) {
  int dist[G->V], enParent[G->V], res=0;
  QUEUE q = QUEUEinit(G->V);
  for (int v=0; v<G->V; v++) dist[v]=-1;
  QUEUEput(q, v1);
  dist[v1]=0;
  enParent[v1]=1;
  while (!QUEUEempty(q)) {
    int v = QUEUEget(q);
    if (dist[v]>k)
      break; // superata distanza massima k
    if (v==v2) {
      res=1; break; // raggiunto nodo destinazione
    }
    if (enParent[v]) {
      // se cammino in salita abilitato, visita i genitori
      int p1 = pArray[v].p1;
      int p2 = pArray[v].p2;
      if (p1>=0 && dist[p1]<0) {
        dist[p1]=dist[v]+1; QUEUEput(q,p1); enParent[p1]=1;
      }
      if (p2>=0 && dist[p2]<0) {
        dist[p2]=dist[v]+1; QUEUEput(q,p2); enParent[p2]=1;
      }
    }
    // ora cammino in discesa, disabilitando la risalita
    for (link t=G->ladj[v]; t != G->z; t = t->next) {
      if (dist[t->v]<0) {
        dist[t->v]=dist[v]+1; QUEUEput(q,t->v); enParent[t->v]=0;
      }
    }
  }
  QUEUEfree(q);
  return res;
}

int relationshipK(Graph G, char *name1, Date d1, char *name2, Date d2, int k) {
  par_t *pArray = malloc(G->V * sizeof (*pArray));
  int v1, v2;
  Key k1, k2;
  
  for (int v=0; v<G->V; v++)
    pArray[v].p1 = pArray[v].p2 = -1;
  for (int v=0; v<G->V; v++) {
    // percorri flgli/e di v e assegna v come genitore
    for (link t=G->ladj[v]; t != G->z; t = t->next) {
      struct parents *p = &pArray[t->v];
      if (p->p1==-1) p->p1 = v;
      else p->p2 = v;
    }
  }
  strcpy(k1.name,name1); k1.bd=d1;
  strcpy(k2.name,name2); k2.bd=d2;
  v1 = STsearch(G->tab, k1);
  v2 = STsearch(G->tab, k2);
  int res = bfsK(G, pArray, v1, v2, k);

  free(pArray);
  return res;
}
  
static int bfsDownCount(Graph G, int v0, int k) {
  int dist[G->V], cnt=0;
  QUEUE q = QUEUEinit(G->V);
  for (int v=0; v<G->V; v++) dist[v]=-1;
  QUEUEput(q, v0);
  dist[v0]=0;
  while (!QUEUEempty(q)) {
    int v = QUEUEget(q);
    if (dist[v]>k)
      break; // superata distanza massima k
    // cammino in discesa su nodi figli/adiacenti 
    cnt++;
    for (link t=G->ladj[v]; t != G->z; t = t->next) {
      if (dist[t->v]<0) {
        dist[t->v]=dist[v]+1; QUEUEput(q,t->v);
      }
    }
  }
  QUEUEfree(q);
  return cnt;
}

void largestLineageK(Graph G, int k) {
  int max=0, vmax=-1, cnt_v;
  for (int v=0; v<G->V; v++) {
    cnt_v = bfsDownCount(G, v, k);
    if (cnt_v > max) {
      max=cnt_v;
      vmax=v;
    }
  }
  Key k_max = STsearchByIndex(G->tab,vmax);
  printf("la persona con più discendenza di grado %d è %s\n",
         k, k_max.name);
  printf("nata il %d/%d/%d\n", k_max.bd.dd,k_max.bd.mm,k_max.bd.yy);
}


/* FINE FUNZIONI DEL COMPITO DI ESAME 26/1/2026 */

static Edge EDGEcreate(int v, int w, int wt) {
  Edge e;
  e.v = v;
  e.w = w;
  e.wt = wt;
  return e;
}

static link NEW(int v, int wt, link next) {
  link x = malloc(sizeof *x);
  if (x == NULL)
    return NULL;
  x-> en = 1; /* ADDED */
  x->v = v;
  x->wt = wt;
  x->next = next;
  return x;
}

Graph GRAPHinit(int V) {
  int v;
  Graph G = malloc(sizeof *G);
  if (G == NULL)
    return NULL;

  G->V = V;
  G->E = 0;
  G->z = NEW(-1, 0, NULL);
  if (G->z == NULL)
    return NULL;
  G->ladj = malloc(G->V*sizeof(link));
  if (G->ladj == NULL)
    return NULL;
  for (v = 0; v < G->V; v++)
    G->ladj[v] = G->z;
  G->tab = STinit(V);
  if (G->tab == NULL)
    return NULL;
  return G;
}

void GRAPHfree(Graph G) {
  int v;
  link t, next;
  for (v=0; v < G->V; v++)
    for (t=G->ladj[v]; t != G->z; t = next) {
      next = t->next;
      free(t);
    }
  STfree(G->tab);
  free(G->ladj);
  free(G->z);
  free(G);
}

Graph GRAPHload(FILE *fin) {
  int V, i, id1, id2, wt;
  Key k1, k2;
  Graph G;

  fscanf(fin, "%d", &V);
  G = GRAPHinit(V);

  for (i=0; i<V; i++) {
    if (KeyRead(fin,&k1)) {
      // ignora data morte
      fscanf(fin, "%*d/%*d/%*d ");
      STinsert(G->tab, k1, i);
    }
  }

  while(KeyRead(fin,&k1)&&KeyRead(fin,&k2)) {
    id1 = STsearch(G->tab, k1);
    id2 = STsearch(G->tab, k2);
    if (id1 >= 0 && id2 >=0)
      GRAPHinsertE(G, id1, id2, wt);
  }
  return G;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
  insertE(G, EDGEcreate(id1, id2, wt));
}

void  GRAPHshowList(Graph G) {
  int v;
  link t, next;

  for (v=0; v < G->V; v++) {
    printf("%d ", v);
    for (t=G->ladj[v]; t != G->z; t = next) {
      next = t->next;
      printf("-> %d/%d ", t->v, t->wt);
    }
    printf("\n");
  }
}

void  GRAPHedges(Graph G, Edge *a) {
  int v, E = 0;
  link t;
  for (v=0; v < G->V; v++)
    for (t=G->ladj[v]; t != G->z; t = t->next)
      if (v < t->v)
        a[E++] = EDGEcreate(v, t->v, t->wt);
}

void GRAPHstore(Graph G, FILE *fout) {
  int i;
  Edge *a;

  a = malloc(G->E * sizeof(Edge));
  if (a == NULL)
    return;

  GRAPHedges(G, a);

  fprintf(fout, "%d\n", G->V);
  for (i = 0; i < G->V; i++) {
    Key k = STsearchByIndex(G->tab, i);
    fprintf(fout, "%s %d/%d/%d\n", k.name, k.bd.dd, k.bd.mm, k.bd.yy); 
  }
  for (i = 0; i < G->E; i++) {
    Key k1 = STsearchByIndex(G->tab, a[i].v);
    Key k2 = STsearchByIndex(G->tab, a[i].w);
    fprintf(fout, "%s %d/%d/%d %s %d/%d/%d\n",
            k1.name, k1.bd.dd, k1.bd.mm, k1.bd.yy,
            k2.name, k2.bd.dd, k2.bd.mm, k2.bd.yy);
  }
  free(a);
}

static void  insertE(Graph G, Edge e) {
  int v = e.v, w = e.w, wt = e.wt;

  G->ladj[v] = NEW(w, wt, G->ladj[v]);
  G->ladj[w] = NEW(v, wt, G->ladj[w]);
  G->E++;
}

