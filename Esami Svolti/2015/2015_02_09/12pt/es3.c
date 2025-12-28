#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* Serve per INT_MIN */

/*
 * ======================================================================================
 * NOME FUNZIONE: isSafe
 * ======================================================================================
 * SCOPO:
 * Funzione di utilità per verificare se una mossa è valida.
 * Controlla se le coordinate sono dentro la mappa e se la cella non è già stata visitata.
 *
 * PARAMETRI:
 * * r, c : Coordinate riga e colonna candidate.
 * * N, M : Dimensioni della matrice.
 * * visited : Matrice booleana (0 o 1) che tiene traccia del cammino corrente.
 *
 * RITORNA:
 * 1 (Vero) se la cella è sicura, 0 (Falso) altrimenti.
 * ======================================================================================
 */
int isSafe(int r, int c, int N, int M, int **visited) {
    /*
     * DETTAGLIO SINTATTICO:
     * L'operatore && (AND logico) valuta da sinistra a destra.
     * Se (r >= 0) è falso, il resto non viene valutato. Questo previene accessi
     * fuori dai limiti dell'array visited.
     */
    return (r >= 0 && r < N && c >= 0 && c < M && visited[r][c] == 0);
}

/*
 * ======================================================================================
 * NOME FUNZIONE: findPathRecursive
 * ======================================================================================
 * SCOPO:
 * Funzione "worker" ricorsiva che implementa l'algoritmo di Backtracking (DFS).
 * Esplora tutti i cammini possibili dalla posizione corrente all'arrivo.
 *
 * PARAMETRI:
 * * map : La matrice dei valori.
 * * visited : Matrice per tracciare i nodi visitati nel cammino corrente.
 * * N, M : Dimensioni.
 * * r, c : Posizione corrente dell'esploratore.
 * * currSum : La somma accumulata fino a questo punto.
 * * currSteps : Il numero di passi fatti fino a questo punto.
 * * bestSum : (Puntatore) Il record globale della somma massima trovata finora.
 * * minSteps : (Puntatore) Il record globale dei passi minimi per quella somma massima.
 *
 * CONCETTI CHIAVE:
 * 1. Backtracking: Segniamo la cella come visitata, proviamo tutte le strade,
 * e POI la "s-visitiamo" (backtrack) per permettere ad altri cammini di usarla.
 * 2. Puntatori per Output: Usiamo puntatori (*bestSum) per mantenere lo stato
 * attraverso le chiamate ricorsive senza dover ritornare strutture complesse.
 * ======================================================================================
 */
void findPathRecursive(int **map, int **visited, int N, int M, int r, int c, 
                       int currSum, int currSteps, int *bestSum, int *minSteps) {
    
    /* Aggiorniamo la somma corrente aggiungendo il valore della cella in cui siamo */
    currSum += map[r][c];
    
    /* --- 1. CASO BASE: ARRIVO RAGGIUNTO --- */
    /* L'arrivo è sempre in basso a destra (N-1, M-1) */
    if (r == N - 1 && c == M - 1) {
        /*
         * LOGICA DI OTTIMIZZAZIONE:
         * 1. Se abbiamo trovato una somma strettamente maggiore del record attuale:
         * Aggiorniamo sia la somma record che i passi.
         * 2. Se la somma è UGUALE al record, ma i passi sono MINORI:
         * Aggiorniamo i passi (prediligiamo il percorso più breve).
         */
        if (currSum > *bestSum) {
            *bestSum = currSum;
            *minSteps = currSteps;
        } else if (currSum == *bestSum) {
            if (currSteps < *minSteps) {
                *minSteps = currSteps;
            }
        }
        return; /* Torniamo indietro per esplorare altre strade */
    }

    /* --- 2. MARCATURA VISITA --- */
    visited[r][c] = 1;

    /* --- 3. ESPLORAZIONE VICINI --- */
    /*
     * Definizione delle 8 direzioni possibili (adiacenti + diagonali).
     * Usiamo due array paralleli per iterare facilmente con un ciclo for.
     */
    int rowDir[] = {-1, -1, -1,  0, 0,  1, 1, 1};
    int colDir[] = {-1,  0,  1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int nextR = r + rowDir[i];
        int nextC = c + colDir[i];

        if (isSafe(nextR, nextC, N, M, visited)) {
            /* Chiamata ricorsiva: passo incrementato di 1 */
            findPathRecursive(map, visited, N, M, nextR, nextC, 
                              currSum, currSteps + 1, bestSum, minSteps);
        }
    }

    /* --- 4. BACKTRACKING (S-MARCATURA) --- */
    /*
     * FONDAMENTALE: Prima di risalire allo stack frame precedente, dobbiamo
     * rendere questa cella nuovamente disponibile per altri percorsi alternativi
     * che potrebbero passare di qui arrivando da un'altra parte.
     */
    visited[r][c] = 0;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: solveMap
 * ======================================================================================
 * SCOPO:
 * Wrapper function (involucro). Prepara le strutture dati necessarie (matrice visited),
 * inizializza le variabili e lancia la ricorsione.
 *
 * PARAMETRI:
 * * map : Matrice NxM di input.
 * * N, M : Dimensioni.
 *
 * GESTIONE MEMORIA:
 * Alloca dinamicamente la matrice 'visited' e si assicura di liberarla alla fine
 * per evitare Memory Leaks.
 * ======================================================================================
 */
void solveMap(int **map, int N, int M) {
    /* --- 1. ALLOCAZIONE MATRICE VISITED --- */
    /*
     * Usiamo malloc per creare un array di puntatori (le righe).
     * sizeof(int *) è la dimensione di un puntatore (solitamente 8 byte su 64bit).
     */
    int **visited = (int **)malloc(N * sizeof(int *));
    if (visited == NULL) {
        printf("Errore memoria\n");
        return;
    }

    for (int i = 0; i < N; i++) {
        /*
         * calloc è preferibile a malloc qui perché inizializza tutto a 0.
         * 0 = non visitato. Risparmiamo un doppio ciclo di inizializzazione.
         */
        visited[i] = (int *)calloc(M, sizeof(int));
        if (visited[i] == NULL) {
             /* In un codice reale qui servirebbe una funzione di cleanup per liberare le righe precedenti */
             exit(1); 
        }
    }

    /* --- 2. INIZIALIZZAZIONE VARIABILI --- */
    /*
     * bestSum parte da INT_MIN (il numero intero più piccolo possibile)
     * perché la mappa potrebbe contenere numeri negativi.
     * minSteps parte da INT_MAX per essere sicuri di minimizzarlo.
     */
    int bestSum = INT_MIN;
    int minSteps = INT_MAX;

    /* --- 3. LANCIO ALGORITMO --- */
    /* Partenza: (0,0), somma parziale 0, passi 0 */
    findPathRecursive(map, visited, N, M, 0, 0, 0, 0, &bestSum, &minSteps);

    /* --- 4. OUTPUT RISULTATI --- */
    if (bestSum != INT_MIN) {
        printf("Risultato Trovato:\n");
        printf("Somma Massima: %d\n", bestSum);
        printf("Passi Minimi : %d (nota: contando 0 passi per la partenza)\n", minSteps);
    } else {
        printf("Nessun percorso trovato (impossibile in una griglia connessa).\n");
    }

    /* --- 5. PULIZIA MEMORIA (FREE) --- */
    /*
     * Ordine inverso all'allocazione:
     * 1. Liberiamo le singole righe.
     * 2. Liberiamo il vettore dei puntatori.
     */
    for (int i = 0; i < N; i++) {
        free(visited[i]);
    }
    free(visited);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    int N = 3;
    int M = 3;

    printf("--- Test Esercizio Mappa ---\n");

    /* --- 1. COSTRUZIONE DATI (MATRICE ESEMPIO) --- */
    /*
     * Ricostruiamo la matrice dell'immagine:
     * 1  2 -3
     * 9 -9  7
     * 0  1  4
     */
    int **mappa = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) mappa[i] = (int *)malloc(M * sizeof(int));

    mappa[0][0] = 1; mappa[0][1] = 2;  mappa[0][2] = -3;
    mappa[1][0] = 9; mappa[1][1] = -9; mappa[1][2] = 7;
    mappa[2][0] = 0; mappa[2][1] = 1;  mappa[2][2] = 4;

    /* Stampa matrice */
    printf("Matrice Input:\n");
    for(int i=0; i<N; i++) {
        for(int j=0; j<M; j++) printf("%3d ", mappa[i][j]);
        printf("\n");
    }

    /* --- 2. ESECUZIONE --- */
    /*
     * Soluzione attesa dall'immagine:
     * Percorso: 1 -> 9 -> 1 -> 4 (Somma: 15? No, guardiamo l'immagine a destra)
     * L'immagine mostra: 1 -> 2 -> -9 (no) -> ...
     *
     * Analizziamo il percorso disegnato a destra nell'immagine:
     * (0,0)[1] -> (1,1)[-9] -> (0,1)[2] -> (1,2)[7] -> (2,2)[4] ? No, non si può ripassare.
     *
     *
     * Vediamo il percorso ottimale per ottenere 24:
     * (0,0)=1 -> (1,0)=9 -> (2,1)=1 -> (1,2)=7 -> (0,1)=2 -> (2,2)=4
     * Somma: 1+9+1+7+2+4 = 24.
     * Nota: Questo richiede movimenti diagonali e su/giù liberi.
     */
    solveMap(mappa, N, M);

    /* --- 3. PULIZIA --- */
    for (int i = 0; i < N; i++) free(mappa[i]);
    free(mappa);

    return 0;
}