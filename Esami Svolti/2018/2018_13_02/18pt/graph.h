#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "st.h"

/* --- TIPO OPACO GRAFO --- */
typedef struct graph *Graph;

/* --- FUNZIONI DI GESTIONE --- */

/*
    Scopo: Legge il grafo dal file specificato.
    File di input: Formato N, nodi (id x y), archi (id1 id2).
    Ritorna: Puntatore al Grafo caricato.
*/
Graph GRAPHload(char *filename);

/*
    Scopo: Libera tutta la memoria del grafo.
*/
void GRAPHfree(Graph G);

/*
    Scopo: Verifica quale soluzione (file1 o file2) è migliore.
    Logica: Carica gli archi extra, verifica connettività e calcola diametro.
*/
void GRAPHcheckSolutions(Graph G, char *fileSol1, char *fileSol2);

/*
    Scopo: Trova l'insieme di archi ottimale (cardinalità minima, poi diametro minimo).
*/
void GRAPHfindBestConnection(Graph G);

/* --- UTILITIES PER DEBUG --- */
void GRAPHstore(Graph G, FILE *fout);

#endif