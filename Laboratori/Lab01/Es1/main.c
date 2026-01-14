/*
 * File: main.c
 * Scopo: Driver per testare la funzione cercaRegexp leggendo da file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regexp.h"

#define MAX_LINE 256 // Lunghezza massima ipotetica di una riga del file

int main() {
    /* --- 1. APERTURA FILE E VARIABILI --- */
    FILE *fp;
    char buffer[MAX_LINE];
    char temp_src[100], temp_reg[100]; // Buffer temporanei per la lettura

    printf("--- INIZIO PROGRAMMA DI TEST REGEX ---\n\n");

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire input.txt\n");
        return 1;
    }

    /* --- 2. LETTURA E TEST --- */
    // Leggiamo coppie di stringhe: sorgente e regex
    while (fscanf(fp, "%s %s", temp_src, temp_reg) == 2) {
        
        /* * Gestione Memoria (MALLOC):
         * Anche se potremmo usare temp_src, per esercizio allochiamo
         * dinamicamente lo spazio esatto necessario per le stringhe.
         * sizeof(char) e' sempre 1, ma lo scrivo per chiarezza didattica.
         * Aggiungiamo +1 per il terminatore '\0'.
         */
        
        int len_src = strlen(temp_src);
        int len_reg = strlen(temp_reg);

        char *d_src = (char *)malloc((len_src + 1) * sizeof(char));
        char *d_reg = (char *)malloc((len_reg + 1) * sizeof(char));

        if (d_src == NULL || d_reg == NULL) {
            printf("Errore critico di memoria (malloc fallita)\n");
            exit(1);
        }

        // Copiamo dai buffer temporanei alla memoria heap
        strcpy(d_src, temp_src);
        strcpy(d_reg, temp_reg);

        /* --- 3. CHIAMATA ALLA FUNZIONE --- */
        printf("Testando -> Src: '%s' | Regex: '%s'\n", d_src, d_reg);
        
        char *risultato = cercaRegexp(d_src, d_reg);

        /* --- 4. VERIFICA E STAMPA --- */
        if (risultato != NULL) {
            // Calcoliamo l'indice sottraendo i puntatori (Aritmetica dei puntatori)
            // (risultato - d_src) ci da' la distanza in celle di memoria (indice)
            int index = (int)(risultato - d_src);
            printf("  [SUCCESSO] Trovato all'indice %d. Sottostringa: '%s'\n", index, risultato);
        } else {
            printf("  [FALLIMENTO] Nessuna occorrenza trovata.\n");
        }
        
        printf("--------------------------------------------------\n");

        /* --- 5. PULIZIA MEMORIA (FREE) --- */
        // Fondamentale: liberare la memoria allocata nel ciclo
        // per evitare Memory Leaks (perdita di memoria).
        free(d_src);
        free(d_reg);
    }

    fclose(fp);
    printf("\n--- FINE TEST ---\n");

    return 0;
}