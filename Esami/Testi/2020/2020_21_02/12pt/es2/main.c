#include <stdio.h>
#include "list.h"

int main() {
    printf("--- ESERCIZIO SWAP LISTA ADT ---\n");

    /* 1. CREAZIONE LISTA DI ESEMPIO */
    // Valori: 10, 20, 30, 40, 50, 60, 70
    list L = LISTinit();
    LISTadd(L, 10);
    LISTadd(L, 20);
    LISTadd(L, 30);
    LISTadd(L, 40);
    LISTadd(L, 50);
    LISTadd(L, 60);
    LISTadd(L, 70);

    /* TEST 1: k = 0 (Scambio estremi: 10 e 70) */
    printf("\nTest k=0 (Atteso: 70 20 ... 60 10)\n");
    printf("Prima: "); LISTprint(L);
    swap(L, 0);
    printf("Dopo : "); LISTprint(L);

    /* RESET LISTA (Rimetto a posto per il prossimo test) */
    // Rieseguo swap(L, 0) per tornare all'originale
    swap(L, 0); 
    printf("(Ripristino lista originale...)\n");

    /* TEST 2: k = 2 (Scambio 30 e 50) */
    // 10 20 [30] 40 [50] 60 70 -> Deve diventare -> 10 20 50 40 30 60 70
    printf("\nTest k=2 (Atteso: 10 20 50 40 30 60 70)\n");
    printf("Prima: "); LISTprint(L);
    swap(L, 2);
    printf("Dopo : "); LISTprint(L);

    /* TEST 3: k = 6 (Come k=0 ma indici invertiti) */
    // La lista ora è "sporca" dal test precedente, resettiamo per chiarezza o verifichiamo logica.
    // Resettiamo a: 10 20 30 40 50 60 70
    swap(L, 2); 

    printf("\nTest k=6 (Equivalente a k=0: scambio 70 e 10)\n");
    printf("Prima: "); LISTprint(L);
    swap(L, 6);
    printf("Dopo : "); LISTprint(L);

    /* TEST 4: k > 6 (Fuori range) */
    printf("\nTest k=10 (Nessun cambiamento)\n");
    swap(L, 10);
    printf("Dopo : "); LISTprint(L);

    /* PULIZIA FINALE */
    LISTfree(L);

    return 0;
}