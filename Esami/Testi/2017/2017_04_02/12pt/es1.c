#include <stdio.h>
#include <stdlib.h>

/* * SCOPO: Calcolare la somma degli elementi di ogni "cornice" concentrica di una matrice
 * quadrata N x N e memorizzare i risultati in un vettore allocato dinamicamente.
 *
 * PARAMETRI:
 * - int **mat: Rappresenta la matrice (array di array). Passiamo un puntatore doppio 
 * per accedere agli elementi mat[i][j].
 * - int N: La dimensione della matrice quadrata.
 * - int **vet: Un TRIPLO PUNTATORE (indiretto). Perché? Perché la funzione deve 
 * allocare memoria per il vettore e vogliamo che il 'main' veda questo 
 * nuovo indirizzo. Usiamo **vet perché nel main abbiamo un 'int *v' 
 * e passiamo '&v'.
 *
 * CONCETTI CHIAVE: 
 * L'uso di 'int **vet' è fondamentale: se passassimo solo 'int *vet', la modifica 
 * all'indirizzo (la malloc) rimarrebbe locale alla funzione (passaggio per valore). 
 * Usando il puntatore al puntatore, modifichiamo direttamente la variabile del chiamante.
 */
void sommaCornici(int **mat, int N, int **vet) {
    
    /* --- 1. DETERMINAZIONE DIMENSIONE E ALLOCAZIONE --- */
    
    // In una matrice dispari N x N, il numero di cornici è N/2 + 1.
    // Esempio N=5: 5/2 = 2; 2 + 1 = 3 cornici.
    int num_cornici = (N / 2) + 1;

    /* * Alloco il vettore dei risultati. 
     * *vet: accedo alla cella di memoria puntata dal parametro (che è il puntatore nel main).
     * sizeof(int) * num_cornici: calcolo lo spazio necessario in byte.
     */
    *vet = (int *)malloc(sizeof(int) * num_cornici);

    // Controllo sempre che la malloc sia andata a buon fine per evitare crash.
    if (*vet == NULL) {
        fprintf(stderr, "Errore di allocazione memoria!\n");
        exit(1);
    }

    /* --- 2. LOGICA DI CALCOLO DELLE CORNICI --- */
    
    /* * Iteriamo su ogni cornice 'k'.
     * La cornice 0 è quella esterna, la cornice 1 è quella interna successiva, e così via.
     */
    for (int k = 0; k < num_cornici; k++) {
        int somma = 0;
        
        // Definiamo i limiti dell'indice per la cornice corrente
        int inizio = k;
        int fine = N - 1 - k;

        // Caso particolare: se inizio == fine, siamo sull'elemento centrale (singola cella)
        if (inizio == fine) {
            somma = mat[inizio][fine];
        } 
        else {
            // 1. Somma riga superiore (da inizio a fine)
            for (int j = inizio; j <= fine; j++) {
                somma += mat[inizio][j];
            }
            // 2. Somma riga inferiore (da inizio a fine)
            for (int j = inizio; j <= fine; j++) {
                somma += mat[fine][j];
            }
            // 3. Somma colonna sinistra (escludendo gli angoli già presi sopra)
            for (int i = inizio + 1; i < fine; i++) {
                somma += mat[i][inizio];
            }
            // 4. Somma colonna destra (escludendo gli angoli già presi sopra)
            for (int i = inizio + 1; i < fine; i++) {
                somma += mat[i][fine];
            }
        }

        /* * Memorizzo il risultato. 
         * (*vet)[k] significa: prendi l'indirizzo contenuto in 'vet' (il vettore), 
         * spostati alla posizione 'k' e scrivi il valore. 
         * Le parentesi sono d'obbligo perché l'operatore [] ha precedenza su *.
         */
        (*vet)[k] = somma;
    }
}

/* --- MAIN DI TEST --- */
int main() {
    int N = 5;
    
    // 1. Allocazione dinamica della matrice 5x5 (array di puntatori)
    int **matrice = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrice[i] = (int *)malloc(N * sizeof(int));
    }

    // 2. Popolamento matrice con i dati dell'esempio in immagine
    int dati[5][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 0},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 0},
        {1, 1, 1, 1, 1}
    };

    printf("Matrice in ingresso (%dx%d):\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrice[i][j] = dati[i][j];
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    // 3. Preparazione del puntatore per il vettore dei risultati
    int *risultati = NULL; 

    // Chiamata alla funzione: passiamo l'indirizzo del puntatore (&risultati)
    sommaCornici(matrice, N, &risultati);

    // 4. Stampa dei risultati
    int num_c = (N / 2) + 1;
    printf("\nSomme delle %d cornici (dalla esterna alla interna):\n", num_c);
    for (int k = 0; k < num_c; k++) {
        printf("Cornice %d: %d\n", k, risultati[k]);
    }

    /* --- 5. GESTIONE DELLA MEMORIA (PULIZIA) --- */
    
    // Liberiamo il vettore allocato dentro la funzione
    free(risultati);

    // Liberiamo ogni riga della matrice
    for (int i = 0; i < N; i++) {
        free(matrice[i]);
    }
    // Liberiamo il puntatore alle righe
    free(matrice);

    return 0;
}