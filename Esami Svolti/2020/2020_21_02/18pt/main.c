#include "ristorante.h"

/* --- MAIN --- */

/*
 * Parametri del main:
 * argc: numero argomenti
 * argv: array di stringhe argomenti (argv[1] sarà P)
 */
int main(int argc, char *argv[]) {
    
    // Controllo argomenti
    if (argc != 2) {
        printf("Uso corretto: %s <numero_piatti_P>\n", argv[0]);
        return -1;
    }

    int P = atoi(argv[1]); // Converto stringa in intero
    if (P <= 0) {
        printf("P deve essere positivo.\n");
        return -1;
    }

    int N;
    // 1. Caricamento dati
    Piatto *elenco = leggiPiatti("piatti.txt", &N);
    printf("Caricati %d piatti dal file.\n\n", N);

    // 2. Generazione Menu (Algoritmo + BST)
    printf("Generazione dei menu con P=%d...\n", P);
    BSTNode *alberoMenu = generaMenu(elenco, N, P);

    // 3. Stampa Risultati (Visita BST)
    printf("\n--- ELENCO MENU POSSIBILI (Ordinati) ---\n");
    stampaBST(alberoMenu, elenco);

    // 4. Pulizia Memoria
    freeBST(alberoMenu);
    free(elenco);

    return 0;
}