#include <stdio.h>
#include <stdlib.h>

// --- UTILITÀ ---

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// ============================================================
// 1. MERGESORT (Stabile, O(N log N))
// ============================================================

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Allocazione array temporanei (NON è in-place)
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    // Copia dati
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    
    // Fusione
    while (i < n1 && j < n2) {
        // "<=" garantisce la STABILITÀ (preferisco chi era a sinistra)
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copia residui
    while (i < n1) { arr[k++] = L[i++]; }
    while (j < n2) { arr[k++] = R[j++]; }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// ============================================================
// 2. QUICKSORT - SCHEMA LOMUTO (Pivot Ultimo Elemento)
// Facile da scrivere, fa più scambi di Hoare.
// ============================================================

int partitionLomuto(int arr[], int low, int high) {
    int pivot = arr[high]; // Pivot = Ultimo
    int i = (low - 1);     // Indice dei "piccoli"

    for (int j = low; j <= high - 1; j++) {
        // Se l'elemento corrente è minore del pivot
        if (arr[j] < pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    // Metti il pivot al suo posto finale (dopo l'ultimo piccolo)
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortLomuto(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionLomuto(arr, low, high);
        quickSortLomuto(arr, low, pi - 1);
        quickSortLomuto(arr, pi + 1, high);
    }
}

// ============================================================
// 3. QUICKSORT - SCHEMA HOARE (Pivot Ultimo Elemento)
// Due indici che convergono. Più efficiente (meno swap).
// ============================================================

int partitionHoare(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    int j = high; // Parte dalla posizione del pivot

    while (1) {
        // Cerca elemento a sinistra che dovrebbe stare a destra
        // (i si ferma su numeri >= pivot)
        do { i++; } while (arr[i] < pivot);

        // Cerca elemento a destra che dovrebbe stare a sinistra
        // (j si ferma su numeri <= pivot)
        // Controllo j > low per sicurezza bordi
        do { j--; } while (j > low && arr[j] > pivot);

        // Se si sono incrociati, fine
        if (i >= j) break;

        // Scambia gli elementi fuori posto
        swap(&arr[i], &arr[j]);
    }
    
    // Metti il pivot (che è in 'high') nella posizione di taglio 'i'
    swap(&arr[i], &arr[high]);
    return i;
}

void quickSortHoare(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionHoare(arr, low, high);
        quickSortHoare(arr, low, pi - 1);
        quickSortHoare(arr, pi + 1, high);
    }
}

// ============================================================
// MAIN DI TEST
// ============================================================
int main() {
    // Array di test identici
    int arr1[] = {10, 7, 8, 9, 1, 5, 20, 3};
    int arr2[] = {10, 7, 8, 9, 1, 5, 20, 3};
    int arr3[] = {10, 7, 8, 9, 1, 5, 20, 3};
    int n = sizeof(arr1) / sizeof(arr1[0]);

    // 1. Test Mergesort
    printf("--- Mergesort ---\n");
    printArray(arr1, n);
    mergeSort(arr1, 0, n - 1);
    printArray(arr1, n);

    // 2. Test Quicksort Lomuto
    printf("\n--- Quicksort (Lomuto) ---\n");
    printArray(arr2, n);
    quickSortLomuto(arr2, 0, n - 1);
    printArray(arr2, n);

    // 3. Test Quicksort Hoare
    printf("\n--- Quicksort (Hoare) ---\n");
    printArray(arr3, n);
    quickSortHoare(arr3, 0, n - 1);
    printArray(arr3, n);

    return 0;
}