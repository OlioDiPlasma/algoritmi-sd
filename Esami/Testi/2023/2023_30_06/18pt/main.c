#include <stdio.h>
#include <stdlib.h>
#include "puzzle.h"

/*
 * ======================================================================================
 * FILE: main.c
 * ======================================================================================
 * SCOPO:
 * Driver del programma. Simula lo svolgimento dell'esame eseguendo
 * sequenzialmente i due task richiesti: Verifica e Ottimizzazione.
 * ======================================================================================
 */

int main() {
    printf("=== ESAME PROGRAMMAZIONE: SLIDING PUZZLE ===\n");

    /* --- 1. CARICAMENTO INIZIALE --- */
    /* Carichiamo la griglia dal file di testo nella memoria (Heap) */
    Game *g = load_game("grid.txt");
    
    /* Controllo difensivo: se il file non esiste, terminiamo */
    if (g == NULL) {
        printf("Errore critico: impossibile caricare grid.txt\n");
        return 1;
    }

    printf("Griglia caricata (%dx%d).\n", g->N, g->N);
    print_grid(g); // Mostra lo stato iniziale a video

    /* ==================================================================================
     * FASE 1: PROBLEMA DI VERIFICA
     * ================================================================================== */
    printf("--- FASE 1: VERIFICA MOSSE ---\n");
    
    /* Chiamiamo la funzione che legge 'mosse.txt' e applica le mosse.
     * ATTENZIONE: Questa funzione MODIFICA la griglia puntata da 'g'!
     * Le tessere verranno spostate fisicamente in memoria.
     */
    problema_verifica(g, "mosse.txt");


    /* ==================================================================================
     * FASE INTERMEDIA: RESET CRITICO DELLO STATO
     * ==================================================================================
     * PROBLEMA:
     * La griglia 'g' ora è modificata (e probabilmente risolta).
     * Se passassimo questo 'g' alla fase di ottimizzazione, la funzione direbbe
     * subito "Ho vinto in 0 mosse!" perché i tubi sono già collegati.
     *
     * SOLUZIONE:
     * Dobbiamo "buttare via" la griglia sporca e ricaricarne una pulita dal disco.
     */
    
    printf("\n--- RESET DEL GIOCO (Ripristino stato iniziale) ---\n");
    
    free_game(g);              // 1. Distruggo la griglia modificata (evito memory leak)
    g = load_game("grid.txt"); // 2. Ricarico la griglia vergine dal file
    
    if (g == NULL) return 1;   // Controllo paranoia


    /* ==================================================================================
     * FASE 2: PROBLEMA DI OTTIMIZZAZIONE
     * ================================================================================== */
    printf("\n--- FASE 2: RICERCA SOLUZIONE (OTTIMIZZAZIONE) ---\n");
    
    /* Ora 'g' è pulito, quindi l'algoritmo dovrà lavorare davvero per trovare
     * la sequenza di mosse. Proviamo con una profondità massima di 10. */
    problema_ottimizzazione(g, 10);

    /* --- PULIZIA FINALE --- */
    /* Come bravi programmatori C, liberiamo l'ultima istanza della griglia prima di uscire */
    free_game(g);
    
    printf("\n=== TERMINE PROGRAMMA (Memoria liberata) ===\n");

    return 0;
}