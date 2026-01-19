#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

// Prototipi delle funzioni presenti in solver.c (o meglio, in un file .h separato)
void leggi_griglia(char *nome_file, Griglia *g);
void verifica_proposta(Griglia g);
void trova_soluzione_ottima(Griglia g);
void libera_griglia(Griglia *g);

int main() {
    Griglia g;

    /* --- 1. CARICAMENTO DATI --- */
    printf("Lettura file griglia.txt...\n");
    leggi_griglia("griglia.txt", &g);
    
    printf("Griglia %dx%d caricata. Celle bianche totali: %d\n", 
           g.nr, g.nc, g.celle_bianche_totali);

    /* --- 2. VERIFICA PROPOSTA (Punto 2 esercizio) --- */
    verifica_proposta(g);

    /* --- 3. RICERCA SOLUZIONE OTTIMA (Punto 3 esercizio) --- */
    trova_soluzione_ottima(g);

    /* --- 4. PULIZIA MEMORIA --- */
    libera_griglia(&g);

    printf("\nEsecuzione terminata.\n");
    return 0;
}