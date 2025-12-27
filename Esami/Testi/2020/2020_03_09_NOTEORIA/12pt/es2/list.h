#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE TIPO ADT DI I CLASSE
 * list_t è un puntatore a una struttura definita solo nell'implementazione (.c).
 * Questo nasconde i dettagli (incapsulamento).
 */
typedef struct list_dummy *list_t;

/*
 * PROTOPI FUNZIONI
 */

/* Inizializza una nuova lista vuota */
list_t LISTinit();

/* * Inserisce un valore. 
 * Nota: La traccia mette "..." ma logicamente serve il valore intero.
 * Assumiamo la firma: void LISTinsert(list_t L, int posizione, int val);
 */
void LISTinsert(list_t L, int posizione, int val);

/* Funzione extra per visualizzare la lista (utile per il debug/test) */
void LISTprint(list_t L);

/* Libera la memoria */
void LISTfree(list_t L);

#endif