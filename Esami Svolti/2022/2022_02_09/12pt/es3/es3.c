#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: check_sums_recursive (Core / Worker)
 * ======================================================================================
 * SCOPO:
 * Esplora ricorsivamente le combinazioni per verificare se è possibile riempire
 * i "contenitori" definiti da S usando gli elementi di I.
 *
 * PARAMETRI:
 * - I: Vettore degli elementi disponibili (Input).
 * - dimI: Dimensione di I.
 * - i_idx: Indice corrente che stiamo valutando in I (livello della ricorsione).
 * - S: Vettore delle somme obiettivo (Input).
 * - dimS: Dimensione di S.
 * - current_S: Vettore di appoggio che mantiene le somme parziali accumulate finora.
 *
 * RITORNO:
 * - 1 (true) se abbiamo trovato una soluzione valida.
 * - 0 (false) se questo ramo non porta a una soluzione.
 * ======================================================================================
 */
int check_sums_recursive(int *I, int dimI, int i_idx, int *S, int dimS, int *current_S) {

    /* --- 1. CONTROLLO CONDIZIONE DI VITTORIA (Base Case) --- */
    /*
     * Dobbiamo verificare se TUTTE le somme parziali in current_S
     * corrispondono esattamente agli obiettivi in S.
     */
    int completi = 1;
    for (int k = 0; k < dimS; k++) {
        if (current_S[k] != S[k]) {
            completi = 0;
            break; /* Basta uno sbagliato per non aver finito */
        }
    }
    if (completi) return 1; /* Trovata soluzione! Risaliamo. */


    /* --- 2. CONTROLLO FINE ELEMENTI (Base Case Fail) --- */
    /* Se abbiamo finito gli elementi di I ma non abbiamo soddisfatto la condizione sopra */
    if (i_idx == dimI) return 0;


    /* --- 3. LOGICA RICORSIVA (Backtracking) --- */
    
    /* OPZIONE A: SCARTA l'elemento corrente I[i_idx].
     * Proviamo a risolvere il problema saltando questo numero.
     */
    if (check_sums_recursive(I, dimI, i_idx + 1, S, dimS, current_S)) {
        return 1; /* Se saltandolo troviamo una soluzione, ritorniamo 1 a catena */
    }

    /* OPZIONE B: USA l'elemento corrente.
     * Dobbiamo provare a inserirlo in uno qualsiasi dei "contenitori" S,
     * ma solo se c'è spazio (Pruning).
     */
    for (int k = 0; k < dimS; k++) {

        /* --- PRUNING (Potatura) --- */
        /* Controlliamo se aggiungendo I[i_idx] al contenitore k "sforiamo" il target S[k].
         * Se sforiamo, non entriamo nemmeno nell'if (tagliamo il ramo).
         */
        if (current_S[k] + I[i_idx] <= S[k]) {
            
            /* DO (Fai): Aggiungi l'elemento alla somma parziale k */
            current_S[k] += I[i_idx];

            /* RECURSE (Scendi): Chiama la funzione per il prossimo elemento */
            if (check_sums_recursive(I, dimI, i_idx + 1, S, dimS, current_S)) {
                return 1; /* Trovato! */
            }

            /* BACKTRACK (Disfa): Se la chiamata sopra ha ritornato 0 (fallimento),
             * dobbiamo annullare la mossa per provare il prossimo contenitore k
             * o per risalire.
             */
            current_S[k] -= I[i_idx];
        }
    }

    /* Se nessuna opzione (nè scartarlo, nè metterlo in alcun contenitore) funziona: */
    return 0;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: risolvi_problema (Wrapper)
 * ======================================================================================
 * SCOPO:
 * Prepara le strutture dati necessarie (il vettore current_S) e lancia la ricorsione.
 *
 * GESTIONE MEMORIA:
 * Alloca current_S usando calloc per garantire che parta da zero.
 * Libera la memoria prima di ritornare il risultato.
 * ======================================================================================
 */
int risolvi_problema(int *I, int dimI, int *S, int dimS) {
    /*
     * Allocazione vettore di appoggio per le somme correnti.
     * Usiamo calloc(num, size) perché inizializza tutto a 0 automaticamente.
     * Se usassimo malloc, conterrebbe valori "spazzatura".
     */
    int *current_S = (int *)calloc(dimS, sizeof(int));
    if (current_S == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* Chiamata alla funzione ricorsiva (parte dall'indice 0 di I) */
    int risultato = check_sums_recursive(I, dimI, 0, S, dimS, current_S);

    /* Pulizia memoria */
    free(current_S);

    return risultato;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- TEST CASE 1 (Dall'immagine) --- */
    printf("--- TEST 1 ---\n");
    int I1[] = {1, 2, 3, 4, 5, 6};
    int S1[] = {1, 7, 7};
    /* Notare: sizeof(I1)/sizeof(I1[0]) calcola automaticamente la lunghezza */
    
    if (risolvi_problema(I1, 6, S1, 3)) {
        printf("Es. 1: POSSIBILE (Corretto)\n");
    } else {
        printf("Es. 1: NON POSSIBILE (Errore)\n");
    }

    /* --- TEST CASE 2 (Dall'immagine) --- */
    printf("\n--- TEST 2 ---\n");
    int I2[] = {1, 3, 4, 5, 5};
    int S2[] = {7, 7};

    if (risolvi_problema(I2, 5, S2, 2)) {
        printf("Es. 2: POSSIBILE (Errore)\n");
    } else {
        printf("Es. 2: NON POSSIBILE (Corretto)\n");
    }

    /* --- TEST AGGIUNTIVO: Somma troppo grande --- */
    printf("\n--- TEST 3 (Fallimento immediato) ---\n");
    int I3[] = {1, 1};
    int S3[] = {100}; // Impossibile
    if (risolvi_problema(I3, 2, S3, 1)) printf("Possibile\n");
    else printf("Non Possibile\n");

    return 0;
}