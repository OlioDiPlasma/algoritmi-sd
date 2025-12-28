#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: calcolaSommaDiagonali
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria ("helper"). Data una coordinata (riga, colonna) su cui posizioniamo
 * ipoteticamente l'alfiere, calcola la somma di tutti i pezzi presenti sulle due
 * diagonali che si incrociano in quel punto.
 *
 * PARAMETRI:
 * * board : (int **) La matrice che rappresenta la scacchiera.
 * N       : (int) La dimensione del lato della scacchiera (NxN).
 * r       : (int) L'indice di riga dell'alfiere.
 * c       : (int) L'indice di colonna dell'alfiere.
 *
 * CONCETTI CHIAVE:
 * 1. Movimento Diagonale: Un alfiere si muove modificando simultaneamente riga e colonna.
 * - Alto-Sinistra: riga diminuisce, colonna diminuisce (r-1, c-1).
 * - Alto-Destra:   riga diminuisce, colonna aumenta    (r-1, c+1).
 * - Basso-Sinistra: riga aumenta, colonna diminuisce   (r+1, c-1).
 * - Basso-Destra:  riga aumenta, colonna aumenta       (r+1, c+1).
 * 2. Controllo dei Bordi: Fondamentale verificare sempre che gli indici siano
 * >= 0 e < N prima di accedere alla matrice `board[i][j]`.
 * ======================================================================================
 */
int calcolaSommaDiagonali(int **board, int N, int r, int c) {
    int somma = 0;
    int i, j;

    /* --- 1. DIAGONALE ALTO-SINISTRA --- */
    /* Partiamo dalla cella e andiamo indietro sia di riga che di colonna */
    for (i = r - 1, j = c - 1; i >= 0 && j >= 0; i--, j--) {
        /*
         * DETTAGLIO SINTATTICO:
         * Accediamo alla cella board[i][j]. Se board è int**, board[i] è un puntatore
         * alla riga i-esima, e [j] accede all'elemento j-esimo di quella riga.
         */
        somma += board[i][j];
    }

    /* --- 2. DIAGONALE ALTO-DESTRA --- */
    /* Riga diminuisce, colonna aumenta */
    for (i = r - 1, j = c + 1; i >= 0 && j < N; i--, j++) {
        somma += board[i][j];
    }

    /* --- 3. DIAGONALE BASSO-SINISTRA --- */
    /* Riga aumenta, colonna diminuisce */
    for (i = r + 1, j = c - 1; i < N && j >= 0; i++, j--) {
        somma += board[i][j];
    }

    /* --- 4. DIAGONALE BASSO-DESTRA --- */
    /* Riga aumenta, colonna aumenta */
    for (i = r + 1, j = c + 1; i < N && j < N; i++, j++) {
        somma += board[i][j];
    }

    /* Nota: Non sommiamo board[r][c] perché il testo dice che l'alfiere va posto su una
       cella VUOTA (valore 0), quindi sommare 0 è ininfluente. */
    
    return somma;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaMigliorPosizioneAlfiere
 * ======================================================================================
 * SCOPO:
 * Scorre tutta la matrice. Per ogni cella vuota (valore 0), calcola quanto varrebbe
 * la somma dei pezzi attaccati se mettessimo lì l'alfiere. Tiene traccia del massimo
 * trovato e stampa le coordinate.
 *
 * PARAMETRI:
 * * board : (int **) Puntatore doppio alla scacchiera.
 * N       : (int) Dimensione N della scacchiera.
 *
 * CONCETTI CHIAVE:
 * 1. Ricerca del Massimo: Usiamo una variabile `maxSomma` inizializzata a -1
 * per essere sicuri di aggiornarla al primo colpo valido.
 * 2. Passaggio per Valore vs Riferimento: Qui non dobbiamo ritornare valori al main
 * ma solo stampare a video, quindi la funzione è `void`.
 * ======================================================================================
 */
void trovaMigliorPosizioneAlfiere(int **board, int N) {
    int maxSomma = -1;
    int bestR = -1;
    int bestC = -1;

    /* --- 1. SCANSIONE DELLA MATRICE --- */
    /* Usiamo due cicli for annidati per visitare ogni casella [i][j] */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            /* --- 2. VERIFICA CELLA VUOTA --- */
            /* Il testo dice: "le caselle vuote assumono valore uguale a 0".
               L'alfiere va piazzato SOLO su caselle vuote. */
            if (board[i][j] == 0) {
                
                /* Calcoliamo il punteggio per questa posizione candidata */
                int sommaCorrente = calcolaSommaDiagonali(board, N, i, j);

                /* --- 3. AGGIORNAMENTO DEL MASSIMO --- */
                /* Se troviamo una somma maggiore della precedente, salviamo tutto */
                if (sommaCorrente > maxSomma) {
                    maxSomma = sommaCorrente;
                    bestR = i;
                    bestC = j;
                }
            }
        }
    }

    /* --- 4. OUTPUT RISULTATO --- */
    if (bestR != -1) {
        /*
         * NOTA DIDATTICA:
         * Gli informatici contano da 0.
         * Gli esseri umani (e il testo dell'esercizio) contano da 1.
         * Quindi stampiamo `bestR + 1` e `bestC + 1`.
         */
        printf("Posizione Ottima: Riga %d, Colonna %d (Somma: %d)\n", 
               bestR + 1, bestC + 1, maxSomma);
    } else {
        printf("Nessuna cella vuota disponibile.\n");
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare la matrice dell'esempio (4x4), popolarla e testare la funzione.
 * ======================================================================================
 */
int main() {
    int N = 4;

    printf("--- 1. ALLOCAZIONE MATRICE DINAMICA ---\n");
    /*
     * GESTIONE MEMORIA (MALLOC DI MATRICE):
     * Per creare una matrice int[N][N] dinamica, dobbiamo usare un DOPPIO PUNTATORE.
     * Passo A: Allocare un array di N puntatori (le righe).
     */
    int **scacchiera = (int **)malloc(N * sizeof(int *));
    if (scacchiera == NULL) { 
        fprintf(stderr, "Errore malloc righe\n"); 
        return 1; 
    }

    /* Passo B: Per ogni riga, allocare un array di N interi (le colonne) */
    for (int i = 0; i < N; i++) {
        scacchiera[i] = (int *)malloc(N * sizeof(int));
        if (scacchiera[i] == NULL) {
            /* Se fallisce qui dovremmo liberare le righe precedenti... per brevità usciamo */
            fprintf(stderr, "Errore malloc colonne\n");
            exit(1);
        }
    }

    /* --- 2. INIZIALIZZAZIONE DATI (HARDCODED) --- */
    /* Copiamo i dati dell'immagine 4x4
       Riga 1: 0 3 4 0
       Riga 2: 1 0 6 6
       Riga 3: 1 3 9 0
       Riga 4: 0 0 3 1
    */
    // Riga 0 (che nel testo è riga 1)
    scacchiera[0][0] = 0; scacchiera[0][1] = 3; scacchiera[0][2] = 4; scacchiera[0][3] = 0;
    // Riga 1 (testo riga 2)
    scacchiera[1][0] = 1; scacchiera[1][1] = 0; scacchiera[1][2] = 6; scacchiera[1][3] = 6;
    // Riga 2 (testo riga 3)
    scacchiera[2][0] = 1; scacchiera[2][1] = 3; scacchiera[2][2] = 9; scacchiera[2][3] = 0;
    // Riga 3 (testo riga 4)
    scacchiera[3][0] = 0; scacchiera[3][1] = 0; scacchiera[3][2] = 3; scacchiera[3][3] = 1;

    /* Stampa di controllo della matrice */
    printf("Scacchiera Input:\n");
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%d ", scacchiera[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    /*
     * Risultato atteso dall'immagine:
     * Cella di riga 4, colonna 2 (che per noi è indices [3][1]).
     * Somma attesa: 16.
     * Verifica:
     * Da [3][1] (val 0):
     * - Alto-Sx: [2][0] = 1
     * - Alto-Dx: [2][2] = 9 -> poi da lì [1][3] = 6. Totale diag = 15.
     * - Basso: fuori bordo.
     * Totale 1 + 15 = 16. Corretto.
     */
    trovaMigliorPosizioneAlfiere(scacchiera, N);

    /* --- 4. PULIZIA MEMORIA (FREE) --- */
    /*
     * GESTIONE MEMORIA (FREE):
     * L'ordine di liberazione è INVERSO all'allocazione.
     * Prima liberiamo il contenuto (le righe singole).
     */
    for (int i = 0; i < N; i++) {
        free(scacchiera[i]);
    }
    /* Poi liberiamo il contenitore (l'array di puntatori) */
    free(scacchiera);

    return 0;
}