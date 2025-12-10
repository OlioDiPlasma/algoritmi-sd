#ifndef LISTA_H
#define LISTA_H

/* * DEFINIZIONE TIPO OPACO (ADT I CLASSE)
 * L'utente sa che esiste un tipo LIST, ma non sa com'è fatto dentro.
 * Questo puntatore gestirà l'intera lista.
 */
typedef struct record_lista *LIST;

/* Prototipo della funzione richiesta */
void f(LIST l, int a, int b);

/* Funzioni helper per il test (non richieste esplicitamente ma necessarie per usare l'ADT) */
LIST crea_lista_vuota();
void inserisci_testa(LIST l, int val);
void stampa_lista(LIST l);

#endif