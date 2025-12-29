#include <stdio.h>
#include "cruci.h" // <--- IMPORTANTE: .h NON .c

int main() {
    printf("--- ESAME CRUCIPUZZLE ---\n");

    // 1. Caricamento
    GRID g = carica_griglia("griglia.txt");
    WORDS w = carica_parole("parole.txt");
    printf("Griglia %dx%d e %d parole caricate.\n", g.R, g.C, w.n_parole);

    // 2. Verifica (opzionale se non hai il file proposta.txt funzionante)
    // SOL prop = carica_proposta("proposta.txt", w);
    // verifica_soluzione(g, w, prop);
    // libera_soluzione(prop);

    // 3. Soluzione Ottima
    SOL ottima = trova_max_soluzione(g, w);
    
    printf("\n*** SOLUZIONE OTTIMA ***\n");
    printf("Valore: %d, Parole usate: %d\n", ottima.total_val, ottima.n_found);
    for(int i=0; i<ottima.n_found; i++) {
        Placement p = ottima.placements[i];
        printf("-> %s (%d) @ [%d,%d] dir:%d\n", 
               w.elenco[p.word_idx].str, w.elenco[p.word_idx].val, p.r, p.c, p.dir);
    }

    // 4. Free
    libera_griglia(g);
    libera_parole(w);
    libera_soluzione(ottima);

    return 0;
}