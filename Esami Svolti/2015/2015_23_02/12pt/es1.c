#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: invertSequence
 * ======================================================================================
 * SCOPO:
 * Copia gli elementi dal vettore 'v1' al vettore 'v2', ma con una regola speciale:
 * ogni sottosequenza di numeri crescenti in 'v1' deve essere scritta in ordine decrescente
 * (invertita) in 'v2'.
 *
 * Esempio:
 * v1: {1, 2, 3, 0, 4, 5}
 * ^-----^  ^  ^---^
 * Seq 1    Seq2 Seq3
 *
 * v2: {3, 2, 1, 0, 5, 4}
 *
 * PARAMETRI:
 * * v1 : (int *) Puntatore al vettore sorgente.
 * n    : (int) Numero totale di elementi.
 * * v2 : (int *) Puntatore al vettore destinazione (già allocato dal chiamante).
 *
 * CONCETTI CHIAVE:
 * 1. Identificazione dei 'Run': Dobbiamo scorrere v1 e capire dove finisce una sequenza
 * crescente. Una sequenza finisce quando il numero successivo è MINORE o UGUALE al corrente,
 * oppure quando finisce l'array.
 * 2. Short-Circuit Evaluation: Nel controllo if(i == n-1 || v1[i+1] <= v1[i]), l'ordine è vitale.
 * Controlliamo prima se siamo alla fine (i == n-1). Se è vero, il C non valuta la seconda parte,
 * evitando di leggere memoria fuori dall'array (Segmentation Fault).
 * ======================================================================================
 */
void invertSequence(int *v1, int n, int *v2) {
    
    /* --- 1. DEFINIZIONE VARIABILI DI STATO --- */
    
    /* 'start' tiene traccia dell'indice dove è iniziata la sequenza crescente corrente */
    int start = 0; 
    
    /* 'k' è l'indice di scrittura per il vettore v2. Avanza man mano che scriviamo. */
    int k = 0;

    /* --- 2. CICLO DI SCANSIONE (VETTORE SORGENTE) --- */
    for (int i = 0; i < n; i++) {
        
        /* * VERIFICA FINE SEQUENZA
         * Una sequenza crescente termina se:
         * A. Siamo arrivati all'ultimo elemento dell'array (i == n - 1).
         * B. Il prossimo elemento (v1[i+1]) è minore o uguale al corrente (rottura crescenza).
         */
        if (i == n - 1 || v1[i + 1] <= v1[i]) {
            
            /* --- 3. COPIA E INVERSIONE --- */
            /*
             * Abbiamo individuato un blocco che va da 'start' a 'i'.
             * Dobbiamo copiarlo in v2, ma al contrario.
             * Usiamo un indice 'j' che parte dalla fine del blocco (i) e torna indietro fino a 'start'.
             */
            for (int j = i; j >= start; j--) {
                
                /*
                 * DETTAGLIO SINTATTICO:
                 * v2[k] accede alla cella k-esima del vettore destinazione.
                 * v1[j] accede alla cella j-esima del vettore sorgente.
                 * Assegniamo il valore e POI incrementiamo k (k++) per la prossima scrittura.
                 */
                v2[k] = v1[j];
                k++; 
            }
            
            /* * Prepariamo 'start' per la prossima sequenza.
             * La prossima sequenza inizierà necessariamente all'elemento successivo a 'i'.
             */
            start = i + 1;
        }
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Testare la funzione con l'array fornito nell'esempio dell'esercizio.
 * ======================================================================================
 */
int main() {
    /* --- 1. DATI DI INPUT --- */
    /* Dimensione dell'array nell'esempio */
    int n = 10;
    
    /* Allocazione statica temporanea per inizializzare i valori facilmente */
    int input_temp[] = {1, 2, 3, 4, 5, 0, 12, 13, 14, 2};

    printf("--- TEST invertSequence ---\n");

    /* --- 2. ALLOCAZIONE DINAMICA MEMORIA --- */
    /*
     * GESTIONE MEMORIA (MALLOC):
     * Allocazione dei due vettori nello Heap.
     * n * sizeof(int) calcola i byte totali necessari (es. 10 * 4 byte = 40 byte).
     */
    int *v1 = (int *)malloc(n * sizeof(int));
    int *v2 = (int *)malloc(n * sizeof(int));

    /* Controllo sempre se malloc ha restituito NULL (memoria piena) */
    if (v1 == NULL || v2 == NULL) {
        fprintf(stderr, "Errore fatale: Memoria insufficiente.\n");
        return 1;
    }

    /* Copiamo i dati di test nel vettore dinamico v1 */
    for (int i = 0; i < n; i++) {
        v1[i] = input_temp[i];
    }

    /* --- 3. STAMPA PRIMA --- */
    printf("Vettore v1 (Input) : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", v1[i]);
    }
    printf("\n");

    /* --- 4. ESECUZIONE FUNZIONE --- */
    /* Passiamo i puntatori agli array e la dimensione */
    invertSequence(v1, n, v2);

    /* --- 5. STAMPA DOPO --- */
    printf("Vettore v2 (Output): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", v2[i]);
    }
    printf("\n");

    /* Verifica visiva basata sull'esempio */
    printf("Atteso             : 5 4 3 2 1 14 13 12 0 2\n");

    /* --- 6. PULIZIA MEMORIA (FREE) --- */
    /*
     * GESTIONE MEMORIA (FREE):
     * Liberiamo lo spazio occupato. Se non lo facciamo, creiamo un Memory Leak.
     * v1 e v2 non saranno più utilizzabili dopo queste righe.
     */
    free(v1);
    free(v2);

    return 0;
}