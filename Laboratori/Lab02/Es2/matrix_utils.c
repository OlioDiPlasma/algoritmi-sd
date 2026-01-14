#include "matrix_utils.h"

/* * ======================================================================================
 * Funzione: malloc2dR
 * --------------------------------------------------------------------------------------
 * Scopo:       Allocare dinamicamente una matrice di interi in memoria heap.
 * Parametri:   
 * - nr: numero di righe
 * - nc: numero di colonne
 * Concetti Chiave: 
 * Si usa un doppio puntatore (int **) perche' una matrice dinamica in C
 * e' spesso rappresentata come un array di puntatori (le righe), dove ogni
 * puntatore punta a un array di interi (le colonne).
 * ======================================================================================
 */
int **malloc2dR(int nr, int nc) {
    /* --- 1. ALLOCAZIONE VETTORE DI PUNTATORI --- */
    /* Alloco lo spazio per 'nr' puntatori a intero. 
       mat e' un puntatore a puntatore. */
    int **mat = (int **)malloc(nr * sizeof(int *));
    
    if (mat == NULL) {
        printf("Errore allocazione righe\n");
        exit(1);
    }

    /* --- 2. ALLOCAZIONE DELLE RIGHE --- */
    for (int i = 0; i < nr; i++) {
        /* Per ogni riga, alloco un array di 'nc' interi */
        mat[i] = (int *)malloc(nc * sizeof(int));
        if (mat[i] == NULL) {
            printf("Errore allocazione colonne\n");
            exit(1); // In un codice robusto bisognerebbe fare free delle righe precedenti
        }
    }

    return mat; /* Restituisco l'indirizzo base della matrice */
}

/* * ======================================================================================
 * Funzione: free2d
 * --------------------------------------------------------------------------------------
 * Scopo:       Liberare correttamente la memoria di una matrice dinamica.
 * Concetti Chiave:
 * Bisogna liberare PRIMA le singole righe (i dati interni) e POI
 * il vettore di puntatori. L'ordine inverso causerebbe memory leak
 * o crash (accesso a memoria gia' liberata).
 * ======================================================================================
 */
void free2d(int **mat, int nr) {
    if (mat == NULL) return;
    
    /* Libero ogni singola riga */
    for (int i = 0; i < nr; i++) {
        free(mat[i]);
    }
    /* Libero il contenitore delle righe */
    free(mat);
}

/* * ======================================================================================
 * Funzione: leggi_file
 * --------------------------------------------------------------------------------------
 * Scopo:       Apre il file, legge dimensioni, alloca la matrice e riempie i dati.
 * File di input: Richiede un file formattato come specificato (R C \n dati...).
 * Parametri:
 * - filename: nome del file
 * - nr, nc: Puntatori a interi. Servono per "restituire" le dimensioni lette
 * al main (Passaggio per riferimento).
 * ======================================================================================
 */
int **leggi_file(char *filename, int *nr, int *nc) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(1);
    }

    /* Leggo le dimensioni e le salvo nelle variabili puntate da nr e nc */
    /* Dereferenzio (*nr) per scrivere nella memoria del main */
    if (fscanf(fp, "%d %d", nr, nc) != 2) {
        printf("Errore lettura dimensioni\n");
        exit(1);
    }

    /* Chiamo la funzione di allocazione creata prima */
    int **mat = malloc2dR(*nr, *nc);

    /* --- LETTURA DATI --- */
    for (int i = 0; i < *nr; i++) {
        for (int j = 0; j < *nc; j++) {
            fscanf(fp, "%d", &mat[i][j]); 
            /* &mat[i][j] e' l'indirizzo della cella specifica dove scrivere il numero */
        }
    }

    fclose(fp);
    return mat;
}

/* * ======================================================================================
 * Funzione: separa
 * --------------------------------------------------------------------------------------
 * Scopo:       Analizza la matrice come una scacchiera, separa i valori "bianchi"
 * da quelli "neri" in due nuovi vettori dinamici.
 * Parametri:
 * - mat: la matrice sorgente
 * - nr, nc: dimensioni
 * - v_bianchi: TRIPLO PUNTATORE (o puntatore a puntatore a int).
 * PERCHE'? Dobbiamo allocare un vettore dentro questa funzione e far
 * puntare la variabile del main a questa nuova memoria. 
 * Quindi passiamo l'indirizzo del puntatore del main: &vett.
 * - len_bianchi: puntatore per restituire la lunghezza del vettore.
 * ======================================================================================
 */
void separa(int **mat, int nr, int nc, 
            int **v_bianchi, int *len_bianchi, 
            int **v_neri, int *len_neri) {
    
    /* --- 1. CONTEGGIO DIMENSIONI --- */
    /* Per allocare la memoria esatta (senza sprechi), contiamo prima quanti elementi
       vanno nei bianchi e quanti nei neri.
       Logica scacchiera: 
       Se (riga + colonna) e' PARI -> Cella "Bianca" (o viceversa, convenzione)
       Se (riga + colonna) e' DISPARI -> Cella "Nera"
    */
    int count_w = 0;
    int count_b = 0;

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if ((i + j) % 2 == 0) {
                count_w++;
            } else {
                count_b++;
            }
        }
    }

    /* Assegno le dimensioni trovate alle variabili del main tramite dereferenziazione */
    *len_bianchi = count_w;
    *len_neri = count_b;

    /* --- 2. ALLOCAZIONE VETTORI DI OUTPUT --- */
    /* Qui usiamo *v_bianchi. 
       v_bianchi e' int** (indirizzo del puntatore nel main).
       *v_bianchi e' int* (il puntatore vero e proprio nel main).
       Stiamo dicendo: "Il puntatore nel main deve ora puntare a questo nuovo blocco di malloc"
    */
    *v_bianchi = (int *)malloc(count_w * sizeof(int));
    *v_neri = (int *)malloc(count_b * sizeof(int));

    if (*v_bianchi == NULL || *v_neri == NULL) {
        printf("Errore malloc vettori output\n");
        exit(1);
    }

    /* --- 3. RIEMPIMENTO VETTORI --- */
    /* Usiamo due indici ausiliari per scorrere i nuovi vettori */
    int idx_w = 0;
    int idx_b = 0;

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            int valore = mat[i][j]; // Accedo al valore nella matrice
            
            if ((i + j) % 2 == 0) {
                /* Scrivo nel vettore bianco dereferenziato all'indice corrente */
                (*v_bianchi)[idx_w] = valore; 
                /* Nota sintattica: (*v_bianchi) accede al vettore, [idx_w] accede alla cella */
                idx_w++;
            } else {
                (*v_neri)[idx_b] = valore;
                idx_b++;
            }
        }
    }
}