#include <stdlib.h>
#include <string.h>
#include "st.h"

/* --- 1. STRUTTURA DATI --- */
struct symboltable {
    char **a;   /* Array di stringhe (array di puntatori a char) */
    int maxN;   /* Capacità massima */
    int size;   /* Numero attuale di elementi */
};

/* --- 2. IMPLEMENTAZIONE --- */

ST STinit(int maxN) {
    ST st = malloc(sizeof(*st));
    st->maxN = maxN;
    st->size = 0;
    /* Allocazione dinamica dell'array di stringhe.
       Attenzione: st->a è un char**, quindi allochiamo spazio per 'maxN' puntatori a char.
    */
    st->a = malloc(maxN * sizeof(char *));
    return st;
}

void STfree(ST st) {
    int i;
    /* Dobbiamo liberare ogni singola stringa prima di liberare l'array che le contiene */
    for (i = 0; i < st->size; i++) {
        free(st->a[i]);
    }
    free(st->a); /* Libero l'array di puntatori */
    free(st);    /* Libero la struttura principale */
}

int STinsert(ST st, char *label) {
    int i;
    /* Controllo se esiste già */
    i = STsearch(st, label);
    if (i != -1) return i;

    /* Inserimento nuova stringa */
    if (st->size >= st->maxN) return -1; // Tabella piena

    /* strdup è molto comoda: fa malloc + strcpy in un colpo solo.
       Ricordarsi che questa memoria va liberata con free!
    */
    st->a[st->size] = strdup(label);
    return st->size++;
}

int STsearch(ST st, char *label) {
    int i;
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->a[i], label) == 0)
            return i;
    }
    return -1;
}

char *STsearchByIndex(ST st, int index) {
    if (index < 0 || index >= st->size) return NULL;
    return st->a[index];
}