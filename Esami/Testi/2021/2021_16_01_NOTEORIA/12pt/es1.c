#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Utile per gestire i valori minimi/massimi degli interi

/* ============================================================================
 * FUNZIONE: f
 * ============================================================================
 * SCOPO:
 * 1. Trovare l'elemento massimo assoluto all'interno della matrice M.
 * 2. Identificare tutte le sottomatrici quadrate possibili.
 * 3. Calcolare la somma degli elementi di ogni sottomatrice quadrata.
 * 4. Contare quante di queste somme sono strettamente minori del massimo trovato al punto 1.
 *
 * PARAMETRI:
 * - int **M : Puntatore doppio (matrice dinamica). Punta a un array di puntatori alle righe.
 * - int r   : Numero totale di righe della matrice.
 * - int c   : Numero totale di colonne della matrice.
 *
 * CONCETTI CHIAVE:
 * - Matrice come int **: In C, una matrice allocata dinamicamente è spesso gestita
 * come un vettore di puntatori. M[i] è l'indirizzo della riga i-esima.
 * - Sottomatrici Quadrate: Una sottomatrice è definita dal suo angolo in alto a sinistra (i, j)
 * e dalla sua dimensione (lato). Dobbiamo iterare su tutte le posizioni possibili e,
 * per ogni posizione, su tutte le dimensioni possibili che non escano dai bordi.
 * ============================================================================
 */
int f(int **M, int r, int c) {

    /* --- 1. VALIDAZIONE INIZIALE --- */
    // Controllo difensivo: se la matrice è vuota o i puntatori sono nulli, ritorno 0.
    if (M == NULL || r <= 0 || c <= 0) {
        return 0;
    }

    /* --- 2. RICERCA DEL MASSIMO ELEMENTO --- */
    // Inizializziamo il massimo col primo elemento.
    // NOTA: Non usare 0, perché la matrice potrebbe contenere solo numeri negativi!
    int max_val = M[0][0];

    // Scorro tutte le righe
    for (int i = 0; i < r; i++) {
        // Scorro tutte le colonne
        for (int j = 0; j < c; j++) {
            // Sintassi: M[i][j] accede al valore intero alla riga i, colonna j.
            if (M[i][j] > max_val) {
                max_val = M[i][j]; // Aggiorno il massimo se trovo un valore più grande
            }
        }
    }

    /* --- 3. ANALISI DELLE SOTTOMATRICI QUADRATE --- */
    int count = 0; // Contatore per le sottomatrici che soddisfano la condizione

    // Iteriamo su ogni possibile "angolo in alto a sinistra" della sottomatrice.
    // 'start_row' è la riga di partenza, 'start_col' è la colonna di partenza.
    for (int start_row = 0; start_row < r; start_row++) {
        for (int start_col = 0; start_col < c; start_col++) {

            // Ora dobbiamo decidere la dimensione (lato) della sottomatrice quadrata.
            // Una sottomatrice quadrata ha lato 'dim'.
            // Il lato minimo è 1.
            // Il lato massimo è limitato dai bordi della matrice principale.
            // Non possiamo andare oltre (r - start_row) né oltre (c - start_col).
            int max_dim_r = r - start_row;
            int max_dim_c = c - start_col;
            
            // Operatore ternario: prendo il più piccolo tra i due limiti per garantire la forma quadrata.
            int max_dim = (max_dim_r < max_dim_c) ? max_dim_r : max_dim_c;

            // Loop per far crescere la dimensione del quadrato da 1 fino al massimo possibile
            for (int dim = 1; dim <= max_dim; dim++) {

                /* --- 4. CALCOLO SOMMA SOTTOMATRICE --- */
                int current_sum = 0;

                // Loop interni per sommare gli elementi del quadrato corrente
                // Le righe vanno da start_row a start_row + dim - 1
                for (int i = 0; i < dim; i++) {
                    for (int j = 0; j < dim; j++) {
                        // Accesso alla cella relativa alla sottomatrice
                        current_sum += M[start_row + i][start_col + j];
                    }
                }

                /* --- 5. VERIFICA CONDIZIONE --- */
                // Se la somma è minore del massimo globale trovato all'inizio
                if (current_sum < max_val) {
                    count++; // Incremento il contatore
                }
            }
        }
    }

    return count;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Creare un ambiente controllato per testare la funzione 'f'.
 * Dimostra l'allocazione dinamica di una matrice frastagliata (array di array)
 * e la sua corretta deallocazione.
 * ============================================================================
 */
int main() {
    // Definiamo dimensioni piccole per testare manualmente
    int r = 3;
    int c = 3;

    /* --- 1. ALLOCAZIONE DELLA MATRICE (MALLOC) --- */
    // Passo A: Allocare l'array di puntatori alle righe.
    // sizeof(int*) perché ogni elemento è un puntatore a un intero.
    int **mat = (int **)malloc(r * sizeof(int *));
    if (mat == NULL) {
        printf("Errore malloc righe\n");
        return -1;
    }

    // Passo B: Per ogni riga, allocare l'array di colonne (gli interi veri e propri).
    for (int i = 0; i < r; i++) {
        // sizeof(int) perché qui memorizziamo i numeri veri.
        mat[i] = (int *)malloc(c * sizeof(int));
        if (mat[i] == NULL) {
            // In un programma reale, qui dovremmo liberare le righe precedenti prima di uscire
            printf("Errore malloc colonne\n");
            return -1;
        }
    }

    /* --- 2. POPOLAMENTO DATI (HARDCODED) --- */
    // Esempio:
    //  5  2  1
    //  1  9  1
    //  1  1  1
    //
    // Il Massimo è 9.
    // Sottomatrici quadrate:
    // Dim 1: 5, 2, 1, 1, 9, 1, 1, 1, 1 -> Tutti < 9 tranne il 9 stesso (8 sottomatrici valide)
    // Dim 2 (esempi):
    //   Top-Left (0,0): 5+2+1+9 = 17 (NO, > 9)
    //   ... e così via.
    
    mat[0][0] = 5; mat[0][1] = 2; mat[0][2] = 1;
    mat[1][0] = 1; mat[1][1] = 9; mat[1][2] = 1;
    mat[2][0] = 1; mat[2][1] = 1; mat[2][2] = 1;

    printf("--- MATRICE ---\n");
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    int risultato = f(mat, r, c);

    /* --- 4. OUTPUT RISULTATI --- */
    printf("\n--- RISULTATO ---\n");
    printf("Numero di sottomatrici quadrate con somma < max: %d\n", risultato);

    /* --- 5. GESTIONE MEMORIA (FREE) --- */
    // Regola d'oro: Libera in ordine INVERSO rispetto all'allocazione.
    
    // Prima libero le singole righe (gli array di int)
    for (int i = 0; i < r; i++) {
        free(mat[i]); // Libero la memoria puntata da mat[i]
    }

    // Poi libero l'array dei puntatori (la "colonna vertebrale" della matrice)
    free(mat); // Libero il puntatore doppio

    return 0;
}