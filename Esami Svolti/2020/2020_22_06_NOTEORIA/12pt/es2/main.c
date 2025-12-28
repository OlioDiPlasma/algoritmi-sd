#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * MAIN DI TEST
 * Scopo: Ricreare l'esempio esatto della traccia.
 */
int main() {
    printf("--- ESERCIZIO PRODOTTO CARTESIANO ORDINATO ---\n");

    /* DATI DELL'ESEMPIO */
    // v1 = {1, 2, 3}
    int v1[] = {1, 2, 3};
    int d1 = 3;

    // v2 = {3, 4, 5, 6}
    int v2[] = {3, 4, 5, 6};
    int d2 = 4;

    /* ESECUZIONE */
    printf("Calcolo prodotto cartesiano...\n");
    list_t L = prodCart(v1, d1, v2, d2);

    /* VERIFICA RISULTATI */
    // L'output dovrebbe mostrare i prodotti in ordine crescente:
    // 1*3=3, 1*4=4, 1*5=5, 2*3=6, 1*6=6, 2*4=8 ... ecc.
    listPrint(L);

    /* PULIZIA MEMORIA */
    listFree(L);
    printf("Memoria liberata.\n");

    return 0;
}

