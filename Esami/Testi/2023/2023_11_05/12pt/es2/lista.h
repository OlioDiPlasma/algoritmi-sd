#ifndef LISTA_H
#define LISTA_H

/* --- DEFINIZIONE TIPI --- */

/* * NODE: definito come "quasi ADT". 
 * La struttura è visibile qui, quindi chi include questo file può accedere a 'val' e 'next'.
 */
struct node {
    int val;
    struct node *next;
};

/* * LIST: ADT di I Categoria.
 * Rappresentiamo la lista semplicemente come un puntatore al primo nodo.
 * Non usiamo sentinelle (nodi fittizi), quindi la lista vuota è NULL.
 */
typedef struct node* LIST;

/* --- PROTOTIPI --- */

/*
 * Funzione richiesta dall'esercizio.
 * Riceve la lista (puntatore alla testa) e la modifica in-place.
 */
void f(LIST l);

/* Funzioni helper per il test (non richieste esplicitamente ma necessarie per il main) */
LIST crea_nodo(int valore);
void stampa_lista(LIST l);
void libera_lista(LIST l);

#endif