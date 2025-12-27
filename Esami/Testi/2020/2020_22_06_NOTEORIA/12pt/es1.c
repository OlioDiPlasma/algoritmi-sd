#include <stdio.h>
#include <stdlib.h>

/* =================================================================================
 * FUNZIONE AUSILIARIA: check_square
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Verifica se due sottomatrici quadrate di dimensione kxk sono identiche.
 * Questa funzione serve a mantenere pulito il codice principale.
 *
 * PARAMETRI:
 * - int **M1: Prima matrice.
 * - int r1_start, c1_start: Coordinate angolo in alto a sinistra in M1.
 * - int **M2: Seconda matrice.
 * - int r2_start, c2_start: Coordinate angolo in alto a sinistra in M2.
 * - int k: Dimensione del lato del quadrato da controllare.
 *
 * RITORNA:
 * 1 (Vero) se sono identiche, 0 (Falso) altrimenti.
 * =================================================================================
 */
int check_square(int **M1, int r1_start, int c1_start, 
                 int **M2, int r2_start, int c2_start, int k) {
    
    // Scorriamo le righe del quadrato (da 0 a k-1)
    for (int i = 0; i < k; i++) {
        // Scorriamo le colonne del quadrato
        for (int j = 0; j < k; j++) {
            // Confrontiamo le celle corrispondenti.
            // Attenzione all'aritmetica degli indici: 
            // sommiamo l'offset di partenza (start) all'indice corrente (i o j).
            if (M1[r1_start + i][c1_start + j] != M2[r2_start + i][c2_start + j]) {
                return 0; // Trovata una differenza: non sono uguali.
            }
        }
    }
    return 1; // Tutto uguale.
}

/* =================================================================================
 * FUNZIONE PRINCIPALE: subMat
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Trova la più grande sottomatrice quadrata comune a M1 e M2.
 *
 * PARAMETRI:
 * - int **M1, int r1, int c1: Prima matrice e sue dimensioni.
 * - int **M2, int r2, int c2: Seconda matrice e sue dimensioni.
 * - int *dim: Puntatore a intero per "restituire" al main la dimensione trovata (passaggio per riferimento).
 *
 * RITORNA:
 * Un puntatore doppio (int **) alla nuova matrice allocata contenente il risultato.
 * Restituisce NULL se non trova nulla (o dimensione 0).
 *
 * CONCETTI CHIAVE:
 * 1. Strategia "Greedy" sui loop: Partiamo dalla dimensione 'k' più grande possibile
 * e scendiamo verso 1. In questo modo, appena troviamo un match, sappiamo già
 * che è il "più grande" e possiamo uscire dalla funzione (return immediato).
 * 2. Allocazione del Risultato: La matrice 'res' deve essere allocata con malloc
 * perché deve sopravvivere alla fine della funzione per essere usata nel main.
 * =================================================================================
 */
int **subMat(int **M1, int r1, int c1, int **M2, int r2, int c2, int *dim) {
    
    /* --- 1. DETERMINAZIONE DIMENSIONE MASSIMA --- */
    // La sottomatrice quadrata non può essere più grande del lato più piccolo 
    // tra tutte le dimensioni di M1 e M2.
    int max_k = r1;
    if (c1 < max_k) max_k = c1;
    if (r2 < max_k) max_k = r2;
    if (c2 < max_k) max_k = c2;

    /* --- 2. CICLO SULLA DIMENSIONE (Dal più grande al più piccolo) --- */
    for (int k = max_k; k >= 1; k--) {
        
        /* --- 3. SCANSIONE MATRICE M1 --- */
        // Cerchiamo tutti i possibili quadrati di lato k in M1.
        // I limiti dei cicli sono r1-k e c1-k per non uscire dai bordi.
        for (int i = 0; i <= r1 - k; i++) {
            for (int j = 0; j <= c1 - k; j++) {
                
                /* --- 4. SCANSIONE MATRICE M2 --- */
                // Per ogni quadrato candidato in M1, controlliamo se esiste in M2.
                for (int p = 0; p <= r2 - k; p++) {
                    for (int q = 0; q <= c2 - k; q++) {
                        
                        // Chiamata alla funzione helper per il confronto
                        if (check_square(M1, i, j, M2, p, q, k)) {
                            
                            /* --- 5. MATCH TROVATO! ALLOCAZIONE --- */
                            // Se siamo qui, abbiamo trovato il quadrato comune più grande.
                            *dim = k; // Salviamo la dimensione per il main

                            // Allocazione array di puntatori (righe)
                            int **res = (int **)malloc(k * sizeof(int *));
                            if (res == NULL) exit(1);

                            // Allocazione e copia dei dati
                            for (int row = 0; row < k; row++) {
                                res[row] = (int *)malloc(k * sizeof(int));
                                for (int col = 0; col < k; col++) {
                                    // Copiamo i dati da M1 (o M2, sono uguali)
                                    res[row][col] = M1[i + row][j + col];
                                }
                            }
                            
                            return res; // Ritorniamo il puntatore alla nuova matrice
                        }
                    }
                }
            }
        }
    }

    // Caso base: Nessuna sottomatrice comune trovata
    *dim = 0;
    return NULL;
}

/* =================================================================================
 * FUNZIONI UTILI PER IL TEST (Allocazione/Deallocazione matrice)
 * =================================================================================
 */
int **allocate_matrix(int r, int c) {
    int **mat = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        mat[i] = (int *)malloc(c * sizeof(int));
    }
    return mat;
}

void free_matrix(int **mat, int r) {
    if (mat == NULL) return;
    for (int i = 0; i < r; i++) {
        free(mat[i]); // Libero le righe
    }
    free(mat); // Libero il vettore di puntatori
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Riprodurre l'esempio dell'immagine (M1 4x4, M2 3x4) e verificare il risultato.
 * =================================================================================
 */
int main() {
    printf("--- TEST SUBMAT ---\n");

    /* DATI DELL'ESEMPIO */
    int r1 = 4, c1 = 4;
    int r2 = 3, c2 = 4;

    int **M1 = allocate_matrix(r1, c1);
    int **M2 = allocate_matrix(r2, c2);

    // Popolamento M1 (1 2 3 4 / 1 1 3 4 / 1 2 3 4 / 1 2 4 4)
    int data1[4][4] = {
        {1, 2, 3, 4},
        {1, 1, 3, 4},
        {1, 2, 3, 4},
        {1, 2, 4, 4}
    };
    
    // Popolamento M2 (1 9 3 5 / 9 2 3 5 / 1 2 4 5)
    int data2[3][4] = {
        {1, 9, 3, 5},
        {9, 2, 3, 5},
        {1, 2, 4, 5}
    };

    // Copia dai dati statici a quelli dinamici
    for(int i=0; i<r1; i++) for(int j=0; j<c1; j++) M1[i][j] = data1[i][j];
    for(int i=0; i<r2; i++) for(int j=0; j<c2; j++) M2[i][j] = data2[i][j];

    /* CHIAMATA FUNZIONE */
    int dim_res;
    int **res = subMat(M1, r1, c1, M2, r2, c2, &dim_res);

    /* STAMPA RISULTATI */
    if (res != NULL) {
        printf("Sottomatrice comune massima trovata (dim: %d):\n", dim_res);
        for(int i=0; i<dim_res; i++) {
            printf("| ");
            for(int j=0; j<dim_res; j++) {
                printf("%d ", res[i][j]);
            }
            printf("|\n");
        }
        printf("Atteso dall'esempio:\n| 2 3 |\n| 2 4 |\n");
    } else {
        printf("Nessuna sottomatrice comune trovata.\n");
    }

    /* FREE MEMORY */
    free_matrix(M1, r1);
    free_matrix(M2, r2);
    free_matrix(res, dim_res); // Importante: liberare anche il risultato!

    return 0;
}