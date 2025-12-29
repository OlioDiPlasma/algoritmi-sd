/*
 * File: main.c
 * Scopo: Testare il programma completo.
 * Compilazione: gcc main.c nonogram.c -o nonogram
 * Esecuzione: ./nonogram vincoli.txt verifica.txt output_soluzione.txt
 */

#include "nonogram.h"

int main(int argc, char *argv[]) {
    // Controllo argomenti. Servono 3 file: vincoli, verifica_input, output
    if (argc != 4) {
        printf("Uso: %s <file_vincoli> <file_verifica> <file_output>\n", argv[0]);
        return 1;
    }

    printf("--- INIZIO PROGRAMMA ---\n");

    /* --- PARTE 1: Caricamento Dati --- */
    printf("[1] Caricamento vincoli da '%s'...\n", argv[1]);
    Schema *gioco = carica_vincoli(argv[1]);
    if (gioco == NULL) return -1;
    printf("    Vincoli caricati. Griglia %dx%d.\n", gioco->R, gioco->C);

    /* --- PARTE 2: Verifica Soluzione Esistente --- */
    printf("[2] Verifica soluzione manuale da '%s'...\n", argv[2]);
    leggi_soluzione_da_file(gioco, argv[2]);
    
    if (verifica_soluzione(gioco)) {
        printf("    RISULTATO: La soluzione nel file e' VALIDA.\n");
    } else {
        printf("    RISULTATO: La soluzione nel file NON e' valida.\n");
    }

    /* --- PARTE 3: Risoluzione Automatica --- */
    printf("[3] Calcolo soluzione ricorsiva...\n");
    // Pulisce la griglia e cerca da zero
    if (risolvi_nonogram(gioco)) {
        printf("    Soluzione trovata! Salvataggio in '%s'...\n", argv[3]);
        salva_soluzione(gioco, argv[3]);
        
        // Stampa a video per controllo rapido
        printf("    Anteprima griglia:\n");
        for(int i=0; i<gioco->R; i++) {
            for(int j=0; j<gioco->C; j++) {
                printf("%c ", gioco->griglia[i][j] ? '#' : '.');
            }
            printf("\n");
        }
    } else {
        printf("    Nessuna soluzione trovata.\n");
    }

    /* --- PULIZIA --- */
    libera_schema(gioco);
    printf("--- FINE PROGRAMMA ---\n");

    return 0;
}