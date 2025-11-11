/* =================================================================
 * File: RecursiveSort.c
 * Descrizione: Implementazione degli algoritmi di ordinamento
 * ricorsivi (Quicksort e Mergesort).
 * =================================================================
 */

#include "RecursiveSort.h"
#include <stdio.h>  // Per I/O (opzionale, per debug)
#include <stdlib.h> // Per malloc() e free() (usati da Mergesort)

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
 * IMPLEMENTAZIONE QUICKSORT
 * =================================================================
 */

/**
 * @brief Partizionamento (schema di Lomuto).
 * Prende l'ultimo elemento (A[r]) come pivot.
 * Mette tutti gli elementi < pivot a sinistra,
 * e tutti gli elementi >= pivot a destra.
 */
int partition(Item *A, int l, int r) {
    int i, j;
    Item pivot = A[r]; // Scegli l'ultimo elemento come pivot
    
    // 'i' è l'indice dell'ultimo elemento < del pivot
    i = l - 1; 
    
    // Scansiona l'array da 'l' a 'r-1'
    for (j = l; j < r; j++) {
        // Se l'elemento corrente A[j] è minore del pivot...
        if (ITEMless(A[j], pivot)) {
            // ...incrementa 'i' e scambia A[i] con A[j]
            i++;
            Swap(A, i, j);
        }
    }
    
    // Metti il pivot al suo posto finale (scambiandolo con A[i+1])
    Swap(A, i + 1, r);
    
    // Ritorna l'indice dove si trova ora il pivot
    return i + 1;
}

/**
 * @brief Funzione ricorsiva di Quicksort.
 */
void QuickSortR(Item *A, int l, int r) {
    int q; // Indice del pivot
    
    // Caso base: se il sotto-array ha 0 o 1 elementi, è già ordinato
    if (r <= l) {
        return;
    }
    
    // 1. Partiziona l'array (e ottieni l'indice del pivot 'q')
    q = partition(A, l, r);
    
    // 2. Ricorsione a sinistra (elementi < pivot)
    QuickSortR(A, l, q - 1);
    
    // 3. Ricorsione a destra (elementi > pivot)
    QuickSortR(A, q + 1, r);
}

/**
 * @brief Funzione Wrapper (principale) per QuickSort.
 */
void QuickSort(Item *A, int N) {
    // Avvia la ricorsione sull'intero array (da 0 a N-1)
    QuickSortR(A, 0, N - 1);
}


/* =================================================================
 * IMPLEMENTAZIONE MERGESORT (Top-Down, Ricorsivo)
 * =================================================================
 */

/**
 * @brief Funzione di Fusione (Merge).
 * Fonde A[l...q] e A[q+1...r] (copiandoli prima in B).
 */
void Merge(Item *A, Item *B, int l, int q, int r) {
    int i, j, k;

    // 1. Copia l'intero segmento A[l...r] nell'array di appoggio B
    for (i = l; i <= r; i++) {
        B[i] = A[i];
    }
    
    // 2. Fondi B[l...q] e B[q+1...r] di nuovo in A[l...r]
    
    i = l;     // Puntatore per la metà sinistra (in B)
    j = q + 1; // Puntatore per la metà destra (in B)
    
    // Cicla sull'array A (indice 'k')
    for (k = l; k <= r; k++) {
        // Se la metà sinistra è esaurita...
        if (i > q) {
            A[k] = B[j++]; // ...copia dalla destra
        }
        // Se la metà destra è esaurita...
        else if (j > r) {
            A[k] = B[i++]; // ...copia dalla sinistra
        }
        // Se l'elemento a destra B[j] è minore di quello a sinistra B[i]...
        else if (ITEMless(B[j], B[i])) {
            A[k] = B[j++]; // ...copia B[j] (stabile)
        }
        // Altrimenti (B[i] <= B[j])...
        else {
            A[k] = B[i++]; // ...copia B[i]
        }
    }
}

/**
 * @brief Funzione ricorsiva di MergeSort.
 */
void MergeSortR(Item *A, Item *B, int l, int r) {
    int q; // Indice del punto medio
    
    // Caso base: se il sotto-array ha 0 o 1 elementi, è già ordinato
    if (r <= l) {
        return;
    }
    
    // 1. Trova il punto medio
    q = (l + r) / 2;
    
    // 2. Ricorsione a sinistra
    MergeSortR(A, B, l, q);
    
    // 3. Ricorsione a destra
    MergeSortR(A, B, q + 1, r);
    
    // 4. Fondi (Merge) le due metà ordinate
    Merge(A, B, l, q, r);
}

/**
 * @brief Funzione Wrapper (principale) per MergeSort.
 */
void MergeSort(Item *A, int N) {
    // 1. Alloca l'array di appoggio 'B' (grande quanto A)
    Item *B = (Item *)malloc(N * sizeof(Item));
    if (B == NULL) {
        // Gestione errore memoria
        return; 
    }
    
    // 2. Avvia la ricorsione sull'intero array
    MergeSortR(A, B, 0, N - 1);
    
    // 3. Libera l'array di appoggio
    free(B);
}


/* =================================================================
 * IMPLEMENTAZIONE MERGESORT (Bottom-Up, Iterativo)
 * =================================================================
 */

/**
 * @brief Trova il minimo tra due interi (helper per BottomUp)
 */
static int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * @brief Ordinamento MergeSort Iterativo (non ricorsivo).
 */
void BottomUpMergeSort(Item *A, int N) {
    int l, r, q;
    int sz; // 'sz' è la dimensione dei sotto-array da fondere
    
    // 1. Alloca l'array di appoggio 'B' (grande quanto A)
    Item *B = (Item *)malloc(N * sizeof(Item));
    if (B == NULL) {
        return;
    }
    
    // Ciclo esterno: 'sz' (size) raddoppia ad ogni iterazione
    // (fonde array di 1, poi di 2, poi di 4, 8, ...)
    for (sz = 1; sz < N; sz = sz + sz) { // sz = sz * 2
        
        // Ciclo interno: 'l' (left) scorre l'array a passi di 2*sz
        for (l = 0; l < N - sz; l += sz + sz) { // l = l + 2*sz
            
            // Calcola il centro (q) e la fine (r)
            q = l + sz - 1;
            r = min(l + sz + sz - 1, N - 1);
            
            // Fonde A[l...q] e A[q+1...r]
            Merge(A, B, l, q, r);
        }
    }
    
    // 3. Libera l'array di appoggio
    free(B);
}