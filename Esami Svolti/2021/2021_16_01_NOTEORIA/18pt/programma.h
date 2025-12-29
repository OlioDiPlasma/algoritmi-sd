#ifndef PROGRAMMA_H
#define PROGRAMMA_H

#include "gestione_articoli.h"

/* Definiamo una struct wrapper per il programma */
typedef struct {
    int **griglia; // Matrice R x S che contiene gli ID degli articoli (-1 se vuota)
    int R;         // Sale
    int S;         // Slot temporali
} Programma;

/* Inizializzazione e Pulizia */
Programma init_programma(int R, int S);
void free_programma(Programma p);
void stampa_programma(Programma p, Articoli art_db);

/* --- PUNTO 4.2.3: VERIFICA --- */
int verifica_programma_da_file(char *filename, Programma p, Articoli art_db);

/* --- PUNTO 4.2.4: OTTIMIZZAZIONE --- */
void genera_programma_ottimo(Programma p, Articoli art_db);

#endif