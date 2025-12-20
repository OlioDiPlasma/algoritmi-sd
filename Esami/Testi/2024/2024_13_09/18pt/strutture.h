#ifndef STRUTTURE_H
#define STRUTTURE_H

#include <stdio.h>
#include <stdlib.h>

// --- DEFINIZIONE TIPI ---

typedef struct {
    int N;
    char **nomi;
} Elenco_t, *ELENCO;

typedef struct {
    int N;
    int **mat;
} DistMatr_t, *DISTMATR;

typedef struct {
    int N;
    int *is_sede; // 1 se è sede, 0 altrimenti
    int num_sedi;
} Sedi_t, *SEDI;

typedef struct {
    int N;
    int *assegnazioni; // Indice della sede assegnata
    float media;
} Servizi_t, *SERVIZI;

// --- PROTOTIPI FUNZIONI (strutture.c) ---
ELENCO ELENCOload(FILE *f);
DISTMATR DISTMATRload(FILE *f, int N);
SEDI SEDIinit(int N);
void SEDIadd(SEDI s, int index);
void freeAll(ELENCO e, DISTMATR d, SEDI s, SERVIZI ser);

// Domanda 6: Verifica
int checkSedi(DISTMATR d, int MAXD, int MINS, SEDI s);

// --- PROTOTIPI ALGORITMO (algoritmo.c) ---
// Domanda 7: Ottimizzazione
SERVIZI bestPart(ELENCO e, DISTMATR d, SEDI s, int MAXD, int MINS);

#endif