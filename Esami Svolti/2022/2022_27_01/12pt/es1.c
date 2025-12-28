#include <stdio.h>
#include <stdlib.h>

/* ========================================================================= */
/* FUNZIONE f                                   */
/* ========================================================================= */

/*
 * Scopo:
 * Generare una nuova matrice M' derivata da M, eliminando tutte le righe
 * e le colonne che contengono almeno un valore 0 (null).
 *
 * Parametri:
 * - int **M:      La matrice di input (array di puntatori alle righe).
 * - int r:        Numero di righe di M.
 * - int c:        Numero di colonne di M.
 * - int ***M_out: (OUTPUT) Indirizzo dove scriveremo il puntatore alla nuova matrice.
 * - int *r_out:   (OUTPUT) Indirizzo dove scriveremo il numero di righe della nuova matrice.
 * - int *c_out:   (OUTPUT) Indirizzo dove scriveremo il numero di colonne della nuova matrice.
 *
 * Concetti Chiave:
 * - Triplo Puntatore (int ***M_out): Usiamo un triplo puntatore perché dobbiamo
 * allocare una matrice (che è int**) e far sì che il chiamante (main) veda
 * il nuovo indirizzo. È come dire: "Dammi la scatola dove tieni il tuo puntatore alla matrice,
 * così posso metterne uno nuovo dentro".
 * - Array di flag: Usiamo due array temporanei per segnarci quali righe e quali
 * colonne sono "buone" da tenere.
 */
void f(int **M, int r, int c, int ***M_out, int *r_out, int *c_out) {

    /* --- 1. ANALISI: IDENTIFICAZIONE RIGHE E COLONNE VALIDE --- */
    
    // Allocazione flag temporanei nello Stack (o Heap se r/c fossero enormi).
    // Usiamo calloc per inizializzare tutto a 0, ma qui userò malloc + loop per chiarezza didattica.
    // good_rows[i] sarà 1 se la riga i non ha zeri, 0 altrimenti.
    int *good_rows = (int *)malloc(r * sizeof(int));
    int *good_cols = (int *)malloc(c * sizeof(int));

    int new_r = 0; // Contatore per le nuove righe
    int new_c = 0; // Contatore per le nuove colonne

    // Scansione per le righe valide
    for (int i = 0; i < r; i++) {
        int ha_zeri = 0;
        for (int j = 0; j < c; j++) {
            if (M[i][j] == 0) {
                ha_zeri = 1;
                break; // Appena trovo uno zero, la riga è "sporca"
            }
        }
        if (ha_zeri == 0) {
            good_rows[i] = 1; // La teniamo
            new_r++;
        } else {
            good_rows[i] = 0; // La scartiamo
        }
    }

    // Scansione per le colonne valide
    for (int j = 0; j < c; j++) {
        int ha_zeri = 0;
        for (int i = 0; i < r; i++) {
            if (M[i][j] == 0) {
                ha_zeri = 1;
                break;
            }
        }
        if (ha_zeri == 0) {
            good_cols[j] = 1; // La teniamo
            new_c++;
        } else {
            good_cols[j] = 0; // La scartiamo
        }
    }

    /* --- 2. GESTIONE CASO MATRICE VUOTA --- */
    if (new_r == 0 || new_c == 0) {
        // Dereferenziamo i puntatori di output per restituire dimensioni nulle
        *r_out = 0;
        *c_out = 0;
        *M_out = NULL;
        
        // Pulizia memoria temporanea
        free(good_rows);
        free(good_cols);
        return;
    }

    /* --- 3. ALLOCAZIONE NUOVA MATRICE --- */
    
    // 1. Alloco l'array dei puntatori alle righe (spina dorsale)
    // sizeof(int *) perché ogni cella conterrà un indirizzo.
    int **mat = (int **)malloc(new_r * sizeof(int *));
    
    // 2. Alloco le righe vere e proprie
    for (int i = 0; i < new_r; i++) {
        mat[i] = (int *)malloc(new_c * sizeof(int));
    }

    /* --- 4. COPIA DEI DATI (POPOLAMENTO) --- */
    
    int curr_r = 0; // Indice per scorrere le righe della NUOVA matrice
    
    // Scorriamo le righe della VECCHIA matrice
    for (int i = 0; i < r; i++) {
        // Se la riga vecchia è segnata come buona:
        if (good_rows[i] == 1) {
            
            int curr_c = 0; // Indice per scorrere le colonne della NUOVA matrice
            
            // Scorriamo le colonne della VECCHIA matrice
            for (int j = 0; j < c; j++) {
                // Se la colonna vecchia è segnata come buona:
                if (good_cols[j] == 1) {
                    // Copiamo il valore
                    mat[curr_r][curr_c] = M[i][j];
                    curr_c++; // Avanziamo nella colonna nuova
                }
            }
            curr_r++; // Avanziamo nella riga nuova solo dopo aver riempito la riga
        }
    }

    /* --- 5. RESTITUZIONE RISULTATI (OUTPUT PARAMETERS) --- */
    
    // Scriviamo nelle variabili del main tramite i puntatori ricevuti
    *M_out = mat;  // Dereferenzio M_out (***) per scrivere il puntatore (**)
    *r_out = new_r;
    *c_out = new_c;

    /* --- 6. PULIZIA --- */
    // Liberiamo gli array di supporto, non servono più
    free(good_rows);
    free(good_cols);
}

/* ========================================================================= */
/* MAIN DI TEST                                 */
/* ========================================================================= */

int main() {
    printf("--- Inizio Test Matrice ---\n");

    /* --- CREAZIONE DATI HARDCODED (Dinamici) --- */
    // Dobbiamo simulare una matrice dinamica int** per passarla alla funzione.
    // Esempio traccia:
    // 1 2 3
    // 4 0 9  (contiene 0 -> riga 1 da eliminare, colonna 1 da eliminare)
    // 7 8 9
    
    int r = 3, c = 3;
    int **M = (int **)malloc(r * sizeof(int *));
    for(int i=0; i<r; i++) M[i] = (int *)malloc(c * sizeof(int));

    // Riempimento manuale
    M[0][0]=1; M[0][1]=2; M[0][2]=3;
    M[1][0]=4; M[1][1]=0; M[1][2]=9; // Lo 0 è qui!
    M[2][0]=7; M[2][1]=8; M[2][2]=9;

    printf("Matrice Originale (%dx%d):\n", r, c);
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) printf("%d ", M[i][j]);
        printf("\n");
    }

    /* --- VARIABILI PER IL RISULTATO --- */
    int **M_prime = NULL; // Qui finirà la nuova matrice
    int r_prime, c_prime; // Qui finiranno le nuove dimensioni

    /* --- CHIAMATA ALLA FUNZIONE --- */
    // Passiamo gli indirizzi (&) perché la funzione deve modificarle
    f(M, r, c, &M_prime, &r_prime, &c_prime);

    /* --- STAMPA RISULTATI --- */
    printf("\nMatrice Derivata (%dx%d):\n", r_prime, c_prime);
    if (M_prime != NULL) {
        for(int i=0; i<r_prime; i++) {
            for(int j=0; j<c_prime; j++) printf("%d ", M_prime[i][j]);
            printf("\n");
        }
    } else {
        printf("Matrice vuota.\n");
    }

    /* --- GESTIONE MEMORIA (FREE) --- */
    // 1. Liberiamo la matrice originale
    for(int i=0; i<r; i++) free(M[i]);
    free(M);

    // 2. Liberiamo la nuova matrice (se esiste)
    if (M_prime != NULL) {
        for(int i=0; i<r_prime; i++) free(M_prime[i]);
        free(M_prime);
    }
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}