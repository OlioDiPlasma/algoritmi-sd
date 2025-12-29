/* * File: cruciverba.h
 * Scopo: Definizioni delle strutture dati e prototipi delle funzioni.
 */

#ifndef CRUCIVERBA_H
#define CRUCIVERBA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 21 // Lunghezza max parole (20 car + \0)

/* --- DEFINIZIONE TIPI --- */

/* * Rappresenta un singolo "slot" nello schema dove va inserita una parola.
 * Memorizza dove inizia, quanto è lunga e la direzione.
 */
typedef struct {
    int riga;
    int col;
    int lunghezza;
    char direzione; // 'O'rizzontale o 'V'erticale
} slot_t;

/* * Wrapper per lo schema.
 * Contiene le dimensioni, la griglia di caratteri e l'elenco degli slot.
 */
typedef struct {
    int R, C;           // Dimensioni griglia
    char **griglia;     // La matrice di caratteri (il cruciverba vero e proprio)
    int n_slot;         // Numero di parole da inserire
    slot_t *slots;      // Vettore dinamico degli slot
} schema_t;

/* * Wrapper per le parole.
 * Per accesso O(1) per lunghezza, usiamo un array di array di stringhe.
 * elenco[5] conterrà tutte le parole di lunghezza 5.
 */
typedef struct {
    int n_parole_tot;
    // elenco[L] è un puntatore a un vettore di stringhe di lunghezza L.
    // Assumiamo max lunghezza 20. Usiamo indice 0 per lunghezza 0 (inutilizzato) fino a 20.
    char ***elenco_per_lunghezza; 
    int *conta_per_lunghezza; // Quante parole ci sono per ogni lunghezza
    int **usata; // Flag per sapere se una parola è già stata usata nella ricorsione
} parole_t;

/* --- PROTOTIPI FUNZIONI --- */

// Caricamento dati
schema_t *leggiSchema(char *filename);
parole_t *leggiParole(char *filename);

// Funzione di verifica (Punto 1.2.3)
int verificaSchema(schema_t *s, parole_t *p);

// Funzione wrapper di ricerca (Punto 1.2.4)
void solve(schema_t *s, parole_t *p);

// Funzioni di utilità e memoria
void stampaGriglia(schema_t *s);
void liberaSchema(schema_t *s);
void liberaParole(parole_t *p);

#endif