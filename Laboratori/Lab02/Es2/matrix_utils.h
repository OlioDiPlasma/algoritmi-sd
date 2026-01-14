#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdio.h>
#include <stdlib.h>

/* * Prototipo per allocazione matrice (Style 1: Return value)
 * Restituisce un puntatore doppio (la matrice).
 */
int **malloc2dR(int nr, int nc);

/* * Prototipo per liberare la memoria della matrice 
 */
void free2d(int **mat, int nr);

/* * Funzione per leggere il file e costruire la matrice.
 * Usa puntatori per restituire nr e nc al main.
 */
int **leggi_file(char *filename, int *nr, int *nc);

/* * Funzione 'separa' richiesta dall'esercizio.
 * Prende in input la matrice e restituisce (tramite parametri per riferimento)
 * due vettori dinamici e le loro dimensioni.
 */
void separa(int **mat, int nr, int nc, 
            int **v_bianchi, int *len_bianchi, 
            int **v_neri, int *len_neri);

#endif