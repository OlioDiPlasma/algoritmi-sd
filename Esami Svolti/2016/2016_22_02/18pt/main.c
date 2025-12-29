/*
 * File: main.c
 * Scopo: Testare le funzionalità richieste.
 * Compilazione: gcc main.c grafo.c -o esame
 */

#include "grafo.h"

int main() {
    printf("--- INIZIO ESECUZIONE ---\n");

    /* 1. Caricamento e Verifica Congruenza */
    /* Se ci sono errori di colore, il programma terminerà o segnalerà errore */
    Grafo *g = caricaGrafo("grafo.txt");

    if (g == NULL) {
        printf("Errore nel caricamento del grafo.\n");
        return -1;
    }

    printf("Grafo caricato con %d vertici.\n", g->nV);

    /* 2. Ricerca Cammino Massimo (Il cuore da 18 punti) */
    cercaCamminoMassimo(g);

    /* Pulizia finale */
    liberaGrafo(g);
    
    printf("\n--- FINE ESECUZIONE ---\n");
    return 0;
}