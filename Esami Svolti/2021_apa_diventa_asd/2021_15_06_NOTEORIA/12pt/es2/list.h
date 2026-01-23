#ifndef LIST_H
#define LIST_H

/* * DEFINIZIONE ADT I CLASSE
 * Definiamo LIST come puntatore a una struttura incompleta.
 * Questo nasconde i dettagli implementativi al main (Information Hiding).
 */
typedef struct list_struct *LIST;

/* Prototipo della funzione richiesta */
void split(LIST l, char *str, char sep);

/* Funzioni di utilità per testare (necessarie perché l'ADT è opaco) */
LIST list_create();
void list_print(LIST l);
void list_destroy(LIST l);

#endif