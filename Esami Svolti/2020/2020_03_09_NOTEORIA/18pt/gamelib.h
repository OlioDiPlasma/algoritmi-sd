/* * File: gamelib.h
 * Scopo: Dichiarazioni delle funzioni e delle strutture per il gioco delle regioni.
 */

#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- STRUTTURA DATI --- */
/*
 * Anche se il testo chiede di passare spesso R e C separatamente,
 * definire una struct aiuta a mantenere le dimensioni associate alla matrice.
 * Per rispettare i prototipi richiesti, useremo int** ma internamente
 * o nel main questa struct è utile.
 */
typedef struct {
    int R, C;
    int **grid;
} Griglia;

/* --- PROTOTIPI RICHIESTI DAL TESTO --- */

/* * Legge la mappa da file. 
 * Ritorna un puntatore doppio (la matrice).
 * Modifica R e C per riferimento (puntatori).
 * [cite: 79]
 */
int** leggiMappa(FILE *fin, int *R, int *C);

/*
 * Verifica se una griglia nel file 'fn' è valida rispetto alla 'mappa' originale.
 * [cite: 84]
 */
int verifica(char *fn, int **mappa_originale);

/*
 * Risolve il puzzle cercando di minimizzare i numeri distinti usati.
 * [cite: 89]
 */
void solve(int **m, int R, int C);

#endif