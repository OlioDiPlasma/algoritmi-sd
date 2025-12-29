/* * File: graph.h
 * Scopo: Definizione delle strutture dati e prototipi delle funzioni.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 21 // Lunghezza massima stringa ID + terminatore

/* --- STRUTTURE DATI --- */

// 1. Tabella di Simboli (Symbol Table)
// Serve per mappare le stringhe (nomi) in interi (indici della matrice)
typedef struct {
    char **names;   // Vettore di stringhe (array dinamico di char*)
    int size;       // Numero attuale di vertici
    int max_size;   // Dimensione allocata
} SymbolTable;

// 2. Grafo
// Usiamo una Matrice di Adiacenza perché dobbiamo verificare spesso se esiste un arco
// tra due nodi (test di completezza cricca è O(1)).
typedef struct {
    int **adj;      // Matrice pesata (0 se non c'è arco, peso > 0 se c'è)
    int V;          // Numero di vertici
    int E;          // Numero di archi
    SymbolTable st; // Tabella per convertire nomi <-> indici
} Graph;

// 3. Struttura per memorizzare una Cricca (lista di indici)
typedef struct {
    int *nodes;     // Array dinamico degli indici dei nodi nella cricca
    int count;      // Numero di nodi nella cricca
} Clique;

/* --- PROTOTIPI FUNZIONI --- */

// Gestione Grafo
Graph *loadGraph(const char *filename);
void freeGraph(Graph *G);
int getIndex(SymbolTable *st, char *name); // Cerca o aggiunge nome

// Funzioni richieste dal testo
int checkSolution(Graph *G, char *filename); // Verifica se è cricca massimale
void solveCliques(Graph *G); // Trova tutte le cricche massimali e il ciclo Hamiltoniano

#endif