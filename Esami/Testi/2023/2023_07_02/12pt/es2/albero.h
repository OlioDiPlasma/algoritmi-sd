#ifndef ALBERO_H
#define ALBERO_H

/* * Definizione del tipo T come ADT di I Classe.
 * L'utente vede solo il puntatore, la struct è nascosta nel file .c
 */
typedef struct nodo *T;

/* Prototipo della funzione wrapper richiesta */
int f(T t);

#endif