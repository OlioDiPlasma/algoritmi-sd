#ifndef ALBERO_H  /* --- GUARDIA DI INCLUSIONE (Evita doppie inclusioni) --- */
#define ALBERO_H

/* * DEFINIZIONE TIPO OPACO (ADT PRIMA CATEGORIA)
 * L'utente sa che esiste 'nTREE', ma non sa che è un puntatore a 'struct tree_wrapper'.
 * La struct vera sarà definita solo nel .c.
 */
typedef struct tree_wrapper *nTREE;

/* --- FUNZIONI UTENTE (API) --- */

/* Inizializza un albero vuoto */
nTREE initTree();

/* Libera tutta la memoria */
void freeTree(nTREE t);

/* * Funzione richiesta dall'esercizio.
 * Restituisce per riferimento i conteggi dei nodi con 1, 2 o 3 figli.
 */
void countIf(nTREE t, int *count1, int *count2, int *count3);

/* * FUNZIONE DI TEST (Helper)
 * Poiché il main non può accedere ai campi 'root' o 'figli' (sono privati nel .c),
 * creiamo una funzione qui che popola l'albero per noi.
 */
void popola_albero_per_test(nTREE t);

#endif