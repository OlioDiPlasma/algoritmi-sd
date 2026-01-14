#include "matrix_utils.h"

/*
 * ======================================================================================
 * Funzione: main
 * --------------------------------------------------------------------------------------
 * Scopo:       Testare le funzioni create. 
 * 1. Legge file.
 * 2. Chiama separa().
 * 3. Stampa risultati.
 * 4. Pulisce la memoria.
 * ======================================================================================
 */
int main() {
    int nr, nc;
    int **matrice = NULL;

    /* --- 1. LETTURA E ALLOCAZIONE MATRICE --- */
    printf("--- Inizio Lettura File ---\n");
    matrice = leggi_file("mat.txt", &nr, &nc);
    
    printf("Matrice letta (%d x %d):\n", nr, nc);
    for(int i=0; i<nr; i++) {
        for(int j=0; j<nc; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* --- 2. PREPARAZIONE PER 'SEPARA' --- */
    /* Dichiaro i puntatori per i vettori risultato.
       Inizialmente non puntano a nulla, la malloc verra' fatta DENTRO la funzione separa. */
    int *vettore_bianchi = NULL;
    int *vettore_neri = NULL;
    int dim_b = 0, dim_n = 0;

    /* --- 3. CHIAMATA A 'SEPARA' --- */
    /* Concetto chiave: Passo l'INDIRIZZO dei puntatori (&vettore_bianchi).
       Questo perche' la funzione deve modificare dove questi puntatori "puntano". 
       (Passaggio per riferimento di puntatore) */
    separa(matrice, nr, nc, &vettore_bianchi, &dim_b, &vettore_neri, &dim_n);

    /* --- 4. STAMPA RISULTATI (VERIFICA) --- */
    printf("--- Risultati Separazione ---\n");
    
    printf("Celle Bianche (totale %d): ", dim_b);
    for(int i=0; i<dim_b; i++) {
        printf("%d ", vettore_bianchi[i]);
    }
    printf("\n");

    printf("Celle Nere    (totale %d): ", dim_n);
    for(int i=0; i<dim_n; i++) {
        printf("%d ", vettore_neri[i]);
    }
    printf("\n\n");

    /* --- 5. PULIZIA MEMORIA (OTTIMIZZAZIONE/NO LEAK) --- */
    /* Fondamentale liberare tutto cio' che e' stato allocato con malloc */
    printf("--- Pulizia Memoria ---\n");
    
    free2d(matrice, nr);       // Libera la matrice
    free(vettore_bianchi);     // Libera il vettore dinamico bianchi
    free(vettore_neri);        // Libera il vettore dinamico neri

    printf("Memoria liberata con successo. Fine programma.\n");

    return 0;
}