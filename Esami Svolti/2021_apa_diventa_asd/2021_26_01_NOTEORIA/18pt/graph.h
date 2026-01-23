#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

/* --- DEFINIZIONE TIPI --- */
/*
 * Definiamo Graph come puntatore opaco per nascondere l'implementazione (Information Hiding).
 * Path invece lo esponiamo perché spesso il main deve leggerne i campi (ricchezza, mosse).
 */
typedef struct graph *Graph;

typedef struct {
    char **stanze;  // Array di stringhe (nomi delle stanze nel percorso)
    int num_stanze; // Quante stanze ci sono nel percorso
    int ricchezza;  // Ricchezza totale accumulata
} Path;

/* --- PROTOTIPI DI FUNZIONE --- */

/* * Scopo: Carica il grafo dal file di testo.
 * Concetto: Usa un approccio 'Adjacency List' o matrice a seconda della densità. 
 * Qui useremo liste di adiacenza per flessibilità.
 */
Graph GRAPHload(FILE *fp);

/* * Scopo: Libera la memoria del grafo. 
 * Concetto: Evitare memory leak è cruciale in C.
 */
void GRAPHfree(Graph g);

/* === PARTE 3.2.3: VERIFICA === */
/*
 * Scopo: Legge un percorso da file.
 */
Path GRAPHpathLoad(FILE *fp);

/*
 * Scopo: Verifica se un percorso è valido dati i vincoli M (mosse) e PF (punti ferita).
 * Ritorna: 1 se valido, 0 se invalido. Aggiorna la ricchezza in p.
 */
int GRAPHpathCheck(Graph g, Path *p, int M, int PF);

/* === PARTE 3.2.4: OTTIMIZZAZIONE === */
/*
 * Scopo: Trova il percorso che massimizza la ricchezza.
 * Concetto: Wrapper per la funzione ricorsiva.
 */
Path GRAPHpathBest(Graph g, int M, int PF);

/* Funzione helper per stampare un percorso */
void PATHprint(Path p);
void PATHfree(Path p);

#endif