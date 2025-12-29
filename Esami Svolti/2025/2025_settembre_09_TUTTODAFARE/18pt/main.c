#include "trasporti.h"

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: 
 * 1. Caricare i dati dai file.
 * 2. Definire MP e MV (input hardcoded o letti).
 * 3. Eseguire l'ottimizzazione.
 * 4. Stampare i risultati formattati.
 * ============================================================================
 */
int main() {
    // 1. Lettura Input
    printf("--- Caricamento Dati ---\n");
    PersoneWrapper P = leggi_persone("persone.txt");
    VincoliWrapper V = leggi_vincoli("vincoli.txt");

    // Parametri Minibus (Esempio)
    int MP = 4; // Max 4 persone
    int MV = 6; // Max 6 valigie

    printf("Persone: %d, Vincoli: %d\n", P.n_persone, V.n_vincoli);
    printf("Capacita' Bus -> Persone: %d, Valigie: %d\n", MP, MV);

    // 2. Preparazione Struttura Soluzione
    Soluzione best_sol;
    best_sol.assegnazioni = (int *)malloc(P.n_persone * sizeof(int));
    // Importante: inizializziamo a valori nulli/sicuri
    for(int i=0; i<P.n_persone; i++) best_sol.assegnazioni[i] = -1;

    // 3. Chiamata alla funzione (Domanda 7)
    printf("\n--- Calcolo Ottimizzazione in corso... ---\n");
    calcola_distribuzione_ottima(P, V, MP, MV, &best_sol);

    // 4. Stampa Risultati
    printf("\n--- RISULTATO OTTIMO ---\n");
    printf("Minibus necessari: %d\n", best_sol.n_minibus_usati);
    printf("KM totali percorsi: %.2f\n", best_sol.distanza_totale);

    for (int b = 0; b < best_sol.n_minibus_usati; b++) {
        printf("\n[MINIBUS %d]:\n", b+1);
        float dist_parziale = 0;
        int val_parz = 0;
        
        // Stampa chi c'è dentro
        for (int i = 0; i < P.n_persone; i++) {
            if (best_sol.assegnazioni[i] == b) {
                printf("  - %s (Valigie: %d, Dist: %.1f)\n", 
                       P.elenco[i].nome, P.elenco[i].num_valigie, P.elenco[i].distanza);
                val_parz += P.elenco[i].num_valigie;
            }
        }
        printf("  > Totale Valigie: %d/%d\n", val_parz, MV);
    }

    // 5. Verifica Validità su un bus specifico (Test Domanda 6)
    // Creiamo un test case: verifichiamo se il primo bus della soluzione è valido
    printf("\n--- Test Verifica (Domanda 6) sul Bus 1 ---\n");
    // Estraiamo gli indici del primo bus
    int *test_bus = malloc(P.n_persone * sizeof(int));
    int count = 0;
    for(int i=0; i<P.n_persone; i++) {
        if(best_sol.assegnazioni[i] == 0) test_bus[count++] = i;
    }
    
    if (verifica_validita(test_bus, count, P, V, MP, MV)) {
        printf("Il Bus 1 e' VALIDO.\n");
    } else {
        printf("Il Bus 1 NON e' valido (Errore logico!).\n");
    }
    free(test_bus);

    // Pulizia
    libera_tutto(P, V, best_sol);
    
    return 0;
}