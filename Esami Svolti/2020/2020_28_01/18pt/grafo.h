#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE STRUTTURE DATI --- */

// Rappresentiamo un arco come una coppia di indici
typedef struct {
    int u; // Indice del primo vertice nell'array
    int v; // Indice del secondo vertice
    int attivo; // 1 = esiste, 0 = rimosso logicamente (per il punto 3)
} Arco;

// Rappresentiamo il vertice con le sue informazioni e la lista di adiacenza
typedef struct NodoAdj {
    int v_index; // Indice del vicino
    struct NodoAdj *next;
} NodoAdj;

typedef struct {
    char id[11]; // Identificatore (max 10 char + \0)
    int grado;   // Grado corrente (dinamico)
    int grado_originale; // Utile per reset o verifiche
    int attivo;  // 1 = fa parte del grafo/k-core, 0 = rimosso
    NodoAdj *head; // Testa della lista di adiacenza
} Vertice;

typedef struct {
    Vertice *vettVertici; // Array dinamico di vertici
    int nV;               // Numero vertici
    Arco *vettArchi;      // Array dinamico di tutti gli archi (utile per il punto 3)
    int nE;               // Numero archi
} Grafo;

/* --- PROTOTIPI DI FUNZIONE --- */

// Lettura e Creazione
Grafo* leggiGrafo(char *filename);
void liberaGrafo(Grafo *g);
void stampaGrafo(Grafo *g);

// Punto 2: k-core
void calcolaKCore(Grafo *g, int k);

// Punto 3: j-edge-connected
// Verifica se rimuovendo < j archi si sconnette (no) e se esiste un set di j archi che sconnette (si)
void verificaJEdgeConnected(Grafo *g, int j);

#endif