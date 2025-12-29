#include "grafo.h"

int main(int argc, char *argv[]) {
    // Verifica argomenti
    if (argc < 2) {
        printf("Uso: %s <file_input>\n", argv[0]);
        // Per facilitare il debug su VSCode senza argomenti, forzo un nome file di default se manca
        printf("Uso file di default 'input.txt'...\n");
        // exit(1);
    }
    
    char *filename = (argc >= 2) ? argv[1] : "input.txt";

    /* --- 1. LETTURA GRAFO --- */
    printf("--- FASE 1: Caricamento Grafo ---\n");
    Grafo *g = leggiGrafo(filename);
    if (g == NULL) return -1;
    stampaGrafo(g);

    /* --- 2. CALCOLO K-CORE --- */
    // Lettura k da tastiera
    int k;
    printf("\nInserisci k per il calcolo del k-core: ");
    scanf("%d", &k);
    
    // Nota: calcolaKCore modifica lo stato 'attivo' dei vertici
    calcolaKCore(g, k);

    /* --- 3. VERIFICA J-EDGE-CONNECTED --- */
    // Lettura j da tastiera
    int j;
    printf("\nInserisci j per la verifica j-edge-connected: ");
    scanf("%d", &j);

    // Questa funzione resetterà internamente i flag 'attivo' per lavorare sul grafo originale
    verificaJEdgeConnected(g, j);

    /* --- 4. LIBERA MEMORIA --- */
    liberaGrafo(g);
    printf("\nMemoria liberata. Programma terminato.\n");

    return 0;
}