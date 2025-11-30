#include <stdio.h>
#include "LIST.h" // Includiamo la nostra definizione

int main() {
    /* --- 1. CREAZIONE DATI DI PROVA --- */
    /* Esempio dall'immagine: 1, -2, 3, 8, 5, 6, 7, 9 */
    /* Indici originali:      0   1  2  3  4  5  6  7 */
    /* Da cancellare (0,3,6): 1,     8,        7     */
    
    LIST my_list = creaListaVuota();
    int valori[] = {1, -2, 3, 8, 5, 6, 7, 9};
    
    for (int i = 0; i < 8; i++) {
        aggiungiInCoda(my_list, valori[i]);
    }

    printf("--- LISTA ORIGINALE ---\n");
    printf("Atteso: 1 -> -2 -> 3 -> 8 -> 5 -> 6 -> 7 -> 9 -> NULL\n");
    printf("Reale : ");
    stampaLista(my_list);

    /* --- 2. ESECUZIONE FUNZIONE F --- */
    printf("\n--- ESECUZIONE f(l) ---\n");
    f(my_list);

    /* --- 3. VERIFICA RISULTATI --- */
    /* Risultato atteso: -2, 3, 5, 6, 9 */
    printf("Atteso: -2 -> 3 -> 5 -> 6 -> 9 -> NULL\n");
    printf("Reale : ");
    stampaLista(my_list);

    /* --- 4. PULIZIA MEMORIA --- */
    distruggiLista(my_list);
    
    return 0;
}