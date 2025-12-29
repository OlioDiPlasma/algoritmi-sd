#ifndef GESTIONE_ARTICOLI_H
#define GESTIONE_ARTICOLI_H

#include "articolo.h"

/* ADT I Classe: Puntatore a una struct opaca (definita nel .c) */
typedef struct tabella_articoli *Articoli;

/* Inizializza e carica dal file */
Articoli carica_articoli(char *filename);

/* Getter per accedere ai dati dall'esterno */
int get_num_articoli(Articoli a);
Articolo get_articolo_by_index(Articoli a, int index);

/* Libera tutta la memoria della collezione */
void free_articoli(Articoli a);

#endif