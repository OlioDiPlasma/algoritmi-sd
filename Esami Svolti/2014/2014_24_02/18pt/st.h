#ifndef ST_H
#define ST_H

/* Definizione della struttura per la Symbol Table (ADT di I categoria) */
typedef struct symboltable *ST;

/* --- PROTOTIPI --- */

/* Inizializza una ST vuota con capacita' iniziale max_n */
ST STinit(int max_n);

/* Dealloca completamente la ST */
void STfree(ST st);

/* Inserisce una stringa nella ST se non esiste gia'. Restituisce l'indice associato. */
int STinsert(ST st, char *nome);

/* Cerca una stringa e restituisce l'indice, oppure -1 se non trovata */
int STsearch(ST st, char *nome);

/* Dato un indice, restituisce la stringa originale (utile per la stampa) */
char *STsearchByIndex(ST st, int indice);

/* Restituisce il numero di elementi attuali nella ST */
int STcount(ST st);

#endif