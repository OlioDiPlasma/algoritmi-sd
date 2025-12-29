#include <stdlib.h>
#include <string.h>
#include "st.h"

/* --- 1. STRUTTURA DATI --- */
struct symboltable {
    char **a;   /* Array di stringhe (puntatore a puntatori di char) */
    int maxN;   /* Dimensione massima allocata */
    int size;   /* Numero attuale di elementi */
};

/* --- 2. INIZIALIZZAZIONE --- */
/*
 * Scopo: Allocare la struttura principale della ST.
 * Parametri: maxN (stima o numero massimo di vertici).
 */
ST STinit(int maxN) {
    ST st = malloc(sizeof(*st));
    st->a = malloc(maxN * sizeof(char *)); /* Alloco l'array di puntatori */
    st->maxN = maxN;
    st->size = 0;
    return st;
}

/* --- 3. FREE --- */
void STfree(ST st) {
    int i;
    for (i = 0; i < st->size; i++) {
        free(st->a[i]); /* Libero ogni singola stringa */
    }
    free(st->a); /* Libero l'array di puntatori */
    free(st);    /* Libero la struct */
}

/* --- 4. RICERCA E INSERIMENTO --- */
/*
 * Scopo: Trovare l'indice di una stringa.
 * Se non esiste, la inserisce (strategia per caricamento grafo).
 */
int STinsert(ST st, char *val) {
    int i;
    /* Cerca se esiste già (ricerca lineare, sufficiente per esami) */
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->a[i], val) == 0)
            return i;
    }
    /* Se non esiste e c'è spazio, inserisci */
    if (st->size < st->maxN) {
        st->a[st->size] = strdup(val); /* strdup alloca e copia la stringa */
        return st->size++;
    }
    return -1; /* Errore: tabella piena */
}

int STsearch(ST st, char *val) {
    int i;
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->a[i], val) == 0)
            return i;
    }
    return -1;
}

char* STsearchByIndex(ST st, int index) {
    if (index >= 0 && index < st->size)
        return st->a[index];
    return NULL;
}

int STcount(ST st) {
    return st->size;
}