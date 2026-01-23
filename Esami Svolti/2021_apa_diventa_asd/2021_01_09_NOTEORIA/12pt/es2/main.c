#include <stdio.h>
#include "list.h"

/* =========================================================================
 * MAIN DI TEST
 * =========================================================================
 * SCOPO:
 * Riprodurre l'esempio della traccia d'esame.
 * div = 3
 * L = 9 <-> 8 <-> 7 <-> 6 <-> 5 <-> 4 <-> 3 <-> 2 <-> 1 <-> 0
 * Risultato atteso: 9 <-> 6 <-> 3 <-> 0
 * ========================================================================= */
int main() {
    printf("--- INIZIO TEST PURGE ---\n");

    /* 1. Creazione Lista */
    LIST myL = LISTinit();

    /* Inseriamo i dati.
     * Nota: LISTaddHead inserisce in testa. Per avere 9->8->...->0
     * dobbiamo inserirli al contrario (da 0 a 9) oppure inserirli
     * nell'ordine e accettare che la lista sia costruita a ritroso.
     * Per semplicità, inserisco da 0 a 9 così 9 diventa la testa finale.
     */
    for (int i = 0; i <= 9; i++) {
        LISTaddHead(myL, i);
    }
    /* Ora la lista è: 9 <-> 8 <-> 7 <-> 6 <-> 5 <-> 4 <-> 3 <-> 2 <-> 1 <-> 0 */

    /* Stampa stato iniziale */
    printf("Stato Iniziale:\n");
    LISTprint(myL);

    /* 2. Chiamata alla funzione purge */
    int div = 3;
    printf("\nEsecuzione purge con div = %d...\n", div);
    purge(myL, div);

    /* 3. Stampa Risultati */
    printf("Stato Finale (Atteso: 9 <-> 6 <-> 3 <-> 0):\n");
    LISTprint(myL);

    /* 4. Pulizia Memoria */
    LISTdestroy(myL);
    
    printf("--- FINE TEST ---\n");
    return 0;
}