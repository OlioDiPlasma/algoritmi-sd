#include <stdio.h>
#include <stdlib.h>

/* * ======================================================================================
 * NOME FUNZIONE: elaboraMatrice
 * ======================================================================================
 * * SCOPO:
 * Generare una nuova matrice M' dove ogni elemento [i][j] è la somma degli elementi
 * sulle sue diagonali (principale e secondaria), ma considerando SOLAMENTE gli 
 * elementi che si trovano in colonne >= j.
 *
 * PARAMETRI:
 * - int **M:        Puntatore alla matrice di input (sola lettura).
 * - int r:          Numero di righe della matrice.
 * - int c:          Numero di colonne della matrice.
 * - int ***M_prime: PUNTATORE TRIPLO all'indirizzo dove salveremo la nuova matrice.
 * (Output parameter).
 *
 * CONCETTI CHIAVE:
 * 1. Perché "int ***M_prime"? 
 * In C, i parametri sono passati per valore. Se passassimo solo "int **", 
 * l'allocazione fatta dentro la funzione modificherebbe solo la copia locale 
 * della variabile. Per modificare il puntatore 'matrice' dichiarato nel main, 
 * dobbiamo passare il suo indirizzo di memoria (&matrice). Quindi:
 * Main ha (int**) -> passiamo (&int**) -> la funzione riceve (int***).
 *
 * 2. Logica Geometrica:
 * Il problema chiede di sommare le diagonali solo se "colonna_vicino >= colonna_attuale".
 * Geometricamente, questo significa guardare solo "a destra" (o sulla stessa colonna).
 * - Diagonale (South-East): riga cresce, colonna cresce (col > j -> OK).
 * - Antidiagonale (North-East): riga cala, colonna cresce (col > j -> OK).
 * - Le diagonali verso sinistra (South-West e North-West) hanno col < j, quindi
 * vengono ignorate a priori per specifica del problema.
 * ======================================================================================
 */
void elaboraMatrice(int **M, int r, int c, int ***M_prime) {

    /* --- 1. ALLOCAZIONE DELLA MATRICE DI DESTINAZIONE --- */
    
    /* * Dereferenziamo M_prime (*M_prime) per accedere alla variabile puntatore del main.
     * Usiamo malloc per creare un array di 'r' puntatori a interi (le righe).
     * sizeof(int*) è fondamentale: stiamo allocando indirizzi, non interi!
     */
    *M_prime = (int **)malloc(r * sizeof(int *));
    
    /* Controllo difensivo: se malloc fallisce, terminiamo per evitare crash */
    if (*M_prime == NULL) {
        printf("Errore: Memoria insufficiente per le righe.\n");
        exit(1);
    }

    /* Ora allochiamo le colonne per ogni riga */
    for (int i = 0; i < r; i++) {
        /*
         * (*M_prime)[i]: Accediamo all'i-esimo puntatore riga appena allocato.
         * Allociamo 'c' interi per ogni riga. Qui usiamo sizeof(int).
         * NOTA SINTATTICA: Le parentesi (*M_prime) sono obbligatorie perché 
         * l'operatore [] ha precedenza su *. Senza parentesi cercheremmo di 
         * accedere a M_prime[i] (errato) e poi dereferenziarlo.
         */
        (*M_prime)[i] = (int *)malloc(c * sizeof(int));
        
        if ((*M_prime)[i] == NULL) {
            printf("Errore: Memoria insufficiente per la riga %d.\n", i);
            exit(1);
        }
    }

    /* --- 2. LOGICA DI CALCOLO --- */

    /* Iteriamo su ogni cella della matrice originale come "centro" del calcolo */
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            
            /* Inizializziamo la somma con l'elemento stesso (contato una sola volta) */
            int somma = M[i][j];

            /* * Strategia: Non serve controllare "se colonna >= j" all'interno di un loop
             * che controlla tutte le direzioni. Sappiamo geometricamente che:
             * - Spostandoci a Destra-Giù (i+k, j+k), la colonna aumenta -> VALID
             * - Spostandoci a Destra-Su (i-k, j+k), la colonna aumenta -> VALID
             * - Spostandoci a Sinistra, la colonna diminuisce -> INVALID (saltiamo)
             */

            /* A. Scansione Diagonale Principale verso il basso (South-East) */
            /* Partiamo dalla cella successiva (k=1) */
            int k = 1; 
            while (1) {
                int r_vicino = i + k;
                int c_vicino = j + k;

                /* Se usciamo dai bordi della matrice, ci fermiamo */
                if (r_vicino >= r || c_vicino >= c) break;

                /* Aggiungiamo il valore. Qui c_vicino è sicuramente > j */
                somma += M[r_vicino][c_vicino];
                k++;
            }

            /* B. Scansione Antidiagonale verso l'alto (North-East) */
            k = 1;
            while (1) {
                int r_vicino = i - k;
                int c_vicino = j + k;

                /* * Controllo bordi:
                 * r_vicino < 0: siamo usciti sopra
                 * c_vicino >= c: siamo usciti a destra
                 */
                if (r_vicino < 0 || c_vicino >= c) break;

                somma += M[r_vicino][c_vicino];
                k++;
            }

            /* --- 3. ASSEGNAZIONE RISULTATO --- */
            /* Scriviamo il risultato nella matrice allocata dinamica */
            (*M_prime)[i][j] = somma;
        }
    }
}

/* * ======================================================================================
 * FUNZIONI DI UTILITÀ (Stampa e Free)
 * ======================================================================================
 */
void stampaMatrice(int **M, int r, int c, char *nome) {
    printf("Matrice %s:\n", nome);
    for (int i = 0; i < r; i++) {
        printf("| ");
        for (int j = 0; j < c; j++) {
            /* %4d serve per allineare bene i numeri (padding di 4 spazi) */
            printf("%4d ", M[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

void liberaMatrice(int **M, int r) {
    /* Bisogna liberare PRIMA le righe, POI il vettore dei puntatori */
    for (int i = 0; i < r; i++) {
        free(M[i]);
    }
    free(M);
}

/* * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Qui simuliamo l'esame: creiamo dati hardcoded, chiamiamo la funzione e verifichiamo.
 */
int main() {
    int r = 3;
    int c = 3;

    /* --- 1. PREPARAZIONE DATI INPUT --- */
    /* Allocazione manuale della matrice M per l'esempio */
    int **M = (int **)malloc(r * sizeof(int *));
    for(int i=0; i<r; i++) M[i] = (int *)malloc(c * sizeof(int));

    /* Riempimento con i dati dell'esempio dell'immagine:
       1 2 3
       4 5 6
       7 8 9
    */
    int counter = 1;
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            M[i][j] = counter++;
        }
    }

    /* Variabile puntatore che ospiterà la matrice risultato */
    int **M_output = NULL;

    printf("--- INIZIO PROGRAMMA ---\n\n");
    stampaMatrice(M, r, c, "M (Input)");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /* * NOTA IMPORTANTE: Passiamo &M_output (indirizzo del puntatore).
     * M_output è di tipo (int**), quindi &M_output è (int***).
     */
    elaboraMatrice(M, r, c, &M_output);

    /* --- 3. VERIFICA RISULTATI --- */
    /*
     * Output atteso secondo l'immagine:
     * 15  8  3
     * 14 17  6
     * 15 14  9
     */
    stampaMatrice(M_output, r, c, "M' (Risultato)");

    /* --- 4. PULIZIA MEMORIA --- */
    /* È fondamentale in C liberare tutta la memoria allocata con malloc */
    liberaMatrice(M, r);       // Libera input
    liberaMatrice(M_output, r); // Libera output creato nella funzione

    printf("--- MEMORIA LIBERATA CORRETTAMENTE ---\n");
    return 0;
}