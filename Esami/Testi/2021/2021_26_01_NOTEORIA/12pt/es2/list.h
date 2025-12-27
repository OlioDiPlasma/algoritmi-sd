#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE TIPO OPACO (ADT I CLASSE)
 * Chi usa LIST vede solo un puntatore, non sa che dentro ci sono 'head', 'tail' o altro.
 * Questo garantisce l'incapsulamento.
 */
typedef struct list_s *LIST;

/* PROTOTIPI */
LIST LIST_new();                    // Crea lista vuota
void LIST_append(LIST l, int val);  // Aggiunge in coda (per popolare i dati di prova)
void LIST_print(LIST l);            // Stampa (per debug)
void LIST_free(LIST l);             // Distrugge tutto

/* * FUNZIONE RICHIESTA DALL'ESERCIZIO
 * Modifica la lista 'l' rimuovendo gli elementi NON presenti in 'v'.
 */
void f(LIST l, int *v, int n);

#endif