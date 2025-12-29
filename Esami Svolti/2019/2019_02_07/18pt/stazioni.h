#ifndef STAZIONI_H
#define STAZIONI_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* --- 1. STRUTTURE DATI --- */

/* * Scopo: Raggruppare tutti i dati immutabili del problema.
 * Perché una struct? Per evitare di passare 4-5 parametri separati a ogni funzione ricorsiva.
 * Rende il codice più pulito e la firma delle funzioni più leggera.
 */
typedef struct {
    int N;              // Numero di Comuni
    int *pop;           // Vettore popolazione (dim N)
    int **dist;         // Matrice adiacenza distanze (NxN)
    int *stazComune;    // Max stazioni per ogni comune (dim N) - per Obj 2
} Mappa;

/* --- 2. PROTOTIPI FUNZIONI --- */

// Caricamento e gestione memoria
Mappa caricaDati(const char *filename);
void liberaMappa(Mappa m);

// Funzione richiesto: Verifica proposta da file
int verificaProposta(const char *filename, Mappa m, int distMax);

// Funzione richiesto: Obiettivo 1 (Minimizzare numero stazioni per copertura)
void risolviObiettivo1(Mappa m, int distMax);

// Funzione richiesto: Obiettivo 2 (Minimizzare costo pesato con numero fisso stazioni)
void risolviObiettivo2(Mappa m, int numStazTotali);

#endif