#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE ADT DI I CLASSE
 * L'utente vede solo il puntatore 'list_t'. La struttura reale è nascosta nel .c.
 */
typedef struct list_dummy *list_t;

/*
 * PROTOTIPO FUNZIONE RICHIESTA
 * Genera il prodotto cartesiano ordinato per prodotto crescente.
 */
list_t prodCart(int *v1, int d1, int *v2, int d2);

/*
 * FUNZIONI DI UTILITÀ PER IL TEST
 * (Non strettamente richieste dalla traccia per la logica, ma indispensabili per vedere il risultato)
 */
void listPrint(list_t L); // Stampa le coppie
void listFree(list_t L);  // Libera la memoria

#endif