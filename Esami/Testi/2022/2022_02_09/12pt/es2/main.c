#include <stdio.h>
#include "albero.h" /* Includo solo l'interfaccia */

int main() {
    printf("--- TEST MODULARE ADT ALBERO ---\n");

    /* 1. Creazione e Popolamento */
    /* Non facciamo malloc qui, usiamo le funzioni dell'ADT */
    nTREE mioAlbero = initTree();
    
    /* * NOTA: Non possiamo fare 'mioAlbero->root = ...' qui!
     * Il compilatore darebbe errore: "dereferencing pointer to incomplete type".
     * Dobbiamo chiedere alla libreria di popolarlo per noi.
     */
    popola_albero_per_test(mioAlbero);

    /* 2. Esecuzione Funzione */
    int c1, c2, c3;
    countIf(mioAlbero, &c1, &c2, &c3);

    /* 3. Stampa */
    printf("Risultati:\n");
    printf("- Nodi con 1 figlio : %d\n", c1);
    printf("- Nodi con 2 figli  : %d\n", c2);
    printf("- Nodi con 3 figli  : %d\n", c3);

    /* 4. Pulizia */
    freeTree(mioAlbero);
    
    return 0;
}