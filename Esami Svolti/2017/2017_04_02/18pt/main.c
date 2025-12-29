/* * File: main.c
 * Scopo: Testare il programma come richiesto dalla traccia.
 * Compilazione: gcc main.c graph.c -o esame
 * Esecuzione: ./esame grafo.txt proposta.txt
 */

#include "graph.h"

int main(int argc, char *argv[]) {
    // Controllo argomenti riga di comando
    if (argc != 3) {
        printf("Uso: %s <file_grafo> <file_proposta>\n", argv[0]);
        return 1;
    }

    // 1. Caricamento Grafo
    printf("--- CARICAMENTO GRAFO ---\n");
    Graph *G = loadGraph(argv[1]);
    printf("Grafo caricato: %d vertici, %d archi.\n", G->V, G->E);

    // 2. Verifica Soluzione (Punto 1 della traccia)
    checkSolution(G, argv[2]);

    // 3. & 4. & 5. Trova Cricche Massimali, Max Vertici e Cicli Hamiltoniani
    solveCliques(G);

    // Pulizia memoria
    freeGraph(G);

    return 0;
}