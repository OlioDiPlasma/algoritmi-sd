#include "grafo.h"

int main() {
    // 1. Caricamento Grafo
    printf("--- CARICAMENTO GRAFO ---\n");
    Graph *G = loadGraph("grafo.txt");
    if (!G) {
        printf("Errore: impossibile aprire grafo.txt\n");
        return 1;
    }
    printf("Grafo caricato: %d vertici, %d archi.\n\n", G->V, G->E);

    // 2. Verifica (Opzionale/Se richiesto da testo)
    // Proviamo a verificare se esiste una "soluzione.txt" precedente
    printf("--- VERIFICA SOLUZIONE ESISTENTE ---\n");
    FILE *f_check = fopen("soluzione.txt", "r");
    if (f_check) {
        fclose(f_check);
        if (verifySolution(G, "soluzione.txt")) {
            printf("La soluzione in 'soluzione.txt' e' VALIDA.\n");
        } else {
            printf("La soluzione in 'soluzione.txt' NON e' valida.\n");
        }
    } else {
        printf("Nessun file 'soluzione.txt' trovato per la verifica.\n");
    }
    printf("\n");

    // 3. Ricerca e Ottimizzazione
    printf("--- RICERCA OTTIMO (Triangle Packing) ---\n");
    Packing *best = findMaxTrianglePacking(G);
    
    printf("Trovato packing di cardinalita': %d\n", best->n_triangles);
    printf("Triangoli:\n");
    for(int i=0; i<best->n_triangles; i++) {
        printf("(%d, %d, %d) ", best->tris[i].u, best->tris[i].v, best->tris[i].w);
    }
    printf("\n");

    saveSolution(best, "soluzioneTROVATA.txt");
    printf("Soluzione ottima salvata in 'soluzioneTROVATA.txt'.\n");

    // Pulizia
    freePacking(best);
    freeGraph(G);

    return 0;
}