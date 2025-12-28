#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: displSquare
 * ============================================================================
 * SCOPO:
 * Identificare e stampare a video tutte le sottomatrici QUADRATE contenute
 * in una matrice rettangolare di partenza.
 *
 * PARAMETRI:
 * - int **A : Puntatore doppio (matrice dinamica). Punta a un array di
 * puntatori, dove ogni puntatore rappresenta una riga.
 * - int n   : Numero totale di righe della matrice A.
 * - int m   : Numero totale di colonne della matrice A.
 *
 * CONCETTI CHIAVE:
 * 1. Dimensione della sottomatrice (dim): Una matrice quadrata è definita
 * dal suo lato. La dimensione minima è 1, la massima è il minimo tra
 * n (righe) e m (colonne).
 * 2. Scorrimento "Finestra Mobile": Dobbiamo far scorrere la nostra
 * sottomatrice quadrata partendo dall'angolo in alto a sinistra.
 * 3. Limiti dei cicli: Se ho una matrice quadrata di lato 'dim', non posso
 * iniziare a stamparla dalla riga n-1, altrimenti "sbordo". L'ultima
 * riga valida di inizio è (n - dim).
 * ============================================================================
 */
void displSquare(int **A, int n, int m) {

    /* --- 1. CALCOLO DIMENSIONE MASSIMA --- */
    /*
     * Una matrice quadrata non può essere più grande del lato più corto
     * della matrice originale.
     * Esempio: se ho 2 righe e 3 colonne, la sottomatrice quadrata massima è 2x2.
     * Usiamo l'operatore ternario per trovare il minimo tra n ed m.
     */
    int max_dim = (n < m) ? n : m;

    /* --- 2. CICLO SULLE DIMENSIONI (dim) --- */
    /*
     * Iteriamo per tutte le possibili dimensioni 'dim'.
     * Partiamo da sottomatrici 1x1, poi 2x2, fino a max_dim x max_dim.
     */
    for (int dim = 1; dim <= max_dim; dim++) {

        printf("\n--- Sottomatrici %dx%d ---\n", dim, dim);

        /* --- 3. CICLO SULLA RIGA DI PARTENZA (r) --- */
        /*
         * 'r' è l'indice della riga dell'angolo in alto a sinistra della sottomatrice.
         * Fino a dove può arrivare 'r'?
         * Deve fermarsi quando (r + dim) supera n.
         * Quindi la condizione è: r <= n - dim.
         */
        for (int r = 0; r <= n - dim; r++) {

            /* --- 4. CICLO SULLA COLONNA DI PARTENZA (c) --- */
            /*
             * 'c' è l'indice della colonna dell'angolo in alto a sinistra.
             * Analogamente alle righe: c <= m - dim.
             */
            for (int c = 0; c <= m - dim; c++) {

                /*
                 * Arrivati qui, abbiamo definito una sottomatrice che:
                 * - Inizia alla riga 'r', colonna 'c'.
                 * - Ha dimensione 'dim' x 'dim'.
                 * Ora dobbiamo solo STAMPARLA.
                 */
                printf("Matrice in pos [%d][%d]:\n", r, c);

                /* --- 5. STAMPA DELLA SOTTOMATRICE --- */
                /*
                 * Usiamo due indici relativi 'i' e 'j' che vanno da 0 a dim-1.
                 * Per accedere all'elemento reale in A, sommiamo l'offset di partenza:
                 * A[r + i][c + j]
                 */
                for (int i = 0; i < dim; i++) {
                    for (int j = 0; j < dim; j++) {
                        /*
                         * DETTAGLIO SINTATTICO:
                         * A[r+i] -> accede al puntatore della riga specifica.
                         * [c+j]  -> dereferenzia quel puntatore alla posizione corretta
                         * per ottenere l'intero (int).
                         */
                        printf("%d ", A[r + i][c + j]);
                    }
                    printf("\n"); // A capo alla fine di ogni riga della sottomatrice
                }
                printf("\n"); // Spaziatura tra una matrice e l'altra
            }
        }
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Creare l'ambiente per testare la funzione.
 * Poiché il prototipo richiede int **A, NON possiamo passare una matrice
 * statica (es. int A[2][3]) direttamente, perché in C array statici 2D e
 * doppi puntatori non sono la stessa cosa in memoria.
 * Dobbiamo simulare una matrice dinamica con malloc.
 * ============================================================================
 */
int main() {
    /* Dati dell'esempio nell'immagine */
    int n = 2; // Righe
    int m = 3; // Colonne

    /* --- 1. ALLOCAZIONE DELLA MATRICE (Gestione Memoria) --- */
    printf("Allocazione matrice %dx%d...\n", n, m);

    /*
     * Passo A: Alloco l'array di puntatori alle righe.
     * sizeof(int*) è fondamentale perché stiamo allocando PUNTATORI, non interi.
     */
    int **matrice = (int **)malloc(n * sizeof(int *));
    if (matrice == NULL) {
        perror("Errore malloc righe");
        return 1;
    }

    /*
     * Passo B: Per ogni riga, alloco l'array di interi (le colonne).
     * Qui uso sizeof(int) perché ora sto allocando spazio per i numeri veri.
     */
    for (int i = 0; i < n; i++) {
        matrice[i] = (int *)malloc(m * sizeof(int));
        if (matrice[i] == NULL) {
            perror("Errore malloc colonne");
            // Nota: in un codice di produzione dovremmo liberare le righe precedenti
            return 1;
        }
    }

    /* --- 2. INIZIALIZZAZIONE DATI --- */
    /*
     * Riempiamo la matrice come nell'esempio dell'immagine:
     * 0 1 2
     * 3 4 5
     */
    int contatore = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrice[i][j] = contatore++;
        }
    }

    /* Visualizziamo la matrice intera per chiarezza */
    printf("Matrice Originale:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    printf("\n--- Inizio Analisi Sottomatrici ---\n");
    displSquare(matrice, n, m);

    /* --- 4. DEALLOCAZIONE (IMPORTANTE!) --- */
    /*
     * La regola d'oro: l'ordine di 'free' è inverso rispetto a 'malloc'.
     * Prima libero il contenuto (le singole righe), poi il contenitore (l'array di puntatori).
     */
    for (int i = 0; i < n; i++) {
        free(matrice[i]); // Libero la riga i-esima
    }
    free(matrice); // Libero l'array di puntatori principale
    printf("\nMemoria liberata con successo.\n");

    return 0;
}