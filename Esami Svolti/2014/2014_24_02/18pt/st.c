#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "st.h"

/*
 * STRUTTURA DATI NASCOSTA
 * Usiamo un vettore dinamico di stringhe.
 * L'indice nell'array corrisponde all'ID del vertice.
 */
struct symboltable {
    char **a;    /* Array di puntatori a stringhe (nomi dei vertici) */
    int maxN;    /* Capacita' massima attuale */
    int size;    /* Numero attuale di elementi inseriti */
};

/* --- 1. INIZIALIZZAZIONE --- */
ST STinit(int max_n) {
    ST st = malloc(sizeof(*st));
    /*
     * Allocazione dell'array di stringhe.
     * sizeof(char *) perche' ogni cella contiene un INDIRIZZO di memoria (una stringa).
     */
    st->a = malloc(max_n * sizeof(char *));
    st->maxN = max_n;
    st->size = 0;
    return st;
}

/* --- 2. GESTIONE MEMORIA --- */
void STfree(ST st) {
    int i;
    /* Dobbiamo liberare ogni singola stringa allocata con strdup */
    for (i = 0; i < st->size; i++) {
        free(st->a[i]);
    }
    /* Poi liberiamo l'array di puntatori */
    free(st->a);
    /* Infine la struttura principale */
    free(st);
}

/* --- 3. INSERIMENTO E RICERCA --- */
int STinsert(ST st, char *nome) {
    int i;
    /* Prima controlliamo se esiste gia' (ricerca lineare) */
    i = STsearch(st, nome);
    if (i != -1) return i; /* Trovato, ritorno il suo indice esistente */

    /* Controllo sicurezza: array pieno? (Non richiesto esplicitamente ma buona norma) */
    if (st->size >= st->maxN) {
        /* Qui potremmo fare realloc, per semplicita' stampiamo errore */
        printf("Errore: ST piena\n");
        return -1;
    }

    /*
     * strdup: alloca memoria per la stringa e la copia.
     * Importante per non dipendere dal buffer di lettura del main.
     */
    st->a[st->size] = strdup(nome);
    return st->size++; /* Ritorna l'indice attuale e poi incrementa size */
}

int STsearch(ST st, char *nome) {
    int i;
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->a[i], nome) == 0)
            return i;
    }
    return -1; /* Non trovato */
}

char *STsearchByIndex(ST st, int indice) {
    if (indice < 0 || indice >= st->size) return NULL;
    return st->a[indice];
}

int STcount(ST st) {
    return st->size;
}