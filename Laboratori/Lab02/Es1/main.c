/*
 * File: main.c
 * Scopo: Leggere il file input.txt e testare la funzione gcd_ricorsivo.
 */

#include <stdio.h>
#include <stdlib.h>
#include "gcd.h"

int main() {
    /* --- 1. DICHIARAZIONE VARIABILI E PUNTATORI A FILE --- */
    // Definiamo un puntatore a FILE. E' una struttura che contiene le info sullo stream.
    FILE *fp;
    int num1, num2, risultato;

    // Nome del file di input
    char *nome_file = "input.txt";

    /* --- 2. APERTURA FILE --- */
    // 'r' sta per read (lettura).
    fp = fopen(nome_file, "r");

    // Controllo fondamentale: il puntatore e' NULL se il file non esiste.
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", nome_file);
        // Ritorniamo un codice di errore al sistema operativo
        return 1;
    }

    printf("--- INIZIO TEST MCD (BINARY GCD) ---\n");
    printf("Lettura dal file: %s\n\n", nome_file);

    /* --- 3. CICLO DI LETTURA --- */
    // fscanf ritorna il numero di campi letti correttamente. 
    // Finche' legge 2 interi (num1 e num2), il ciclo continua.
    // Usiamo l'indirizzo (&num1) perche' fscanf deve scrivere in quelle variabili.
    while (fscanf(fp, "%d %d", &num1, &num2) == 2) {
        
        // Chiamata alla funzione implementata in gcd.c
        risultato = gcd_ricorsivo(num1, num2);

        // Stampa formattata senza accenti per evitare problemi di encoding
        printf("MCD di %d e %d -> Risultato: %d\n", num1, num2, risultato);
    }

    /* --- 4. CHIUSURA E PULIZIA --- */
    // Chiudere sempre i file per rilasciare le risorse al sistema operativo.
    fclose(fp);
    
    printf("\n--- TEST COMPLETATO ---\n");

    return 0;
}