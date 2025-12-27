#include <stdio.h>
#include "ntree.h"

int main() {
    printf("--- INIZIO TEST ALBERO N-ARIO ---\n");

    /* --- 1. INIZIALIZZAZIONE --- */
    nTREE albero = ntree_init();

    /* --- 2. CREAZIONE STRUTTURA DATI --- */
    /*
     * Costruiamo questo albero per il test (N=5):
     *
     * ROOT (Grado 2: ha figli A e B) -> CONTA (Default)
     * /    \
     * A      B
     * * Dettaglio A (Grado 3: ha figli A1, A2, A3):
     * Grado Padre(ROOT)=2. Grado A=3. 3 > 2 -> CONTA.
     * * Dettaglio B (Grado 0: foglia):
     * Grado Padre(ROOT)=2. Grado B=0. 0 non > 2 -> NON CONTA.
     *
     * Dettaglio A1, A2, A3 (Grado 0):
     * Grado Padre(A)=3. Grado Loro=0. 0 non > 3 -> NON CONTANO.
     *
     * RISULTATO ATTESO: 2 (Root e A).
     */

    printf("Creazione nodi...\n");
    ntree_insert_root(albero, "ROOT");
    
    // Figli di ROOT
    ntree_insert_child(albero, "ROOT", "A");
    ntree_insert_child(albero, "ROOT", "B");

    // Figli di A
    ntree_insert_child(albero, "A", "A1");
    ntree_insert_child(albero, "A", "A2");
    ntree_insert_child(albero, "A", "A3");

    /* --- 3. ESECUZIONE FUNZIONE --- */
    printf("Calcolo nodi con grado > padre...\n");
    int risultato = countIf(albero);

    /* --- 4. STAMPA RISULTATI --- */
    printf("Risultato ottenuto: %d\n", risultato);
    printf("Risultato atteso:   2\n");

    if (risultato == 2) {
        printf("--> TEST SUPERATO!\n");
    } else {
        printf("--> TEST FALLITO.\n");
    }

    /* --- 5. PULIZIA MEMORIA --- */
    /* Fondamentale liberare tutto prima di chiudere */
    ntree_free(albero);

    return 0;
}