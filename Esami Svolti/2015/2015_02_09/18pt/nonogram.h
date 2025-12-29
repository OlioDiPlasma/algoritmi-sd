/*
 * File: nonogram.h
 * Scopo: Definizione delle strutture dati e prototipi delle funzioni.
 */

#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <stdio.h>
#include <stdlib.h>

/* --- STRUTTURE DATI --- */

/* * Struct: Vincolo
 * Scopo: Rappresenta i numeri a lato di UNA riga o sopra UNA colonna.
 * Esempio: "2 1 3" significa n_blocchi=2, e l'array lunghezze contiene {1, 3}.
 */
typedef struct {
    int n_blocchi;      // Quanti gruppi di celle nere ci sono
    int *lunghezze;     // Array dinamico con la lunghezza di ogni gruppo
} Vincolo;

/*
 * Struct: Schema
 * Scopo: Contiene tutto il mondo del gioco: griglia e regole.
 */
typedef struct {
    int R, C;               // Numero Righe (N) e Colonne (M)
    int **griglia;          // Matrice dinamica del gioco (0=bianco, 1=nero)
    Vincolo *vincoli_righe; // Array di vincoli per le righe (dimensione R)
    Vincolo *vincoli_col;   // Array di vincoli per le colonne (dimensione C)
} Schema;

/* --- PROTOTIPI FUNZIONI --- */

// Funzioni di Gestione Memoria e Caricamento
Schema* carica_vincoli(const char *filename);
void libera_schema(Schema *s);
void leggi_soluzione_da_file(Schema *s, const char *filename);
void salva_soluzione(Schema *s, const char *filename);

// Funzioni Logiche (Punto 2: Verifica)
int verifica_soluzione(Schema *s);

// Funzioni Ricorsive (Punto 3: Solver)
int risolvi_nonogram(Schema *s);

#endif