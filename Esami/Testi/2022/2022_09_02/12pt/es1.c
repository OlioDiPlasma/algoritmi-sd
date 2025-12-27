#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: f
 * ============================================================================
 *
 * SCOPO:
 * Genera una nuova matrice M' derivata da M, mantenendo solo gli elementi
 * che si trovano all'incrocio di righe pari e colonne pari (indici 0, 2, 4...).
 *
 * PARAMETRI:
 * - int **M:          Puntatore alla matrice originale (input).
 * - int r:            Numero di righe della matrice originale (input).
 * - int c:            Numero di colonne della matrice originale (input).
 * - int ***M_prime:   INDIRIZZO della variabile puntatore che conterrà la nuova
 * matrice nel main. Usiamo un triplo puntatore per poter
 * modificare il puntatore originale del chiamante.
 * - int *r_prime:     Puntatore per restituire il numero di righe della nuova matrice.
 * - int *c_prime:     Puntatore per restituire il numero di colonne della nuova matrice.
 *
 * CONCETTI CHIAVE:
 * 1. Triplo Puntatore (int ***): Poiché dobbiamo allocare la memoria dentro la
 * funzione e far sì che il 'main' veda questa nuova memoria, non basta
 * restituire la matrice (la funzione è void), dobbiamo scrivere all'indirizzo
 * del puntatore matrice del main.
 * 2. Calcolo dimensioni: Se abbiamo N elementi, quanti sono pari?
 * La formula è (N + 1) / 2. Esempio: su 3 elementi (0,1,2), i pari sono 2 (0,2).
 * (3+1)/2 = 2. Corretto.
 * ============================================================================
 */
void f(int **M, int r, int c, int ***M_prime, int *r_prime, int *c_prime) {

    /* --- 1. CALCOLO DELLE NUOVE DIMENSIONI --- */
    /*
     * Calcoliamo quante righe e colonne avrà la nuova matrice.
     * Divisione intera: (5 + 1) / 2 = 3. Indici: 0, 2, 4.
     */
    int nuovi_r = (r + 1) / 2;
    int nuovi_c = (c + 1) / 2;

    /*
     * Salviamo queste dimensioni nelle variabili puntate dai parametri,
     * così il main saprà quanto è grande la nuova matrice.
     * SINTASSI: *r_prime accede alla cella di memoria puntata da r_prime.
     */
    *r_prime = nuovi_r;
    *c_prime = nuovi_c;

    /* --- 2. ALLOCAZIONE DELLA MATRICE DI PUNTATORI (RIGHE) --- */
    /*
     * Allocazione dell'array di puntatori alle righe.
     * ATTENZIONE ALLA SINTASSI:
     * (*M_prime) dereferenzia il triplo puntatore. Stiamo assegnando l'indirizzo
     * restituito da malloc alla variabile 'int **' che vive nel main.
     */
    *M_prime = (int **)malloc(nuovi_r * sizeof(int *));
    
    /* Controllo se l'allocazione è fallita */
    if (*M_prime == NULL) {
        printf("Errore: Memoria insufficiente per le righe.\n");
        exit(1);
    }

    /* --- 3. ALLOCAZIONE DELLE COLONNE E COPIA DATI --- */
    /*
     * Usiamo due set di indici:
     * - i, j: per scorrere la matrice ORIGINALE (M).
     * - new_i, new_j: per riempire la NUOVA matrice (M_prime).
     */
    int new_i = 0; 
    
    /* Iteriamo sulla matrice originale saltando di 2 (i += 2) per prendere solo righe pari */
    for (int i = 0; i < r; i += 2) {
        
        /*
         * Allocazione della singola riga corrente nella nuova matrice.
         * SINTASSI: (*M_prime)[new_i]
         * Le parentesi sono vitali! Senza, l'operatore [] avrebbe precedenza su *.
         * Stiamo dicendo: "Vai alla matrice puntata (*M_prime), e prendi l'elemento [new_i]".
         */
        (*M_prime)[new_i] = (int *)malloc(nuovi_c * sizeof(int));
        
        if ((*M_prime)[new_i] == NULL) {
            printf("Errore: Memoria insufficiente per le colonne.\n");
            exit(1);
        }

        int new_j = 0;
        /* Iteriamo sulle colonne originali saltando di 2 */
        for (int j = 0; j < c; j += 2) {
            
            /*
             * COPIA DEL VALORE
             * Preleviamo M[i][j] (originale) e lo mettiamo nella nuova cella.
             */
            (*M_prime)[new_i][new_j] = M[i][j];
            
            new_j++; /* Incrementiamo l'indice colonna della nuova matrice */
        }
        
        new_i++; /* Incrementiamo l'indice riga della nuova matrice */
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Creare i dati come nell'esempio, chiamare f, stampare e pulire.
 */
int main() {
    /* --- 1. SETUP DATI ESEMPIO (Hardcoded come da traccia) --- */
    int r = 3;
    int c = 4;
    
    /*
     * Alloco dinamicamente M per simulare un contesto reale.
     * M è un int**
     */
    int **M = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        M[i] = (int *)malloc(c * sizeof(int));
    }

    /* Riempimento manuale come da esempio nell'immagine */
    /* Riga 0: 1 2 3 4 */
    M[0][0] = 1; M[0][1] = 2; M[0][2] = 3; M[0][3] = 4;
    /* Riga 1: 5 6 7 8 */
    M[1][0] = 5; M[1][1] = 6; M[1][2] = 7; M[1][3] = 8;
    /* Riga 2: 9 0 1 2 */
    M[2][0] = 9; M[2][1] = 0; M[2][2] = 1; M[2][3] = 2;

    printf("--- Matrice Originale M (%dx%d) ---\n", r, c);
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }

    /* --- 2. PREPARAZIONE VARIABILI OUTPUT --- */
    int **M_nuova = NULL; // Questa variabile verrà modificata dalla funzione f
    int r_nuova, c_nuova;

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    /*
     * NOTA CRUCIALE: Passiamo &M_nuova.
     * M_nuova è di tipo int**, quindi &M_nuova è di tipo int***.
     * Questo soddisfa il prototipo della funzione.
     */
    f(M, r, c, &M_nuova, &r_nuova, &c_nuova);

    /* --- 4. STAMPA RISULTATI --- */
    printf("\n--- Matrice Derivata M' (%dx%d) ---\n", r_nuova, c_nuova);
    for(int i=0; i<r_nuova; i++) {
        for(int j=0; j<c_nuova; j++) {
            printf("%d ", M_nuova[i][j]);
        }
        printf("\n");
    }

    /* --- 5. GESTIONE MEMORIA (FREE) --- */
    /* È fondamentale liberare tutta la memoria allocata per evitare memory leaks */
    
    /* Libero M originale */
    for(int i=0; i<r; i++) free(M[i]);
    free(M);

    /* Libero M_nuova creata dalla funzione f */
    for(int i=0; i<r_nuova; i++) free(M_nuova[i]);
    free(M_nuova);

    return 0;
}