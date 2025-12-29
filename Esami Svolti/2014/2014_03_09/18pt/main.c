#include "grafo.h"

/*
 * Esempio invocazione: 
 * ./programma Nodi.txt 2 2 Output.txt
 * argv[0] = programma
 * argv[1] = Nodi.txt
 * argv[2] = k (distanza max)
 * argv[3] = m (grado max)
 * argv[4] = Output.txt (nome file output per generazione)
 */
int main(int argc, char *argv[]) {
    
    /* Controllo argomenti. Ne servono 5 totali (nome prog + 4 argomenti) */
    if (argc != 5) {
        printf("Uso: %s <file_nodi> <k> <m> <file_output>\n", argv[0]);
        return 1;
    }

    char *fileNodi = argv[1];
    int k = atoi(argv[2]);
    int m = atoi(argv[3]);
    char *fileOutput = argv[4];

    /* --- 1. CARICAMENTO DATI --- */
    printf("Lettura nodi da %s...\n", fileNodi);
    Grafo *g = leggiNodi(fileNodi);
    printf("Letti %d nodi.\n", g->N);

    /* --- 2. MENU SCELTA --- */
    int scelta;
    printf("\nSeleziona modalita':\n");
    printf("1. Verifica una soluzione da file (es. Archi.txt)\n");
    printf("2. Genera automaticamente soluzione ottima\n");
    printf("Scelta: ");
    if (scanf("%d", &scelta) != 1) return 0;

    if (scelta == 1) {
        /* Chiediamo il nome del file archi da verificare */
        char fileArchi[50];
        printf("Inserisci nome file archi (es. Archi.txt): ");
        scanf("%s", fileArchi);
        
        verificaSoluzione(g, fileArchi, k, m);
        
    } else if (scelta == 2) {
        /* Generazione automatica */
        generaSoluzioneOttima(g, fileOutput, k, m);
        
    } else {
        printf("Scelta non valida.\n");
    }

    /* --- 3. PULIZIA FINALE --- */
    liberaGrafo(g);

    return 0;
}