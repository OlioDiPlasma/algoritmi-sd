#include <stdio.h>
#include <stdlib.h> // Per malloc/free
#include "lista.h"

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Riproduciamo l'esempio: 1 -> 1 -> 2 -> 2 -> 2 -> 5 -> 7 -> 7
 * Output atteso: 1 -> 2 -> 5 -> 7
 * ======================================================================================
 */
int main() {
    // --- 1. COSTRUZIONE LISTA HARDCODED ---
    // Costruiamo la lista manualmente collegando i nodi
    LIST head = crea_nodo(1);
    head->next = crea_nodo(1);
    head->next->next = crea_nodo(2);
    head->next->next->next = crea_nodo(2);
    head->next->next->next->next = crea_nodo(2);
    head->next->next->next->next->next = crea_nodo(5);
    head->next->next->next->next->next->next = crea_nodo(7);
    head->next->next->next->next->next->next->next = crea_nodo(7);

    printf("--- LISTA ORIGINALE ---\n");
    stampa_lista(head);

    // --- 2. CHIAMATA ALLA FUNZIONE ---
    f(head);

    // --- 3. VERIFICA ---
    printf("\n--- LISTA COMPATTATA ---\n");
    stampa_lista(head);

    // --- 4. PULIZIA ---
    libera_lista(head);

    return 0;
}