/* * esame.h
 * Scopo: Definizione delle strutture dati e prototipi.
 * Questo file agisce come "contratto" tra il main e l'implementazione.
 */

#ifndef ESAME_H
#define ESAME_H

#include <stdio.h>
#include <stdlib.h>

// Definiamo booleani per leggibilità
typedef int bool;
#define TRUE 1
#define FALSE 0

// Valori della griglia
#define BIANCO 0
#define NERO 1

// --- STRUTTURE DATI ---

// Rappresenta un singolo quadrato (sottoregione)
typedef struct {
    int r, c;   // Coordinate in alto a sinistra
    int lato;   // Dimensione del lato
} Quadrato;

// Rappresenta la griglia di gioco
typedef struct {
    int nr, nc;     // Numero righe e colonne
    int **celle;    // Matrice dinamica (doppio puntatore)
} Griglia;

// Rappresenta una soluzione (insieme di quadrati)
typedef struct {
    Quadrato *quadrati; // Vettore dinamico di quadrati
    int n_quadrati;     // Numero attuale di quadrati usati
} Soluzione;

// --- PROTOTIPI FUNZIONI ---

// Gestione Griglia
Griglia caricaGriglia(const char *filename);
void liberaGriglia(Griglia g);
void stampaGriglia(Griglia g);

// Parte 1: Verifica
int verificaCopertura(Griglia g, const char *filename_proposta);

// Parte 2: Ottimizzazione (Algoritmo)
void trovaCoperturaOttima(Griglia g);

#endif