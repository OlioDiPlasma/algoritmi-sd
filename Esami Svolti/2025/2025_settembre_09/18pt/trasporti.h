#ifndef TRASPORTI_H
#define TRASPORTI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> // Per FLT_MAX

/* --- DEFINIZIONE STRUTTURE DATI (DOMANDA 5) --- */

// Struttura per rappresentare una singola persona
typedef struct {
    char *nome;         // Stringa allocata dinamicamente
    char *indirizzo;    // Stringa allocata dinamicamente
    int num_valigie;
    float distanza;     // Distanza casa-aeroporto
    int id;             // Identificativo univoco (utile per array)
} Persona;

// Struttura Wrapper per l'elenco delle persone (ADT PERSONS)
typedef struct {
    Persona *elenco;    // Vettore dinamico di persone
    int n_persone;      // Dimensione del vettore
} PersoneWrapper;

// Struttura per un vincolo tra due persone
typedef struct {
    int id_persona1;
    int id_persona2;
    int stesso_indirizzo; // 1 = SI, 0 = NO (booleano simulato)
    int stesso_bus;       // 1 = SI, 0 = NO (devono viaggiare insieme)
    float tratto_comune;  // Km in comune (da sottrarre se viaggiano insieme)
} Vincolo;

// Struttura Wrapper per i vincoli (ADT CONSTRAINTS)
typedef struct {
    Vincolo *elenco;    // Vettore dinamico di vincoli
    int n_vincoli;
} VincoliWrapper;

// Struttura per rappresentare una Soluzione (ADT SOL)
// Rappresentiamo la soluzione come un vettore di assegnazioni:
// assegnazioni[i] = k significa che la persona i va sul minibus k.
typedef struct {
    int *assegnazioni;      // Vettore di dimensione n_persone
    int n_minibus_usati;
    float distanza_totale;  // Metrica secondaria di ottimizzazione
} Soluzione;

/* --- PROTOTIPI FUNZIONI --- */

// Funzioni di utilità e caricamento (non richieste esplicitamente ma necessarie)
PersoneWrapper leggi_persone(char *filename);
VincoliWrapper leggi_vincoli(char *filename);
void libera_tutto(PersoneWrapper p, VincoliWrapper v, Soluzione s);

// DOMANDA 6: Verifica validità di un carico
int verifica_validita(int *persone_nel_bus, int n_in_bus, PersoneWrapper P, VincoliWrapper V, int MP, int MV);

// DOMANDA 7: Funzione ricorsiva di ottimizzazione
void calcola_distribuzione_ottima(PersoneWrapper P, VincoliWrapper V, int MP, int MV, Soluzione *best_sol);

#endif