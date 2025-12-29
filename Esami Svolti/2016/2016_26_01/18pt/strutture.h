#ifndef STRUTTURE_H
#define STRUTTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/* --- DEFINIZIONE STRUTTURE --- */
/* Rappresenta una singola citta' con i suoi dati */
typedef struct {
    char nome[21];      /* Max 20 char + terminatore '\0' */
    int popolazione;    /* In migliaia, usato per il peso */
    int dist_start;     /* Distanza dall'inizio della strada (km 0) */
} Citta;

/* --- PROTOTIPI DI FUNZIONE --- */

/* * Legge il file, alloca il vettore di citta' e restituisce N.
 * Usa un puntatore doppio per 'elenco' perche' deve modificarne l'indirizzo base (malloc).
 */
int leggiFile(char *filename, Citta **elenco);

/*
 * Genera una matrice NxN con le distanze reciproche.
 * Elemento [i][j] = distanza tra citta' i e citta' j.
 */
int **creaMatriceDistanze(Citta *elenco, int n);

/*
 * Calcola la Somma Distanze (SD) per una specifica configurazione.
 * Questa e' la funzione O(N) richiesta per valutare una soluzione.
 */
long long calcolaSD(int *indici_ato, int k, int n, Citta *elenco, int **matrice_dist);

/*
 * Wrapper per l'algoritmo ricorsivo (Power Set / Combinazioni).
 * Trova la configurazione ottima.
 */
void trovaSoluzioneOttima(int n, int k, Citta *elenco, int **matrice_dist);

/* * Funzione per liberare la memoria della matrice dinamica 
 */
void liberaMatrice(int **matrice, int n);

#endif