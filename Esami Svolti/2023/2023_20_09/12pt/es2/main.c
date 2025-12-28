#include <stdio.h>
#include <stdlib.h>
#include "lista.h" // FONDAMENTALE: Include le definizioni

int main() {
    /* Creazione tramite funzione helper, non malloc diretta */
    LIST my_list = crea_lista(); 

    /* Inserimento dati: a a b b b c d d */
    append(my_list, 'a');
    append(my_list, 'a');
    append(my_list, 'b');
    append(my_list, 'b');
    append(my_list, 'b');
    append(my_list, 'c');
    append(my_list, 'd');
    append(my_list, 'd');

    int k = 3;

    printf("--- Lista Iniziale ---\n");
    print_list(my_list);

    printf("\nEseguo compattazione con k = %d...\n", k);
    f(my_list, k);

    printf("\n--- Lista Post-Compattazione ---\n");
    printf("Atteso:  a <-> a <-> b <-> c <-> d <-> d\n");
    printf("Ottenuto: ");
    print_list(my_list);

    /* Pulizia memoria tramite helper */
    free_list(my_list);

    return 0;
}