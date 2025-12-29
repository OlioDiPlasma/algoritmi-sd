#include <stdio.h>
#include "grafo.h"

/*
 * Scopo: Testare le funzionalità implementate in grafo.c
 * Concetti Chiave: 
 * - Modularità: Il main non conosce la struttura interna delle funzioni.
 */
int main() {
    printf("=== ESECUZIONE MULTIFILE: DOMATIC PARTITION ===\n");

    // 1. Lettura
    Grafo *g = leggiGrafo("g.txt");
    printf("[Main] Grafo caricato con successo (%d vertici).\n", g->V);

    // 2. Verifica (Punto 2 esercizio)
    printf("\n--- Test Verifica Proposta ---\n");
    if (verificaPropostaFile(g, "proposta.txt")) {
        printf("[Main] La proposta e' VALIDA.\n");
    } else {
        printf("[Main] La proposta NON e' valida.\n");
    }

    // 3. Ottimizzazione (Punto 3 esercizio)
    printf("\n--- Test Ottimizzazione ---\n");
    trovaDomaticPartitionMassima(g);

    // 4. Pulizia
    liberaGrafo(g);
    printf("\n[Main] Memoria liberata. Fine.\n");
    
    return 0;
}