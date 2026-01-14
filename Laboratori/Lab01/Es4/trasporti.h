#ifndef TRASPORTI_H
#define TRASPORTI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 31

typedef enum {
    r_stampa, r_stampa_file, 
    r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, 
    r_ricerca_codice, r_ricerca_partenza,
    r_fine, r_err
} comando_e;

typedef struct {
    int a, m, g; 
} Data;

typedef struct {
    int h, m, s; 
} Ora;

typedef struct {
    char codice[MAX_STR];
    char partenza[MAX_STR];
    char destinazione[MAX_STR];
    Data data;
    Ora ora_part;
    Ora ora_arr;
    int ritardo;
} Corsa;

/* Lettura dati */
comando_e leggiComando(void);
Corsa* leggiFile(const char* nomeFile, int* n_corse);

/* Funzioni di visualizzazione (Base) */
void stampaCorsa(Corsa *c); // Accetta puntatore a singola corsa

/* --- NOVITÀ ESERCIZIO 4: Gestione Vettori di Puntatori --- */
/* Inizializza un vettore di puntatori puntando agli elementi del vettore base */
void inizializzaPuntatori(Corsa **vett_rif, Corsa *vett_dati, int n);

/* Stampa usando il vettore di puntatori (indirezione) */
void stampaDaPuntatori(Corsa **vett_rif, int n);

/* Funzioni di Ordinamento su Vettori di Puntatori */
/* Notare: non modificano Corsa*, ma riordinano Corsa** */
void ordinaVettorePuntatori(Corsa **vett_rif, int n, int tipo_ord);

/* Costanti per il tipo di ordinamento da passare alla funzione generica */
#define ORD_DATA 0
#define ORD_CODICE 1
#define ORD_PARTENZA 2
#define ORD_ARRIVO 3

/* Ricerca Dicotomica su vettore di PUNTATORI */
void ricercaDicotomicaSuPuntatori(Corsa **vett_ordinato, int n);

#endif