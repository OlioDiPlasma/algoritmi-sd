#include "grafo.h"

/*
 * Compilazione suggerita (da terminale/vscode):
 * gcc main.c grafo.c -o esame
 * * Esecuzione:
 * ./esame grafo.txt verifica.txt risultato.txt
 */

int main(int argc, char *argv[]) {
    /* Controllo argomenti linea di comando */
    if (argc != 4) {
        printf("Errore: Numero di parametri errato.\n");
        printf("Uso: %s <file_grafo> <file_verifica> <file_output>\n", argv[0]);
        return 1;
    }

    FILE *f_grafo, *f_verifica;
    Graph *G;

    /* --- 1. CARICAMENTO GRAFO --- */
    f_grafo = fopen(argv[1], "r");
    if (f_grafo == NULL) {
        printf("Errore apertura file grafo %s\n", argv[1]);
        return 1;
    }

    printf("Caricamento grafo in corso...\n");
    G = initGraph();
    loadGraph(G, f_grafo);
    fclose(f_grafo);
    
    printGraph(G); /* Visualizza struttura per debug */

    /* --- 2. VERIFICA --- */
    f_verifica = fopen(argv[2], "r");
    if (f_verifica == NULL) {
        printf("Errore apertura file verifica %s\n", argv[2]);
        freeGraph(G);
        return 1;
    }

    printf("\n--- Esecuzione Verifica ---\n");
    if (verificaIndipendente(G, f_verifica)) {
        printf("RISULTATO: Il file %s contiene un Insieme Indipendente VALIDO.\n", argv[2]);
    } else {
        printf("RISULTATO: Il file %s NON contiene un Insieme Indipendente.\n", argv[2]);
    }
    fclose(f_verifica);

    /* --- 3. IDENTIFICAZIONE MASSIMALE (OTTIMIZZAZIONE) --- */
    printf("\n--- Ricerca Insieme Indipendente Massimale ---\n");
    int card = trovaInsiemeMassimale(G, argv[3]);
    
    printf("Insieme Massimale identificato e salvato in %s.\n", argv[3]);
    printf("Numero di Indipendenza (cardinalita' insieme trovato): %d\n", card);

    /* --- 4. PULIZIA --- */
    freeGraph(G);
    
    return 0;
}