#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: genera_matrice_somme
 * ======================================================================================
 * SCOPO:
 * Data una matrice M, ne genera una nuova M_prime dove ogni cella contiene la somma
 * dei k elementi adiacenti nelle 4 direzioni (Nord, Sud, Est, Ovest), escluso
 * l'elemento centrale stesso.
 *
 * PARAMETRI:
 * - int **M:          La matrice di input (già allocata e popolata).
 * - int r:            Numero di righe.
 * - int c:            Numero di colonne.
 * - int k:            Distanza massima (raggio) per la somma degli adiacenti.
 * - int ***M_prime:   Puntatore all'indirizzo della variabile che conterrà la nuova
 * matrice. Usiamo un triplo puntatore (***) perché dobbiamo
 * modificare un puntatore doppio (**) dichiarato nel chiamante.
 *
 * CONCETTI CHIAVE:
 * 1. Passaggio per Riferimento di Puntatori: Vogliamo che la funzione 'crei' una
 * matrice e la consegni al main. Poiché una matrice è `int **`, per modificarla
 * e farla puntare a nuova memoria, dobbiamo passare il suo indirizzo: `&matrice`.
 * Quindi il parametro diventa `int ***`.
 * 2. Controllo dei Limiti (Boundary Check): Quando sommiamo i vicini, dobbiamo
 * sempre verificare di non uscire dalla matrice (indice < 0 o >= dimensione).
 * ======================================================================================
 */
void genera_matrice_somme(int **M, int r, int c, int k, int ***M_prime) {

    /* --- 1. ALLOCAZIONE DELLA NUOVA MATRICE (M_prime) --- */
    
    /* * Dereferenziamo M_prime (*M_prime) per accedere alla variabile puntatore doppio
     * che si trova nel main. Le stiamo assegnando un vettore di puntatori alle righe.
     */
    *M_prime = (int **)malloc(r * sizeof(int *));
    
    if (*M_prime == NULL) {
        printf("Errore di allocazione memoria (righe).\n");
        exit(1);
    }

    for (int i = 0; i < r; i++) {
        /*
         * Per ogni riga, dereferenziamo (*M_prime) e accediamo all'indice [i]
         * usando l'aritmetica dei puntatori implicita nelle parentesi quadre.
         * Allociamo lo spazio per 'c' interi per ogni riga.
         */
        (*M_prime)[i] = (int *)malloc(c * sizeof(int));
        
        if ((*M_prime)[i] == NULL) {
            printf("Errore di allocazione memoria (colonne).\n");
            exit(1);
        }
    }

    /* --- 2. LOGICA DI CALCOLO --- */
    
    /* Iteriamo su ogni cella della matrice originale */
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            
            int somma = 0;

            /* * Ora dobbiamo esplorare le 4 direzioni fino a distanza k.
             * Usiamo un ciclo 'd' che va da 1 a k (distanza dall'elemento centrale).
             */
            for (int d = 1; d <= k; d++) {
                
                /* * DIREZIONE ALTO (UP): riga diminuisce (i - d)
                 * Controllo: l'indice di riga deve essere >= 0
                 */
                if (i - d >= 0) {
                    somma += M[i - d][j];
                }

                /* * DIREZIONE BASSO (DOWN): riga aumenta (i + d)
                 * Controllo: l'indice di riga deve essere < r
                 */
                if (i + d < r) {
                    somma += M[i + d][j];
                }

                /* * DIREZIONE SINISTRA (LEFT): colonna diminuisce (j - d)
                 * Controllo: l'indice di colonna deve essere >= 0
                 */
                if (j - d >= 0) {
                    somma += M[i][j - d];
                }

                /* * DIREZIONE DESTRA (RIGHT): colonna aumenta (j + d)
                 * Controllo: l'indice di colonna deve essere < c
                 */
                if (j + d < c) {
                    somma += M[i][j + d];
                }
            }

            /* --- 3. ASSEGNAZIONE DEL RISULTATO --- */
            /* Scriviamo la somma calcolata nella cella corrispondente della nuova matrice */
            (*M_prime)[i][j] = somma;
        }
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare i dati dell'esempio, chiamare la funzione e verificare l'output.
 * Nota bene la gestione della memoria: tutto ciò che allochiamo va liberato!
 * ======================================================================================
 */
int main() {
    int r = 5; // Righe dell'esempio
    int c = 4; // Colonne dell'esempio
    int k = 2; // Valore k dell'esempio

    /* --- CREAZIONE DATI DI INPUT (Hardcoded come nell'immagine) --- */
    
    // Allocazione matrice M input
    int **M = (int **)malloc(r * sizeof(int *));
    for(int i = 0; i < r; i++) {
        M[i] = (int *)malloc(c * sizeof(int));
    }

    // Popolamento manuale per replicare l'esempio esatto
    // Riga 0: 1 2 3 4
    M[0][0]=1;  M[0][1]=2;  M[0][2]=3;  M[0][3]=4;
    // Riga 1: 2 3 -2 5
    M[1][0]=2;  M[1][1]=3;  M[1][2]=-2; M[1][3]=5;
    // Riga 2: 1 2 3 4
    M[2][0]=1;  M[2][1]=2;  M[2][2]=3;  M[2][3]=4;
    // Riga 3: -2 -3 -1 0
    M[3][0]=-2; M[3][1]=-3; M[3][2]=-1; M[3][3]=0;
    // Riga 4: -5 1 -2 9
    M[4][0]=-5; M[4][1]=1;  M[4][2]=-2; M[4][3]=9;

    /* Variabile per ospitare il risultato.
     * Nota: È dichiarata come puntatore doppio, ma non la allochiamo qui!
     * Sarà la funzione 'f' ad allocarla.
     */
    int **M_result = NULL;

    printf("--- Matrice Originale ---\n");
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++) printf("%3d ", M[i][j]);
        printf("\n");
    }

    /* --- CHIAMATA ALLA FUNZIONE --- */
    /* * ATTENZIONE: Passiamo &M_result.
     * M_result è di tipo (int **).
     * &M_result è di tipo (int ***), esattamente quello che la funzione si aspetta.
     */
    genera_matrice_somme(M, r, c, k, &M_result);

    /* --- STAMPA RISULTATI --- */
    printf("\n--- Matrice Calcolata (k=%d) ---\n", k);
    printf("Risultato atteso (prima riga): 8 13 8 14\n");
    printf("Risultato ottenuto:\n");
    
    for (int i = 0; i < r; i++) {
        printf("| ");
        for (int j = 0; j < c; j++) {
            printf("%3d ", M_result[i][j]);
        }
        printf(" |\n");
    }

    /* --- PULIZIA MEMORIA (Memory Leak Prevention) --- */
    /* Libero M input */
    for (int i = 0; i < r; i++) free(M[i]);
    free(M);

    /* Libero M_result output */
    for (int i = 0; i < r; i++) free(M_result[i]);
    free(M_result);

    return 0;
}