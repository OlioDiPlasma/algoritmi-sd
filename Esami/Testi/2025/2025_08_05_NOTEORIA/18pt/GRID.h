#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdlib.h>

/* * Struttura Grid come da specifiche (Pag. 6)
 * Rappresenta la scacchiera di gioco.
 */
typedef struct grid_s {
    int nr;     /* Numero di righe */
    int nc;     /* Numero di colonne */
    char **mat; /* Matrice allocata dinamicamente */
} Grid;

/* Prototipi delle funzioni pubbliche */
Grid GRIDinit(int nr, int nc);
Grid GRIDfile(FILE *fp);
void GRIDfree(Grid g);
void GRIDprint(Grid g);

#endif