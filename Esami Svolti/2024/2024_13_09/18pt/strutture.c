#include "strutture.h"
#include <string.h>

// Carica l'elenco delle città
ELENCO ELENCOload(FILE *f) {
    ELENCO e = malloc(sizeof(Elenco_t));
    if (fscanf(f, "%d", &e->N) != 1) return NULL;
    e->nomi = malloc(e->N * sizeof(char*));
    for (int i = 0; i < e->N; i++) {
        e->nomi[i] = malloc(50 * sizeof(char));
        fscanf(f, "%s", e->nomi[i]);
    }
    return e;
}

// Carica la matrice delle distanze
DISTMATR DISTMATRload(FILE *f, int N) {
    DISTMATR d = malloc(sizeof(DistMatr_t));
    d->N = N;
    d->mat = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        d->mat[i] = malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            fscanf(f, "%d", &d->mat[i][j]);
        }
    }
    return d;
}

// Inizializza la struttura Sedi
SEDI SEDIinit(int N) {
    SEDI s = malloc(sizeof(Sedi_t));
    s->N = N;
    s->num_sedi = 0;
    s->is_sede = calloc(N, sizeof(int));
    return s;
}

// Aggiunge una sede (utile per il main)
void SEDIadd(SEDI s, int index) {
    if (index >= 0 && index < s->N && s->is_sede[index] == 0) {
        s->is_sede[index] = 1;
        s->num_sedi++;
    }
}

// --- DOMANDA 6: checkSedi ---
int checkSedi(DISTMATR d, int MAXD, int MINS, SEDI s) {
    int i, j, count, ok;

    // 1. Controllo COPERTURA: Ogni città non-sede deve avere una sede vicina (<= MAXD)
    for (i = 0; i < d->N; i++) {
        if (s->is_sede[i] == 0) { // Se è un cliente
            ok = 0;
            for (j = 0; j < d->N; j++) {
                if (s->is_sede[j] && d->mat[i][j] <= MAXD) {
                    ok = 1;
                    break;
                }
            }
            if (!ok) return 0; // Città isolata trovata
        }
    }

    // 2. Controllo CAPACITÀ: Ogni sede deve avere almeno MINS clienti potenziali
    for (j = 0; j < d->N; j++) {
        if (s->is_sede[j]) { // Se è una sede
            count = 0;
            for (i = 0; i < d->N; i++) {
                // Conto le città NON-sede che sono a portata
                if (!s->is_sede[i] && d->mat[i][j] <= MAXD) {
                    count++;
                }
            }
            if (count < MINS) return 0; // Sede inutile
        }
    }

    return 1;
}

void freeAll(ELENCO e, DISTMATR d, SEDI s, SERVIZI ser) {
    if (e) {
        for(int i=0; i<e->N; i++) free(e->nomi[i]);
        free(e->nomi); free(e);
    }
    if (d) {
        for(int i=0; i<d->N; i++) free(d->mat[i]);
        free(d->mat); free(d);
    }
    if (s) { free(s->is_sede); free(s); }
    if (ser) { free(ser->assegnazioni); free(ser); }
}