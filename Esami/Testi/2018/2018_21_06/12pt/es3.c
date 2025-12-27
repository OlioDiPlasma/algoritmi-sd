#include <stdio.h>
#include <stdlib.h>

//A NOI è USCITA L'ULTIMA MA ANDAVANO BENE ANCHE LE ALTRE SOTTOSEQUENZE


/*
 * ======================================================================================
 * FUNZIONE: maxSubSeq
 * ======================================================================================
 * SCOPO:
 * Calcola la lunghezza della più lunga sottosequenza STRETTAMENTE crescente di un vettore X
 * e riempie il vettore Y con una di queste sottosequenze.
 * Utilizza l'approccio della Programmazione Dinamica.
 *
 * PARAMETRI:
 * - int *X: Il vettore di input contenente la sequenza di numeri interi.
 * - int N: La lunghezza del vettore X.
 * - int *Y: Un vettore (già allocato dal chiamante) dove scriveremo la sottosequenza risultato.
 * Deve essere grande almeno quanto N nel caso peggiore.
 *
 * RITORNA:
 * - int: La lunghezza 'k' della sottosequenza trovata.
 *
 * CONCETTI CHIAVE:
 * - Programmazione Dinamica (DP): Invece di ricalcolare, salviamo in un array ausiliario (L)
 * la lunghezza della LIS che termina in ogni posizione 'i'.
 * - Array dei Predecessori (P): Per ricostruire la sequenza (non solo sapere quanto è lunga),
 * dobbiamo sapere "da quale indice veniamo". P[i] conterrà l'indice dell'elemento precedente
 * a X[i] nella sottosequenza ottima.
 * - Ricostruzione a ritroso: Una volta trovato l'ultimo elemento della sequenza ottimale,
 * torniamo indietro usando l'array P per riempire Y.
 * ======================================================================================
 */
int maxSubSeq(int *X, int N, int *Y) {
    /* --- 1. GESTIONE CASI LIMITE --- */
    if (N <= 0 || X == NULL) return 0;

    /* --- 2. ALLOCAZIONE STRUTTURE DATI DI SUPPORTO --- */
    
    /* L[i] conterrà la lunghezza della LIS che termina all'indice i.
       Usiamo malloc: chiediamo al sistema operativo N * sizeof(int) byte. */
    int *L = (int *)malloc(N * sizeof(int));
    
    /* P[i] conterrà l'indice del predecessore di X[i] nella LIS.
       Serve per ricostruire il percorso alla fine. */
    int *P = (int *)malloc(N * sizeof(int));

    /* Controllo difensivo: sempre verificare se malloc ha avuto successo! */
    if (L == NULL || P == NULL) {
        printf("Errore: memoria insufficiente per i vettori di supporto.\n");
        exit(1);
    }

    /* --- 3. INIZIALIZZAZIONE --- */
    /* Inizialmente, ogni elemento è una sottosequenza crescente lunga 1 (se stesso).
       Il predecessore è -1 (nessuno). */
    for (int i = 0; i < N; i++) {
        L[i] = 1;
        P[i] = -1;
    }

    /* --- 4. LOGICA CORE (Programmazione Dinamica) - O(N^2) --- */
    /* * Ciclo esterno (i): consideriamo ogni elemento X[i] come possibile fine della sequenza.
     * Ciclo interno (j): guardiamo tutti gli elementi precedenti (da 0 a i-1).
     */
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            
            /* CONDIZIONE 1: Crescenza stretta (X[j] < X[i]) 
             * CONDIZIONE 2: Miglioramento (L[j] + 1 > L[i])
             * Se attaccando X[i] alla sequenza che finisce in X[j] otteniamo una lunghezza
             * maggiore di quella che avevamo trovato finora per X[i], aggiorniamo.
             */
            if (X[j] < X[i] && L[j] + 1 > L[i]) {
                L[i] = L[j] + 1; // La nuova lunghezza è quella di j + 1 (l'elemento i)
                P[i] = j;        // Il padre di i è j
            }
        }
    }

    /* --- 5. INDIVIDUAZIONE DEL MASSIMO --- */
    /* Abbiamo riempito L. Ora dobbiamo trovare QUAL è l'indice dove finisce 
       la sottosequenza più lunga in assoluto. */
    int maxLength = 0;
    int indexMax = 0; // L'indice dell'ultimo elemento della LIS
    
    for (int i = 0; i < N; i++) {
        if (L[i] > maxLength) {
            maxLength = L[i];
            indexMax = i;
        }
    }

    /* --- 6. RICOSTRUZIONE SEQUENZA (Backtracking) --- */
    /* Ora sappiamo che la sequenza è lunga 'maxLength' e finisce a 'indexMax'.
       Dobbiamo riempire Y. Attenzione: risaliamo dai padri, quindi andiamo a ritroso! */
    
    int curr = indexMax;
    int k = maxLength; // Indice per scorrere Y (partiamo dalla fine)

    /* Ciclo finché ci sono predecessori (o finché k > 0) */
    while (curr != -1 && k > 0) {
        /*
         * DETTAGLIO SINTATTICO:
         * Y[k - 1] = X[curr];
         * Accediamo all'array Y. Siccome k parte dalla lunghezza (es. 6), l'ultimo indice
         * valido è 5. Quindi usiamo k-1. Poi decrementiamo k.
         */
        Y[k - 1] = X[curr]; 
        k--;                // Spostiamo il cursore di Y indietro
        curr = P[curr];     // Saltiamo al predecessore salvato in P
    }

    /* --- 7. PULIZIA MEMORIA --- */
    /* Fondamentale liberare la memoria temporanea allocata per L e P per evitare Memory Leak.
       Non liberiamo Y perché serve al chiamante. */
    free(L);
    free(P);

    return maxLength;
}

/*
 * ======================================================================================
 * FUNZIONE: main
 * ======================================================================================
 * SCOPO:
 * Ambiente di test per verificare la funzione. 
 * Crea i dati dell'esempio, alloca lo spazio per il risultato, chiama la funzione e stampa.
 * ======================================================================================
 */
int main() {
    printf("--- INIZIO TEST LIS ---\n");

    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /* Vettore X fornito dall'esempio del testo */
    int X[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    int N = sizeof(X) / sizeof(X[0]); // Calcolo automatico dimensione

    /* --- 2. ALLOCAZIONE VETTORE RISULTATO --- */
    /* Non sappiamo a priori quanto sarà lunga la sottosequenza, ma non può essere
       più lunga di N. Allocare N è la scelta sicura. */
    int *Y = (int *)malloc(N * sizeof(int));
    if (Y == NULL) {
        perror("Errore malloc nel main");
        return 1;
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    printf("Analisi vettore X di %d elementi...\n", N);
    int lunghezza = maxSubSeq(X, N, Y);

    /* --- 4. STAMPA RISULTATI --- */
    printf("\nLunghezza massima calcolata: %d\n", lunghezza);
    printf("Sottosequenza trovata in Y: ");
    
    printf("[ ");
    for (int i = 0; i < lunghezza; i++) {
        /* Stampa formattata degli elementi validi di Y */
        printf("%d ", Y[i]);
    }
    printf("]\n");

    /* --- 5. PULIZIA FINALE --- */
    free(Y); // Liberiamo il buffer che abbiamo creato nel main
    printf("\n--- FINE TEST ---\n");

    return 0;
}