/*
 * UTENTE: Studente
 * DATA: 13/02/2024
 * DESCRIZIONE: Header per ADT Grafo modificato per il problema dei cammini pesati
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. DEFINIZIONE TIPI --- */
// Struttura per i dati del nodo (Nome + Valore)
typedef struct {
    char *name; // Stringa allocata dinamicamente
    int val;    // Valore intero del nodo
} NodeData;

// Struttura Grafo (Puntatore opaco)
typedef struct graph *Graph;

/* --- 2. PROTOTIPI FUNZIONI --- */
// Inizializzazione e caricamento
Graph GRAPHload(FILE *f);
void GRAPHfree(Graph G);

// Getter semplici per accedere ai dati interni (incapsulamento)
int GRAPHgetNV(Graph G);
char* GRAPHgetName(Graph G, int id);
int GRAPHgetVal(Graph G, int id);

// Iteratore sugli archi (per la ricorsione)
// Ritorna il primo arco incidente su v o il successivo
struct edge_node; // Forward declaration interna
typedef struct edge_node *Link;
Link GRAPHgetHead(Graph G, int v);
Link GRAPHgetNext(Link t);
int GRAPHgetDest(Link t);

#endif