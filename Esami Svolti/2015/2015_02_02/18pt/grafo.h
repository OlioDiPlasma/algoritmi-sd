#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 16 /* 15 caratteri + terminatore '\0' */

/* * STRUTTURA GRAFO
 * Utilizziamo una Matrice di Adiacenza per rappresentare le connessioni.
 * E' efficiente per verificare se due nodi sono amici (O(1)).
 */
typedef struct {
    char **nomi;      /* Array di stringhe per mappare indice <-> nome */
    int **adj;        /* Matrice di adiacenza (0 o 1) */
    int n_nodi;       /* Numero attuale di nodi */
    int max_nodi;     /* Capacita' attuale (utile se ridimensionassimo, qui semplifichiamo) */
} Grafo;

/* * PROTOTIPI DELLE FUNZIONI
 * Qui dichiariamo cosa il nostro programma sa fare.
 */

/* Gestione Grafo e File */
Grafo *iniziaGrafo();
int aggiungiNodo(Grafo *g, char *nome);
void aggiungiArco(Grafo *g, char *nome1, char *nome2);
Grafo *leggiGrafo(char *filename);
void liberaGrafo(Grafo *g);

/* Funzioni Richieste dall'Esercizio */
/* PUNTO 2: Verifica */
void verificaSoluzione(Grafo *g, char *filename);

/* PUNTO 3: Ottimizzazione */
void calcolaMinimoColori(Grafo *g);

#endif