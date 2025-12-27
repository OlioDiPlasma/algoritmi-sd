#ifndef LIST_H
#define LIST_H

/* =========================================================================
 * DEFINIZIONE TIPO ADT (I CLASSE)
 * =========================================================================
 * LIST viene definito come puntatore a una struttura incompleta.
 * Questo garantisce l'incapsulamento: il main non può accedere a l->head
 * direttamente, ma deve passare attraverso le funzioni.
 *
 * NOTA: Per risolvere il problema del prototipo 'void purge(LIST l)',
 * LIST non può essere un semplice puntatore al primo nodo (Node*).
 * Se lo fosse, cancellando il primo nodo non potremmo aggiornare il puntatore
 * nel main (perché passato per valore).
 * QUINDI: LIST sarà un puntatore a una struttura "wrapper" che contiene
 * il puntatore alla testa.
 */
typedef struct list_wrapper *LIST;

/* Funzioni di utilità per il test (non richieste esplicitamente ma necessarie) */
LIST LISTinit();
void LISTaddHead(LIST l, int val);
void LISTprint(LIST l);
void LISTdestroy(LIST l);

/*
 * IL PROTOTIPO RICHIESTO DALL'ESERCIZIO
 */
void purge(LIST l, int div);

#endif