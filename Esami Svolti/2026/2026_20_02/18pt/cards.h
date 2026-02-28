#ifndef CARDS_H
#define CARDS_H

#define MAXS 10

typedef struct cardlist *CardLIST;
typedef struct sol *SOL;
typedef enum {cuori, quadri, fiori, picche} seed_e;

typedef struct {
  char name[MAXS];
  seed_e seed;
  int val;  
} Card;

int checkCardLists(CardLIST S1, CardLIST S2);
SOL bestSequences(CardLIST S);
void CardLISTstore(CardLIST cl, FILE *f);
CardLIST CardLISTread(FILE *f);
CardLIST SOLgetList(SOL sol, int id);
void CardLISTfree(CardLIST cl);
void SOLfree(SOL sol);

#endif
