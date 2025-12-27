#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE DEI TIPI
 * LIST è un "puntatore opaco". Chi usa questo header sa che esiste una LIST,
 * ma non sa come è fatta dentro (struct list_s). Questo garantisce
 * l'incapsulamento (ADT I Classe).
 */
typedef struct list_s *LIST;

/* PROTOTIPI */

// Crea una lista vuota
LIST LIST_new();

// Inserisce un elemento in coda (utile per il main)
void LIST_append(LIST l, int val);

// Stampa la lista (utile per il debug)
void LIST_print(LIST l);

// Distrugge la lista e libera la memoria
void LIST_free(LIST l);

/* * FUNZIONE RICHIESTA DALL'ESERCIZIO
 * Ritorna un puntatore a LIST (quindi un array di LIST).
 * Imposta *n con la dimensione di tale array.
 */
LIST* split(LIST l, int *n);

#endif