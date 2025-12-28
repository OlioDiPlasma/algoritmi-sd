#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Serve per INT_MAX

/* ============================================================================
 * FUNZIONE HELPER: solve (Backtracking / Ricorsione)
 * ============================================================================
 * Scopo:
 * Esplora tutte le combinazioni possibili di numeri per trovare quelli che sommano a k.
 * Aggiorna costantemente la soluzione "più corta" (min) e "più lunga" (max) trovata.
 *
 * Parametri:
 * - arr: il vettore dei numeri (la matrice linearizzata).
 * - n: dimensione totale del vettore (r * c).
 * - k: il target somma da raggiungere.
 * - pos: indice corrente nell'array che stiamo valutando (livello della ricorsione).
 * - curr_sum: somma accumulata finora nel ramo corrente.
 * - curr_sol: array temporaneo che contiene i numeri scelti nel ramo corrente.
 * - curr_len: quanti numeri ci sono in curr_sol.
 * - best_min: array dove salviamo la soluzione di cardinalità minima.
 * - len_min: puntatore alla lunghezza della soluzione minima (inizialmente INT_MAX).
 * - best_max: array dove salviamo la soluzione di cardinalità massima.
 * - len_max: puntatore alla lunghezza della soluzione massima (inizialmente 0).
 *
 * Concetti Chiave:
 * - Modello "Includo / Escludo": Per ogni numero, facciamo due chiamate ricorsive:
 * 1. Lo prendo nel sottoinsieme.
 * 2. Non lo prendo.
 * - Pruning (Potatura): Se `curr_sum > k`, ci fermiamo subito (inutile continuare ad aggiungere numeri positivi).
 * ============================================================================
 */
void solve(int *arr, int n, int k, int pos, int curr_sum, 
           int *curr_sol, int curr_len, 
           int *best_min, int *len_min, 
           int *best_max, int *len_max) {
    
    /* --- 1. CASO BASE: SOMMA RAGGIUNTA --- */
    if (curr_sum == k) {
        /*
         * Abbiamo trovato UN sottoinsieme valido.
         * Ora controlliamo se è un nuovo record (minimo o massimo).
         */

        /* Controllo Cardinalità Minima */
        /* Accedo al valore puntato da len_min per il confronto */
        if (curr_len < *len_min) {
            *len_min = curr_len; // Aggiorno il record di lunghezza
            // Copio il contenuto della soluzione corrente in quella "best"
            for (int i = 0; i < curr_len; i++) {
                best_min[i] = curr_sol[i];
            }
        }

        /* Controllo Cardinalità Massima */
        if (curr_len > *len_max) {
            *len_max = curr_len;
            for (int i = 0; i < curr_len; i++) {
                best_max[i] = curr_sol[i];
            }
        }
        return; // Trovata soluzione su questo ramo, torniamo indietro
    }

    /* --- 2. CASO BASE: FINE ARRAY O SOMMA ECCESSIVA --- */
    /* Se siamo arrivati alla fine dei numeri (pos == n) o abbiamo sforato k */
    if (pos == n || curr_sum > k) {
        return;
    }

    /* --- 3. PASSO RICORSIVO (STRATEGIA INCLUDE/EXCLUDE) --- */
    
    /* OPZIONE A: PRENDO il numero arr[pos] */
    curr_sol[curr_len] = arr[pos]; // Lo salvo nel buffer temporaneo
    solve(arr, n, k, pos + 1, curr_sum + arr[pos], curr_sol, curr_len + 1,
          best_min, len_min, best_max, len_max);

    /* OPZIONE B: NON PRENDO il numero arr[pos] (Lo salto) */
    /* Non modifico curr_sol né curr_len, passo solo al prossimo indice */
    solve(arr, n, k, pos + 1, curr_sum, curr_sol, curr_len,
          best_min, len_min, best_max, len_max);
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: f
 * ============================================================================
 * Scopo:
 * Wrapper che prepara i dati per la ricorsione. Linearizza la matrice e alloca
 * la memoria per salvare i risultati.
 *
 * Parametri:
 * - mat: la matrice di input (doppio puntatore).
 * - r, c: righe e colonne.
 * - k: target somma.
 *
 * Concetti Chiave:
 * - Linearizzazione: Trasformiamo int** in int* per semplificare la vita alla ricorsione.
 * Matrice 2D -> Indice unico i.
 * - Puntatori ai contatori: Passiamo gli indirizzi (&min_len) alla funzione ricorsiva
 * così che possa modificarli permanentemente.
 * ============================================================================
 */
void f(int **mat, int r, int c, int k) {
    int n = r * c; // Numero totale di elementi

    /* --- 1. ALLOCAZIONE E LINEARIZZAZIONE --- */
    /* Creiamo un array lineare per contenere tutti i numeri della matrice */
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) return; 

    /* Copiamo i dati dalla matrice all'array */
    int k_idx = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            /* mat[i][j] accede all'elemento riga i, colonna j */
            arr[k_idx++] = mat[i][j];
        }
    }

    /* --- 2. PREPARAZIONE STRUTTURE PER SOLUZIONI --- */
    /* Alloco array per tenere traccia delle soluzioni. 
       Dimensione massima n (caso pessimo: prendo tutti i numeri). */
    int *curr_sol = (int *)malloc(n * sizeof(int));
    int *best_min = (int *)malloc(n * sizeof(int));
    int *best_max = (int *)malloc(n * sizeof(int));

    /* Inizializzo le lunghezze dei record */
    int len_min = INT_MAX; // Parto da infinito: la prima soluzione sarà sicuramente < infinito
    int len_max = 0;       // Parto da 0

    /* --- 3. CHIAMATA AL MOTORE RICORSIVO --- */
    /* Passo &len_min e &len_max per modificarli dentro solve */
    solve(arr, n, k, 0, 0, curr_sol, 0, best_min, &len_min, best_max, &len_max);

    /* --- 4. STAMPA RISULTATI --- */
    if (len_max == 0 && len_min == INT_MAX) {
        printf("Nessuna soluzione trovata per k=%d\n", k);
    } else {
        /* Stampa cardinalità minima */
        printf("Sottoinsieme cardinalita' minima (dim %d): ", len_min);
        printf("(");
        for (int i = 0; i < len_min; i++) {
            printf("%d", best_min[i]);
            if (i < len_min - 1) printf(", ");
        }
        printf(")\n");

        /* Stampa cardinalità massima */
        printf("Sottoinsieme cardinalita' massima (dim %d): ", len_max);
        printf("(");
        for (int i = 0; i < len_max; i++) {
            printf("%d", best_max[i]);
            if (i < len_max - 1) printf(", ");
        }
        printf(")\n");
    }

    /* --- 5. PULIZIA MEMORIA (CRUCIALE!) --- */
    free(arr);
    free(curr_sol);
    free(best_min);
    free(best_max);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo:
 * Creare la matrice dell'esempio e testare la funzione.
 */
int main() {
    int r = 3, c = 4;
    int k = 10;

    /* Allocazione dinamica matrice (array di puntatori) */
    int **mat = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        mat[i] = (int *)malloc(c * sizeof(int));
    }

    /* Riempimento con i dati dell'esempio nell'immagine */
    /*
       0 1 2 3
     0|2 3 1 2
     1|6 4 2 5
     2|2 4 3 2
    */
    // Riga 0
    mat[0][0] = 2; mat[0][1] = 3; mat[0][2] = 1; mat[0][3] = 2;
    // Riga 1
    mat[1][0] = 6; mat[1][1] = 4; mat[1][2] = 2; mat[1][3] = 5;
    // Riga 2
    mat[2][0] = 2; mat[2][1] = 4; mat[2][2] = 3; mat[2][3] = 2;

    printf("--- INIZIO TEST ---\n");
    printf("Cerco sottoinsiemi con somma %d...\n", k);
    
    f(mat, r, c, k);

    printf("--- FINE TEST ---\n");

    /* Libero la matrice del main */
    for (int i = 0; i < r; i++) free(mat[i]);
    free(mat);

    return 0;
}