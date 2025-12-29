/*
 * File: main.c
 * Scopo: Testare l'intera applicazione.
 */

#include "cruciverba.h"

int main() {
    printf("--- INIZIO PROGRAMMA CRUCIVERBA ---\n");

    // 1. Caricamento Dati
    // Assicurati di aver creato schema.txt e parole.txt
    printf("[1] Caricamento Schema...\n");
    schema_t *s = leggiSchema("schema.txt");
    if (!s) return -1;
    
    printf("    Griglia %dx%d caricata con %d slot.\n", s->R, s->C, s->n_slot);

    printf("[2] Caricamento Parole...\n");
    parole_t *p = leggiParole("parole.txt");
    printf("    Totale parole caricate: %d\n", p->n_parole_tot);

    // 2. Test Verifica (Opzionale, su griglia vuota fallirà o darà true se non ci sono vincoli)
    // Qui serve solo per dimostrare la chiamata richiesta dal punto 1.2.3
    printf("[3] Test Verifica (pre-soluzione)...\n");
    if (verificaSchema(s, p)) {
        printf("    Schema valido (strano se vuoto!)\n");
    } else {
        printf("    Schema non ancora completo/valido (Corretto).\n");
    }

    // 3. Risoluzione
    printf("[4] Avvio Ricerca Soluzione...\n");
    solve(s, p);

    // 4. Test Verifica Finale
    printf("[5] Test Verifica (post-soluzione)...\n");
    if (verificaSchema(s, p)) {
        printf("    Verifica passata: La soluzione rispetta il dizionario.\n");
    } else {
        printf("    Verifica fallita.\n");
    }

    // 5. Pulizia Memoria (Fondamentale!)
    liberaSchema(s);
    liberaParole(p);
    
    printf("--- FINE PROGRAMMA ---\n");
    return 0;
}