#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: displRanking
 * ============================================================================
 * SCOPO:
 * Calcolare e stampare, per ogni giornata di campionato (colonna), l'indice
 * della squadra (riga) che è in testa alla classifica (capolista).
 * La classifica è data dalla somma dei punti accumulati dalla prima giornata
 * fino a quella corrente.
 *
 * PARAMETRI:
 * - int **C : La matrice dei punteggi (n righe x m colonne).
 * C[i][j] sono i punti della squadra 'i' nella giornata 'j'.
 * - int n   : Numero di squadre (righe).
 * - int m   : Numero di giornate (colonne).
 *
 * CONCETTI CHIAVE:
 * 1. Accumulatore (Vettore 'classifica'): Non possiamo determinare la capolista
 * guardando solo i punti della giornata 'j'. Dobbiamo mantenere un array
 * che memorizza il punteggio TOTALE di ogni squadra aggiornato passo passo.
 * 2. Scansione per Colonne: Il ciclo esterno deve essere sulle GIORNATE (colonne),
 * perché l'output richiesto è "per ogni giornata".
 * ============================================================================
 */
void displRanking(int **C, int n, int m) {

    /* --- 1. ALLOCAZIONE ARRAY DI SUPPORTO --- */
    /*
     * Ci serve un "taccuino" dove segnare i punti totali di ogni squadra.
     * Usiamo 'calloc' invece di 'malloc' per due motivi:
     * 1. Alloca lo spazio per 'n' interi.
     * 2. Inizializza tutto a 0 (fondamentale, perché partiremo sommando).
     */
    int *punti_totali = (int *)calloc(n, sizeof(int));
    if (punti_totali == NULL) {
        printf("Errore di memoria\n");
        return;
    }

    /* --- 2. LOGICA PRINCIPALE (Ciclo sulle Giornate) --- */
    /*
     * L'esercizio chiede l'output "per ogni giornata".
     * Quindi il ciclo esterno scorre le colonne 'j' da 0 a m-1.
     */
    for (int j = 0; j < m; j++) {

        int max_punti = -1;      // Terrà traccia del punteggio più alto trovato oggi
        int capolista_idx = -1;  // Terrà traccia dell'indice della squadra vincente

        /* --- 3. AGGIORNAMENTO CLASSIFICA (Ciclo sulle Squadre) --- */
        /*
         * Per ogni giornata, aggiorniamo il punteggio di TUTTE le squadre
         * sommando i punti ottenuti nella colonna 'j' corrente.
         */
        for (int i = 0; i < n; i++) {
            /*
             * DETTAGLIO SINTATTICO:
             * punti_totali[i] : Il punteggio accumulato fino a ieri.
             * C[i][j]         : I punti fatti oggi (riga i, colonna j).
             * +=              : Somma e assegna.
             */
            punti_totali[i] += C[i][j];

            /*
             * Controllo se questa squadra è la nuova capolista.
             * Usiamo '>' stretto: in caso di parità, manteniamo la squadra
             * con l'indice più basso (la prima trovata), come convenzione standard.
             */
            if (punti_totali[i] > max_punti) {
                max_punti = punti_totali[i];
                capolista_idx = i;
            }
        }

        /* --- 4. STAMPA RISULTATO GIORNATA --- */
        printf("%d ", capolista_idx);
    }
    printf("\n"); // A capo finale per pulizia

    /* --- 5. GESTIONE MEMORIA --- */
    /*
     * Abbiamo allocato 'punti_totali' con calloc, ora dobbiamo liberarlo
     * prima che la funzione termini per evitare Memory Leaks.
     */
    free(punti_totali);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Ricostruire la matrice dell'esempio (4x3) e testare la funzione.
 * Dati Esempio:
 * Sq 0: 3 1 0
 * Sq 1: 0 1 1
 * Sq 2: 1 1 1
 * Sq 3: 1 1 3
 * Risultato atteso: 0 0 3
 * ============================================================================
 */
int main() {
    int n = 4; // Squadre
    int m = 3; // Giornate

    /* --- 1. ALLOCAZIONE MATRICE DINAMICA --- */
    /*
     * Passo A: Allocare l'array di puntatori (le righe).
     * sizeof(int*) perché ogni cella conterrà un indirizzo di memoria.
     */
    int **matrice = (int **)malloc(n * sizeof(int *));

    /*
     * Passo B: Allocare le colonne per ogni riga.
     * sizeof(int) perché qui conserviamo i numeri veri (0, 1, 3).
     */
    for (int i = 0; i < n; i++) {
        matrice[i] = (int *)malloc(m * sizeof(int));
    }

    /* --- 2. INIZIALIZZAZIONE DATI (Hardcoded dall'esempio) --- */
    /* Riga 0 */
    matrice[0][0] = 3; matrice[0][1] = 1; matrice[0][2] = 0;
    /* Riga 1 */
    matrice[1][0] = 0; matrice[1][1] = 1; matrice[1][2] = 1;
    /* Riga 2 */
    matrice[2][0] = 1; matrice[2][1] = 1; matrice[2][2] = 1;
    /* Riga 3 */
    matrice[3][0] = 1; matrice[3][1] = 1; matrice[3][2] = 3;

    printf("--- Inizio Calcolo Classifica ---\n");
    printf("Risultato atteso: 0 0 3\n");
    printf("Risultato reale : ");

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    displRanking(matrice, n, m);

    /* --- 4. LIBERAZIONE MEMORIA (Fondamentale!) --- */
    /*
     * Ordine inverso rispetto alla malloc:
     * Prima libero le singole righe, poi il contenitore delle righe.
     */
    for (int i = 0; i < n; i++) {
        free(matrice[i]);
    }
    free(matrice);

    return 0;
}