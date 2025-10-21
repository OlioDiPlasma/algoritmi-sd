#include <stdio.h>

/*
 * 1. Soluzione Divide et Impera (Non Tail-Recursive)
 */
int sum_divide_impera(int A[], int l, int r) {
    // Condizione di terminazione (caso base)
    if (l == r) {
        return A[l];
    }

    // 1. Divide
    int m = (l + r) / 2;

    // 2. Impera (chiamate ricorsive)
    int sum_sx = sum_divide_impera(A, l, m);
    int sum_dx = sum_divide_impera(A, m + 1, r);

    // 3. Combina
    return sum_sx + sum_dx;
}

/*
 * 2. Soluzione Tail-Recursive (con helper)
 * Questo approccio segue Decrease and Conquer (n -> n-1)
 */
int sum_tail_helper(int A[], int i, int n, int acc) {
    // Condizione di terminazione
    if (i == n) {
        return acc;
    }

    // Il calcolo (acc + A[i]) è fatto PRIMA della chiamata ricorsiva
    // L'ultima operazione è la chiamata a sé stessa
    return sum_tail_helper(A, i + 1, n, acc + A[i]);
}

// Funzione "wrapper" per la tail-recursion
int sum_tail(int A[], int n) {
    return sum_tail_helper(A, 0, n, 0); // Inizia da indice 0 con accumulatore 0
}


/*
 * 3. Soluzione Iterativa
 */
int sum_iterative(int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}


/*
 * Main per testare le funzioni
 */
int main() {
    int A[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(A) / sizeof(A[0]);

    printf("Array: [1, 2, 3, 4, 5, 6, 7, 8]\n");
    printf("Dimensione: %d\n\n", n);

    // Test Punto 1
    int result_di = sum_divide_impera(A, 0, n - 1);
    printf("1. Divide et Impera: %d\n", result_di);

    // Test Punto 2
    int result_tail = sum_tail(A, n);
    printf("2. Tail Recursive:   %d\n", result_tail);

    // Test Punto 3
    int result_iter = sum_iterative(A, n);
    printf("3. Iterativa:        %d\n", result_iter);

    return 0;
}