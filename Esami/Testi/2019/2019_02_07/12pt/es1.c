#include <stdio.h>
#include <stdlib.h> /* Necessario per malloc, free, abs */

/*
 * ======================================================================================
 * NOME FUNZIONE: generaMatriceDiagonali
 * ======================================================================================
 * SCOPO:
 * Allocare dinamicamente una matrice quadrata NxN e riempirla secondo la logica:
 * matrice[i][j] = valore assoluto della differenza tra i e j.
 *
 * PARAMETRI:
 * - int N: La dimensione del lato della matrice quadrata.
 *
 * RITORNA:
 * - int**: Un puntatore a puntatore (doppio puntatore) che rappresenta la matrice
 * allocata nello Heap.
 *
 * CONCETTI CHIAVE:
 * - Allocazione Dinamica (malloc): Usiamo malloc perché N non è noto a tempo di
 * compilazione, ma viene passato a runtime.
 * - Matrice come array di puntatori: In C, una matrice dinamica è spesso gestita
 * come un array di puntatori alle righe. Ecco perché il tipo di ritorno è int**.
 * ======================================================================================
 */
int** generaMatriceDiagonali(int N) {

    /* --- 1. ALLOCAZIONE DEL VETTORE DI PUNTATORI (Le Righe) --- */
    /*
     * Spiegazione Sintattica:
     * (int**) -> Casting esplicito. Dice al compilatore: "tratta quest'area di memoria
     * come un puntatore a puntatore a interi".
     * malloc(...) -> Chiede al sistema operativo un blocco di memoria nello HEAP.
     * N * sizeof(int*) -> Calcoliamo la dimensione totale. Ci servono N "puntatori",
     * quindi moltiplichiamo N per la grandezza di un puntatore (int*).
     *
     * Nota Bene: Qui stiamo allocando solo la "colonna vertebrale" della matrice,
     * ovvero l'elenco delle righe. Non abbiamo ancora le celle vere e proprie.
     */
    int **matrice = (int**)malloc(N * sizeof(int*));

    /* Controllo difensivo: se la RAM è piena, malloc ritorna NULL */
    if (matrice == NULL) {
        printf("Errore: memoria insufficiente per le righe.\n");
        exit(1);
    }

    /* --- 2. ALLOCAZIONE DELLE COLONNE E RIEMPIMENTO --- */
    for (int i = 0; i < N; i++) {

        /*
         * Allocazione della singola riga i-esima.
         * sizeof(int) -> Qui usiamo la grandezza di un intero vero e proprio (4 byte solitamente),
         * perché la riga contiene dati, non indirizzi.
         */
        matrice[i] = (int*)malloc(N * sizeof(int));

        /* Controllo difensivo per la singola riga */
        if (matrice[i] == NULL) {
            printf("Errore: memoria insufficiente per la riga %d.\n", i);
            /* In un codice di produzione reale, qui dovremmo liberare le righe
               precedentemente allocate prima di uscire, per evitare memory leak parziali */
            exit(1);
        }

        /* --- 3. LOGICA DI RIEMPIMENTO --- */
        for (int j = 0; j < N; j++) {
            /*
             * Calcolo la distanza tra riga e colonna.
             * Esempio: Riga 0, Colonna 3 -> diff = -3.
             * Esempio: Riga 3, Colonna 0 -> diff = 3.
             */
            int diff = i - j;

            /*
             * Valore assoluto manuale (per didattica, oppure si usa abs() di stdlib).
             * Se la differenza è negativa, la inverto.
             * Questo crea l'effetto "specchio" rispetto alla diagonale.
             */
            if (diff < 0) {
                diff = -diff;
            }

            /*
             * Assegnazione:
             * matrice[i] -> Accede al puntatore della riga i.
             * [j] -> Aritmetica dei puntatori: si sposta di j posizioni e dereferenzia
             * la cella per scriverci dentro.
             */
            matrice[i][j] = diff;
        }
    }

    return matrice;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: stampaMatrice
 * ======================================================================================
 * SCOPO:
 * Visualizzare la matrice a video formattata correttamente.
 *
 * PARAMETRI:
 * - int** matrice: Il puntatore alla struttura dati.
 * - int N: La dimensione.
 * ======================================================================================
 */
void stampaMatrice(int **matrice, int N) {
    printf("\n--- Matrice Generata (N=%d) ---\n", N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            /* %3d riserva 3 spazi per ogni numero, garantendo l'allineamento visivo */
            printf("%3d ", matrice[i][j]);
        }
        printf("\n"); // A capo alla fine di ogni riga
    }
    printf("-----------------------------\n");
}

/*
 * ======================================================================================
 * NOME FUNZIONE: liberaMatrice
 * ======================================================================================
 * SCOPO:
 * Restituire la memoria al sistema operativo (evitare Memory Leaks).
 *
 * CONCETTI CHIAVE:
 * - Ordine di deallocazione: Dobbiamo liberare "dall'interno verso l'esterno".
 * Prima liberiamo le singole righe (i dati), poi liberiamo il vettore dei puntatori.
 * Se facessimo il contrario, perderemmo i riferimenti alle righe e non potremmo più liberarle!
 * ======================================================================================
 */
void liberaMatrice(int **matrice, int N) {
    /* 1. Libero ogni singola riga */
    for (int i = 0; i < N; i++) {
        free(matrice[i]); // Libera il blocco di interi puntato da matrice[i]
    }

    /* 2. Libero il vettore dei puntatori alle righe */
    free(matrice); // Libera la "colonna vertebrale"
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /*
     * Usiamo l'esempio dell'immagine (N=5) per verificare la correttezza,
     * ma il codice funziona per qualsiasi N positivo.
     */
    int N = 5;

    printf("Inizio programma. Generazione matrice per N = %d...\n", N);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /*
     * La variabile 'miaMatrice' (nello stack del main) riceverà
     * l'indirizzo di memoria della matrice allocata nello heap.
     */
    int **miaMatrice = generaMatriceDiagonali(N);

    /* --- 3. STAMPA DEI RISULTATI --- */
    stampaMatrice(miaMatrice, N);

    /* --- 4. PULIZIA DELLA MEMORIA --- */
    /*
     * Fondamentale: ogni malloc deve avere una free corrispondente.
     * Passiamo N perché la funzione deve sapere quante righe liberare.
     */
    liberaMatrice(miaMatrice, N);

    printf("Memoria liberata correttamente. Programma terminato.\n");

    return 0;
}