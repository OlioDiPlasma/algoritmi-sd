#include <stdio.h>
#include <stdlib.h>

/* =========================================================================
 * INTESTAZIONE FUNZIONE
 * =========================================================================
 * SCOPO:
 * Individua e stampa tutte le coppie di persone (indici i, j) che hanno
 * esattamente 'k' amici in comune.
 *
 * PARAMETRI:
 * - M: La matrice delle adiacenze (doppio puntatore a intero).
 * M[i][j] == 1 significa che i e j sono amici.
 * - N: La dimensione della matrice quadrata (numero di persone).
 * - k: Il numero esatto di amici in comune richiesto.
 *
 * CONCETTI CHIAVE:
 * 1. Rappresentazione delle relazioni: Usiamo una matrice di adiacenza.
 * Se la riga 'i' ha un 1 alla colonna 'x', 'i' è amico di 'x'.
 * 2. Amici in comune: Due persone 'i' e 'j' hanno l'amico 'x' in comune
 * se e solo se M[i][x] == 1 AND M[j][x] == 1.
 * 3. Simmetria: Poiché l'amicizia è reciproca (coppie non ordinate),
 * analizziamo solo le coppie dove i < j per evitare duplicati
 * (es. analizzo (1,2) ma non (2,1)) e l'auto-amicizia (1,1).
 * ========================================================================= */

void stampa_coppie_k_amici(int **M, int N, int k) {
    
    printf("Coppie con esattamente %d amici in comune:\n", k);

    /* --- 1. CICLO ESTERNO (Prima persona della coppia) --- */
    /* Scorriamo da 0 fino al penultimo elemento (N-1 non avrebbe successivi con cui fare coppia) */
    for (int i = 0; i < N; i++) {

        /* --- 2. CICLO INTERMEDIO (Seconda persona della coppia) --- */
        /* Partiamo da j = i + 1.
         * Perché?
         * - Se partissimo da 0: controlleremmo due volte la stessa coppia (i,j) e (j,i).
         * - Se partissimo da i: controlleremmo una persona con se stessa (i,i).
         * Partendo da i+1 garantiamo coppie uniche e distinte. */
        for (int j = i + 1; j < N; j++) {

            int amici_comuni = 0; // Contatore per la coppia corrente (i, j)

            /* --- 3. LOGICA DI CONTEGGIO (Intersezione) --- */
            /* Scorriamo tutte le possibili terze persone 'x' per vedere se sono amiche di entrambi */
            for (int x = 0; x < N; x++) {
                
                /*
                 * DETTAGLIO SINTATTICO:
                 * M[i][x]: Accedo alla riga i, colonna x (amicizia tra i e x).
                 * M[j][x]: Accedo alla riga j, colonna x (amicizia tra j e x).
                 * &&: Operatore logico AND. Vero solo se entrambi sono 1.
                 */
                if (M[i][x] == 1 && M[j][x] == 1) {
                    amici_comuni++;
                }
            }

            /* --- 4. VERIFICA E STAMPA --- */
            if (amici_comuni == k) {
                printf("(%d, %d) ", i, j);
            }
        }
    }
    printf("\n"); // A capo finale per pulizia
}

/* =========================================================================
 * MAIN DI TEST
 * =========================================================================
 * SCOPO:
 * Creare la matrice dell'esempio (6x6), popolarla con i dati dell'immagine,
 * e verificare che l'output corrisponda (k=2).
 * ========================================================================= */
int main() {
    int N = 6; // Dimensione matrice esempio
    int k = 2; // Valore dedotto dall'esempio (vedi analisi successiva)

    printf("--- INIZIO PROGRAMMA ---\n");

    /* --- 1. ALLOCAZIONE DINAMICA MATRICE --- */
    /*
     * Perché malloc?
     * Per simulare un caso reale dove N non è noto a priori.
     * M è un array di puntatori (le righe).
     */
    int **M = (int **)malloc(N * sizeof(int *));
    if (M == NULL) { printf("Errore malloc righe"); exit(1); }

    for (int i = 0; i < N; i++) {
        /* Ogni riga è un array di N interi */
        M[i] = (int *)malloc(N * sizeof(int));
        if (M[i] == NULL) { printf("Errore malloc colonna"); exit(1); }
    }

    /* --- 2. POPOLAMENTO DATI (Hardcoded dall'immagine) --- */
    /*
     * Copio i valori esattamente come appaiono nella matrice M dell'immagine.
     * 1 = amico, 0 = non amico.
     */
    
    // Riga 0: 0 1 1 0 0 0
    int row0[] = {0, 1, 1, 0, 0, 0};
    // Riga 1: 1 0 0 1 1 1
    int row1[] = {1, 0, 0, 1, 1, 1};
    // Riga 2: 1 0 0 0 1 0
    int row2[] = {1, 0, 0, 0, 1, 0};
    // Riga 3: 0 1 0 0 1 1
    int row3[] = {0, 1, 0, 0, 1, 1};
    // Riga 4: 0 1 1 1 0 1
    int row4[] = {0, 1, 1, 1, 0, 1};
    // Riga 5: 0 1 0 1 1 0
    int row5[] = {0, 1, 0, 1, 1, 0};

    /* Copio i valori nelle righe allocate dinamicamente */
    for(int j=0; j<N; j++) M[0][j] = row0[j];
    for(int j=0; j<N; j++) M[1][j] = row1[j];
    for(int j=0; j<N; j++) M[2][j] = row2[j];
    for(int j=0; j<N; j++) M[3][j] = row3[j];
    for(int j=0; j<N; j++) M[4][j] = row4[j];
    for(int j=0; j<N; j++) M[5][j] = row5[j];

    /* --- 3. ESECUZIONE FUNZIONE --- */
    /* Passiamo la matrice, la dimensione e il k richiesto */
    stampa_coppie_k_amici(M, N, k);

    /* --- 4. GESTIONE MEMORIA (PULIZIA) --- */
    /*
     * Regola: Liberare in ordine inverso all'allocazione.
     * Prima le righe (i figli), poi il vettore dei puntatori (il padre).
     */
    for (int i = 0; i < N; i++) {
        free(M[i]); // Libero la singola riga
    }
    free(M); // Libero l'array di puntatori
    
    printf("--- FINE PROGRAMMA ---\n");

    return 0;
}