/* file: titoli.h */
#ifndef TITOLI_H
#define TITOLI_H

#include "quotazioni.h"

typedef struct bst_titoli *TITOLI;

/* Inizializza collezione titoli */
TITOLI titoli_init();

/* Legge file e popola la struttura */
void titoli_leggi_file(TITOLI *t, char *filename);

/* Cerca un titolo e ritorna il puntatore alle sue quotazioni (per fare query su di esse) */
/* Ritorna NULL se non trovato */
QUOTAZIONI titoli_cerca(TITOLI t, char *codice);

/* Wrapper per la funzionalità completa di stampa/debug */
void titoli_stampa_tutto(TITOLI t);

void titoli_free(TITOLI t);

#endif