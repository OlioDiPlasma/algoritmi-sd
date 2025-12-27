#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main() {
    /* --- 1. PREPARAZIONE DATI --- */
    printf("--- Inizio Test Huffman Wrapper ---\n");

    // Creiamo l'albero definito manualmente in huffman.c
    // Struttura: A=0, B=10, C=11
    H albero = crea_albero_test_hardcoded();

    // Stringa di input: "0" (A), "10" (B), "11" (C), "0" (A)
    // Risultato atteso: "ABCA"
    char *input_code = "010110"; 

    printf("Input codificato: %s\n", input_code);
    printf("Codici attesi: A=0, B=10, C=11\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    // La funzione decode alloca nuova memoria e ci ritorna il puntatore
    char *testo_in_chiaro = decode(albero, input_code);

    /* --- 3. STAMPA RISULTATI --- */
    if (testo_in_chiaro != NULL) {
        printf("Risultato decodifica: %s\n", testo_in_chiaro);
    } else {
        printf("Errore nella decodifica.\n");
    }

    /* --- 4. PULIZIA MEMORIA (Memory Leak Prevention) --- */
    // Dobbiamo liberare la stringa allocata dentro 'decode'
    free(testo_in_chiaro);
    printf("Memoria stringa liberata.\n");

    // Dobbiamo liberare l'albero
    libera_albero(albero);
    printf("Memoria albero liberata.\n");

    return 0;
}