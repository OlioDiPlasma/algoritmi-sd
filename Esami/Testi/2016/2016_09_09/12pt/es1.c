#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Necessario per INT_MIN

/* ============================================================================
 * FUNZIONE: subMatMax
 * ============================================================================
 * SCOPO:
 * Scandisce una matrice rettangolare per individuare la sottomatrice quadrata
 * di dimensione n x n che possiede la somma degli elementi massima.
 * Restituisce tale somma massima.
 *
 * PARAMETRI:
 * - mat: int ** (Doppio puntatore)
 * Rappresenta la matrice di interi. È un puntatore a un array di puntatori
 * (le righe), ognuno dei quali punta a un array di interi (le colonne).
 * - r:   int
 * Il numero totale di righe della matrice 'mat'.
 * - c:   int
 * Il numero totale di colonne della matrice 'mat'.
 * - n:   int
 * La dimensione (lato) della sottomatrice quadrata da cercare.
 *
 * CONCETTI CHIAVE:
 * 1. Scorrimento della Finestra: Non possiamo iterare fino all'ultima riga o colonna.
 * Se cerchiamo un quadrato di lato 'n', l'ultimo indice di riga valido da cui
 * partire (angolo in alto a sinistra) è 'r - n'.
 * 2. Doppio Puntatore (**mat): In C, le matrici allocate dinamicamente non sono
 * blocchi contigui unici come gli array statici mat[R][C], ma spesso vettori
 * di puntatori. Usiamo mat[i][j] che viene tradotto in *(*(mat + i) + j).
 * ============================================================================
 */
int subMatMax(int **mat, int r, int c, int n) {

    /* --- 1. CONTROLLI DI SICUREZZA --- */
    /* Verifica se la matrice esiste e se le dimensioni della sottomatrice
       sono compatibili con la matrice originale. */
    if (mat == NULL) {
        printf("Errore: Puntatore a NULL.\n");
        return 0;
    }
    if (n > r || n > c) {
        printf("Errore: Dimensione n (%d) troppo grande per la matrice (%dx%d).\n", n, r, c);
        return 0;
    }

    /* Inizializziamo maxSum con il valore intero più piccolo possibile.
       Perché? Se la matrice contenesse tutti numeri negativi (es. -5, -10),
       inizializzare a 0 sarebbe sbagliato perché 0 sarebbe maggiore della somma reale. */
    int maxSum = INT_MIN; 

    /* --- 2. LOGICA DI SCANSIONE (Ancoraggio in alto a sinistra) --- */
    
    /* Ciclo esterno: scorre le righe possibili per l'angolo in alto a sinistra della sottomatrice.
       SINTASSI: i <= r - n
       Spiegazione: Se ho 3 righe e cerco un quadrato di lato 2 (n=2), 
       posso partire solo dalla riga 0 e dalla riga 1. (3 - 2 = 1). 
       Se partissi dalla riga 2, andrei fuori memoria (riga 3 non esiste). */
    for (int i = 0; i <= r - n; i++) {

        /* Ciclo intermedio: scorre le colonne possibili per l'angolo in alto a sinistra. */
        for (int j = 0; j <= c - n; j++) {

            /* --- 3. CALCOLO SOMMA SOTTOMATRICE CORRENTE --- */
            /* Ora che siamo ancorati in mat[i][j], dobbiamo sommare 
               tutti gli elementi del quadrato n x n che parte da qui. */
            
            int currentSum = 0; // Accumulatore temporaneo

            /* Scorriamo le n righe della sottomatrice */
            for (int ki = 0; ki < n; ki++) {
                /* Scorriamo le n colonne della sottomatrice */
                for (int kj = 0; kj < n; kj++) {
                    
                    /* SINTASSI: mat[i + ki][j + kj]
                       Accediamo alla riga 'i' (offset base) spostata di 'ki',
                       e alla colonna 'j' (offset base) spostata di 'kj'. 
                       È equivalente a dire: *(*(mat + i + ki) + j + kj) */
                    currentSum += mat[i + ki][j + kj];
                }
            }

            /* --- 4. AGGIORNAMENTO DEL MASSIMO --- */
            /* Se la somma appena calcolata è maggiore del record attuale, aggiorniamo. */
            if (currentSum > maxSum) {
                maxSum = currentSum;
                /* Nota didattica: Se l'esercizio chiedesse anche GLI INDICI della
                   sottomatrice migliore, qui dovremmo salvarci 'i' e 'j' in variabili apposite. */
            }
        }
    }

    return maxSum;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO: Creare un ambiente controllato per testare la funzione.
 * Dobbiamo allocare dinamicamente una matrice per simulare l'input reale
 * che la funzione si aspetta (int **).
 * ============================================================================
 */
int main() {
    /* Parametri dell'esempio nell'immagine */
    int R = 3; // Righe
    int C = 4; // Colonne
    int N = 2; // Dimensione sottomatrice

    printf("--- INIZIO TEST ---\n");

    /* --- 1. ALLOCAZIONE DELLA MATRICE (Gestione Memoria) --- */
    
    /* Passo A: Allocare l'array di puntatori alle righe.
       SINTASSI: (int **)malloc(...) 
       Stiamo chiedendo spazio per 'R' puntatori. Ogni cella conterrà un indirizzo di memoria.
       sizeof(int*) è fondamentale perché stiamo allocando puntatori, non interi semplici. */
    int **matrice = (int **)malloc(R * sizeof(int *));
    if (matrice == NULL) {
        fprintf(stderr, "Errore di allocazione memoria (righe).\n");
        return -1;
    }

    /* Passo B: Allocare ogni singola riga. */
    for (int i = 0; i < R; i++) {
        /* Qui allochiamo 'C' interi per ogni riga.
           Uso sizeof(int) perché qui conserviamo i dati veri e propri. */
        matrice[i] = (int *)malloc(C * sizeof(int));
        if (matrice[i] == NULL) {
            fprintf(stderr, "Errore di allocazione memoria (colonne).\n");
            // In un programma reale qui bisognerebbe liberare le righe già allocate prima di uscire
            return -1;
        }
    }

    /* --- 2. INIZIALIZZAZIONE DATI (Hardcoded come da esempio) --- */
    /*
       Matrice esempio:
       0  1  2  3  (indici colonna)
    0 [5, 2, 3, 1]
    1 [3, 1, 6, 4]
    2 [3, 0, 5, 2]
    */
    
    // Riga 0
    matrice[0][0] = 5; matrice[0][1] = 2; matrice[0][2] = 3; matrice[0][3] = 1;
    // Riga 1
    matrice[1][0] = 3; matrice[1][1] = 1; matrice[1][2] = 6; matrice[1][3] = 4;
    // Riga 2
    matrice[2][0] = 3; matrice[2][1] = 0; matrice[2][2] = 5; matrice[2][3] = 2;

    /* Stampa della matrice per verifica visiva */
    printf("Matrice di input (%dx%d):\n", R, C);
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    printf("\nRicerca sottomatrice massima di lato %d...\n", N);
    int risultato = subMatMax(matrice, R, C, N);

    /* --- 4. STAMPA RISULTATI --- */
    printf("Risultato atteso: 17 (sottomatrice 6,4,5,2)\n");
    printf("Risultato ottenuto: %d\n", risultato);

    if (risultato == 17) {
        printf("-> TEST SUPERATO.\n");
    } else {
        printf("-> TEST FALLITO.\n");
    }

    /* --- 5. PULIZIA DELLA MEMORIA (Fondamentale!) --- */
    /* Regola d'oro: Per ogni malloc deve esserci una free.
       L'ordine di deallocazione è INVERSO all'allocazione.
       Prima liberiamo le righe (i dati), poi il vettore dei puntatori. */
    
    for (int i = 0; i < R; i++) {
        free(matrice[i]); // Libera l'array di interi della riga i
    }
    free(matrice); // Libera l'array di puntatori
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}