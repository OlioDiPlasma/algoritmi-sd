#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> /* Per usare true/false, rende il codice più leggibile */

/*
 * Funzione Helper: is_vowel
 * -------------------------
 * Scopo: Determina se un carattere è una vocale minuscola.
 * Ritorna: true se è vocale, false altrimenti.
 */
bool is_vowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

/*
 * ======================================================================================
 * FUNZIONE: filter_matrix
 * ======================================================================================
 *
 * Scopo:
 * Analizza la matrice M e ne crea una nuova (M') contenente solo l'intersezione
 * delle righe e delle colonne che NON contengono vocali nella matrice originale.
 *
 * Parametri:
 * - char **M:       La matrice originale (array di puntatori a stringhe/char).
 * - int r, int c:   Dimensioni originali (righe, colonne).
 * - char ***outM:   Puntatore alla variabile che conterrà l'indirizzo della nuova matrice.
 * - int *outR:      Puntatore dove scriveremo il numero di righe della nuova matrice.
 * - int *outC:      Puntatore dove scriveremo il numero di colonne della nuova matrice.
 *
 * Concetti Chiave (Il Triplo Puntatore):
 * Perché `char ***outM`?
 * Nel main avremo una variabile `char **nuovaMatrice`. Vogliamo che la funzione
 * modifichi il VALORE di questa variabile (cioè l'indirizzo di memoria a cui punta).
 * In C, per modificare una variabile di tipo T, dobbiamo passare T*.
 * Qui T è `char**`, quindi passiamo `char***`.
 * Useremo la dereferenziazione `*outM` per accedere alla variabile del main.
 * ======================================================================================
 */
void filter_matrix(char **M, int r, int c, char ***outM, int *outR, int *outC) {

    /* --- 1. ANALISI ---
       Dobbiamo prima capire quali righe e quali colonne sono "salve".
       Usiamo due array di flag (booleani) per segnarlo.
    */

    // Allocazione dinamica di array temporanei per i flag.
    // Usiamo calloc per inizializzare tutto a false (0), ipotizzando inizialmente che non ci siano vocali.
    // Tuttavia, per logica "keep", inizializziamo a true e mettiamo false se troviamo vocali.
    bool *keepRow = (bool *)malloc(r * sizeof(bool));
    bool *keepCol = (bool *)malloc(c * sizeof(bool));

    // Inizializziamo tutto a "true" (da tenere), poi escludiamo se troviamo vocali.
    for (int i = 0; i < r; i++) keepRow[i] = true;
    for (int j = 0; j < c; j++) keepCol[j] = true;

    // Scansione della matrice originale M
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            // Accediamo al carattere alla riga i, colonna j: M[i][j]
            if (is_vowel(M[i][j])) {
                // Se trovo una vocale, invalido sia la riga corrente che la colonna corrente.
                keepRow[i] = false;
                keepCol[j] = false;
            }
        }
    }

    /* --- 2. CALCOLO DIMENSIONI ---
       Contiamo quanti true sono rimasti per determinare le dimensioni della nuova matrice.
    */
    int newR = 0;
    int newC = 0;

    for (int i = 0; i < r; i++) {
        if (keepRow[i]) newR++;
    }
    for (int j = 0; j < c; j++) {
        if (keepCol[j]) newC++;
    }

    /* --- 3. GESTIONE OUTPUT E CASO LIMITE ---
       Scriviamo le nuove dimensioni nelle variabili puntate dai parametri.
    */
    // Dereferenzio i puntatori per scrivere nella memoria del chiamante (main)
    *outR = newR;
    *outC = newC;

    // Caso speciale: matrice nulla (0x0)
    if (newR == 0 || newC == 0) {
        *outM = NULL; // Assegno NULL al puntatore nel main
        // Importante: liberare la memoria temporanea prima di uscire!
        free(keepRow);
        free(keepCol);
        return;
    }

    /* --- 4. ALLOCAZIONE NUOVA MATRICE ---
       Qui usiamo *outM per dire "assegna questo indirizzo alla variabile nel main".
    */

    // Passo A: Allocare l'array di puntatori alle righe
    // Sintassi: (*outM) è la variabile 'matrice' del main.
    *outM = (char **)malloc(newR * sizeof(char *));

    // Passo B: Allocare ogni singola riga
    for (int i = 0; i < newR; i++) {
        // (*outM)[i] accede all'i-esimo puntatore riga della nuova matrice
        (*outM)[i] = (char *)malloc(newC * sizeof(char));
    }

    /* --- 5. POPOLAMENTO (COPIA DATI) ---
       Dobbiamo copiare i dati da M a *outM saltando le righe/colonne scartate.
       Servono indici separati per la nuova matrice.
    */
    int current_new_row = 0;

    for (int i = 0; i < r; i++) {
        // Se la riga i originale è da scartare, saltiamo al prossimo giro del ciclo for (continue)
        if (!keepRow[i]) continue;

        int current_new_col = 0; // Reset indice colonna per ogni nuova riga valida
        for (int j = 0; j < c; j++) {
            // Se la colonna j originale è da scartare, saltiamo
            if (!keepCol[j]) continue;

            // COPIA EFFETTIVA:
            // M[i][j] -> dato originale
            // (*outM)[current_new_row][current_new_col] -> destinazione
            (*outM)[current_new_row][current_new_col] = M[i][j];

            current_new_col++; // Avanziamo nella nuova colonna solo se abbiamo copiato
        }
        current_new_row++; // Avanziamo nella nuova riga solo se era valida
    }

    /* --- 6. PULIZIA LOCALE --- */
    // Liberiamo gli array di supporto, non servono più.
    free(keepRow);
    free(keepCol);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Dati dell'esempio del testo (4 righe, 5 colonne)
    int R = 4, C = 5;

    // Creazione matrice M "hardcoded" per il test
    // Usiamo malloc per simulare una situazione realistica dinamica
    char **M = (char **)malloc(R * sizeof(char *));
    
    // Dati grezzi per riempire facilmente
    // Nota: M[0] ha 'a','e' -> VIA. M[2] ha 'e' -> VIA.
    // Col 0 ('a'), Col 1 (ok), Col 2 (ok), Col 3 ('e'), Col 4 (ok).
    char row0[] = {'a', 'c', 'f', 'e', 'g'};
    char row1[] = {'z', 'y', 't', 't', 'p'};
    char row2[] = {'q', 'w', 'j', 'e', 't'};
    char row3[] = {'p', 'l', 'l', 'n', 'm'};
    
    // Assegnazione e allocazione righe
    M[0] = malloc(C); for(int k=0; k<C; k++) M[0][k] = row0[k];
    M[1] = malloc(C); for(int k=0; k<C; k++) M[1][k] = row1[k];
    M[2] = malloc(C); for(int k=0; k<C; k++) M[2][k] = row2[k];
    M[3] = malloc(C); for(int k=0; k<C; k++) M[3][k] = row3[k];

    printf("--- Matrice Originale (%dx%d) ---\n", R, C);
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) printf("%c ", M[i][j]);
        printf("\n");
    }

    // Variabili per ricevere i risultati
    char **newM = NULL; // Qui finirà la nuova matrice
    int newR, newC;     // Qui finiranno le nuove dimensioni

    // CHIAMATA ALLA FUNZIONE
    // Notate l'operatore & (indirizzo di) davanti a newM, newR, newC.
    // Stiamo passando l'indirizzo del puntatore newM, quindi passiamo un char***
    filter_matrix(M, R, C, &newM, &newR, &newC);

    printf("\n--- Matrice Filtrata (%dx%d) ---\n", newR, newC);
    if (newM != NULL) {
        for(int i=0; i<newR; i++) {
            printf("[ ");
            for(int j=0; j<newC; j++) {
                printf("%c ", newM[i][j]);
            }
            printf("]\n");
        }
    } else {
        printf("Matrice vuota risultante.\n");
    }

    // PULIZIA MEMORIA (Fondamentale!)
    // 1. Liberare M originale
    for(int i=0; i<R; i++) free(M[i]);
    free(M);

    // 2. Liberare newM (se esiste)
    if (newM != NULL) {
        for(int i=0; i<newR; i++) free(newM[i]);
        free(newM);
    }

    return 0;
}