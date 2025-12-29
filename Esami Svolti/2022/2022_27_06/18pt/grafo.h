#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- 1. STRUTTURA DATI --- */
/*
 * Usiamo una MATRICE DI ADIACENZA.
 * Perché? Dato che dobbiamo controllare spesso se un nodo è adiacente
 * a un insieme (dominating set), l'accesso diretto adj[u][v] in O(1)
 * è molto comodo didatticamente rispetto alle liste di adiacenza.
 */
typedef struct {
    int V;      // Numero di vertici
    int E;      // Numero di archi (opzionale, ma utile per statistica)
    int **adj;  // Matrice di adiacenza (doppio puntatore)
} Grafo;

/* --- 2. PROTOTIPI FUNZIONI --- */

// Caricamento e gestione memoria
Grafo* leggiGrafo(const char *nomeFile);
void liberaGrafo(Grafo *g);

// Verifica (Punto 2 dell'esercizio)
bool verificaPropostaFile(Grafo *g, const char *nomeFile);

// Ottimizzazione (Punto 3 dell'esercizio)
void trovaDomaticPartitionMassima(Grafo *g);

#endif