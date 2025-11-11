/* =================================================================
 * File: IterativeSort.c
 * Descrizione: Implementazione degli algoritmi di ordinamento iterativi.
 * =================================================================
 */

#include "IterativeSort.h"
#include <stdio.h>  // Per I/O (opzionale, per debug)
#include <stdlib.h> // Per malloc/free (opzionale)
#include <string.h> // Per memset (usato in CountingSort)

/* --- Funzioni Helper (statiche) ---
 * Queste funzioni sono "private" di questo file .c e
 * servono ad astrarre le operazioni di confronto e scambio.
 */

/**
 * @brief Estrae la chiave da un Item.
 * (In questo caso, l'Item è la chiave)
 */
static Key KEYget(Item val) {
    return val;
}

/**
 * @brief Confronta due Item (vero se v1 < v2).
 * (Minore di)
 */
static int ITEMless(Item v1, Item v2) {
    return (KEYget(v1) < KEYget(v2));
}

/**
 * @brief Scambia due elementi nell'array A.
 */
static void Swap(Item A[], int i, int j) {
    Item temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

/* =================================================================
 * IMPLEMENTAZIONE FUNZIONI PUBBLICHE
 * =================================================================
 */

/**
 * @brief Ordinamento Bubble Sort (non ottimizzato).
 * Ad ogni ciclo, "spinge" l'elemento più grande in fondo.
 */
void BubbleSort(Item A[], int N) {
    int i, j;
    // Ciclo esterno: determina la "fine" dell'array non ordinato
    for (i = 0; i < N - 1; i++) {
        // Ciclo interno: scorre e confronta le coppie adiacenti
        for (j = 0; j < N - 1 - i; j++) {
            // Se l'elemento [j] è > del successivo [j+1], scambia
            if (ITEMless(A[j+1], A[j])) {
                Swap(A, j, j + 1);
            }
        }
    }
}

/**
 * @brief Ordinamento Bubble Sort (ottimizzato).
 * Si ferma se un ciclo si conclude senza scambi.
 */
void OptBubbleSort(Item A[], int N) {
    int i, j;
    int swapped = 1; // Flag: 1 = ci sono stati scambi, 0 = no
    
    // Cicla finché 'i' non raggiunge la fine O finché 'swapped' è 0
    for (i = 0; i < N - 1 && swapped; i++) {
        swapped = 0; // Resetta il flag all'inizio di ogni ciclo
        
        // Scorre e confronta le coppie
        for (j = 0; j < N - 1 - i; j++) {
            if (ITEMless(A[j+1], A[j])) {
                Swap(A, j, j + 1);
                swapped = 1; // Segnala che è avvenuto uno scambio
            }
        }
    }
}

/**
 * @brief Ordinamento Selection Sort.
 * Ad ogni ciclo, trova il minimo e lo mette al posto giusto.
 */
void SelectionSort(Item A[], int N) {
    int i, j, min_index;
    
    // Ciclo esterno: 'i' è l'indice del primo elemento non ordinato
    for (i = 0; i < N - 1; i++) {
        // Trova il minimo nell'array non ordinato (da i a N-1)
        min_index = i; // Assumi che il minimo sia 'i'
        for (j = i + 1; j < N; j++) {
            // Se troviamo un elemento più piccolo...
            if (ITEMless(A[j], A[min_index])) {
                min_index = j; // ...aggiorna l'indice del minimo
            }
        }
        
        // Scambia l'elemento in posizione 'i' con il minimo trovato
        if (min_index != i) {
            Swap(A, i, min_index);
        }
    }
}

/**
 * @brief Ordinamento Insertion Sort.
 * Ad ogni ciclo, inserisce l'elemento A[i] al posto giusto
 * nella parte già ordinata (A[0]...A[i-1]).
 */
void InsertionSort(Item A[], int N) {
    int i, j;
    Item temp; // Variabile per salvare l'elemento da inserire
    
    // Ciclo esterno: parte dal secondo elemento (i=1)
    for (i = 1; i < N; i++) {
        temp = A[i]; // Salva l'elemento corrente
        
        // Ciclo interno: scorre all'indietro la parte ordinata
        // per trovare dove inserire 'temp'
        j = i - 1;
        
        // Finché non siamo all'inizio (j>=0) E
        // l'elemento A[j] è PIÙ GRANDE di 'temp'...
        while (j >= 0 && ITEMless(temp, A[j])) {
            // ...sposta A[j] a destra (in A[j+1])
            A[j + 1] = A[j];
            j--; // Vai all'indietro
        }
        
        // Inserisci 'temp' nella "buca" che si è creata
        A[j + 1] = temp;
    }
}

/**
 * @brief Ordinamento Shell Sort (con sequenza di Knuth).
 * Variazione di Insertion Sort che usa "gap" (h) decrescenti.
 */
void ShellSort(Item A[], int N) {
    int i, j, h;
    Item temp;

    // 1. Calcola il 'gap' (h) iniziale (sequenza di Knuth 3*h+1)
    h = 1;
    while (h < N / 3) {
        h = 3 * h + 1; // 1, 4, 13, 40, ...
    }

    // 2. Cicla sui 'gap' (h), partendo dal più grande fino a 1
    while (h >= 1) {
        
        // 3. Esegui un "h-sort" (Insertion Sort a distanza h)
        // Per ogni "sottogruppo"
        for (i = h; i < N; i++) {
            
            // Salva l'elemento da inserire
            temp = A[i];
            
            // Scorre all'indietro con passi di 'h'
            j = i - h;
            
            // Finché j non è fuori e A[j] > temp
            while (j >= 0 && ITEMless(temp, A[j])) {
                // Sposta l'elemento A[j] avanti di 'h'
                A[j + h] = A[j];
                j -= h; // Vai indietro di 'h'
            }
            
            // Inserisci 'temp' nella posizione corretta
            A[j + h] = temp;
        }
        
        // 4. Riduci il gap
        h = h / 3;
    }
}

/**
 * @brief Ordinamento Counting Sort (O(N+k)).
 * Ordina interi nell'intervallo [0...k].
 */
void CountingSort(Item A[], Item B[], int C[], int N, int k) {
    int i;

    // 1. Inizializza l'array dei conteggi C a 0
    // (Presuppone che C sia di dimensione k+1)
    memset(C, 0, (k + 1) * sizeof(int));
    
    // 2. Conta le occorrenze
    // C[x] ora contiene il numero di volte che 'x' appare in A
    for (i = 0; i < N; i++) {
        C[KEYget(A[i])]++;
    }

    // 3. Calcola le posizioni cumulative
    // C[x] ora contiene il numero di elementi <= 'x'
    for (i = 1; i <= k; i++) {
        C[i] = C[i] + C[i - 1];
    }

    // 4. Costruisci l'array di output B
    // Scorre l'input A *all'indietro* (per mantenere la stabilità)
    for (i = N - 1; i >= 0; i--) {
        // Prendi l'elemento A[i] (es. 5)
        // Guarda la sua posizione finale: C[KEYget(A[i])] (es. C[5] = 7)
        // Mettilo nell'array B in posizione (7 - 1) = 6
        B[C[KEYget(A[i])] - 1] = A[i];
        
        // Decrementa il contatore per quel numero
        C[KEYget(A[i])]--;
    }
    
    // Ora l'array B[] contiene gli elementi di A[] in ordine.
    // (Se si vuole ordinare "in-place", si deve copiare B in A)
}