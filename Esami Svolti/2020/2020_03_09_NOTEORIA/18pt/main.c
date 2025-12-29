#include "gamelib.h"

int main() {
    int R, C;
    int **mappa = NULL;

    /* --- PARTE 1: CARICAMENTO --- */
    FILE *f = fopen("griglia.txt", "r");
    if (f == NULL) {
        printf("Errore apertura file griglia.txt\n");
        return 1;
    }
    
    // [cite: 79] Chiamata alla funzione di lettura
    mappa = leggiMappa(f, &R, &C);
    fclose(f);

    printf("Mappa Iniziale (%dx%d):\n", R, C);
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) printf("%d ", mappa[i][j]);
        printf("\n");
    }
    printf("\n-----------------\n");

    /* --- PARTE 2: VERIFICA [cite: 84] --- */
    // Verifichiamo il file 'soluzione.txt' contro la mappa caricata
    printf("Test Verifica su 'soluzione.txt': ");
    if (verifica("soluzione.txt", mappa)) {
        printf("VALIDA\n");
    } else {
        printf("NON VALIDA\n");
    }
    printf("\n-----------------\n");

    /* --- PARTE 3: SOLVE [cite: 89] --- */
    printf("Tentativo di risoluzione automatica...\n");
    solve(mappa, R, C);

    /* --- PULIZIA MEMORIA --- */
    liberaMatrice(mappa, R);

    return 0;
}