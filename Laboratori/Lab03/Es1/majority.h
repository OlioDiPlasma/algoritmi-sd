#ifndef MAJORITY_H
#define MAJORITY_H

/*
 * Scopo:
 * Dichiarazione della funzione principale per trovare l'elemento maggioritario.
 *
 * Parametri:
 * - a: puntatore al primo elemento del vettore (o sotto-vettore).
 * - N: dimensione del vettore (o sotto-vettore).
 *
 * Ritorna:
 * - Il valore dell'elemento maggioritario se esiste (> N/2 occorrenze).
 * - -1 se non esiste.
 */
int majority(int *a, int N);

#endif