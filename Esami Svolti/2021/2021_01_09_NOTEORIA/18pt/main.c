#include "esame.h"

int main() {
    /* --- 1. CARICAMENTO --- */
    printf("--- CARICAMENTO DATI ---\n");
    Grafo *G = load_grafo("grafo.txt");
    if (!G) { printf("Errore apertura grafo.txt\n"); return 1; }
    
    int n_cicli = 0;
    Ciclo *cicli = load_cicli("cicli.txt", &n_cicli);
    printf("Letti %d vertici e %d cicli candidati.\n", G->V, n_cicli);

    /* --- 2. VERIFICA (6.2.3) --- */
    printf("\n--- VERIFICA CICLI ---\n");
    // Filtriamo i cicli: in un caso reale potremmo voler passare alla fase successiva
    // solo i cicli validi. Qui li segnaliamo solo.
    for (int i = 0; i < n_cicli; i++) {
        if (verifica_ciclo(G, cicli[i].nodi, cicli[i].len)) {
            printf("[OK] Ciclo %d valido\n", i);
        } else {
            printf("[NO] Ciclo %d non valido nel grafo\n", i);
        }
    }

    /* --- 3. OTTIMIZZAZIONE (6.2.4) --- */
    printf("\n--- OTTIMIZZAZIONE COPERTURA ---\n");
    
    // Chiamata alla funzione wrapper in esame.c
    Soluzione sol = trova_copertura_ottima(G, cicli, n_cicli);

    if (sol.count > n_cicli) {
        printf("Nessuna copertura trovata con i cicli forniti.\n");
    } else {
        printf("Soluzione Ottima:\n");
        printf("- Numero cicli: %d\n", sol.count);
        printf("- Tipologia: %s\n", sol.disjoint ? "Disgiunta (Ottimo!)" : "Sovrapposta");
        printf("- Cicli usati: ");
        for (int i = 0; i < sol.count; i++) {
            printf("%d ", sol.cicli_scelti[i]);
        }
        printf("\n");
        
        // Stampa dettagliata del contenuto dei cicli scelti
        for (int i = 0; i < sol.count; i++) {
            int id = sol.cicli_scelti[i];
            printf("  -> Ciclo %d: ", id);
            for(int j=0; j<cicli[id].len; j++) printf("%d ", cicli[id].nodi[j]);
            printf("\n");
        }
    }

    /* --- 4. DEALLOCAZIONE --- */
    free(sol.cicli_scelti); // Importante: liberare la memoria allocata dentro la funzione wrapper
    free_cicli(cicli, n_cicli);
    free_grafo(G);

    return 0;
}