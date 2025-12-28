#include <stdio.h>
#include "list.h" // Includiamo il nostro ADT

/*
 * MAIN DI TEST
 */
int main() {
    printf("--- TEST LISTA CIRCOLARE ADT ---\n");

    /* 1. Creazione Lista */
    list_t mylist = LISTinit();

    /* 2. Inserimento in lista vuota (Posizione 0, Valore 10) */
    printf("Inserisco 10 in pos 0...\n");
    LISTinsert(mylist, 0, 10);
    LISTprint(mylist);

    /* 3. Inserimento in coda (simulato pos grande) (Valore 20) */
    printf("Inserisco 20 in pos 1...\n");
    LISTinsert(mylist, 1, 20);
    LISTprint(mylist);

    /* 4. Inserimento in TESTA (Valore 5) -> Dovrebbe diventare la nuova testa */
    printf("Inserisco 5 in pos 0 (nuova testa)...\n");
    LISTinsert(mylist, 0, 5);
    LISTprint(mylist);

    /* 5. Inserimento DUPLICATO (Valore 10) -> Dovrebbe incrementare contatore */
    printf("Inserisco 10 (gia' presente)...\n");
    LISTinsert(mylist, 0, 10); // La posizione viene ignorata se esiste
    LISTprint(mylist);

    /* 6. Inserimento in mezzo (Valore 15 in pos 2) */
    // Attuale: 5 -> 10 -> 20. Pos 2 dovrebbe essere tra 10 e 20.
    printf("Inserisco 15 in pos 2...\n");
    LISTinsert(mylist, 2, 15);
    LISTprint(mylist);

    /* 7. Pulizia Memoria */
    LISTfree(mylist);
    printf("Memoria liberata.\n");

    return 0;
}