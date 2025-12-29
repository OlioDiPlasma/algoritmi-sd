#include "supermercato.h"

int main() {
    /* --- 1. CARICAMENTO DATI --- */
    printf("Caricamento catalogo...\n");
    Catalog *cat = carica_catalogo("catalogo.txt");

    /* --- 2. VERIFICA FUNZIONALITA' (Richiesta 84) --- */
    printf("\n*** TEST 1: Ricerca Offerte per Prodotto ***\n");
    stampa_offerte_per_prodotto(cat, "Pane");
    stampa_offerte_per_prodotto(cat, "Latte");

    /* --- 3. VERIFICA FUNZIONALITA' (Richiesta 85) --- */
    printf("*** TEST 2: Prodotti in comune tra Offerte ***\n");
    prodotti_in_comune(cat, "OFF2", "OFF3"); // Nutella Pane / Pane Zucchero -> Pane comune

    /* --- 4. RISOLUZIONE PROBLEMA (Richiesta 86) --- */
    printf("*** TEST 3: Ottimizzazione Spesa ***\n");
    calcola_spesa_ottima(cat, "spesa.txt");

    /* --- 5. PULIZIA MEMORIA --- */
    libera_catalogo(cat);
    
    return 0;
}