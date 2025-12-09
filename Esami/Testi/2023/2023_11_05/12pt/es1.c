#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: genera_matrice_cumulativa
 * ======================================================================================
 * SCOPO:
 * Data una matrice M, ne genera una nuova M_primo dove ogni elemento [i][j] è la somma
 * degli elementi della riga i (fino a j) e della colonna j (fino a i).
 * La nuova matrice viene allocata dinamicamente all'interno della funzione.
 *
 * PARAMETRI:
 * - int **M        : La matrice di input (già allocata e popolata).
 * - int r          : Numero di righe.
 * - int c          : Numero di colonne.
 * - int ***M_primo : PUNTATORE alla variabile che conterrà l'indirizzo della nuova matrice.
 *
 * CONCETTI CHIAVE:
 * 1. Triplo Puntatore (int ***):
 * Perché usiamo tre asterischi?
 * Vogliamo che il 'main' riceva la nuova matrice creata qui dentro.
 * La matrice è un `int **`. In C, per modificare il valore di una variabile passata
 * come argomento (in questo caso l'indirizzo della matrice), dobbiamo passare il suo
 * indirizzo. Quindi: indirizzo (&) di un `int **` --> `int ***`.
 *
 * 2. Allocazione Dinamica:
 * Usiamo malloc per creare lo spazio nello Heap. Se non lo facessimo, le variabili
 * locali verrebbero distrutte alla fine della funzione.
 * ======================================================================================
 */
void genera_matrice_cumulativa(int **M, int r, int c, int ***M_primo) {

    /* --- 1. ALLOCAZIONE DELLE RIGHE (Vettore di Puntatori) --- */

    /*
     * Spiegazione Sintassi:
     * *M_primo : Dereferenziamo il triplo puntatore una volta. Stiamo accedendo alla
     * variabile 'matrix_out' dichiarata nel main.
     * malloc(r * sizeof(int *)) : Chiediamo memoria per 'r' puntatori (uno per riga).
     * Nota che usiamo sizeof(int *) perché ogni cella conterrà
     * un indirizzo di memoria, non un intero semplice.
     */
    *M_primo = (int **)malloc(r * sizeof(int *));

    // Controllo difensivo: se la malloc fallisce, terminiamo per evitare crash.
    if (*M_primo == NULL) {
        printf("Errore: memoria insufficiente per le righe.\n");
        exit(1);
    }

    /* --- 2. ALLOCAZIONE DELLE COLONNE E LOGICA --- */
    for (int i = 0; i < r; i++) {

        /*
         * Spiegazione Sintassi:
         * (*M_primo)[i] : Le parentesi sono FONDAMENTALI.
         * L'operatore [] ha priorità su *. Senza parentesi, il compilatore
         * capirebbe *(M_primo[i]), che è sbagliato.
         * Noi vogliamo: "Vai all'indirizzo puntato da M_primo, prendi l'i-esimo
         * puntatore e assegnagli nuova memoria".
         * sizeof(int) : Qui allochiamo i dati veri e propri (interi), quindi sizeof(int).
         */
        (*M_primo)[i] = (int *)malloc(c * sizeof(int));

        if ((*M_primo)[i] == NULL) {
            printf("Errore: memoria insufficiente per le colonne della riga %d.\n", i);
            exit(1);
        }

        /* --- 3. CALCOLO DEI VALORI --- */
        // Scorriamo ogni cella della matrice per calcolare il suo nuovo valore
        for (int j = 0; j < c; j++) {
            int somma = 0;

            // a. Somma elementi riga 'i' fino alla colonna 'j'
            //    Scorriamo con un indice 'k' da 0 a j
            for (int k = 0; k <= j; k++) {
                somma += M[i][k]; // M[i][k] è l'accesso standard: riga i, colonna k
            }

            // b. Somma elementi colonna 'j' fino alla riga 'i'
            //    Scorriamo con un indice 'k' da 0 a i
            for (int k = 0; k <= i; k++) {
                somma += M[k][j]; // M[k][j]: riga k (variabile), colonna j (fissa)
            }

            // c. Correzione
            //    Il testo dice: "incluso l'elemento [i][j] originale il cui contributo
            //    è contato una singola volta".
            //    Nei due cicli sopra, abbiamo sommato M[i][j] due volte (una nel ciclo riga,
            //    una nel ciclo colonna). Quindi dobbiamo sottrarlo una volta.
            somma -= M[i][j];

            /*
             * Assegnazione finale:
             * Scriviamo il risultato nella cella allocata della nuova matrice.
             * Ancora una volta, usiamo (*M_primo)[i][j] per accedere correttamente.
             */
            (*M_primo)[i][j] = somma;
        }
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare un ambiente controllato per verificare che la funzione agisca correttamente.
 * Steps:
 * 1. Allocazione Input
 * 2. Popolamento Input (come da esempio)
 * 3. Chiamata Funzione
 * 4. Stampa
 * 5. Pulizia Memoria (Free)
 * ======================================================================================
 */
int main() {
    int r = 3;
    int c = 3;

    // --- 1. PREPARAZIONE DATI INPUT (Allocazione Dinamica) ---
    // Allochiamo la matrice di input M per simulare uno scenario realistico
    int **M = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        M[i] = (int *)malloc(c * sizeof(int));
    }

    // --- 2. POPOLAMENTO HARDCODED (Dati dell'esempio) ---
    // Riga 0: {1, 2, 3}
    M[0][0] = 1; M[0][1] = 2; M[0][2] = 3;
    // Riga 1: {4, 5, 6}
    M[1][0] = 4; M[1][1] = 5; M[1][2] = 6;
    // Riga 2: {7, 8, 9}
    M[2][0] = 7; M[2][1] = 8; M[2][2] = 9;

    printf("--- Matrice Originale M ---\n");
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) printf("%2d ", M[i][j]);
        printf("\n");
    }

    // Variabile che ospiterà il risultato.
    // Inizialmente è un puntatore "vuoto" (o spazzatura), la funzione lo farà puntare alla memoria valida.
    int **matrix_out;

    // --- 3. CHIAMATA ALLA FUNZIONE ---
    // Passiamo &matrix_out (indirizzo di un puntatore doppio) -> ecco il triplo puntatore!
    genera_matrice_cumulativa(M, r, c, &matrix_out);

    // --- 4. VERIFICA RISULTATI ---
    printf("\n--- Matrice Calcolata M' ---\n");
    printf("(Atteso: 1 3 6 / 5 11 18 / 12 22 33)\n");
    for (int i = 0; i < r; i++) {
        printf("{ ");
        for (int j = 0; j < c; j++) {
            // Stampa formattata larga 3 caratteri per allineamento
            printf("%3d ", matrix_out[i][j]);
        }
        printf("}\n");
    }

    // --- 5. GESTIONE MEMORIA (FREE) ---
    // Regola d'oro: Per ogni malloc, deve esserci una free.
    // Prima liberiamo le righe (i dati), poi il vettore dei puntatori.

    // Libero M (Input)
    for (int i = 0; i < r; i++) free(M[i]);
    free(M);

    // Libero M' (Output)
    for (int i = 0; i < r; i++) free(matrix_out[i]);
    free(matrix_out);

    return 0;
}