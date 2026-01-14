#ifndef TRASPORTI_H
#define TRASPORTI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. DEFINIZIONI COSTANTI --- */
#define MAX_STR 31 // 30 caratteri + terminatore '\0'

/* --- 2. ENUMERAZIONI --- */
/* Scopo: Mappare i comandi testuali (es. "date") su numeri interi
   per poter usare uno switch-case nel main.
   Concetto Chiave: L'enum rende il codice leggibile. Invece di ricordare
   che "0" significa "stampa per date", usiamo r_date.
*/
typedef enum {
    r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, 
    r_ordinamento_data, r_ordinamento_codice, r_ordinamento_staz_part, r_ordinamento_staz_arr,
    r_ricerca_codice, r_ricerca_partenza,
    r_stampa, r_fine, r_err
} comando_e;

/* --- 3. STRUTTURE DATI --- */
/*
   Utilizziamo struct annidate per pulizia. 
   Data e Ora sono entita' logiche distinte.
*/
typedef struct {
    int a, m, g; // anno, mese, giorno
} Data;

typedef struct {
    int h, m, s; // ore, minuti, secondi
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

/* --- 4. PROTOTIPI DI FUNZIONE --- */
/* Funzioni di lettura e menu */
comando_e leggiComando(void);
Corsa* leggiFile(const char* nomeFile, int* n_corse);

/* Funzioni di visualizzazione */
void stampaCorsa(Corsa c);
void stampaTutto(Corsa* corse, int n);

/* Esercizio 2: Funzioni di ricerca lineare/filtro */
void elencaPerDate(Corsa* corse, int n);
void elencaPerPartenza(Corsa* corse, int n);
void elencaPerCapolinea(Corsa* corse, int n);
void elencaRitardiInDate(Corsa* corse, int n);
void ritardoTotaleCodice(Corsa* corse, int n);

/* Esercizio 3: Ordinamento e Ricerca Avanzata */
void ordinaDataOra(Corsa* corse, int n);
void ordinaCodice(Corsa* corse, int n);
void ordinaPartenza(Corsa* corse, int n);
void ordinaArrivo(Corsa* corse, int n);

/* Ricerca dicotomica e lineare parziale */
void ricercaDicotomicaCodice(Corsa* corse, int n);
void ricercaLinearePartenza(Corsa* corse, int n);

#endif