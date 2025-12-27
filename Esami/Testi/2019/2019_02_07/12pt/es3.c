#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaEStampaBitonica
 * ======================================================================================
 * SCOPO:
 * Data una sequenza di interi, individua e stampa a video la sottosequenza bitonica
 * (prima crescente, poi decrescente) di lunghezza massima.
 *
 * PARAMETRI:
 * - int *V: Il puntatore al primo elemento del vettore (array) di input.
 * - int N: Il numero totale di elementi nel vettore.
 *
 * CONCETTI CHIAVE:
 * - Programmazione Dinamica: Usiamo due array di supporto (lis e lds) per memorizzare
 * i risultati parziali ed evitare di ricalcolarli continuamente.
 * > lis[i] (Longest Increasing Subsequence): lunghezza della seq. crescente che finisce in i.
 * > lds[i] (Longest Decreasing Subsequence): lunghezza della seq. decrescente che inizia in i.
 * - Ricostruzione della soluzione: Una volta trovate le lunghezze, dobbiamo "tornare indietro"
 * per trovare quali numeri specifici compongono la sequenza.
 * ======================================================================================
 */
void trovaEStampaBitonica(int *V, int N) {

    /* --- 1. ALLOCAZIONE MEMORIA PER STRUTTURE DATI DI SUPPORTO --- */
    
    /* * Allocazione dinamica per l'array LIS (Crescente da sinistra).
     * malloc ritorna un puntatore void*, facciamo il cast a (int*).
     * sizeof(int) ci dà la dimensione in byte di un intero. Moltiplichiamo per N.
     */
    int *lis = (int*)malloc(N * sizeof(int));
    
    /* Allocazione dinamica per l'array LDS (Decrescente verso destra) */
    int *lds = (int*)malloc(N * sizeof(int));

    /* Controllo difensivo: sempre verificare se la memoria è piena */
    if (lis == NULL || lds == NULL) {
        printf("Errore di allocazione memoria.\n");
        exit(1);
    }

    /* --- 2. CALCOLO LIS (Longest Increasing Subsequence) --- */
    /* Inizializziamo ogni posizione a 1, perché ogni numero è una sequenza lunga 1 da solo */
    for (int i = 0; i < N; i++) {
        lis[i] = 1;
    }

    /*
     * Doppio ciclo annidato:
     * Per ogni elemento 'i', guardiamo tutti i precedenti 'j'.
     * Se V[i] è maggiore di V[j], significa che possiamo estendere la sequenza che finiva in 'j'.
     */
    for (int i = 1; i < N; i++) {         // Scorre l'array in avanti
        for (int j = 0; j < i; j++) {     // Controlla tutti i predecessori
            
            /* * Condizione LIS:
             * 1. V[i] > V[j]: Stiamo crescendo.
             * 2. lis[i] < lis[j] + 1: Abbiamo trovato un percorso più lungo di quello che conoscevamo.
             */
            if (V[i] > V[j] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1; // Aggiorniamo la lunghezza massima trovata finora per i
            }
        }
    }

    /* --- 3. CALCOLO LDS (Longest Decreasing Subsequence) --- */
    /* * La logica è identica alla LIS, ma partiamo dalla fine (destra) verso l'inizio (sinistra).
     * Questo ci dice quanto è lunga la discesa partendo da i andando verso destra.
     */
    for (int i = 0; i < N; i++) {
        lds[i] = 1;
    }

    for (int i = N - 2; i >= 0; i--) {    // Scorre l'array all'indietro
        for (int j = N - 1; j > i; j--) { // Controlla tutti i successori (a destra)
            
            /* * Condizione LDS:
             * 1. V[i] > V[j]: Stiamo scendendo (guardando verso destra, i è più alto di j).
             * 2. lds[i] < lds[j] + 1: Trovato un percorso di discesa più lungo.
             */
            if (V[i] > V[j] && lds[i] < lds[j] + 1) {
                lds[i] = lds[j] + 1;
            }
        }
    }

    /* --- 4. INDIVIDUAZIONE DEL PICCO --- */
    /*
     * La lunghezza totale della sequenza bitonica che ha picco in 'i' è:
     * (Lunghezza salita fino a i) + (Lunghezza discesa da i) - 1
     * Togliamo 1 perché l'elemento V[i] viene contato sia nella salita che nella discesa.
     */
    int maxLen = 0;
    int piccoIndex = -1; // Indice dell'elemento che funge da vertice della piramide

    for (int i = 0; i < N; i++) {
        int lunghezzaCorrente = lis[i] + lds[i] - 1;
        if (lunghezzaCorrente > maxLen) {
            maxLen = lunghezzaCorrente;
            piccoIndex = i;
        }
    }

    /* --- 5. RICOSTRUZIONE E STAMPA (Parte Complessa) --- */
    printf("Lunghezza massima: %d\n", maxLen);
    printf("Sequenza: ");

    /* * A. Stampiamo la parte crescente (SINISTRA DEL PICCO)
     * Dobbiamo ricostruire il percorso all'indietro dal picco, ma stamparlo in ordine.
     * Usiamo un array temporaneo o ricorsione. Qui usiamo un array temporaneo per semplicità didattica.
     */
    
    // Allocazione buffer temporaneo per la parte sinistra
    int *bufferSinistra = (int*)malloc(lis[piccoIndex] * sizeof(int)); 
    int count = 0;
    int currIndex = piccoIndex;
    int currLen = lis[piccoIndex]; // Lunghezza che dobbiamo cercare

    // Partiamo dal picco e andiamo indietro
    bufferSinistra[count++] = V[piccoIndex]; // Il picco è l'ultimo della salita
    currLen--;

    for (int k = piccoIndex - 1; k >= 0; k--) {
        /*
         * Cerchiamo un elemento k che:
         * 1. Sia precedente al corrente (k < currIndex) -> già garantito dal loop
         * 2. Sia minore del corrente (V[k] < V[currIndex]) -> condizione di salita
         * 3. Abbia una LIS esattamente inferiore di 1 (lis[k] == currLen) -> è il passo precedente
         */
        if (lis[k] == currLen && V[k] < V[currIndex]) {
            bufferSinistra[count++] = V[k];
            currIndex = k; // Spostiamo il puntatore indietro
            currLen--;     // Cerchiamo il prossimo gradino più basso
        }
    }

    // Stampiamo il buffer al contrario (perché l'abbiamo riempito dal picco all'inizio)
    // Nota: count-1 è il picco, ma lo stamperemo dopo o decidiamo di stamparlo qui.
    // Strategia pulita: Stampiamo bufferSinistra da fine a inizio (escluso picco se vogliamo unirlo dopo, 
    // ma qui stampiamo tutto fino al picco incluso).
    for (int k = count - 1; k >= 0; k--) {
        printf("%d ", bufferSinistra[k]);
    }

    /* * B. Stampiamo la parte decrescente (DESTRA DEL PICCO)
     * Qui è più facile: andiamo in avanti dal picco.
     */
    currIndex = piccoIndex;
    currLen = lds[piccoIndex];
    currLen--; // Cerchiamo il prossimo, che sarà lungo lds[picco] - 1

    for (int k = piccoIndex + 1; k < N; k++) {
        /*
         * Cerchiamo elemento k che:
         * 1. Sia minore del corrente (V[k] < V[currIndex]) -> condizione discesa
         * 2. Abbia una LDS esattamente inferiore di 1 (lds[k] == currLen)
         */
        if (lds[k] == currLen && V[k] < V[currIndex]) {
            printf("%d ", V[k]);
            currIndex = k; // Avanziamo
            currLen--;
        }
    }
    printf("\n");

    /* --- 6. GESTIONE MEMORIA (PULIZIA) --- */
    /* Liberiamo tutta la memoria allocata nello heap */
    free(lis);
    free(lds);
    free(bufferSinistra);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /* Usiamo l'esempio fornito nel testo dell'esercizio */
    int V[] = {4, 2, 5, 9, 7, 6, 10, 3, 1};
    /* Calcolo dinamico della dimensione dell'array: dimensione totale byte / dim un elemento */
    int N = sizeof(V) / sizeof(V[0]);

    printf("--- Test Programmazione Bitonica ---\n");
    printf("Array Input: ");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    trovaEStampaBitonica(V, N);

    /* --- 3. SECONDO TEST (Esempio sequenza solo crescente) --- */
    /* Il testo dice che anche una sequenza solo crescente è bitonica */
    int V2[] = {1, 2, 3, 4, 5};
    int N2 = 5;
    
    printf("\n--- Test Sequenza Solo Crescente ---\n");
    printf("Array Input: ");
    for(int i=0; i<N2; i++) printf("%d ", V2[i]);
    printf("\n");
    trovaEStampaBitonica(V2, N2);

    return 0;
}