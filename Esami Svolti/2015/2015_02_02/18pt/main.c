#include "grafo.h"

/*
 * Scopo: Funzione principale.
 * - Verifica argomenti.
 * - Chiama lettura.
 * - Chiama verifica.
 * - Chiama ottimizzazione.
 * - Pulisce memoria.
 */
int main(int argc, char *argv[]) {
    
    /* Controllo argomenti da riga di comando */
    if (argc != 3) {
        printf("Uso corretto: %s <file_amicizie> <file_proposta>\n", argv[0]);
        return 1;
    }

    /* 1. Lettura e costruzione strutture dati */
    printf("Lettura grafo in corso...\n");
    Grafo *g = leggiGrafo(argv[1]);
    
    if (g == NULL) {
        return 1; /* Errore lettura */
    }

    /* 2. Verifica soluzione manuale (Punto 2) */
    verificaSoluzione(g, argv[2]);

    /* 3. Calcolo soluzione ottima (Punto 3) */
    calcolaMinimoColori(g);

    /* Pulizia finale */
    liberaGrafo(g);

    return 0;
}