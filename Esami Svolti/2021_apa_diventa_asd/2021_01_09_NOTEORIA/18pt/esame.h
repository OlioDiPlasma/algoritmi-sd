#ifndef ESAME_H
#define ESAME_H

#include <stdio.h>
#include <stdlib.h>

/* --- STRUTTURE DATI --- */

typedef struct {
    int V;      // Numero di vertici
    int E;      // Numero di archi
    int **madj; // Matrice di adiacenza
} Grafo;

typedef struct {
    int *nodi; 
    int len;
    int id;     // ID originale del ciclo
} Ciclo;

/* Struttura per contenere il risultato dell'ottimizzazione */
typedef struct {
    int *cicli_scelti; // Array di indici dei cicli scelti
    int count;         // Numero di cicli scelti
    int disjoint;      // 1 se disgiunta, 0 se sovrapposta
} Soluzione;

/* --- PROTOTIPI FUNZIONI (API) --- */

// Gestione Grafo
Grafo* init_grafo(int V);
void free_grafo(Grafo *G);
Grafo* load_grafo(char *filename);

// Gestione Cicli
Ciclo* load_cicli(char *filename, int *num_cicli);
void free_cicli(Ciclo *c, int n);

// 6.2.3 Problema di Verifica
int verifica_ciclo(Grafo *G, int *vett, int len);

// 6.2.4 Problema di Ottimizzazione
// Questa funzione wrapper preparerà tutto per la ricorsione
Soluzione trova_copertura_ottima(Grafo *G, Ciclo *cicli, int n_cicli);

#endif