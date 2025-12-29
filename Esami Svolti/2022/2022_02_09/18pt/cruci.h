#ifndef CRUCI_H
#define CRUCI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 16 // 15 char + terminatore

/* --- STRUTTURE DATI --- */

typedef struct {
    int R, C;
    char **mat;
} GRID;

typedef struct {
    char str[MAX_STR];
    int val;
} Word;

typedef struct {
    Word *elenco;
    int n_parole;
} WORDS;

typedef struct {
    int r, c;
    int dir;      // 0: Orizz, 1: Vert, 2: Diag
    int word_idx; // Indice nel vettore WORDS
} Placement;

typedef struct {
    Placement *placements;
    int n_found;
    int total_val;
} SOL;

/* --- PROTOTIPI FUNZIONI --- */

GRID carica_griglia(char *filename);
WORDS carica_parole(char *filename);
SOL carica_proposta(char *filename, WORDS parole_list);

void libera_griglia(GRID g);
void libera_parole(WORDS w);
void libera_soluzione(SOL s);

int verifica_soluzione(GRID g, WORDS w, SOL s);
SOL trova_max_soluzione(GRID g, WORDS w);

#endif