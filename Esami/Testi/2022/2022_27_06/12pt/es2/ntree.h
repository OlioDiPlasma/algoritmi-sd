#ifndef NTREE_H
#define NTREE_H

/* --- DEFINIZIONE COSTANTI --- */
/* Definiamo N, il grado massimo dell'albero (numero massimo di figli).
 * In un caso reale potrebbe essere definito dinamicamente, ma qui lo fissiamo a 5.
 */
#define N 5

/* --- DEFINIZIONE TIPI (ADT DI PRIMA CATEGORIA) --- */
/* * "nTREE" è un puntatore opaco a una struttura "n_tree_s".
 * L'utente nel main userà solo "nTREE" senza sapere cosa c'è dentro.
 * Questo garantisce l'incapsulamento dei dati.
 */
typedef struct n_tree_s *nTREE;

/* --- PROTOTIPI DELLE FUNZIONI --- */

/* * Scopo: Inizializza un albero vuoto.
 * Ritorna: Un puntatore (handle) all'ADT nTREE.
 */
nTREE ntree_init();

/* * Scopo: Crea un nuovo nodo e lo collega all'albero (funzione di supporto per il test).
 */
void ntree_insert_root(nTREE t, char *key);
void ntree_insert_child(nTREE t, char *parent_key, char *child_key);

/* * Scopo: Funzione richiesta dall'esercizio.
 * Conta i nodi con grado > del padre.
 */
int countIf(nTREE t);

/* * Scopo: Libera tutta la memoria dell'albero.
 */
void ntree_free(nTREE t);

#endif