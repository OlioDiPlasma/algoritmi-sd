#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE TIPO ADT (OPACO)
 * "list" è un puntatore a una struttura "list_dummy" che l'utente non conosce.
 * Questo garantisce l'incapsulamento (Information Hiding).
 */
typedef struct list_dummy *list;

/* * FUNZIONE RICHIESTA DALLA TRACCIA
 */
void swap(list L, int k);

/* * FUNZIONI DI UTILITÀ (Per far funzionare il test)
 */
list LISTinit();                  // Crea lista vuota
void LISTadd(list L, int val);    // Aggiunge in coda
void LISTprint(list L);           // Stampa
void LISTfree(list L);            // Libera memoria

#endif