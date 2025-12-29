/* grafo.h */
#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONI COSTANTI --- */
#define MAX_STR 21 // Lunghezza massima stringa (20 + \0)

/* --- STRUTTURE DATI --- */

/* * Struttura Grafo
 * - N: numero di nodi attuali.
 * - nomi: Array di stringhe. nomi[i] è il nome del nodo con indice i.
 * - adj: Matrice di adiacenza (allocata dinamicamente). adj[i][j] = 1 se esiste arco i->j.
 */
typedef struct {
    int N;
    char **nomi; 
    int **adj;
} Grafo;

/* --- PROTOTIPI DI FUNZIONE --- */

// Inizializzazione e Caricamento
Grafo *inizializzaGrafo();
int getIndice(Grafo *g, char *nome); // Cerca o aggiunge un nome, ritorna l'indice
Grafo *leggiGrafo(char *filename);
void liberaGrafo(Grafo *g);

// Punto 3: Verifica Kernel
// Ritorna 1 se la lista di vertici (stringhe) è un Kernel, 0 altrimenti.
int verificaKernel(Grafo *g, char *fileCandidati);
// Funzione helper interna (ma esposta per modularità) che lavora su indici
int isKernel(Grafo *g, int *insieme, int dim);

// Punto 4: Kernel Minimo
// Trova il kernel di cardinalità minore e lo salva su file.
void trovaEScriviKernelMinimo(Grafo *g, char *fileOutput);

// Punto 5: Cammino Semplice con max nodi Kernel
// Ritorna la lunghezza del cammino.
int calcolaMaxNodiKernel(Grafo *g, char *fileOutputPunto4); // Legge il kernel dal file del punto 4

#endif