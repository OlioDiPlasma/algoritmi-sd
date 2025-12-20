#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main() {
    /* CREAZIONE GRAFO DI TEST
       Immaginiamo 5 nodi (0,1,2,3,4).
       Creiamo due componenti separate logicamente, ma collegate da archi costosi.
       Componente 1 potenziale: 0-1-2
       Componente 2 potenziale: 3-4
    */
    int V = 5;
    Graph g = GRAPHinit(V);

    printf("--- 1. Caricamento Grafo ---\n");
    /* Archi "interni" a gruppi */
    GRAPHinsertE(g, 0, 1, 10);
    GRAPHinsertE(g, 1, 2, 10);
    GRAPHinsertE(g, 0, 2, 10); // Questo crea un ciclo 0-1-2 se presi tutti
    
    GRAPHinsertE(g, 3, 4, 20);

    /* Archi "ponte" costosi o alternativi */
    GRAPHinsertE(g, 2, 3, 50); // Collega le due isole

    /* Se il grafo è disconnesso (es. se rimuovessimo 2-3), avremmo 2 componenti.
       Se è tutto connesso, ne abbiamo 1.
       Proviamo inizialmente lasciando tutto abilitato.
    */

    /* --- 2. CALCOLO COMPONENTI CONNESSE INIZIALI --- */
    CC cc = CCgen(g);
    printf("Numero Componenti Connesse iniziali: %d\n", cc->nCC);
    /* Poiché esiste l'arco 2-3 (costo 50), il grafo dovrebbe essere connesso (1 CC).
       Quindi dobbiamo scegliere V - 1 = 4 archi per fare un Spanning Tree.
       Attenzione: abbiamo 5 nodi e 5 archi totali (0-1, 1-2, 0-2, 3-4, 2-3).
       Uno di questi crea ciclo (il triangolo 0-1-2). L'algoritmo deve scartarne uno.
    */

    /* --- 3. OTTIMIZZAZIONE --- */
    printf("\n--- 3. Ricerca Alberi Ricoprenti Ottimi ---\n");
    Edge *bestSol = GRAPHgenOptTrees(g, cc);

    if (bestSol != NULL) {
        int k = V - cc->nCC;
        printf("Archi selezionati per l'ottimo:\n");
        for (int i = 0; i < k; i++) {
            printf("Arco %d-%d (peso %d)\n", bestSol[i].v, bestSol[i].w, bestSol[i].wt);
        }
        /* Importante: liberare la memoria della soluzione */
        free(bestSol);
    }

    /* --- 4. PULIZIA --- */
    CCfree(cc);
    GRAPHfree(g);

    return 0;
}