#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* Necessario per INT_MIN (il numero intero più piccolo possibile) */



//COMMENTO SUL RISULTATO : prende 2,3 ma anche 1,3,1 andrebbe bene semplicemente esce prima 2,3

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaSottoVettoreProprioMax
 * ======================================================================================
 * SCOPO:
 * Trovare e stampare il sottovettore contiguo con la somma degli elementi massima,
 * rispettando il vincolo che il sottovettore sia "proprio" (lunghezza < N).
 *
 * PARAMETRI:
 * - int *V: Puntatore al primo elemento del vettore (array).
 * - int N: Dimensione totale del vettore.
 *
 * CONCETTI CHIAVE:
 * - Sottovettore Proprio: Se il vettore ha lunghezza N, noi possiamo guardare solo
 * sottosequenze di lunghezza da 1 a N-1.
 * - Forza Bruta Ottimizzata (O(N^2)): Usiamo due cicli annidati.
 * Il ciclo esterno fissa l'inizio (start), il ciclo interno allarga la finestra (end).
 * Manteniamo una somma corrente per evitare un terzo ciclo di calcolo (che porterebbe a O(N^3)).
 * - Gestione dei massimi: Inizializziamo il massimo a un valore bassissimo (INT_MIN)
 * per gestire correttamente anche vettori contenenti solo numeri negativi.
 * ======================================================================================
 */
void trovaSottoVettoreProprioMax(int *V, int N) {
    
    /* --- 1. INIZIALIZZAZIONE VARIABILI DI SUPPORTO --- */
    /* * Inizializziamo maxSomma al minimo intero rappresentabile. 
     * Se usassimo 0, sbaglieremmo nel caso di un array tipo {-5, -2, -9}.
     */
    long long maxSomma = LLONG_MIN; 
    /* Usiamo long long per evitare overflow se la somma supera i limiti di int */
    
    int bestStart = -1; // Indice di inizio del miglior sottovettore trovato
    int bestEnd = -1;   // Indice di fine del miglior sottovettore trovato

    /* --- 2. LOGICA A DOPPIO CICLO (Finestra Mobile) --- */
    
    /* Ciclo esterno: 'i' rappresenta l'indice di INIZIO del sottovettore */
    for (int i = 0; i < N; i++) {
        
        long long sommaCorrente = 0; // Azzero la somma per ogni nuovo punto di partenza
        
        /* Ciclo interno: 'j' rappresenta l'indice di FINE del sottovettore.
         * Parte da 'i' e scorre fino alla fine del vettore.
         */
        for (int j = i; j < N; j++) {
            
            /* * Aritmetica dei Puntatori / Accesso Array:
             * Accediamo al valore nella cella di memoria all'offset 'j' rispetto a V.
             * Aggiungiamo questo valore alla somma parziale.
             */
            sommaCorrente += V[j];

            /* Calcoliamo la lunghezza attuale del sottovettore [i...j] */
            int lunghezzaAttuale = j - i + 1;

            /* --- 3. VERIFICA VINCOLO "SOTTOVETTORE PROPRIO" --- */
            /* * Il testo dice: "Un sottovettore si dice proprio se contiene un numero 
             * di celle strettamente inferiore a quello del vettore da cui deriva".
             * Quindi: lunghezzaAttuale < N.
             */
            if (lunghezzaAttuale < N) {
                
                /* Se abbiamo trovato una nuova somma massima, aggiorniamo i record */
                if (sommaCorrente > maxSomma) {
                    maxSomma = sommaCorrente;
                    bestStart = i;
                    bestEnd = j;
                }
            }
        }
    }

    /* --- 4. VISUALIZZAZIONE RISULTATI --- */
    if (bestStart != -1) {
        printf("Sottovettore a somma massima (%lld): [ ", maxSomma);
        /* Ripercorriamo gli indici salvati per stampare gli elementi */
        for (int k = bestStart; k <= bestEnd; k++) {
            printf("%d ", V[k]);
        }
        printf("]\n");
    } else {
        /* Caso limite: Vettore vuoto o N=1 (non esistono sottovettori propri) */
        printf("Nessun sottovettore proprio trovato (N troppo piccolo).\n");
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI E ALLOCAZIONE MEMORIA --- */
    /* * Creiamo i dati dell'esempio nell'immagine: V = {-1, 2, 3, -6, 1, 3, 1}
     * Dimensione N = 7.
     */
    int N = 7;
    
    /* * MALLOC: Allocazione dinamica nello HEAP.
     * Chiediamo al sistema operativo uno spazio contiguo di N * 4 byte (se int è 4 byte).
     * (int*) è il cast esplicito del puntatore generico ritornato da malloc.
     */
    int *V = (int*)malloc(N * sizeof(int));

    /* Controllo difensivo fondamentale: la memoria è stata concessa? */
    if (V == NULL) {
        perror("Errore malloc");
        exit(1);
    }

    /* Riempimento manuale del vettore (come da esempio) */
    /* Nota: V[0] è equivalente a *(V + 0) */
    V[0] = -1;
    V[1] = 2;
    V[2] = 3;
    V[3] = -6;
    V[4] = 1;
    V[5] = 3;
    V[6] = 1;

    printf("--- Vettore Iniziale ---\n");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    trovaSottoVettoreProprioMax(V, N);

    /* --- 3. PULIZIA DELLA MEMORIA (FREE) --- */
    /* * FREE: Restituiamo la memoria al sistema operativo.
     * Se non lo facessimo, avremmo un "Memory Leak" (perdita di memoria),
     * che in programmi a lunga esecuzione consumerebbe tutta la RAM.
     */
    free(V);
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}

