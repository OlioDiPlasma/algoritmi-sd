#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Riprodurre l'esempio del testo.
 * Lista: 3 -> 10 -> 6 -> 3 -> 1 -> 4
 * Vettore: {1, 2, 3}
 * Output atteso: 3 -> 3 -> 1
 * ============================================================================
 */
int main() {
    /* --- 1. CREAZIONE DATI --- */
    LIST l = LIST_new();
    
    // Inserimento valori come da esempio
    LIST_append(l, 3);
    LIST_append(l, 10);
    LIST_append(l, 6);
    LIST_append(l, 3);
    LIST_append(l, 1);
    LIST_append(l, 4);

    // Creazione vettore whitelist
    int v[] = {1, 2, 3};
    int n = 3; // Dimensione vettore

    printf("--- STATO INIZIALE ---\n");
    printf("Vettore v: {1, 2, 3}\n");
    printf("Lista L:   ");
    LIST_print(l);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    f(l, v, n);

    /* --- 3. VERIFICA RISULTATI --- */
    printf("\n--- DOPO FILTRAGGIO ---\n");
    printf("Lista L':  "); // Atteso: 3 -> 3 -> 1 -> NULL
    LIST_print(l);

    /* --- 4. PULIZIA MEMORIA --- */
    LIST_free(l);
    
    return 0;
}