/*
 * FILE: progetto.h
 * SCOPO: Definizione delle strutture dati e prototipi delle funzioni.
 * CONCETTI CHIAVE:
 * - Separazione interfaccia/implementazione: Il .h dice "cosa" si fa, il .c "come".
 * - Strutture: Organizziamo i dati logicamente (Mappa, Risorsa).
 */

#ifndef PROGETTO_H
#define PROGETTO_H

#include <stdio.h>
#include <stdlib.h>

// Rappresenta una coordinata o una cella
typedef struct {
    int r; // Riga
    int c; // Colonna
} Coordinate;

// Struttura principale della Mappa
typedef struct {
    int nr, nc;     // Dimensioni: Righe, Colonne
    int **griglia;  // Matrice dinamica: 0=libera, -1=ostacolo
} Mappa;

// Struttura per gestire la proposta e la soluzione
typedef struct {
    int k;              // Raggio di copertura
    int n_risorse;      // Numero di risorse (Z)
    Coordinate *risorse;// Array dinamico delle posizioni delle risorse
    int **copertura;    // Matrice copertura proposta (0=nulla, i=risorsa i)
} Soluzione;

// --- PROTOTIPI ---

// Caricamento dati
Mappa* leggiMappa(char *filename);
Soluzione* leggiProposta(char *filename, int nr, int nc);

// Logica Core
int** inizializzaMatrice(int nr, int nc, int val);
void liberaMatrice(int **mat, int nr);
void liberaMappa(Mappa *m);
void liberaSoluzione(Soluzione *s);

// Verifica e Algoritmi
int calcolaCopertura(Mappa *m, Soluzione *s, int **matriceRisultato);
int verificaAmmissibilita(Mappa *m, Soluzione *s);
void generaCorretto(Mappa *m, Soluzione *s);
void cercaOttimo(Mappa *m, int k, int z_target);

#endif