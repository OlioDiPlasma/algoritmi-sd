#include <stdio.h>
#include <stdlib.h>
#include "gestione_articoli.h"
#include "programma.h"

int main(int argc, char *argv[]) {
    /* 1. GESTIONE PARAMETRI
     * Se non passati, usiamo default R=2, S=5 per coerenza col file di test
     */
    int R = 2, S = 8; 
    if (argc >= 3) {
        R = atoi(argv[1]);
        S = atoi(argv[2]);
    }

    printf("--- CONFIGURAZIONE: %d Sale, %d Slot ---\n", R, S);

    /* 2. CARICAMENTO DATI */
    Articoli art_db = carica_articoli("articoli.txt");
    if (art_db == NULL) {
        printf("Errore apertura articoli.txt\n");
        return -1;
    }
    printf("Caricati %d articoli.\n", get_num_articoli(art_db));

    /* 3. INIZIALIZZAZIONE STRUTTURE */
    Programma p = init_programma(R, S);

    /* --- PARTE 1: VERIFICA (4.2.3) --- */
    printf("\n>>> TEST VERIFICA <<<\n");
    // Nota: resetto la griglia se necessario, ma qui è vuota all'inizio
    // Creiamo una copia temporanea o puliamo dopo la verifica?
    // Per semplicità, eseguiamo verifica e poi puliamo per l'ottimizzazione.
    if (verifica_programma_da_file("soluzione_input.txt", p, art_db)) {
        stampa_programma(p, art_db);
    }
    
    // PULIZIA PER OTTIMIZZAZIONE
    // Resettiamo la griglia a -1
    for(int i=0; i<R; i++) 
        for(int j=0; j<S; j++) 
            p.griglia[i][j] = -1;

    /* --- PARTE 2: OTTIMIZZAZIONE (4.2.4) --- */
    printf("\n>>> TEST OTTIMIZZAZIONE <<<\n");
    genera_programma_ottimo(p, art_db);

    /* 4. FREE MEMORIA TOTALE */
    free_programma(p);
    free_articoli(art_db);

    return 0;
}