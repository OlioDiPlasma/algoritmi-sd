#ifndef PRONTO_SOCCORSO_H
#define PRONTO_SOCCORSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>

/* --- STRUTTURE DATI --- */

// ELENCO: Rappresenta l'insieme delle città
typedef struct {
    int n;          // Numero totale di città (N)
    char **nomi;    // Vettore di stringhe
} ELENCO;

// DISTMATR: Matrice delle distanze NxN
typedef struct {
    int n;          // Dimensione matrice (NxN)
    int **mat;      // Matrice allocata dinamicamente
} DISTMATR;

// SEDI: Un sottoinsieme di M città scelte come Pronto Soccorso
typedef struct {
    int m;          // Numero di sedi da scegliere (M)
    int *elencoSedi;// Vettore di indici che puntano a ELENCO
} SEDI;

// SERVIZI: La soluzione ottima dell'assegnazione
typedef struct {
    int *assegnazioni; // Vettore di dimensione N: assegnazioni[i] = id_sede
    float mediaDist;   // Valore della funzione obiettivo
} SERVIZI;

/* --- PROTOTIPI FUNZIONI --- */
void caricaDati(char *filename, ELENCO *elenco, DISTMATR *dist);
void liberaTutto(ELENCO *elenco, DISTMATR *dist, SEDI *sedi, SERVIZI *servizi);
int checkSedi(DISTMATR dist, SEDI sedi, int MAXD, int MINS);
SERVIZI bestPart(ELENCO elenco, DISTMATR dist, SEDI sedi, int MAXD, int MINS);

#endif