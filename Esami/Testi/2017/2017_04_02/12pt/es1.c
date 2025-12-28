#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: sommaCornici
 * ======================================================================================
 * SCOPO:
 * Calcola la somma degli elementi situati su ciascuna cornice concentrica di una matrice
 * quadrata N x N (con N dispari).
 * Alloca dinamicamente un vettore per ospitare questi risultati e lo restituisce
 * al chiamante tramite un parametro di output.
 *
 * PARAMETRI:
 * - mat:  (int **) Puntatore doppio che rappresenta la matrice quadrata di input.
 * - N:    (int)    Dimensione della matrice (numero di righe e colonne). Deve essere dispari.
 * - vet:  (int **) Puntatore doppio al vettore risultato.
 * Usiamo un puntatore doppio perché la funzione deve ALLOCARE la memoria
 * e restituire il nuovo indirizzo al main.
 *
 * CONCETTI CHIAVE:
 * 1. Passaggio per Riferimento di un Puntatore (int **vet):
 * In C, le funzioni ricevono i parametri per valore (copia). Se passassimo solo `int *vet`,
 * e facessimo `vet = malloc(...)` dentro la funzione, staremmo modificando solo la copia locale
 * del puntatore. Il `main` non vedrebbe mai la memoria allocata.
 * Passando l'indirizzo del puntatore (`&risultato` dal main, che diventa `int **vet` qui),
 * possiamo dereferenziarlo (`*vet = ...`) per modificare il puntatore originale nel main.
 *
 * 2. Geometria della Matrice:
 * Una matrice N x N ha (N/2 + 1) cornici.
 * Esempio N=5: 5/2 = 2 (divisione intera) + 1 = 3 cornici.
 * Indiciamo le cornici con `k`.
 * - La cornice k=0 è la più esterna.
 * - La cornice k va dall'indice `k` all'indice `N-1-k`.
 * ======================================================================================
 */
void sommaCornici(int **mat, int N, int **vet) {

    /* --- 1. CALCOLO DIMENSIONE E ALLOCAZIONE --- */
    // Calcoliamo quante cornici ci sono. Essendo N dispari, c'è sempre un centro.
    // Esempio: N=5 -> cornici = 3.
    int numeroCornici = (N / 2) + 1;

    // Allocazione del vettore risultato.
    // NOTA SINTATTICA: *vet accede alla variabile puntatore nel chiamante.
    // sizeof(int) ci dà la grandezza di un singolo intero.
    *vet = (int *)malloc(numeroCornici * sizeof(int));

    // Controllo difensivo: se malloc fallisce, interrompiamo o gestiamo l'errore.
    if (*vet == NULL) {
        printf("Errore: Impossibile allocare memoria per il vettore.\n");
        exit(1);
    }

    /* --- 2. LOGICA DI SCANSIONE DELLE CORNICI --- */
    // Iteriamo per ogni cornice k, da 0 (esterna) fino al centro.
    for (int k = 0; k < numeroCornici; k++) {
        
        int sommaCorrente = 0;

        // Definiamo i confini della cornice attuale 'k'.
        // 'start' è l'indice in alto a sinistra.
        // 'end' è l'indice in basso a destra.
        int start = k;
        int end = N - 1 - k;

        /* --- 3. CASO SPECIALE: IL CENTRO --- */
        // Se start e end coincidono, siamo nell'elemento centrale (singola cella).
        if (start == end) {
            // Accediamo direttamente all'elemento centrale.
            sommaCorrente = mat[start][end];
        } 
        else {
            /* --- 4. SOMMA DEI LATI (NON CENTRO) --- */
            
            // A. Somma Riga Superiore e Inferiore
            // Iteriamo lungo le colonne 'j' da start a end.
            for (int j = start; j <= end; j++) {
                sommaCorrente += mat[start][j]; // Elemento riga superiore
                sommaCorrente += mat[end][j];   // Elemento riga inferiore
            }

            // B. Somma Colonna Sinistra e Destra
            // ATTENZIONE: Per non sommare due volte gli angoli, dobbiamo restringere il range.
            // Abbiamo già sommato gli angoli nel ciclo precedente (righe).
            // Quindi qui partiamo da start+1 e arriviamo a end-1 (escluso end).
            for (int i = start + 1; i < end; i++) {
                sommaCorrente += mat[i][start]; // Elemento colonna sinistra
                sommaCorrente += mat[i][end];   // Elemento colonna destra
            }
        }

        /* --- 5. SALVATAGGIO NEL VETTORE --- */
        // Assegniamo la somma calcolata alla posizione k del vettore allocato.
        // NOTA SINTATTICA: (*vet)[k]
        // Le parentesi sono OBBLIGATORIE. L'operatore [] ha priorità su *.
        // Senza parentesi, *vet[k] verrebbe interpretato come *(vet[k]), che è sbagliato.
        // Noi vogliamo: "Prendi il puntatore puntato da vet (*vet), e poi accedi all'indice k".
        (*vet)[k] = sommaCorrente;
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare dati di prova, invocare la funzione e verificare i risultati.
 * Include gestione rigorosa della memoria (allocazione e free).
 * ======================================================================================
 */
int main() {
    int N = 5; // Dimensione dispari come da specifica

    /* --- 1. ALLOCAZIONE MATRICE DI PROVA (Heap) --- */
    // Alloco un array di puntatori alle righe
    int **matrice = (int **)malloc(N * sizeof(int *));
    
    // Per ogni riga, alloco le colonne
    for (int i = 0; i < N; i++) {
        matrice[i] = (int *)malloc(N * sizeof(int));
    }

    /* --- 2. POPOLAMENTO DATI --- */
    // Riempio la matrice con i valori dell'esempio nell'immagine per verifica.
    // Riga 0: 1 2 3 4 5
    int dati[5][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 0},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 0},
        {1, 1, 1, 1, 1}
    };

    printf("--- Matrice Input ---\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrice[i][j] = dati[i][j];
            printf("%2d ", matrice[i][j]);
        }
        printf("\n");
    }

    /* --- 3. PREPARAZIONE CHIAMATA --- */
    int *vettoreRisultato = NULL; // Questo puntatore verrà modificato dalla funzione

    // Passo &vettoreRisultato (indirizzo del puntatore) -> diventa int **vet
    sommaCornici(matrice, N, &vettoreRisultato);

    /* --- 4. STAMPA RISULTATI --- */
    printf("\n--- Risultati (Somme Cornici) ---\n");
    int numeroCornici = (N / 2) + 1;
    
    for (int k = 0; k < numeroCornici; k++) {
        printf("Cornice %d (dall'esterno): %d\n", k, vettoreRisultato[k]);
    }

    /* --- VERIFICA MANUALE DELL'ESEMPIO --- */
    // Cornice 0 (Esterna): 
    // Riga su: 1+2+3+4+5 = 15
    // Riga giù: 1+1+1+1+1 = 5
    // Colonna sx (senza angoli): 6+1+6 = 13
    // Colonna dx (senza angoli): 0+5+0 = 5
    // Totale atteso: 15 + 5 + 13 + 5 = 38
    
    // Cornice 1 (Media):
    // Riga su: 7+8+9 = 24
    // Riga giù: 7+8+9 = 24
    // Colonna sx (interno): 2
    // Colonna dx (interno): 4
    // Totale atteso: 24 + 24 + 2 + 4 = 54
    
    // Cornice 2 (Centro): 3

    /* --- 5. PULIZIA MEMORIA (FREE) --- */
    // È fondamentale liberare tutto ciò che abbiamo allocato con malloc.
    
    // A. Libero il vettore risultato creato dentro la funzione
    free(vettoreRisultato);

    // B. Libero la matrice
    for (int i = 0; i < N; i++) {
        free(matrice[i]); // Libero ogni riga
    }
    free(matrice); // Libero l'array di puntatori

    return 0;
}