#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // Necessario per abs()
#include <limits.h> // Necessario per INT_MAX

/*
 * DEFINIZIONE DELLA STRUTTURA
 * Come richiesto dalla traccia. Contiene due puntatori a char (stringhe).
 * Nota: Queste stringhe dovranno essere allocate dinamicamente dentro la funzione!
 */
typedef struct coppia_ {
    char *scelta_A;
    char *scelta_B;
} coppia;

/* =================================================================================
 * FUNZIONE: mySum
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Riceve due matrici quadrate A e B. Calcola le somme di tutte le righe e
 * tutte le colonne di entrambe. Cerca la coppia (elemento di A, elemento di B)
 * la cui somma totale sia più vicina possibile a zero.
 *
 * PARAMETRI:
 * - int **A: Puntatore doppio alla matrice A (array di array).
 * - int nA: Dimensione lato matrice A (nA x nA).
 * - int **B: Puntatore doppio alla matrice B.
 * - int nB: Dimensione lato matrice B.
 *
 * CONCETTI CHIAVE:
 * 1. Rappresentazione delle somme: Per evitare 4 cicli annidati confusi,
 * conviene pre-calcolare tutte le somme possibili.
 * Per una matrice N*N, abbiamo N somme di righe e N somme di colonne.
 * Creeremo un array temporaneo di dimensione 2*N per memorizzarle tutte.
 * Indici 0..(N-1) -> Righe. Indici N..(2N-1) -> Colonne.
 *
 * 2. Allocazione Stringhe: La struct di ritorno contiene `char*`. Non possiamo
 * restituire stringhe statiche o locali (stack) perché verrebbero distrutte
 * alla fine della funzione. Dobbiamo usare malloc.
 * =================================================================================
 */
coppia mySum(int **A, int nA, int **B, int nB) {
    
    /* --- 1. PREPARAZIONE ARRAY TEMPORANEI PER LE SOMME --- */
    
    // Allocazione array per salvare le somme di A.
    // Dimensione: nA (per le righe) + nA (per le colonne)
    int *sommeA = (int *)malloc(2 * nA * sizeof(int));
    
    // Allocazione array per salvare le somme di B.
    int *sommeB = (int *)malloc(2 * nB * sizeof(int));

    // Controllo malloc (buona norma sempre, anche se negli esami a volte si omette)
    if (sommeA == NULL || sommeB == NULL) {
        printf("Errore di allocazione memoria temporanea.\n");
        exit(1);
    }

    /* --- 2. CALCOLO SOMME PER MATRICE A --- */
    
    // Calcoliamo prima le somme delle RIGHE (indici 0 a nA-1)
    for (int i = 0; i < nA; i++) {
        int sommaRiga = 0;
        for (int j = 0; j < nA; j++) {
            // Accesso standard matrice: A[riga][colonna]
            sommaRiga += A[i][j];
        }
        sommeA[i] = sommaRiga; // Salviamo nella prima metà dell'array
    }

    // Calcoliamo le somme delle COLONNE (indici nA a 2*nA-1)
    for (int j = 0; j < nA; j++) {
        int sommaCol = 0;
        for (int i = 0; i < nA; i++) {
            sommaCol += A[i][j]; // Fissiamo la colonna j, scorriamo le righe i
        }
        // Salviamo nella seconda metà. L'indice sarà nA + j
        sommeA[nA + j] = sommaCol;
    }

    /* --- 3. CALCOLO SOMME PER MATRICE B --- */
    // Ripetiamo la logica identica per B
    
    // Righe di B
    for (int i = 0; i < nB; i++) {
        int sommaRiga = 0;
        for (int j = 0; j < nB; j++) {
            sommaRiga += B[i][j];
        }
        sommeB[i] = sommaRiga;
    }

    // Colonne di B
    for (int j = 0; j < nB; j++) {
        int sommaCol = 0;
        for (int i = 0; i < nB; i++) {
            sommaCol += B[i][j];
        }
        sommeB[nB + j] = sommaCol;
    }

    /* --- 4. RICERCA DEL MINIMO (Core Logic) --- */
    
    int min_distanza = INT_MAX; // Inizializziamo con il massimo intero possibile
    int best_idx_A = -1; // Indice nell'array sommeA che ha dato il risultato migliore
    int best_idx_B = -1; // Indice nell'array sommeB che ha dato il risultato migliore

    // Doppio ciclo: confrontiamo ogni somma di A con ogni somma di B
    // Totale iterazioni: (2*nA) * (2*nB)
    for (int i = 0; i < 2 * nA; i++) {
        for (int k = 0; k < 2 * nB; k++) {
            
            // Somma dei valori calcolati
            int sommaTotale = sommeA[i] + sommeB[k];
            
            // Calcoliamo la distanza da zero usando il valore assoluto
            int distanza = abs(sommaTotale); 
            
            // Se troviamo una distanza minore di quella attuale, aggiorniamo il record
            if (distanza < min_distanza) {
                min_distanza = distanza;
                best_idx_A = i;
                best_idx_B = k;
            }
        }
    }

    /* --- 5. FORMATTAZIONE DELL'OUTPUT --- */
    
    coppia result; // Variabile struct locale

    // Allocazione memoria per le stringhe di risultato.
    // "R" + cifre indice + '\0'. 10 char sono più che sufficienti per un int normale.
    result.scelta_A = (char *)malloc(10 * sizeof(char));
    result.scelta_B = (char *)malloc(10 * sizeof(char));

    // Determiniamo se l'indice migliore di A era una Riga o una Colonna
    if (best_idx_A < nA) {
        // È una riga (indici 0..nA-1)
        sprintf(result.scelta_A, "R%d", best_idx_A);
    } else {
        // È una colonna (indici nA..2nA-1). Dobbiamo sottrarre nA per avere l'indice originale (0..nA-1)
        sprintf(result.scelta_A, "C%d", best_idx_A - nA);
    }

    // Stessa logica per B
    if (best_idx_B < nB) {
        sprintf(result.scelta_B, "R%d", best_idx_B);
    } else {
        sprintf(result.scelta_B, "C%d", best_idx_B - nB);
    }

    /* --- 6. PULIZIA MEMORIA TEMPORANEA --- */
    // Non abbiamo più bisogno degli array di somme, ma NON dobbiamo liberare le stringhe dentro result!
    free(sommeA);
    free(sommeB);

    return result;
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Creare i dati dell'esempio (Matrice A 3x3, Matrice B 2x2), chiamare la funzione
 * e verificare che l'output sia corretto ("C2 R1" con somma 2).
 * =================================================================================
 */
int main() {
    /* --- 1. ALLOCAZIONE E CREAZIONE DATI DI PROVA --- */
    int nA = 3;
    int nB = 2;

    // Allocazione Matrice A (Array di puntatori + righe)
    int **A = (int **)malloc(nA * sizeof(int *));
    for(int i=0; i<nA; i++) A[i] = (int *)malloc(nA * sizeof(int));

    // Riempimento A come da esempio
    //  1  2  3
    //  3  2 -3
    // 11 12 -6
    A[0][0]=1;  A[0][1]=2;  A[0][2]=3;
    A[1][0]=3;  A[1][1]=2;  A[1][2]=-3;
    A[2][0]=11; A[2][1]=12; A[2][2]=-6;

    // Allocazione Matrice B
    int **B = (int **)malloc(nB * sizeof(int *));
    for(int i=0; i<nB; i++) B[i] = (int *)malloc(nB * sizeof(int));

    // Riempimento B come da esempio
    // 2 0
    // 7 1
    B[0][0]=2; B[0][1]=0;
    B[1][0]=7; B[1][1]=1;

    /* --- 2. INVOCAZIONE --- */
    printf("Invoco mySum...\n");
    coppia res = mySum(A, nA, B, nB);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Risultato atteso: C2 R1\n");
    printf("Risultato ottenuto: %s %s\n", res.scelta_A, res.scelta_B);

    /* --- 4. FREE MEMORY (Molto Importante!) --- */
    
    // 1. Liberare le stringhe generate dentro mySum
    free(res.scelta_A);
    free(res.scelta_B);

    // 2. Liberare le matrici
    for(int i=0; i<nA; i++) free(A[i]);
    free(A);
    
    for(int i=0; i<nB; i++) free(B[i]);
    free(B);

    return 0;
}