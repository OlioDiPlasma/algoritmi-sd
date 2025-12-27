#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * 1. FUNZIONE PRINCIPALE: flatten
 * ============================================================================
 * SCOPO:
 * Appiattisce una matrice 3D (D^2 livelli x R righe x C colonne)
 * in una matrice 2D ((D*R) righe x (D*C) colonne).
 *
 * PARAMETRI:
 * - M: La matrice tridimensionale sorgente.
 * - D: Dimensione della griglia di livelli (ci sono D*D livelli).
 * - R: Numero di righe per singolo livello.
 * - C: Numero di colonne per singolo livello.
 */
int **flatten(int ***M, int D, int R, int C) {
    /* --- 1. CALCOLO DIMENSIONI OUTPUT --- */
    int out_rows = D * R;
    int out_cols = D * C;

    /* --- 2. ALLOCAZIONE MATRICE DESTINAZIONE --- */
    int **matrice_flat = (int **)malloc(out_rows * sizeof(int *));
    if (matrice_flat == NULL) return NULL;

    for (int i = 0; i < out_rows; i++) {
        matrice_flat[i] = (int *)malloc(out_cols * sizeof(int));
    }

    /* --- 3. LOGICA DI COPIA (MAPPING) --- */
    int num_livelli = D * D;

    for (int k = 0; k < num_livelli; k++) {
        /* * Calcoliamo in che "quadrante" (macro-riga e macro-colonna)
         * finisce l'intero livello k.
         */
        int riga_blocco = k / D; 
        int col_blocco  = k % D;

        /* Iteriamo sui pixel del singolo livello */
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                
                /* Calcoliamo l'indice assoluto nella matrice grande */
                int dest_r = (riga_blocco * R) + r;
                int dest_c = (col_blocco * C) + c;

                /* Copia valore */
                matrice_flat[dest_r][dest_c] = M[k][r][c];
            }
        }
    }
    return matrice_flat;
}

/* ============================================================================
 * 2. FUNZIONI DI SUPPORTO (HELPER)
 * ============================================================================
 * Queste servono per gestire la memoria e stampare. 
 * Devono comparire UNA SOLA VOLTA nel file.
 */

/* Stampa una matrice 2D formattata */
void print_matrix(int **M, int righe, int colonne) {
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("%3d ", M[i][j]);
        }
        printf("\n");
        // Riga vuota estetica tra i blocchi (opzionale, basata su R=3)
        if ((i + 1) % 3 == 0 && i != righe - 1) printf("\n");
    }
}

/* Libera la memoria di una matrice 2D */
void free_matrix_2d(int **M, int righe) {
    for (int i = 0; i < righe; i++) {
        free(M[i]);
    }
    free(M);
}

/* Libera la memoria di una matrice 3D */
void free_matrix_3d(int ***M, int livelli, int righe) {
    for (int i = 0; i < livelli; i++) {
        for (int j = 0; j < righe; j++) {
            free(M[i][j]);
        }
        free(M[i]);
    }
    free(M);
}

/* ============================================================================
 * 3. MAIN DI TEST (SPECIFICO PER L'IMMAGINE 6x6)
 * ============================================================================
 */
int main() {
    printf("--- TEST ESERCIZIO FLATTEN (6x6) ---\n");

    /* DATI RICAVATI DALLA TUA SECONDA IMMAGINE */
    int D = 2;       // Griglia 2x2 livelli
    int R = 3;       // Altezza totale 6 / 2 = 3
    int C = 3;       // Larghezza totale 6 / 2 = 3
    int num_livelli = D * D; // 4 livelli totali

    /* --- A. ALLOCAZIONE INPUT --- */
    int ***M = (int ***)malloc(num_livelli * sizeof(int **));
    for (int k = 0; k < num_livelli; k++) {
        M[k] = (int **)malloc(R * sizeof(int *));
        for (int i = 0; i < R; i++) {
            M[k][i] = (int *)malloc(C * sizeof(int));
        }
    }

    /* --- B. POPOLAMENTO DATI (HARDCODED) --- 
     * Inseriamo i dati manualmente per replicare la tua immagine esattamente.
     */
    
    // Livello 0 (Alto-Sinistra): Inizia con 18
    int dati_L0[3][3] = {{18,19,20}, {9,10,11}, {0,1,2}};
    
    // Livello 1 (Alto-Destra): Inizia con 12
    int dati_L1[3][3] = {{12,13,14}, {6,7,8}, {0,1,2}};
    
    // Livello 2 (Basso-Sinistra): Inizia con 6
    int dati_L2[3][3] = {{6,7,8}, {3,4,5}, {0,1,2}}; // Nota: nell'img c'è un blocco che inizia con 6 qui
    
    // Livello 3 (Basso-Destra): Inizia con 0
    int dati_L3[3][3] = {{0,1,2}, {0,1,2}, {0,1,2}};

    // Copia nei livelli 3D
    for(int r=0; r<3; r++) {
        for(int c=0; c<3; c++) {
            M[0][r][c] = dati_L0[r][c];
            M[1][r][c] = dati_L1[r][c];
            M[2][r][c] = dati_L2[r][c];
            M[3][r][c] = dati_L3[r][c];
        }
    }

    /* --- C. ESECUZIONE --- */
    int **risultato = flatten(M, D, R, C);

    /* --- D. STAMPA E VERIFICA --- */
    if (risultato != NULL) {
        printf("Matrice generata:\n");
        print_matrix(risultato, D * R, D * C);
        
        // Pulizia output
        free_matrix_2d(risultato, D * R);
    }

    // Pulizia input
    free_matrix_3d(M, num_livelli, R);

    return 0;
}