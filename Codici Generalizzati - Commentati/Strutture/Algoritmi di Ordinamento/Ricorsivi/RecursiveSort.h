/* =================================================================
 * File: RecursiveSort.h
 * Descrizione: Interfaccia per algoritmi di ordinamento ricorsivi
 * (Quicksort e Mergesort).
 * =================================================================
 */

#ifndef RECURSIVE_SORT_H
#define RECURSIVE_SORT_H

/*
 * Definizione dell'Item (Dato)
 * Per semplicità, assumiamo che l'Item sia un intero.
 */
typedef int Item;
typedef int Key; // La chiave è l'Item stesso


/* --- QuickSort --- */

/**
 * @brief Funzione Wrapper per QuickSort.
 * Ordina l'array A di N elementi.
 */
void QuickSort(Item *A, int N);

/**
 * @brief Funzione Ricorsiva (helper) per QuickSort.
 * Ordina il sotto-array A dall'indice 'l' (left) all'indice 'r' (right).
 */
void QuickSortR(Item *A, int l, int r);

/**
 * @brief Funzione di Partizionamento (helper per QuickSort).
 * Riorganizza il sotto-array A[l...r] in base a un pivot.
 * @return La posizione finale (indice) del pivot.
 */
int partition(Item *A, int l, int r);


/* --- MergeSort (Top-Down Ricorsivo) --- */

/**
 * @brief Funzione Wrapper per MergeSort.
 * Ordina l'array A di N elementi.
 */
void MergeSort(Item *A, int N);

/**
 * @brief Funzione Ricorsiva (helper) per MergeSort.
 * Ordina il sotto-array A[l...r] usando l'array di appoggio B.
 */
void MergeSortR(Item *A, Item *B, int l, int r);

/**
 * @brief Funzione di Fusione (helper per MergeSort).
 * Fonde i due sotto-array ordinati A[l...q] e A[q+1...r]
 * nell'array di appoggio B, e poi ricopia in A.
 * (Nota: questa implementazione copia prima A in B e poi fonde da B in A)
 */
void Merge(Item *A, Item *B, int l, int q, int r);


/* --- MergeSort (Bottom-Up Iterativo) --- */

/**
 * @brief Ordinamento MergeSort Iterativo (non ricorsivo).
 * Ordina l'array A di N elementi partendo dal basso (fondono
 * coppie di 1, poi di 2, poi di 4, ...).
 */
void BottomUpMergeSort(Item *A, int N);


#endif // RECURSIVE_SORT_H