/*
 * File: regexp.h
 * Scopo: Definizione dei prototipi per la gestione delle regex.
 */

#ifndef REGEXP_H
#define REGEXP_H

/*
 * Funzione: cercaRegexp
 * Scopo: Cerca la prima occorrenza di 'regexp' all'interno di 'src'.
 * Parametri:
 * - src: stringa sorgente (haystack)
 * - regexp: pattern da cercare (needle) con metacaratteri
 * Ritorno:
 * - Puntatore alla prima occorrenza in 'src', oppure NULL se non trovata.
 */
char *cercaRegexp(char *src, char *regexp);

#endif