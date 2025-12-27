#include <stdio.h>
#include <stdlib.h>
#include "parts.h"

/* =========================================================================
 * MAIN DI TEST
 * =========================================================================
 * SCOPO:
 * Creare i dati di prova identici all'immagine fornita e invocare il wrapper.
 * ========================================================================= */
int main() {
    /* 1. Definiamo il target */
    char target[] = "persona";

    /* 2. Definiamo il vettore di parti P (Hardcoded come da esempio)
     * Sintassi inizializzazione struct: {char*, int, int} */
    part P[] = {
        {"p", 0, 1},    // p0
        {"pers", 0, 5}, // p1
        {"er", 1, 4},   // p2
        {"ers", 1, 4},  // p3
        {"sa", 3, 1},   // p4
        {"so", 3, 2},   // p5
        {"ato", 0, 1},  // p6
        {"on", 4, 2},   // p7
        {"ona", 4, 3},  // p8
        {"a", 6, 1}     // p9
    };

    /* Calcolo dimensione array: sizeof totale / sizeof primo elemento */
    int nParts = sizeof(P) / sizeof(P[0]);

    printf("--- INIZIO TEST ---\n");
    
    /* 3. Chiamata al wrapper */
    solve(target, P, nParts);

    printf("--- FINE TEST ---\n");

    return 0;
}