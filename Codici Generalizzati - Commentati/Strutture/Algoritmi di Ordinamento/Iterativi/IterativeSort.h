/* =================================================================
 * File: IterativeSort.h
 * Descrizione: Interfaccia per algoritmi di ordinamento iterativi.
 * =================================================================
 */

#ifndef ITERATIVE_SORT_H
#define ITERATIVE_SORT_H

/*
 * Definizione dell'Item (Dato)
 * Per semplicità, e per la natura di CountingSort,
 * assumiamo che l'Item sia un semplice intero.
 * Le funzioni di ordinamento si baseranno su questa definizione.
 */
typedef int Item;
typedef int Key; // La chiave è l'Item stesso


/*
 * Funzione: BubbleSort
 * Ordina un array usando il metodo Bubble Sort (non ottimizzato).
 * Costo: O(N^2)
 */
void BubbleSort(Item A[], int N);

/*
 * Funzione: OptBubbleSort
 * Ordina un array usando il metodo Bubble Sort ottimizzato,
 * fermandosi se l'array è già ordinato.
 * Costo: O(N^2) [Caso peggiore], O(N) [Caso migliore]
 */
void OptBubbleSort(Item A[], int N);

/*
 * Funzione: SelectionSort
 * Ordina un array usando il metodo Selection Sort.
 * Costo: O(N^2)
 */
void SelectionSort(Item A[], int N);

/*
 * Funzione: InsertionSort
 * Ordina un array usando il metodo Insertion Sort.
 * Costo: O(N^2) [Caso peggiore], O(N) [Caso migliore]
 */
void InsertionSort(Item A[], int N);

/*
 * Funzione: ShellSort
 * Ordina un array usando il metodo Shell Sort con la sequenza di Knuth.
 * Costo: O(N log N) o O(N^(3/2)) (dipende dalla sequenza)
 */
void ShellSort(Item A[], int N);

/*
 * Funzione: CountingSort
 * Ordina un array di interi (non basato su confronti).
 * Richiede che gli elementi siano in un intervallo noto [0...k].
 * Costo: O(N + k)
 * A[] = array di input
 * B[] = array di output (ordinato)
 * C[] = array di appoggio per i conteggi (dimensione k+1)
 * N = numero di elementi
 * k = valore massimo nell'array A
 */
void CountingSort(Item A[], Item B[], int C[], int N, int k);


#endif // ITERATIVE_SORT_H