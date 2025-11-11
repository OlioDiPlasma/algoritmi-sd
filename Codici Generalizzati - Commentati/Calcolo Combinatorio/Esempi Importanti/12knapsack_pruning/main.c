#include <stdio.h>
#include <stdlib.h>
#include "Item.h"    // struttura Item con size e value e funzioni ITEMscan, ITEMshow

/* =============================================================
 * PROTOTIPI
 * ============================================================= */
void powerset(int pos, Item *items, int *sol, int k, int cap, 
              int c_cap, int c_val, int *b_val, int *b_sol);

/* =============================================================
 * MAIN
 * ============================================================= */
int main(void) {
    int i, k, cap;
    int *sol, *b_sol, b_val = 0;
    Item *items;

    // Numero di oggetti
    printf("Input number of elements: ");
    scanf("%d", &k);

    // Allocazione array oggetti e soluzioni
    items = malloc(k * sizeof(Item));
    sol = calloc(k, sizeof(int));    // soluzione corrente (0/1)
    b_sol = calloc(k, sizeof(int));  // soluzione migliore

    // Capacità dello zaino
    printf("Input capacity of knapsack: ");
    scanf("%d", &cap);

    // Inserimento oggetti: ogni oggetto ha peso (size) e valore (value)
    printf("Input elements: \n");
    for (i = 0; i < k; i++)
        items[i] = ITEMscan();

    // Avvio della ricerca ricorsiva del powerset
    powerset(0, items, sol, k, cap, 0, 0, &b_val, b_sol);

    // Stampa della soluzione migliore trovata
    printf("The solution is set { ");
    for (i = 0; i < k; i++)
        if (b_sol[i] != 0)
            ITEMshow(items[i]);
    printf("} \n");
    printf("best value = %d \n", b_val);

    // Liberazione memoria
    free(items);
    free(sol);
    free(b_sol);

    return 0;
}

/* =============================================================
 * powerset(pos, items, sol, k, cap, c_cap, c_val, b_val, b_sol)
 * -------------------------------------------------------------
 * Funzione ricorsiva che genera tutti i sottoinsiemi degli oggetti
 * per risolvere il problema dello zaino.
 *
 * pos   = indice corrente dell'oggetto
 * items = array degli oggetti
 * sol   = soluzione corrente (0/1 per ogni oggetto)
 * k     = numero totale di oggetti
 * cap   = capacità massima dello zaino
 * c_cap = capacità totale degli oggetti inclusi finora
 * c_val = valore totale degli oggetti inclusi finora
 * b_val = valore migliore trovato finora
 * b_sol = soluzione migliore trovata finora
 * ============================================================= */
void powerset(int pos, Item *items, int *sol, int k, int cap, 
              int c_cap, int c_val, int *b_val, int *b_sol) {
    int j;

    // CASO BASE: tutti gli oggetti sono stati considerati
    if (pos >= k) {
        if (c_val > *b_val) {   // se il valore attuale è migliore
            for (j = 0; j < k; j++)
                b_sol[j] = sol[j];  // aggiorna la soluzione migliore
            *b_val = c_val;         // aggiorna il valore migliore
        }
        return;
    }

    /* =========================================================
     * Caso 1: Non posso includere l'oggetto corrente perché
     * supererebbe la capacità dello zaino
     * ========================================================= */
    if ((c_cap + items[pos].size) > cap) {
        sol[pos] = 0;
        powerset(pos + 1, items, sol, k, cap, c_cap, c_val, b_val, b_sol);
        return;
    }

    /* =========================================================
     * Caso 2: Provo a includere l'oggetto corrente
     * ========================================================= */
    sol[pos] = 1;
    c_cap += items[pos].size;
    c_val += items[pos].value;
    powerset(pos + 1, items, sol, k, cap, c_cap, c_val, b_val, b_sol);

    /* =========================================================
     * Caso 3: Backtracking → escludo l'oggetto corrente e
     * continuo a cercare
     * ========================================================= */
    sol[pos] = 0;
    c_cap -= items[pos].size;
    c_val -= items[pos].value;
    powerset(pos + 1, items, sol, k, cap, c_cap, c_val, b_val, b_sol);
}
