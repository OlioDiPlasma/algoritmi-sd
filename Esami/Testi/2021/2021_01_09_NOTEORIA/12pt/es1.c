#include <stdio.h>
#include <stdlib.h>

/* =========================================================================
 * INTESTAZIONE FUNZIONE
 * =========================================================================
 * SCOPO:
 * Genera una matrice di dimensioni d1 x d2 dove ogni cella [i][j]
 * contiene il prodotto: v1[i] * v2[j].
 * La matrice viene allocata dinamicamente all'interno della funzione.
 *
 * PARAMETRI:
 * - v1: Puntatore al primo array (vettore colonna virtuale).
 * - v2: Puntatore al secondo array (vettore riga virtuale).
 * - d1: Dimensione del primo array (numero di righe della matrice risultante).
 * - d2: Dimensione del secondo array (numero di colonne della matrice risultante).
 * - matrice_out: Triplo puntatore (indirizzo della variabile matrice nel main).
 *
 * CONCETTI CHIAVE (Il "perché" tecnico):
 * Perché 'int ***'?
 * Nel main definiremo 'int **M'. Vogliamo che la funzione 'f' usi malloc
 * e assegni il nuovo indirizzo a 'M'. In C, i parametri sono passati per valore
 * (copia). Se passassimo solo 'int **', la funzione modificherebbe solo
 * una copia locale del puntatore.
 * Per modificare il puntatore originale 'M' che risiede nel main, dobbiamo
 * passare il suo indirizzo (&M). L'indirizzo di un 'int **' è un 'int ***'.
 * ========================================================================= */

void f(int *v1, int *v2, int d1, int d2, int ***matrice_out) {

    /* --- 1. ALLOCAZIONE DELLE RIGHE (Vettore di puntatori) --- */

    /*
     * SPIEGAZIONE SINTASSI:
     * *matrice_out: Dereferenziamo il triplo puntatore una volta per accedere
     * alla variabile 'M' del main. Stiamo scrivendo dentro la variabile del chiamante.
     *
     * malloc(d1 * sizeof(int *)): Chiediamo al sistema memoria per 'd1' puntatori.
     * Usiamo 'sizeof(int *)' perché questo array conterrà indirizzi di memoria,
     * non interi semplici.
     */
    *matrice_out = (int **)malloc(d1 * sizeof(int *));

    /* Controllo robustezza: se la malloc fallisce, terminiamo o gestiamo l'errore */
    if (*matrice_out == NULL) {
        printf("Errore: Memoria insufficiente per le righe.\n");
        exit(1);
    }

    /* --- 2. ALLOCAZIONE DELLE COLONNE E LOGICA DI CALCOLO --- */

    for (int i = 0; i < d1; i++) {
        /*
         * ACCESSO ALLA MEMORIA:
         * (*matrice_out)[i]:
         * 1. (*matrice_out) -> prende il puntatore base alla matrice.
         * 2. [i] -> accede all'i-esimo puntatore di riga.
         *
         * Stiamo allocando un array di 'd2' interi per ogni riga.
         */
        (*matrice_out)[i] = (int *)malloc(d2 * sizeof(int));

        /* Controllo errore per la singola riga */
        if ((*matrice_out)[i] == NULL) {
            printf("Errore allocazione riga %d\n", i);
            exit(1);
            /* Nota: in un codice di produzione dovremmo liberare le righe
               precedentemente allocate prima di uscire (rollback). */
        }

        /* --- 3. RIEMPIMENTO CELLE (Il prodotto) --- */
        for (int j = 0; j < d2; j++) {
            /*
             * LOGICA MATEMATICA:
             * Cella [i][j] = elemento i di v1 * elemento j di v2.
             *
             * SINTASSI:
             * Possiamo usare la sintassi array standard sulle variabili dereferenziate.
             * (*matrice_out)[i][j] è equivalente a *(*(*matrice_out + i) + j).
             * Le parentesi (*matrice_out) sono obbligatorie per la precedenza degli operatori.
             */
            (*matrice_out)[i][j] = v1[i] * v2[j];
        }
    }
}

/* =========================================================================
 * MAIN DI TEST
 * ========================================================================= */
int main() {
    /* Dati dell'esempio nell'immagine */
    int v1[] = {2, 4, 6};
    int v2[] = {1, 3, 5, 7};

    /* Calcolo delle dimensioni (sizeof array / sizeof singolo elemento) */
    int d1 = sizeof(v1) / sizeof(v1[0]); // 3
    int d2 = sizeof(v2) / sizeof(v2[0]); // 4

    /*
     * Variabile per la matrice.
     * Inizializziamo a NULL per sicurezza.
     * M è un 'int **' (puntatore a puntatore).
     */
    int **M = NULL;

    printf("--- INIZIO PROGRAMMA ---\n");

    /*
     * CHIAMATA ALLA FUNZIONE
     * Passiamo &M (l'indirizzo di M).
     * Poiché M è 'int **', &M è di tipo 'int ***'.
     * Questo soddisfa il prototipo richiesto.
     */
    f(v1, v2, d1, d2, &M);

    /* --- STAMPA DEI RISULTATI --- */
    printf("Matrice Risultante (%d x %d):\n", d1, d2);
    for (int i = 0; i < d1; i++) {
        printf("[\t"); // Tabulazione estetica
        for (int j = 0; j < d2; j++) {
            /* Accesso normale alla matrice M ora che è stata allocata */
            printf("%d\t", M[i][j]);
        }
        printf("]\n");
    }

    /* --- GESTIONE MEMORIA (PULIZIA) --- */
    /*
     * Regola d'oro: Per ogni malloc deve esserci una free.
     * Dobbiamo liberare prima le "foglie" (le singole righe)
     * e poi il "tronco" (il vettore dei puntatori).
     */
    printf("\nLiberazione memoria in corso...\n");
    for (int i = 0; i < d1; i++) {
        free(M[i]); // Libero la riga i-esima (array di int)
    }
    free(M);        // Libero il vettore dei puntatori
    M = NULL;       // Buona norma: evitare dangling pointers

    printf("Memoria liberata. Fine.\n");

    return 0;
}