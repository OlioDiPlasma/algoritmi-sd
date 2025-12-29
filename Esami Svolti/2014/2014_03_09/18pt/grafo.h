#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. DEFINIZIONE STRUTTURE DATI --- */

/* * Struttura Grafo
 * Usiamo una matrice di adiacenza per rappresentare le connessioni.
 * - nomi: array di stringhe per mappare indice <-> nome nodo (es. 0 <-> "nodoA")
 * - mat: matrice quadrata N x N (1 se c'e' arco, 0 altrimenti)
 * - N: numero di nodi
 */
typedef struct {
    char **nomi;  /* Array dinamico di stringhe (doppio puntatore) */
    int **mat;    /* Matrice di adiacenza (doppio puntatore a int) */
    int N;        /* Numero effettivo di nodi */
} Grafo;

/* Struttura per un singolo Arco (utile per la generazione) */
typedef struct {
    int u;
    int v;
} Arco;

/* --- 2. PROTOTIPI FUNZIONI --- */

/* Caricamento e Inizializzazione */
Grafo *leggiNodi(char *filename);
void inizializzaMatrice(Grafo *g);
int trovaIndice(Grafo *g, char *nome);
void liberaGrafo(Grafo *g);

/* Funzionalita' Principali */
void verificaSoluzione(Grafo *g, char *fileArchi, int k, int m);
void generaSoluzioneOttima(Grafo *g, char *fileOutput, int k, int m);

#endif