#include <stdio.h>
#include <stdlib.h>

// --- UTILITÀ ---
int max(int a, int b) { return (a > b) ? a : b; }

// ============================================================
// 1. APPROCCIO RICORSIVO PURO (Combinatorio)
// Complessità: O(2^N) - Esplora tutte le possibilità (includi/escludi)
// ============================================================

/*
 * arr: array input
 * n: dimensione array
 * prev_idx: indice dell'elemento precedente incluso nella LIS (-1 se nessuno)
 * curr_idx: indice corrente che stiamo valutando
 */
int LIS_Recursive(int arr[], int n, int prev_idx, int curr_idx) {
    // Caso Base: abbiamo scorso tutto l'array
    if (curr_idx == n) {
        return 0;
    }

    // Opzione 1: ESCLUDI l'elemento corrente
    // Passiamo al prossimo indice mantenendo lo stesso 'prev_idx'
    int exclude = LIS_Recursive(arr, n, prev_idx, curr_idx + 1);

    // Opzione 2: INCLUDI l'elemento corrente
    // Possibile solo se è il primo elemento o se è maggiore del precedente
    int include = 0;
    if (prev_idx == -1 || arr[curr_idx] > arr[prev_idx]) {
        include = 1 + LIS_Recursive(arr, n, curr_idx, curr_idx + 1);
    }

    // Ritorna il massimo tra prenderlo o non prenderlo
    return max(include, exclude);
}

// ============================================================
// 2. APPROCCIO PROGRAMMAZIONE DINAMICA (Bottom-Up)
// Complessità: O(N^2)
// ============================================================

int LIS_DynamicProgramming(int arr[], int n) {
    if (n == 0) return 0;

    // Creiamo un array di supporto 'lis'
    // lis[i] conterrà la lunghezza della LIS che TERMINA all'indice i
    int *lis = (int*)malloc(n * sizeof(int));

    // Passo 1: Inizializzazione
    // Ogni elemento è, come minimo, una LIS di lunghezza 1 (se stesso)
    for (int i = 0; i < n; i++) {
        lis[i] = 1;
    }

    // Passo 2: Costruzione della tabella (Bottom-Up)
    // Per ogni elemento 'i' da 1 a n-1
    for (int i = 1; i < n; i++) {
        // Controlliamo tutti gli elementi 'j' precedenti (da 0 a i-1)
        for (int j = 0; j < i; j++) {
            
            // CONDIZIONE CHIAVE LIS:
            // 1. arr[i] > arr[j] -> Posso estendere la sequenza
            // 2. lis[i] < lis[j] + 1 -> Ho trovato una sequenza più lunga di quella che conoscevo
            if (arr[i] > arr[j] && lis[i] < lis[j] + 1) {
                lis[i] = lis[j] + 1;
            }
        }
    }

    // Passo 3: Trovare il massimo nell'array 'lis'
    // Attenzione: la LIS non termina necessariamente all'ultimo elemento dell'array!
    int max_len = 0;
    for (int i = 0; i < n; i++) {
        if (lis[i] > max_len) {
            max_len = lis[i];
        }
    }

    // (Opzionale per debug) Stampa array DP
    printf("Array DP (lunghezze locali): ");
    for(int i=0; i<n; i++) printf("%d ", lis[i]);
    printf("\n");

    free(lis);
    return max_len;
}

// --- MAIN ---
int main() {
    // Esempio Classico
    // Sequenza: 10, 22, 9, 33, 21, 50, 41, 60
    // LIS Possibile: 10, 22, 33, 50, 60 (Lun 5)
    // Oppure: 10, 22, 33, 41, 60 (Lun 5)
    int arr[] = { 10, 22, 9, 33, 21, 50, 41, 60 };
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("--- Longest Increasing Subsequence (LIS) ---\n");
    printf("Array input: ");
    for(int i=0; i<n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    // 1. Test Ricorsivo
    printf("Calcolo Ricorsivo (Lento O(2^N))...\n");
    // prev_idx inizia a -1
    int len_rec = LIS_Recursive(arr, n, -1, 0); 
    printf("Risultato Ricorsivo: %d\n\n", len_rec);

    // 2. Test DP
    printf("Calcolo Programmazione Dinamica (Veloce O(N^2))...\n");
    int len_dp = LIS_DynamicProgramming(arr, n);
    printf("Risultato DP: %d\n", len_dp);

    return 0;
}