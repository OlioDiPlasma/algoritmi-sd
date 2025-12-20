#include "pronto_soccorso.h"

/*
 * Scopo: Legge dal file il numero di città, i nomi e la matrice delle distanze.
 * Parametri:
 * - filename: stringa nome file.
 * - elenco: puntatore all'ADT ELENCO da riempire.
 * - dist: puntatore all'ADT DISTMATR da riempire.
 *
 * Gestione Memoria: Alloca memoria dinamica per matrici e stringhe.
 */
void caricaDati(char *filename, ELENCO *elenco, DISTMATR *dist) {
    /* --- 1. APERTURA FILE --- */
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire %s\n", filename);
        exit(1);
    }

    /* --- 2. LETTURA N E NOMI --- */
    fscanf(fp, "%d", &elenco->n); // Legge la prima riga (N)
    dist->n = elenco->n;          // Sincronizza dimensione matrice

    // Alloco il vettore di puntatori a char (le righe dell'elenco nomi)
    elenco->nomi = (char **)malloc(elenco->n * sizeof(char *));
    
    char buffer[100]; // Buffer temporaneo
    for (int i = 0; i < elenco->n; i++) {
        fscanf(fp, "%s", buffer);
        // strdup alloca la memoria esatta per la stringa e la copia
        elenco->nomi[i] = strdup(buffer); 
    }

    /* --- 3. ALLOCAZIONE MATRICE DISTANZE --- */
    // Alloco il vettore di puntatori alle righe (int*)
    dist->mat = (int **)malloc(dist->n * sizeof(int *));
    for (int i = 0; i < dist->n; i++) {
        // Alloco le colonne per la riga i-esima
        dist->mat[i] = (int *)malloc(dist->n * sizeof(int));
        for (int j = 0; j < dist->n; j++) {
            fscanf(fp, "%d", &dist->mat[i][j]);
        }
    }

    fclose(fp);
}

/*
 * Scopo: Libera tutta la memoria allocata per evitare memory leaks.
 * Concetti Chiave: Bisogna liberare "dall'interno verso l'esterno".
 * Prima le righe/stringhe, poi i vettori contenitori.
 */
void liberaTutto(ELENCO *elenco, DISTMATR *dist, SEDI *sedi, SERVIZI *servizi) {
    // Libera nomi città
    if (elenco->nomi != NULL) {
        for (int i = 0; i < elenco->n; i++) {
            free(elenco->nomi[i]);
        }
        free(elenco->nomi);
    }
    
    // Libera matrice
    if (dist->mat != NULL) {
        for (int i = 0; i < dist->n; i++) {
            free(dist->mat[i]);
        }
        free(dist->mat);
    }

    // Libera vettori semplici
    if (sedi->elencoSedi != NULL) free(sedi->elencoSedi);
    if (servizi->assegnazioni != NULL) free(servizi->assegnazioni);
}