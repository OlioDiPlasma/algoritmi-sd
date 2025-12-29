#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>

/* --- STRUTTURE DATI --- */

/* * Rappresenta una tessera del "domino" idraulico.
 * c1, v1: Colore e valore del tubo orizzontale (se non ruotata)
 * c2, v2: Colore e valore del tubo verticale (se non ruotata)
 */
typedef struct {
    char c1, c2;
    int v1, v2;
    int usata; // Flag per il backtracking: 1 se piazzata, 0 altrimenti
} Tessera;

/*
 * Rappresenta una singola cella della scacchiera.
 */
typedef struct {
    int id_tessera; // Indice nel vettore delle tessere (-1 se vuota)
    int rot;        // 0 = originale, 1 = ruotata di 90 gradi
    int fissa;      // 1 se letta da board.txt (non modificabile), 0 se libera
} Cella;

/* Prototipi delle funzioni (le implementeremo in funzioni.c) */
int calcolaPunteggio(Cella **scacchiera, int R, int C, Tessera *tessere);
void caricaSoluzione(char *filename, Cella **scacchiera, int R, int C, Tessera *tessere);
void risolviBacktracking(int pos, Cella **scacchiera, int R, int C, Tessera *tessere, int T, int *maxPunti, Cella **bestScacchiera);

#endif