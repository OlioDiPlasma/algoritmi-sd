#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    /* --- 1. SETUP --- */
    FILE *fp = fopen("labirinto.txt", "r");
    if (fp == NULL) { printf("Errore apertura labirinto.txt\n"); return 1; }

    /* Concetto: Carichiamo il grafo una volta sola */
    Graph g = GRAPHload(fp);
    fclose(fp);

    /* --- 2. TEST VERIFICA (3.2.3) --- */
    printf("\n--- TEST VERIFICA ---\n");
    fp = fopen("percorso.txt", "r");
    if (fp != NULL) {
        Path p = GRAPHpathLoad(fp);
        fclose(fp);
        
        // Esempio: M=10, PF=3
        int M = 10, PF = 3;
        int valid = GRAPHpathCheck(g, &p, M, PF);
        
        if (valid) {
            printf("Percorso valido! Ricchezza accumulata: %d\n", p.ricchezza);
        } else {
            printf("Percorso NON valido.\n");
        }
        // Pulizia path
        // (Nota: per brevità non ho scritto PATHfree completa, ma servirebbe loop free stringhe)
    } else {
        printf("Crea percorso.txt per testare la verifica.\n");
    }

    /* --- 3. TEST OTTIMIZZAZIONE (3.2.4) --- */
    /* Wrapper function call */
    printf("\n--- TEST OTTIMIZZAZIONE ---\n");
    int M_opt = 6; 
    int PF_opt = 3;
    printf("Cerco percorso ottimo con M=%d, PF=%d...\n", M_opt, PF_opt);
    
    Path best = GRAPHpathBest(g, M_opt, PF_opt);
    
    if (best.num_stanze > 0) {
        PATHprint(best);
    } else {
        printf("Nessun percorso trovato o ricchezza 0.\n");
    }

    /* --- 4. CLEANUP --- */
    GRAPHfree(g);
    
    return 0;
}