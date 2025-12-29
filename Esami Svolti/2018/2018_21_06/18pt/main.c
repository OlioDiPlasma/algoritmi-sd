#include "defines.h"

int main() {
    FILE *fp;
    int T, R, C, i, j;
    Tessera *tessere;
    Cella **scacchiera, **bestScacchiera;

    /* --- CARICAMENTO TESSERE --- */
    fp = fopen("tiles.txt", "r");
    if (fp == NULL) { perror("Errore tiles.txt"); return 1; }

    fscanf(fp, "%d", &T);
    tessere = (Tessera *)malloc(T * sizeof(Tessera));
    
    for (i = 0; i < T; i++) {
        fscanf(fp, " %c %d %c %d", &tessere[i].c1, &tessere[i].v1, &tessere[i].c2, &tessere[i].v2);
        tessere[i].usata = 0;
    }
    fclose(fp);

    /* --- CARICAMENTO SCACCHIERA --- */
    fp = fopen("board.txt", "r");
    if (fp == NULL) { perror("Errore board.txt"); return 1; }
    
    fscanf(fp, "%d %d", &R, &C);

    scacchiera = (Cella **)malloc(R * sizeof(Cella *));
    bestScacchiera = (Cella **)malloc(R * sizeof(Cella *));
    
    for (i = 0; i < R; i++) {
        scacchiera[i] = (Cella *)malloc(C * sizeof(Cella));
        bestScacchiera[i] = (Cella *)malloc(C * sizeof(Cella));
        
        for (j = 0; j < C; j++) {
            fscanf(fp, "%d/%d", &scacchiera[i][j].id_tessera, &scacchiera[i][j].rot);
            
            // CORREZIONE 1: Inizializziamo subito bestScacchiera con lo stato attuale
            // Così se non troviamo soluzioni, almeno stampiamo la scacchiera iniziale e non "garbage"
            bestScacchiera[i][j] = scacchiera[i][j];

            if (scacchiera[i][j].id_tessera != -1) {
                scacchiera[i][j].fissa = 1;
                // Controllo di sicurezza: l'indice deve essere valido
                if(scacchiera[i][j].id_tessera < T) {
                    tessere[scacchiera[i][j].id_tessera].usata = 1; 
                }
            } else {
                scacchiera[i][j].fissa = 0;
            }
        }
    }
    fclose(fp);

    printf("--- DATI CARICATI ---\n");
    printf("Tessere totali: %d, Celle griglia: %d (Richieste: %d)\n", T, R*C, R*C);
    
    // Controllo rapido pre-algoritmo
    if (T < R*C) {
        printf("ATTENZIONE: Tessere insufficienti per coprire la griglia! L'algoritmo fallira'.\n");
    }

    /* --- AVVIO BACKTRACKING --- */
    printf("\n--- AVVIO RICERCA SOLUZIONE OTTIMA ---\n");
    
    int maxPunti = -1; // Rimane -1 se non troviamo nessuna soluzione valida completa
    
    risolviBacktracking(0, scacchiera, R, C, tessere, T, &maxPunti, bestScacchiera);

    if (maxPunti == -1) {
        printf("\nNESSUNA SOLUZIONE COMPLETA TROVATA.\n");
        printf("Verifica che il numero di tessere nel file sia sufficiente per riempire i buchi.\n");
    } else {
        printf("\nSoluzione Trovata! Punteggio Massimo: %d\n", maxPunti);
        printf("Configurazione Ottima (Indice/Rotazione):\n");
        for(i=0; i<R; i++) {
            for(j=0; j<C; j++) {
                printf("%d/%d\t", bestScacchiera[i][j].id_tessera, bestScacchiera[i][j].rot);
            }
            printf("\n");
        }
    }

    /* --- FREE MEMORY --- */
    for (i = 0; i < R; i++) {
        free(scacchiera[i]);
        free(bestScacchiera[i]);
    }
    free(scacchiera);
    free(bestScacchiera);
    free(tessere);

    return 0;
}