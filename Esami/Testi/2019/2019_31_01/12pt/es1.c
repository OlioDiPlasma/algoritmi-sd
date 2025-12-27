#include <stdio.h>
#include <stdlib.h>
#include <math.h>   /* Necessario per la funzione abs() */
#include <limits.h> /* Necessario per INT_MAX */

/*
 * ======================================================================================
 * NOME FUNZIONE: minmaxdiff
 * ======================================================================================
 * SCOPO:
 * La funzione analizza una matrice di interi e cerca una specifica colonna.
 * Per ogni colonna, calcola la "massima differenza" (in valore assoluto) tra 
 * elementi adiacenti (consecutivi sulla stessa colonna).
 * Tra tutte le colonne, deve restituire l'INDICE di quella che ha questa 
 * "massima differenza" più PICCOLA (minima).
 *
 * PARAMETRI:
 * - int **A: Doppio puntatore che rappresenta la matrice (array di array).
 * - int n: Numero di righe.
 * - int m: Numero di colonne.
 *
 * RITORNA:
 * - int: L'indice (0-based) della colonna vincente.
 *
 * CONCETTI CHIAVE:
 * - Matrice come int**: In C, quando passiamo una matrice allocata dinamicamente,
 * usiamo un puntatore a puntatore. A punta a un array di righe; ogni riga è un array di interi.
 * - Logica Minimax: Dobbiamo prima trovare il massimo locale (della colonna) e poi
 * confrontarlo con un minimo globale.
 * ======================================================================================
 */
int minmaxdiff(int **A, int n, int m) {

    /* --- 1. INIZIALIZZAZIONE --- */
    /* * Inizializziamo il "minimo assoluto trovato finora" con il valore più alto possibile
     * per un intero (INT_MAX). In questo modo, qualsiasi differenza calcolata nella
     * prima colonna sarà sicuramente minore e diventerà il nuovo record.
     */
    int minDiffGlobale = INT_MAX;
    int indiceColonnaMigliore = -1;

    /* --- 2. ITERAZIONE SULLE COLONNE --- */
    /* Il ciclo esterno scorre le colonne (j), perché il problema chiede di valutare 
     * una colonna alla volta. */
    for (int j = 0; j < m; j++) {
        
        /* Variabile per tracciare la differenza massima all'interno di QUESTA colonna 'j' */
        int maxDiffLocale = -1; 

        /* --- 3. ITERAZIONE SULLE RIGHE --- */
        /*
         * Scorro le righe dalla 0 alla n-2.
         * Perché n-2? Perché dentro il ciclo confronto l'elemento i con i+1.
         * Se andassi fino a n-1, farei un confronto con n (fuori array -> Crash!).
         */
        for (int i = 0; i < n - 1; i++) {
            
            /*
             * DETTAGLIO SINTATTICO: A[i][j] vs A[i+1][j]
             * A[i] è un puntatore alla riga i-esima.
             * A[i][j] accede al j-esimo intero di quella riga.
             * abs() calcola il valore assoluto (es. abs(-5) = 5).
             */
            int diff = abs(A[i][j] - A[i+1][j]);

            /* Logica di Massimo Locale: cerco la differenza più grande in questa colonna */
            if (diff > maxDiffLocale) {
                maxDiffLocale = diff;
            }
        }

        /* --- 4. CONFRONTO GLOBALE (MINIMAX) --- */
        /*
         * Ho appena finito di analizzare la colonna 'j'. Il suo "punteggio" è maxDiffLocale.
         * Devo controllare se questo punteggio è il più basso visto finora tra tutte le colonne.
         */
        if (maxDiffLocale < minDiffGlobale) {
            minDiffGlobale = maxDiffLocale;
            indiceColonnaMigliore = j; // Mi segno che la colonna 'j' è la nuova vincitrice
        }
    }

    return indiceColonnaMigliore;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Questo main crea la matrice dell'esempio, chiama la funzione e libera la memoria.
 */
int main() {
    /* Dati dell'esempio: 4 righe, 3 colonne */
    int n = 4; 
    int m = 3;

    printf("--- Inizio Programma ---\n");

    /* --- 1. ALLOCAZIONE DINAMICA MATRICE (Gestione Memoria) --- */
    /*
     * Passo A: Allocare il vettore dei puntatori alle righe.
     * int** A è un puntatore che punta a un array di (int*).
     * sizeof(int*) è la dimensione di un indirizzo di memoria (solitamente 8 byte su 64bit).
     */
    int **A = (int **)malloc(n * sizeof(int *));
    if (A == NULL) { exit(1); } // Controllo sempre se la RAM è piena

    /*
     * Passo B: Per ogni riga, allocare l'array di interi vero e proprio.
     */
    for (int i = 0; i < n; i++) {
        A[i] = (int *)malloc(m * sizeof(int));
        if (A[i] == NULL) { exit(1); }
    }

    /* --- 2. POPOLAMENTO DATI (Hardcoded dall'immagine) --- */
    /* * Matrice:
     * 15 13  7
     * 6 18  4
     * 11  4 12
     * 13  9  5
     */
    // Riga 0
    A[0][0] = 15; A[0][1] = 13; A[0][2] = 7;
    // Riga 1
    A[1][0] = 6;  A[1][1] = 18; A[1][2] = 4;
    // Riga 2
    A[2][0] = 11; A[2][1] = 4;  A[2][2] = 12;
    // Riga 3
    A[3][0] = 13; A[3][1] = 9;  A[3][2] = 5;

    /* Stampa della matrice per verifica */
    printf("Matrice Input:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            printf("%3d ", A[i][j]);
        }
        printf("\n");
    }

    /* --- 3. LOGICA (Chiamata funzione) --- */
    int risultato = minmaxdiff(A, n, m);

    /* --- 4. OUTPUT --- */
    /* L'esempio dice che la colonna 0 ha max diff 9, la 1 ha 14, la 2 ha 8.
     * Quindi deve vincere la colonna 2. */
    printf("\nL'indice della colonna con la 'minima differenza massima' e': %d\n", risultato);

    /* --- 5. DEALLOCAZIONE (Free) --- */
    /*
     * Ordine inverso rispetto alla malloc:
     * Prima libero le singole righe (i dati).
     * Poi libero il vettore dei puntatori (la struttura).
     */
    for (int i = 0; i < n; i++) {
        free(A[i]); // Libero la riga i-esima
    }
    free(A); // Libero il vettore delle righe

    printf("\nMemoria liberata. Programma terminato.\n");
    return 0;
}