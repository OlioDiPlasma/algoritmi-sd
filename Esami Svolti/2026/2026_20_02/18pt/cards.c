#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cards.h"

/* Parte relativa all'Item Card. Per I/O si ipotizza un semplice formato per rappresentare
   la carta su file
*/

Card CardMakeVoid(void) {
  Card c={"", cuori /*dummy*/, -1};
  return c;
}

int CardCheckVoid(Card c) {
  return c.val<0;
}

void CardStore(Card c, FILE *f) {
  char seeds[]="CQFP";
  if (c.val<11)
    fprintf(f, "%d ", c.val);
  else 
    fprintf(f, "%c ", c.val==11 ? 'F' : (c.val==12?'D':'R'));
  fprintf(f, "%c\n", seeds[c.seed]);
}

Card CardRead(FILE *f) {
  static char *names[]={"","uno","due","tre","quattro","cinque",
                        "sei","sette","otto","nove","dieci",
                        "fante","donna","re"};
  char valstr[4], seed;
  Card c;
  if (fscanf(f,"%s %c ", valstr, &seed)==EOF)
    return CardMakeVoid();
  if (isdigit(valstr[0]))
    c.val = atoi(valstr);
  else {
    switch (toupper(valstr[0])) {
    case 'F': c.val=11; break;
    case 'D': c.val=12; break;
    case 'R': c.val=13; break;
    default: /* non si gestisce l'errore */
    }
    strcpy(c.name,names[c.val]);
  }
  switch (toupper(seed)) {
  case 'C': c.seed=cuori; break;
  case 'Q': c.seed=quadri; break;
  case 'F': c.seed=fiori; break;
  case 'P': c.seed=picche; break;
  default: /* non si gestisce l'errore */
  }
  return c;
}

/* parte relativa a CardLIST: variante dell'ADT lista.
   si realizzsano solo le funzioni utilizzate: nelo compito di esame era sensato considerare
   standard tutte le funzioni eccetto quelle di lettura e scrittura su file. 
*/

typedef struct node *link;
struct node { Card val; link next; };

struct cardlist {
  link head, tail;
  int N;
};

link NEW(Card val, link next) {
  link x = malloc(sizeof *x) ;
  x->val = val; x->next = next;
  return x;
}

CardLIST CardLISTinit(void) {
  CardLIST cl = malloc(sizeof *cl) ;
  cl->head = cl->tail = NULL;
  cl->N=0;
  return cl;
}

void CardLISTinsTail (CardLIST cl, Card val) {
  if (cl->head == NULL){
    cl->head = cl->tail = NEW(val, NULL) ;
  }
  else {
    cl->tail->next = NEW(val,NULL);
    cl->tail = cl->tail->next;
  }
  cl->N++;
}

void CardLISTfree(CardLIST cl) {
  link x, t;
  for (x=cl->head; x!=NULL; x=t) {
    t=x->next;
    free(x);
  }
  free(cl);
}

void CardLISTstore(CardLIST cl, FILE *f) {
  link x;
  for (x=cl->head; x!=NULL; x=x->next)
    CardStore(x->val,f);
}

CardLIST CardLISTread(FILE *f) {
  Card c;
  CardLIST cl = CardLISTinit();
  int done=0;
  while (!done) {
    c=CardRead(f);
    if (CardCheckVoid(c))
      done=1;
    else
      CardLISTinsTail (cl, c);
  }
  return cl;
}

/* Parte relativa a SOL */

struct sol {
  CardLIST lists[2];
};

void SOLfree(SOL s) {
  CardLISTfree(s->lists[0]);
  CardLISTfree(s->lists[1]);
  free(s);
}

SOL SOLinit(void) {
  SOL s = malloc(sizeof(*s));
  s->lists[0]=CardLISTinit();
  s->lists[1]=CardLISTinit();
  return s;
}


CardLIST SOLgetList(SOL s, int i) {
  return s->lists[i];
}

/* problema di verifica */

/* per evitare un costo quadratico, si scandiscono con costo lineare le due liste,
   contando i valori in due vettori di contatori (una matrice di due righe). A partire dai contatori,
   la verifica vera e propria viene effettuata nella funzione checkCardCounts. La funzione interna
   checkCardCounts verra' usata nella checkSol (problema di ottimizzazione).
*/

int checkCardCounts(int valCount[2][14]) {
  for (int val=1; val<=13; val++) {
    if (valCount[0][val]>0 && valCount[1][val]==0 || 
        valCount[0][val]==0 && valCount[1][val]>0)
      return 0;
  }
  return 1;
}

int checkCardLists(CardLIST S1, CardLIST S2) {
  int valCount[2][14]={0}; 
  link x;
  for (x=S1->head; x!=NULL; x=x->next)
    valCount[0][x->val.val]++;
  for (x=S2->head; x!=NULL; x=x->next)
    valCount[1][x->val.val]++;
  return checkCardCounts(valCount);
}

/* problema di ottimizzazione */


/* Si cercano due sequenza della stessa lunghezza, per cui la struttura dati piu semplice
   (e piu' vicina a quanto fatto nel corso) per gestire la soluzione e' un vettore  in cui
   rappresentare una sola permutazione delle carte disponibili: la prima sequenza corrisponde
   agli indici pari (0,2,4,...), la seconda agli indici dispari (1,3,5,...).
   Il modello più adatto sono le permutazioni semplici (oppure le disposizioni, a patto di
   utilizzarle correttamente) in quanto occorre provare tutti i possibili ordini per le carte
   selezionate.
   NON conviene decisamente utilizzare liste, ne' per l'insieme di carte disponibili ne' per la
   soluzione, all'interno della funzione ricorsiva, in quanto non permettono accesso diretto.
   La lista in ingresso viene quoindi convertita in un vettore (cards). La soluzione viene
   gestita con vettori. Solo al termine si converte la soluzione in due liste.
*/

void perm_sempl(int pos, int mark[], Card cards[],
                int sol[], int bestSol[], int *bestNumP,
                int nFig[2], int n, int nFigTot, int valCount[2][14]);

SOL bestSequences(CardLIST S) { 
  int n=S->N, bestNum=0, nFig[2]={0,0}, nFigTot=0;
  Card *cards = malloc(n*sizeof(Card)); 
  int *mark = calloc(n,sizeof(int));
  int *sol = malloc(n*sizeof(int));
  int *bestSol = malloc(n*sizeof(int));
  int valCount[2][14]={0}; 
  /* passa da lista a vettore */
  link x;
  int i;
  for (i=0, x=S->head; x!=NULL; i++, x=x->next) {
    cards[i]=x->val;
    if (cards[i].val>10) nFigTot++;
  }

  perm_sempl(0, mark, cards, sol, bestSol, &bestNum, nFig, n, nFigTot, valCount);

  /* genera soluzione */
  SOL solList = SOLinit();  
  for (int i=0; i<bestNum; i++)
    CardLISTinsTail (solList->lists[i%2], cards[bestSol[i]]);
  
  free(cards);
  free(mark);
  free(sol);
  free(bestSol);
  return solList;
}

/* verifica per prima cosa i primi due vincoli. Poi si verifica non il terzo vincolo, ma la possibilità
   che il caso migliore (se si usassero tutte le figure presenti) poss soddisfare il vincolo.
   Si potrebbe ulteriormente usare il terzo vincolo con un paradigma che (pur se assimilabile a pruning)
   si chiama "branch and bound": consiste nell'evitare una ricorsione se non ha speranza
   di migliorare l'ottimo attuale; in base al numero di figure ancora disponibili si potrebbe stimare
   la lunghezza massima raggiungibile, verificando la possibilità di migliorare l'ottimo.
   Il paradigma branch and bound non fa parte del programma di Algoritmi.
*/
int prune(int sol[], Card cards[], int pos, int i, int nFig[2], int n, int nFigTot) {
  if (pos<2) return 0; // niente da verificare
  int prev=sol[pos-2];
  if (abs(cards[i].val-cards[prev].val)>2 ||
      cards[i].seed == cards[prev].seed)
    return 1;
  int nNum = pos-(nFig[0]+nFig[1]); // carte numeriche gia' usate
  if (2*nFigTot < (nNum-1)) // non ci sono complessivamente abbastanza figure per raggiungere i numeri
    return 1;
  return 0;
}

int checkSol(int nFig[2], int pos, int valCount[2][14]) {
  if (3*nFig[0]<pos/2 || 3*nFig[0]<pos/2) // le sequenza hanno lunghezza pos/2
    return 0;
  return checkCardCounts(valCount);
}

void updateBestSol(int sol[], int pos, int bestSol[], int *bestNumP) {
  *bestNumP = pos;
  for (int i=0; i<pos; i++)
    bestSol[i]=sol[i];
}

/* modello delle permutazioni semplici adattato al problema:
   il pruning verifica le prime due condizioni.
   Si verifica l'accettabilita' di una soluzione ad ogni ricorsione pari.

*/
void perm_sempl(int pos, int mark[], Card cards[],
                int sol[], int bestSol[], int *bestNumP,
                int nFig[2], int n, int nFigTot, int valCount[2][14]) {
  int i;
  if (pos%2==0) {
    /* numero di carte pari; verifica soluzione */
    if (pos > *bestNumP && checkSol(nFig,pos,valCount))
        updateBestSol(sol, pos, bestSol, bestNumP);
  }
  if (pos >= n) {
    /* carte finite: soluzione gia' gestita */
    return;
  }

  for (i=0; i<n; i++)
    if (!mark[i]) {
      if (!prune(sol,cards,pos,i,nFig,n,nFigTot)) {
        mark[i] = 1;
        sol[pos] = i;
        valCount[pos%2][cards[i].val]++; // conta le carte di questo valore
        if (cards[i].val>10) // figura
          nFig[pos%2]++; // conta figure (evita costo O(pos) in checkSol) 
        perm_sempl(pos+1, mark, cards, sol, bestSol, bestNumP, nFig, n, nFigTot, valCount);
        valCount[pos%2][cards[i].val]--;
        if (cards[i].val>10) // figura
          nFig[pos%2]--;
        mark[i] = 0;
      }
    }
}
