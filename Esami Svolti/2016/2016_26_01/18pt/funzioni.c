#include "strutture.h"

/* * --- FUNZIONE DI LETTURA FILE ---
 * Scopo: Aprire il file, leggere N, allocare dinamicamente l'array e popolarlo.
 * Parametri:
 * - filename: nome del file (da argv).
 * - elenco: puntatore a puntatore a Citta (Citta **). Serve per "restituire" l'array allocato.
 * Ritorna: Il numero di citta' lette (N).
 */
int leggiFile(char *filename, Citta **elenco) {
    FILE *fp;
    int n, i;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", filename);
        exit(1);
    }

    /* Leggo N dalla prima riga */
    if (fscanf(fp, "%d", &n) != 1) {
        printf("Errore nel formato del file (N mancante).\n");
        exit(1);
    }

    /* * --- GESTIONE MEMORIA ---
     * Allocazione Dinamica.
     * Dereferenzio 'elenco' (*elenco) per accedere alla variabile puntatore dichiarata nel main.
     * Le assegniamo un blocco di memoria grande quanto: N * dimensione di una struct Citta.
     */
    *elenco = (Citta *)malloc(n * sizeof(Citta));
    if (*elenco == NULL) {
        printf("Errore di allocazione memoria (malloc failed).\n");
        exit(1);
    }

    /* Ciclo di lettura dati */
    for (i = 0; i < n; i++) {
        /*
         * Accedo ai campi con aritmetica dei vettori: (*elenco)[i].campo
         * Nota: fscanf legge stringa, intero, intero.
         */
        fscanf(fp, "%s %d %d", (*elenco)[i].nome, &(*elenco)[i].popolazione, &(*elenco)[i].dist_start);
    }

    fclose(fp);
    return n;
}

/*
 * --- CREAZIONE MATRICE DISTANZE ---
 * Scopo: Pre-calcolare le distanze per avere accesso O(1) invece di ricalcolarle sempre.
 * Complessita': O(N^2)
 */
int **creaMatriceDistanze(Citta *elenco, int n) {
    int **mat;
    int i, j;

    /* Allocazione dell'array di puntatori alle righe */
    mat = (int **)malloc(n * sizeof(int *));
    if (mat == NULL) exit(1);

    for (i = 0; i < n; i++) {
        /* Allocazione delle colonne per ogni riga */
        mat[i] = (int *)malloc(n * sizeof(int));
        if (mat[i] == NULL) exit(1);
    }

    /* Riempimento matrice simmetrica */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            /* * abs() restituisce il valore assoluto. 
             * Distanza tra citta' A e B = |posA - posB|
             */
            mat[i][j] = abs(elenco[i].dist_start - elenco[j].dist_start);
        }
    }

    return mat;
}

/*
 * --- CALCOLO OBIETTIVO (SD) ---
 * Scopo: Calcolare la "Somma Distanze" pesata per una data selezione di K citta'.
 * Parametri:
 * - indici_ato: array contenente gli INDICI delle citta' scelte come ATO.
 * - k: numero di ATO.
 * - n: numero totale citta'.
 * - elenco: array dati citta' (serve per la popolazione).
 * - matrice_dist: distanze precalcolate.
 * Complessita': O(N * K) che si approssima a O(N) se K << N.
 */
long long calcolaSD(int *indici_ato, int k, int n, Citta *elenco, int **matrice_dist) {
    long long somma_totale = 0;
    int i, j;
    
    /* Per ogni citta' 'i' della regione... */
    for (i = 0; i < n; i++) {
        int min_dist = INT_MAX; /* Inizializzo a infinito */

        /* ...cerco l'ATO piu' vicino tra quelli scelti */
        for (j = 0; j < k; j++) {
            /* L'indice della j-esima ATO scelta e' indici_ato[j] */
            int idx_ato = indici_ato[j];
            int d = matrice_dist[i][idx_ato];
            
            if (d < min_dist) {
                min_dist = d;
            }
        }
        
        /* * Formula: SD += popolazione_i * distMinDaATO_i 
         * Uso long long per evitare overflow se i numeri sono grandi.
         */
        somma_totale += (long long)elenco[i].popolazione * min_dist;
    }

    return somma_totale;
}

/* * --- VARIABILI GLOBALI DI SUPPORTO ALLA RICORSIONE --- 
 * In un contesto d'esame, passarle come parametri e' piu' elegante, 
 * ma usare statiche nel modulo semplifica la firma della ricorsiva.
 */
static long long best_sd = -1;      /* Miglior SD trovata finora (-1 indica non ancora trovata) */
static int *best_sol = NULL;        /* Array per salvare la miglior combinazione */

/*
 * --- CUORE RICORSIVO (Modello Combinatorio) ---
 * pos: livello della ricorsione (quanti elementi ho gia' scelto nel vettore sol)
 * start: indice da cui partire a cercare nel vettore 'elenco' (per evitare ripetizioni e permutazioni)
 */
void combinazioni_r(int pos, int start, int n, int k, int *sol, Citta *elenco, int **matrice_dist) {
    int i;

    /* CASO BASE: Ho selezionato K citta' */
    if (pos == k) {
        /* Calcolo la SD della configurazione corrente */
        long long current_sd = calcolaSD(sol, k, n, elenco, matrice_dist);

        /* Se e' la prima soluzione o e' migliore della precedente, aggiorno */
        if (best_sd == -1 || current_sd < best_sd) {
            best_sd = current_sd;
            /* Copio la soluzione corrente nella "migliore" */
            for (i = 0; i < k; i++) {
                best_sol[i] = sol[i];
            }
        }
        return;
    }

    /* * PASSO RICORSIVO
     * Ciclo da 'start' fino a 'n'.
     * Questo garantisce che prendiamo combinazioni uniche (es. 1-2, ma non 2-1).
     * Ottimizzazione: i < n - (k - pos) + 1 (Pruning inutile in esami base, usiamo i < n)
     */
    for (i = start; i < n; i++) {
        sol[pos] = i; /* Scelgo la citta' all'indice 'i' come candidata */
        
        /* Ricorsione: prossimo elemento (pos+1), partendo da quello successivo (i+1) */
        combinazioni_r(pos + 1, i + 1, n, k, sol, elenco, matrice_dist);
        
        /* Backtracking implicito: al prossimo giro del for, sovrascrivo sol[pos] */
    }
}

/* Wrapper per lanciare la ricorsione */
void trovaSoluzioneOttima(int n, int k, Citta *elenco, int **matrice_dist) {
    int *sol_curr;

    /* Allocazione vettori soluzioni */
    sol_curr = (int *)malloc(k * sizeof(int));
    best_sol = (int *)malloc(k * sizeof(int));
    if (sol_curr == NULL || best_sol == NULL) exit(1);

    /* Inizio ricorsione */
    best_sd = -1; /* Reset */
    combinazioni_r(0, 0, n, k, sol_curr, elenco, matrice_dist);

    /* --- STAMPA RISULTATI --- */
    printf("\n--- RISULTATO OTTIMO ---\n");
    printf("Minimo SD calcolato: %lld\n", best_sd);
    printf("Citta' sedi ATO selezionate:\n");
    for (int i = 0; i < k; i++) {
        /* best_sol contiene gli indici, accedo a elenco per stampare il nome */
        int idx = best_sol[i];
        printf("- %s (Pop: %d, Pos: %d)\n", elenco[idx].nome, elenco[idx].popolazione, elenco[idx].dist_start);
    }

    /* Pulizia locale */
    free(sol_curr);
    free(best_sol);
}

void liberaMatrice(int **matrice, int n) {
    for (int i = 0; i < n; i++) {
        free(matrice[i]);
    }
    free(matrice);
}