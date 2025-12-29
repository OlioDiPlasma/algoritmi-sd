/* main.c */
#include <stdio.h>
#include "grafo.h"

int main(int argc, char *argv[]) {
    /* --- 0. CONTROLLO ARGOMENTI --- */
    if (argc != 4) {
        printf("Uso: %s <file_grafo> <file_candidati> <file_output>\n", argv[0]);
        return 1;
    }

    char *fileGrafo = argv[1];
    char *fileCandidati = argv[2];
    char *fileOutput = argv[3];

    printf("--- INIZIO ELABORAZIONE ---\n");

    /* --- 1. LETTURA GRAFO (Punto 2) --- */
    printf("[1] Caricamento Grafo da %s...\n", fileGrafo);
    Grafo *g = leggiGrafo(fileGrafo);
    if (g == NULL) return 1;
    printf("    Grafo caricato con %d vertici.\n", g->N);

    /* --- 2. VERIFICA KERNEL (Punto 3) --- */
    printf("[2] Verifica Kernel da %s...\n", fileCandidati);
    if (verificaKernel(g, fileCandidati)) {
        printf("    RISULTATO: I vertici formano un KERNEL valido.\n");
    } else {
        printf("    RISULTATO: NON formano un Kernel.\n");
    }

    /* --- 3. KERNEL MINIMO (Punto 4) --- */
    printf("[3] Ricerca Kernel Minimo...\n");
    trovaEScriviKernelMinimo(g, fileOutput);
    printf("    Kernel minimo salvato in %s.\n", fileOutput);

    /* --- 4. CAMMINO OTTIMO (Punto 5) --- */
    printf("[4] Calcolo cammino su nodi Kernel...\n");
    // Nota: Usiamo il fileOutput generato al punto precedente come input qui, come suggerito dalla Nota Bene
    int lunghezza = calcolaMaxNodiKernel(g, fileOutput);
    printf("    Lunghezza del cammino che massimizza i nodi Kernel: %d\n", lunghezza);

    /* --- PULIZIA --- */
    liberaGrafo(g);
    printf("--- TERMINE ---\n");

    return 0;
}