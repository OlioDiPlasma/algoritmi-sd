#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include "st.h"

/* Definizione opaca del Grafo (puntatore a struct nascosta) */
typedef struct grafo *Graph;

/* --- PROTOTIPI --- */

/* Carica il grafo da file. Ritorna il puntatore alla struttura Graph allocata. */
Graph GRAPHload(FILE *fp);

/* Libera tutta la memoria del grafo */
void GRAPHfree(Graph G);

/*
 * VERIFICA REGOLARITA' (Punto richiesto)
 * Stampa se il grafo e' regolare o no.
 */
void GRAPHcheckRegular(Graph G);

/*
 * CAMMINO MASSIMO (Punto richiesto)
 * Dato il nome di un vertice sorgente, trova e stampa il cammino semplice
 * a peso massimo.
 */
void GRAPHpathMaxWeight(Graph G, char *startName);

#endif