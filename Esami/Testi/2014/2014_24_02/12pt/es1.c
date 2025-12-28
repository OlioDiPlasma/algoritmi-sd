#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: ruotaVettore
 * ============================================================================
 * SCOPO:
 * Esegue una rotazione circolare (shift) degli elementi di un vettore.
 * - Se D > 0: Rotazione verso DESTRA (gli ultimi D elementi vanno all'inizio).
 * - Se D < 0: Rotazione verso SINISTRA (i primi |D| elementi vanno alla fine).
 *
 * PARAMETRI:
 * - int *v:  Puntatore al vettore di interi da modificare (in-place).
 * - int N:   Dimensione totale del vettore.
 * - int D:   Numero di posizioni di rotazione (positivo o negativo).
 *
 * CONCETTI CHIAVE:
 * 1. Complessità O(N):
 * Invece di spostare di 1 passo per D volte, usiamo un buffer ausiliario.
 * Salviamo gli elementi che "cadrebbero" fuori dal vettore, spostiamo
 * gli altri, e rimettiamo quelli salvati. Ogni elemento viene toccato
 * un numero costante di volte.
 *
 * 2. Gestione Memoria (Malloc):
 * Il buffer ausiliario ha dimensione variabile |D|. Dobbiamo allocarlo
 * dinamicamente nello HEAP perché non conosciamo D a priori.
 * ============================================================================
 */
void ruotaVettore(int *v, int N, int D) {
    
    /* --- 1. GESTIONE CASI BASE E NORMALIZZAZIONE --- */
    /* Se il vettore è vuoto o la rotazione è 0, non facciamo nulla */
    if (v == NULL || N <= 0 || D == 0) {
        return;
    }

    /* * Normalizziamo D. Se D è maggiore di N (es. ruotare di 12 su un vettore di 10),
     * equivale a ruotare di 2. Usiamo l'operatore modulo %.
     */
    D = D % N; 
    if (D == 0) return; // Se dopo il modulo è 0, fine.

    /* Variabile per l'array ausiliario */
    int *aux = NULL;

    /* --- 2. LOGICA ROTAZIONE A DESTRA (D > 0) --- */
    /* Esempio: [1 2 3 4 5], D=2 -> Output atteso: [4 5 1 2 3]
     * Strategia:
     * A. Salviamo gli ULTIMI D elementi (4, 5) in 'aux'.
     * B. Spostiamo i primi N-D elementi (1, 2, 3) verso destra.
     * C. Copiamo 'aux' all'inizio.
     */
    if (D > 0) {
        
        /* Allocazione memoria per i D elementi che "ruotano" */
        aux = (int *)malloc(D * sizeof(int));
        if (aux == NULL) { printf("Errore Malloc\n"); return; }

        /* A. SALVATAGGIO (Backup degli ultimi D elementi) */
        /* Indici: partiamo da N-D fino a N-1 */
        for (int i = 0; i < D; i++) {
            aux[i] = v[N - D + i]; 
        }

        /* B. SPOSTAMENTO INTERNO (Shift verso destra) */
        /* * CRITICO: Dobbiamo iterare ALL'INDIETRO!
         * Se andassimo in avanti, sovrascriveremmo i dati che dobbiamo ancora spostare.
         * Spostiamo l'elemento in pos 'i' alla pos 'i + D'.
         */
        for (int i = N - D - 1; i >= 0; i--) {
            v[i + D] = v[i];
        }

        /* C. RIPRISTINO (Copia dal buffer all'inizio) */
        for (int i = 0; i < D; i++) {
            v[i] = aux[i];
        }
    }

    /* --- 3. LOGICA ROTAZIONE A SINISTRA (D < 0) --- */
    /* Esempio: [1 2 3 4 5], D=-2 -> Output atteso: [3 4 5 1 2]
     * Strategia:
     * A. Salviamo i PRIMI |D| elementi (1, 2) in 'aux'.
     * B. Spostiamo i restanti verso sinistra.
     * C. Copiamo 'aux' alla fine.
     */
    else if (D < 0) {
        int K = -D; // Rendiamo positivo il numero di posizioni (es. 2)
        
        /* Allocazione memoria per i K elementi */
        aux = (int *)malloc(K * sizeof(int));
        if (aux == NULL) { printf("Errore Malloc\n"); return; }

        /* A. SALVATAGGIO (Backup dei primi K elementi) */
        for (int i = 0; i < K; i++) {
            aux[i] = v[i];
        }

        /* B. SPOSTAMENTO INTERNO (Shift verso sinistra) */
        /* Qui possiamo iterare in avanti tranquillamente */
        for (int i = 0; i < N - K; i++) {
            /* L'elemento che era in i+K viene spostato indietro in i */
            v[i] = v[i + K];
        }

        /* C. RIPRISTINO (Copia dal buffer alla fine) */
        /* La fine inizia all'indice N - K */
        for (int i = 0; i < K; i++) {
            v[N - K + i] = aux[i];
        }
    }

    /* --- 4. PULIZIA MEMORIA --- */
    /* * Regola d'oro: Se usi malloc, devi usare free.
     * Liberiamo il buffer temporaneo prima di uscire dalla funzione.
     */
    if (aux != NULL) {
        free(aux);
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Testare la rotazione con l'esempio specifico fornito nel testo.
 * Input: N=10, D=3.
 * Vettore: 5, 4, 10, -1, 7, 20, 11, -3, 6, 34
 * Atteso: -3, 6, 34, 5, 4, 10, -1, 7, 20, 11
 * ============================================================================
 */
int main() {
    /* Dati di prova hardcoded come da esempio */
    int vettore[] = {5, 4, 10, -1, 7, 20, 11, -3, 6, 34};
    int N = 10;
    int D = 3;

    /* Stampa stato iniziale */
    printf("Vettore Iniziale: [ ");
    for(int i = 0; i < N; i++) printf("%d ", vettore[i]);
    printf("]\n");

    printf("Eseguo rotazione di %d posizioni (destra)...\n", D);

    /* Chiamata alla funzione */
    ruotaVettore(vettore, N, D);

    /* Stampa risultato */
    printf("Vettore Ruotato:  [ ");
    for(int i = 0; i < N; i++) printf("%d ", vettore[i]);
    printf("]\n");

    /* --- VERIFICA AUTOMATICA (Opzionale ma utile) --- */
    /* Controlliamo se corrisponde all'esempio del testo */
    int atteso[] = {-3, 6, 34, 5, 4, 10, -1, 7, 20, 11};
    int corretto = 1;
    for(int i=0; i<N; i++) {
        if(vettore[i] != atteso[i]) corretto = 0;
    }

    if(corretto) printf("\n--> TEST SUPERATO!\n");
    else printf("\n--> TEST FALLITO.\n");

    /* * Nota: Non serve fare 'free(vettore)' qui perché 'vettore' è 
     * allocato staticamente nello STACK del main, non con malloc.
     * La 'free' importante è quella dentro la funzione 'ruotaVettore'.
     */
    return 0;
}