#ifndef ESAME_H
#define ESAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> // Serve per FLT_MAX

#define MAX_STR 31

/* * =======================================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT)
 * =======================================================================================
 * Qui definiamo come sono fatti i nostri dati in memoria.
 */

// Rappresenta il singolo oggetto letto dal file.
typedef struct {
    char nome[MAX_STR]; // Array statico di char per la stringa
    float costo;
    float peso;
    float volume;
    int id; // Mappatura fondamentale: associa un numero intero (0..N-1) al nome.
            // Usare un intero come indice di array è molto più veloce che usare stringhe.
} Oggetto;

// Wrapper (contenitore) per il vettore di oggetti.
// Passare questa struct è più pulito che passare (Oggetto *vett, int n).
typedef struct {
    Oggetto *vettore; // Puntatore al primo elemento dell'array dinamico
    int n_oggetti;    // Quanti elementi ci sono
} Oggetti;

// Struttura per il GRAFO dei vincoli.
// Usiamo una Matrice di Adiacenza.
typedef struct {
    int **matrice; // Doppio puntatore: serve per creare una matrice dinamica [RIGA][COLONNA]
    int dim;       // Dimensione (sarà uguale a n_oggetti)
} Vincoli;

// Struttura per memorizzare il risultato dell'ottimizzazione.
typedef struct {
    int *assegnazioni;   // Array dove l'indice è l'ID oggetto, il valore è il GIORNO assegnato.
                         // Es: assegnazioni[2] = 0 -> L'oggetto con ID 2 va nel Giorno 0.
    int n_giorni_usati;  // Il numero totale di giorni (bin) usati
    float delta_costo;   // La differenza (Max - Min) costo giornaliero
    int valida;          // Flag booleano (0/1): abbiamo trovato una soluzione valida?
} Soluzione;

/* --- PROTOTIPI DI FUNZIONE --- */
// Dichiariamo qui le funzioni affinché il main (o altri file) sappiano che esistono.
void leggi_file(char *fn_obj, char *fn_vinc, Oggetti *objs, Vincoli *vinc);
int verifica_validita(Oggetti objs, Vincoli vinc, float Pmax, float Vmax);
Soluzione calcola_ottimo(Oggetti objs, Vincoli vinc, float Pmax, float Vmax);
void free_tutto(Oggetti *o, Vincoli *v, Soluzione *s);

#endif