/* * File: act.h
 * Scopo: Definizione dell'ADT (Abstract Data Type) per la gestione delle attività.
 * Note Didattiche:
 * - Usiamo 'typedef' per creare alias di tipi complessi, rendendo il codice più leggibile.
 * - L'ADT è un puntatore opaco (ACT), nascondendo i dettagli implementativi al main (incapsulamento).
 */

#ifndef ACT_H
#define ACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --- 1. DEFINIZIONE STRUTTURE DATI --- */

// Lunghezza massima stringa
#define MAXC 21 

// Struttura per una singola attività [cite: 134]
typedef struct {
    char nome[MAXC];
    int inizio;
    int fine;
    int costo;
} attivita_t;

// Struttura per una dipendenza (arco del grafo) [cite: 134]
// act dipende da dep (dep deve finire prima che act inizi)
typedef struct {
    int act; // Indice dell'attività dipendente
    int dep; // Indice dell'attività da cui si dipende
} dep_t;

// Puntatore opaco all'ADT principale [cite: 134]
typedef struct act *ACT;

/* --- 2. PROTOTIPI FUNZIONI --- */

/* * Inizializzazione manuale (helper per il test, non richiesto esplicitamente dalla traccia 
 * ma necessario per far girare il codice).
 */
ACT ACTinit(int na, int nd);
void ACTfree(ACT a);

// Funzioni richieste dalla traccia [cite: 140, 141, 149, 150]
int ACTcheckDep(ACT a);
void ACTprintSrcSnk(ACT a);
int ACTminPers(ACT a);
void ACTbestPlan(ACT a);

// Helper per inserire dati nel main
void ACTaddAttivita(ACT a, int index, char *nome, int start, int end, int cost);
void ACTaddDipendenza(ACT a, int index, int act_idx, int dep_idx);

#endif