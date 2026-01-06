#include "GRID.h"

/* * Scopo: Allocare la struttura dati dinamica per la griglia.
 * Parametri:
 * - nr: numero di righe.
 * - nc: numero di colonne.
 * Concetti Chiave: Allocazione di matrici 2D tramite vettore di puntatori.
 */
Grid GRIDinit(int nr, int nc) {
    Grid g;
    int i;

    /* --- 1. INIZIALIZZAZIONE CAMPI --- */
    g.nr = nr;
    g.nc = nc;

    /* --- 2. ALLOCAZIONE VETTORE DI PUNTATORI (RIGHE) --- */
    /* sizeof(char *) perché allochiamo puntatori, non caratteri ancora! */
    g.mat = (char **) malloc(nr * sizeof(char *));
    if (g.mat == NULL) {
        printf("Errore allocazione memoria righe.\n");
        exit(1);
    }

    /* --- 3. ALLOCAZIONE COLONNE PER OGNI RIGA --- */
    for (i = 0; i < nr; i++) {
        g.mat[i] = (char *) malloc(nc * sizeof(char));
        if (g.mat[i] == NULL) {
            printf("Errore allocazione memoria colonna %d.\n", i);
            exit(1);
        }
    }

    return g;
}

/* * Scopo: Leggere la griglia da file secondo il formato specificato (Pag. 4).
 * Parametri:
 * - fp: puntatore al file già aperto in lettura.
 * Dettaglio Sintattico: Uso di " %c" nella fscanf per saltare spazi bianchi/newline.
 */
Grid GRIDfile(FILE *fp) {
    Grid g;
    int nr, nc, i, j;

    /* Lettura dimensioni dalla prima riga */
    if (fscanf(fp, "%d %d", &nr, &nc) != 2) {
        printf("Errore formato file griglia.\n");
        exit(1);
    }

    /* Allocazione memoria */
    g = GRIDinit(nr, nc);

    /* Lettura caratteri */
    for (i = 0; i < nr; i++) {
        for (j = 0; j < nc; j++) {
            /* Lo spazio prima di %c ignora \n e spazi */
            fscanf(fp, " %c", &g.mat[i][j]);
        }
    }
    return g;
}

/* * Scopo: Liberare la memoria heap per evitare memory leaks.
 * Gestione Memoria: Si libera in ordine inverso all'allocazione (prima le righe, poi il vettore).
 */
void GRIDfree(Grid g) {
    int i;
    if (g.mat != NULL) {
        for (i = 0; i < g.nr; i++) {
            free(g.mat[i]);
        }
        free(g.mat);
    }
}

void GRIDprint(Grid g) {
    int i, j;
    for (i = 0; i < g.nr; i++) {
        for (j = 0; j < g.nc; j++) {
            printf("%c ", g.mat[i][j]);
        }
        printf("\n");
    }
}