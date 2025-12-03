#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

/* Prototipi (se non inclusi altrove) */
int verifica_soluzione(Graph G, int k, const char *file_soluzione);
void solve_optimization(Graph G, int k);

int main() {
    /* 1. CARICAMENTO GRAFO */
    printf("--- Lettura grafo da 'grafo.txt' ---\n");
    Graph G = GRAPHload("grafo.txt");
    
    if (G == NULL) {
        printf("ERRORE: Impossibile aprire o leggere 'grafo.txt'.\n");
        printf("Assicurati che il file esista nella cartella dell'eseguibile.\n");
        return 1;
    }
    printf("Grafo caricato: %d vertici.\n", GRAPHgetV(G));

    int k = 3; // Parametro dell'esercizio

    /* 2. TEST VERIFICA */
    printf("\n--- Test Verifica Soluzione (da 'sol_ok.txt') ---\n");
    // Proviamo a leggere il file manuale
    FILE *fcheck = fopen("sol_ok.txt", "r");
    if (fcheck) {
        fclose(fcheck);
        int valida = verifica_soluzione(G, k, "sol_ok.txt");
        if (valida) 
            printf(">> RISULTATO: La soluzione nel file e' VALIDA.\n");
        else 
            printf(">> RISULTATO: La soluzione nel file NON e' valida.\n");
    } else {
        printf("File 'sol_ok.txt' non trovato. Salto il test di verifica.\n");
    }

    /* 3. TEST OTTIMIZZAZIONE */
    printf("\n--- Test Ottimizzazione (Backtracking) ---\n");
    printf("Cerco il partizionamento a costo minimo con k=%d...\n", k);
    
    solve_optimization(G, k);

    /* 4. PULIZIA */
    GRAPHfree(G);
    return 0;
}