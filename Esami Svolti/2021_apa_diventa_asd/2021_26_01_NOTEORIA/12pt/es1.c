#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: change
 * ============================================================================
 * SCOPO:
 * Modifica la matrice M secondo la regola: se una cella (i,j) contiene inizialmente
 * un valore divisibile per 3, l'intera riga i e l'intera colonna j vengono
 * sovrascritte con il valore 3.
 *
 * PARAMETRI:
 * - int **M : La matrice di interi (puntatore a puntatori).
 * - int r   : Numero di righe.
 * - int c   : Numero di colonne.
 *
 * CONCETTI CHIAVE:
 * - "Valore Iniziale": Il testo specifica che la condizione si basa sul valore
 * presente PRIMA delle modifiche. Se modificassimo la matrice "in-place"
 * (sul posto) durante il controllo, perderemmo l'informazione originale.
 * Esempio: M[0][0] è 1 (no div 3). M[0][1] è 3. Questo fa diventare M[0][0] = 3.
 * Se poi controllassimo M[0][0] e vedessimo 3, penseremmo erroneamente di dover
 * cambiare tutta la colonna 0!
 * - Array di Flag (Supporto): Per risolvere questo, usiamo due array temporanei
 * per "ricordare" quali righe e quali colonne devono essere cambiate.
 * ============================================================================
 */
void change(int **M, int r, int c) {

    /* --- 1. ALLOCAZIONE MEMORIA AUSILIARIA --- */
    /* Usiamo calloc invece di malloc.
     * calloc(numero_elementi, dimensione_elemento) alloca la memoria E la azzera.
     * Questo è fondamentale perché usiamo questi array come booleani (0 = false, 1 = true).
     * Se usassimo malloc, conterrebbero valori "spazzatura".
     */
    int *righe_da_cambiare = (int *)calloc(r, sizeof(int));
    int *colonne_da_cambiare = (int *)calloc(c, sizeof(int));

    // Controllo difensivo sull'allocazione
    if (righe_da_cambiare == NULL || colonne_da_cambiare == NULL) {
        printf("Errore di allocazione memoria ausiliaria.\n");
        exit(1); // Uscita forzata in caso di errore grave
    }

    /* --- 2. LOGICA: SCANSIONE (READ-ONLY) --- */
    // In questa fase leggiamo solo la matrice per identificare i "colpevoli" (multipli di 3).
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            
            // Accesso alla matrice: M[i][j] dereferenzia due volte.
            // Operatore modulo (%): restituisce il resto della divisione.
            if (M[i][j] % 3 == 0) {
                // Trovato un multiplo di 3! Segniamo la riga e la colonna corrispondenti.
                // Non modifichiamo M ora, aggiorniamo solo i nostri "appunti".
                righe_da_cambiare[i] = 1;
                colonne_da_cambiare[j] = 1;
            }
        }
    }

    /* --- 3. LOGICA: APPLICAZIONE MODIFICHE (WRITE) --- */
    // Ora ripercorriamo la matrice e applichiamo le modifiche basandoci sui flag.
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            
            /* Dettaglio Logico:
             * Una cella (i,j) diventa 3 se:
             * - La sua riga (i) è stata marcata (righe_da_cambiare[i] == 1)
             * - OPPURE (||)
             * - La sua colonna (j) è stata marcata (colonne_da_cambiare[j] == 1)
             */
            if (righe_da_cambiare[i] == 1 || colonne_da_cambiare[j] == 1) {
                M[i][j] = 3; // Assegnazione distruttiva
            }
        }
    }

    /* --- 4. GESTIONE MEMORIA (CLEANUP) --- */
    /* Fondamentale: le variabili locali puntatore (righe_da_cambiare) muoiono
     * alla fine della funzione, ma la memoria che puntano (nell'Heap) NO.
     * Dobbiamo liberarla esplicitamente per evitare Memory Leaks.
     */
    free(righe_da_cambiare);
    free(colonne_da_cambiare);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Creare la matrice dell'esempio (3x4), popolare i dati, chiamare change()
 * e stampare il risultato. Dimostra l'uso corretto di malloc/free per matrici.
 * ============================================================================
 */
int main() {
    int r = 3; // Righe
    int c = 4; // Colonne

    /* --- 1. ALLOCAZIONE MATRICE DINAMICA --- */
    // Passo A: Alloco il vettore dei puntatori alle righe (spina dorsale)
    int **mat = (int **)malloc(r * sizeof(int *));
    
    // Passo B: Per ogni riga, alloco il vettore delle colonne
    for (int i = 0; i < r; i++) {
        mat[i] = (int *)malloc(c * sizeof(int));
    }

    /* --- 2. POPOLAMENTO DATI (Esempio del testo) --- */
    // Riga 0: 1 1 3 0
    mat[0][0] = 1; mat[0][1] = 1; mat[0][2] = 3; mat[0][3] = 0;
    // Riga 1: 2 4 6 8
    mat[1][0] = 2; mat[1][1] = 4; mat[1][2] = 6; mat[1][3] = 8;
    // Riga 2: 1 2 1 1
    mat[2][0] = 1; mat[2][1] = 2; mat[2][2] = 1; mat[2][3] = 1;

    // Stampa stato iniziale
    printf("--- MATRICE INIZIALE ---\n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    change(mat, r, c);

    /* --- 4. STAMPA RISULTATO --- */
    printf("\n--- MATRICE MODIFICATA ---\n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }

    /* --- 5. LIBERAZIONE MEMORIA MATRICE --- */
    // Regola: free avviene in ordine inverso alla malloc
    for (int i = 0; i < r; i++) {
        free(mat[i]); // Libero le singole righe (array di int)
    }
    free(mat); // Libero l'array dei puntatori

    return 0;
}