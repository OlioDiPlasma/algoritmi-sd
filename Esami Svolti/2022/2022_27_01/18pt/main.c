#include "esame.h"

/*
 * Main di Test
 * Scopo: Eseguire la pipeline completa: caricamento -> verifica -> ottimizzazione.
 */
int main() {
    // 1. Caricamento Dati
    printf("--- CARICAMENTO GRIGLIA ---\n");
    Griglia g = caricaGriglia("griglia.txt");
    if (g.celle == NULL) return 1;
    stampaGriglia(g);

    // 2. Verifica di una proposta (Parte 1)
    printf("--- VERIFICA PROPOSTA ---\n");
    int res = verificaCopertura(g, "proposta.txt");
    if (res != -1) {
        printf("La proposta e' VALIDA e usa %d regioni.\n\n", res);
    } else {
        printf("La proposta NON e' valida.\n\n");
    }

    // 3. Ricerca Soluzione Ottima (Parte 2)
    printf("--- OTTIMIZZAZIONE ---\n");
    trovaCoperturaOttima(g);

    // 4. Pulizia Finale
    liberaGriglia(g);
    
    return 0;
}