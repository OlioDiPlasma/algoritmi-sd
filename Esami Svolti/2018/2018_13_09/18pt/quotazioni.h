/* file: quotazioni.h */
#ifndef QUOTAZIONI_H
#define QUOTAZIONI_H

#include "data.h"

/* --- ADT I CLASSE --- */
/* Nascondiamo la struttura reale del BST all'utente (Puntatore opaco) */
typedef struct bst_quotazioni *QUOTAZIONI;

/* Inizializza una collezione vuota */
QUOTAZIONI quotazioni_init();

/* Inserisce o aggiorna una transazione per calcolare la quotazione giornaliera */
/* Notare che passiamo il puntatore al puntatore (QUOTAZIONI *) per modificare la radice */
void quotazioni_aggiorna(QUOTAZIONI *q, Data data, float valore, int quantita);

/* Cerca una quotazione specifica */
float quotazioni_cerca_data(QUOTAZIONI q, Data data);

/* Cerca min e max in un intervallo di date (o tutto se date nulle) */
void quotazioni_minmax_intervallo(QUOTAZIONI q, Data d1, Data d2, float *min, float *max);

/* Libera la memoria */
void quotazioni_free(QUOTAZIONI q);

#endif