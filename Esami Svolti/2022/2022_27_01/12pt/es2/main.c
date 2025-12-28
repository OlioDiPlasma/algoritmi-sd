#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int main() {
    printf("--- Inizio Test Verifica BST ---\n");

    /* --- TEST 1: ALBERO VALIDO --- */
    printf("\nCreazione albero valido (10, sx:5, dx:15)...\n");
    BT albero1 = crea_albero_valido();
    
    // Chiamata alla funzione f
    int esito1 = f(albero1);
    
    if (esito1 == 1) printf("Esito: CORRETTO (E' un BST)\n");
    else             printf("Esito: ERRATO (Dovrebbe essere un BST)\n");

    /* --- TEST 2: ALBERO INVALIDO --- */
    printf("\nCreazione albero invalido (10, sx:5, dx:2)...\n");
    BT albero2 = crea_albero_invalido();
    
    // Chiamata alla funzione f
    int esito2 = f(albero2);
    
    if (esito2 == 0) printf("Esito: CORRETTO (Non e' un BST)\n");
    else             printf("Esito: ERRATO (Non dovrebbe essere un BST)\n");

    /* --- PULIZIA MEMORIA --- */
    libera_albero(albero1);
    libera_albero(albero2);
    printf("\nMemoria liberata.\n");

    return 0;
}