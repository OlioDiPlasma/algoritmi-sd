#include <stdio.h>
#include <stdlib.h>

/* =================================================================================
 * FUNZIONE: buildAndPrint
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Decodifica un vettore compresso V e riempie una matrice M di dimensioni r x c.
 * La logica di riempimento è "Row-Major" (per righe): si riempie la prima riga,
 * poi si passa alla seconda, e così via. Infine stampa la matrice.
 *
 * PARAMETRI:
 * - int *V: Puntatore al vettore sorgente contenente i dati compressi (coppie quantità/valore).
 * - int N: Dimensione totale del vettore V (numero di interi).
 * - int **M: Puntatore doppio alla matrice di destinazione (già allocata).
 * - int r: Numero di righe della matrice.
 * - int c: Numero di colonne della matrice.
 *
 * CONCETTI CHIAVE:
 * 1. Navigazione 2D: Dobbiamo tenere traccia di due indici, 'row' (riga corrente)
 * e 'col' (colonna corrente). Quando 'col' raggiunge il limite 'c', dobbiamo
 * resettarlo a 0 e incrementare 'row'.
 * 2. Decompressione: Il vettore V va letto a salti di 2.
 * V[k] è la QUANTITÀ ("quante volte scrivere").
 * V[k+1] è il VALORE ("cosa scrivere").
 * =================================================================================
 */
void buildAndPrint(int *V, int N, int **M, int r, int c) {

    /* --- 1. INIZIALIZZAZIONE INDICI MATRICE --- */
    // 'curr_r' tiene traccia della riga in cui stiamo scrivendo.
    // 'curr_c' tiene traccia della colonna in cui stiamo scrivendo.
    int curr_r = 0;
    int curr_c = 0;

    /* --- 2. LOGICA DI DECODIFICA (Iterazione sul Vettore) --- */
    // Scorriamo il vettore V. Attenzione: incrementiamo k di 2 alla volta (k += 2)
    // perché ogni informazione è composta da una coppia (quantità, valore).
    for (int k = 0; k < N; k += 2) {
        
        // Lettura della coppia dal vettore
        int ripetizioni = V[k];     // Primo elemento della coppia: quante volte scrivere
        int valore = V[k+1];        // Secondo elemento della coppia: il numero da scrivere

        /* --- 3. RIEMPIMENTO MATRICE (Loop interno) --- */
        // Eseguiamo un ciclo per il numero di 'ripetizioni' richieste
        for (int i = 0; i < ripetizioni; i++) {
            
            // CONTROLLO DI SICUREZZA (Opzionale ma buona pratica):
            // Verifichiamo di non uscire dalle righe allocate.
            if (curr_r >= r) {
                printf("Errore: Il vettore V contiene troppi dati per la matrice %dx%d!\n", r, c);
                return;
            }

            // DETTAGLIO SINTATTICO:
            // M[curr_r] accede al puntatore della riga specifica.
            // [curr_c] accede alla singola cella (intero) di quella riga.
            // Assegniamo il 'valore' decompresso alla cella.
            M[curr_r][curr_c] = valore;

            // Avanzamento colonna
            curr_c++; // Ci spostiamo alla cella successiva a destra

            // GESTIONE "A CAPO" (Row-Major Order)
            // Se l'indice di colonna raggiunge il numero massimo di colonne 'c'...
            if (curr_c == c) {
                curr_c = 0; // ...resettiamo la colonna all'inizio (sinistra)...
                curr_r++;   // ...e scendiamo alla riga successiva.
            }
        }
    }

    /* --- 4. STAMPA DEL RISULTATO --- */
    printf("Matrice M (%d x %d):\n", r, c);
    for (int i = 0; i < r; i++) {
        // Stampa estetica: parentesi a inizio riga
        printf("( "); 
        for (int j = 0; j < c; j++) {
            // Stampa del valore formattato
            printf("%2d ", M[i][j]); 
        }
        // Parentesi a fine riga e "a capo"
        printf(")\n"); 
    }
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Creare l'ambiente di prova esatto descritto nell'esempio:
 * 1. Allocare la matrice dinamicamente.
 * 2. Preparare il vettore V.
 * 3. Chiamare la funzione.
 * 4. Liberare (free) la memoria per evitare memory leaks.
 * =================================================================================
 */
int main() {
    printf("--- ESERCIZIO DECOMPRESSIONE MATRICE ---\n\n");

    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    int r = 3;  // Righe
    int c = 5;  // Colonne
    int N = 14; // Dimensione vettore V
    
    // Vettore V hardcoded come da esempio.
    // Coppie: (2 volte 1), (2 volte 17), (1 volta 3), (4 volte 8), etc.
    int V[] = {2, 1, 2, 17, 1, 3, 4, 8, 1, 6, 3, 7, 2, 5};

    /* --- 2. ALLOCAZIONE MATRICE DINAMICA --- */
    // Nota Didattica: Una matrice dinamica in C (int**) si costruisce in due passi.
    
    // PASSO A: Allocare l'array di puntatori alle righe (la "spina dorsale").
    // Usiamo sizeof(int*) perché ogni cella conterrà un indirizzo di memoria.
    int **M = (int **)malloc(r * sizeof(int *));
    
    if (M == NULL) {
        printf("Errore malloc righe\n");
        exit(1);
    }

    // PASSO B: Allocare le singole righe.
    for (int i = 0; i < r; i++) {
        // Usiamo sizeof(int) perché qui memorizziamo i numeri veri e propri.
        M[i] = (int *)malloc(c * sizeof(int));
        if (M[i] == NULL) {
            printf("Errore malloc colonne\n");
            exit(1);
        }
    }

    /* --- 3. ESECUZIONE --- */
    // Passiamo M, le dimensioni e il vettore V alla funzione
    buildAndPrint(V, N, M, r, c);

    /* --- 4. GESTIONE MEMORIA (FREE) --- */
    // Regola d'oro: Liberare in ordine inverso all'allocazione.
    // Prima liberiamo i "figli" (le righe di interi)...
    for (int i = 0; i < r; i++) {
        free(M[i]); // Libera la singola riga i
    }
    
    // ...poi liberiamo il "padre" (l'array di puntatori).
    free(M);
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}