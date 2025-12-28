#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Creare la lista dell'esempio: 3 -> 10 -> 6 -> 3 -> 1 -> 5
 * Verificare che split produca 3 liste corrette.
 * Liberare TUTTA la memoria per evitare memory leaks.
 * ============================================================================
 */
int main() {
    /* --- 1. CREAZIONE DATI HARDCODED --- */
    printf("--- CREAZIONE LISTA ORIGINALE ---\n");
    LIST my_list = LIST_new();
    
    // Inseriamo i valori dell'esempio
    LIST_append(my_list, 3);
    LIST_append(my_list, 10);
    LIST_append(my_list, 6);
    LIST_append(my_list, 3);
    LIST_append(my_list, 1);
    LIST_append(my_list, 5);

    printf("Lista Originale: ");
    LIST_print(my_list);

    /* --- 2. ESECUZIONE DELLA FUNZIONE --- */
    int n = 0; // Qui verrà salvata la dimensione del vettore
    
    // Chiamata: passiamo la lista e l'indirizzo di n
    LIST *vector = split(my_list, &n);

    /* --- 3. STAMPA DEI RISULTATI --- */
    printf("\n--- RISULTATO SPLIT (n = %d) ---\n", n);
    for (int i = 0; i < n; i++) {
        printf("Sottolista %d: ", i);
        // Accesso all'array di liste: vector[i]
        LIST_print(vector[i]);
    }

    printf("\n--- VERIFICA INTEGRITA' ORIGINALE ---\n");
    printf("Lista Originale (deve essere intatta): ");
    LIST_print(my_list);

    /* --- 4. GESTIONE MEMORIA (CLEANUP) --- */
    // Attenzione: dobbiamo liberare:
    // a) Le singole liste dentro il vettore.
    // b) Il vettore stesso.
    // c) La lista originale.

    // a) Libero le sottoliste
    for (int i = 0; i < n; i++) {
        LIST_free(vector[i]);
    }
    // b) Libero l'array di puntatori
    free(vector);

    // c) Libero la lista originale
    LIST_free(my_list);

    return 0;
}