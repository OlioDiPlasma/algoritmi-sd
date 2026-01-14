#include <stdio.h>
#include <stdlib.h> // Necessario per malloc, free, exit
#include "majority.h"

/*
 * Scopo:
 * 1. Leggere input da file.
 * 2. Allocare memoria dinamicamente per il vettore.
 * 3. Chiamare la funzione majority.
 * 4. Gestire la pulizia della memoria.
 */

int main() {
    /* --- 1. APERTURA FILE --- */
    FILE *fp;
    // Apro il file in modalità lettura ("r" - read)
    fp = fopen("input.txt", "r");
    
    // Controllo se il file è stato aperto correttamente (puntatore non NULL)
    if (fp == NULL) {
        fprintf(stderr, "Errore: Impossibile aprire input.txt\n");
        return 1; // Ritorno codice di errore al sistema
    }

    /* --- 2. LETTURA DIMENSIONE --- */
    int N;
    // Leggo un intero dal file e lo salvo nell'indirizzo di N (&N)
    if (fscanf(fp, "%d", &N) != 1) {
        printf("Errore nella lettura di N\n");
        fclose(fp);
        return 1;
    }

    printf("Dimensione letta: %d\n", N);

    /* --- 3. ALLOCAZIONE DINAMICA MEMORIA --- */
    // Malloc riserva un blocco di memoria nello HEAP.
    // Dimensione totale in byte = numero elementi (N) * dimensione di un intero (sizeof(int))
    int *vet = (int *)malloc(N * sizeof(int));

    // E' CRUCIALE verificare se malloc ha avuto successo
    if (vet == NULL) {
        printf("Errore: Memoria insufficiente (Malloc fallita)\n");
        fclose(fp); // Chiudo il file prima di uscire
        return 1;
    }

    /* --- 4. POPOLAMENTO VETTORE --- */
    int i;
    printf("Elementi del vettore: ");
    for (i = 0; i < N; i++) {
        // Leggo il numero successivo e lo metto nella cella i-esima: vet[i] (o *(vet+i))
        fscanf(fp, "%d", &vet[i]);
        printf("%d ", vet[i]);
    }
    printf("\n");

    // Ho finito di leggere, chiudo il file. Risorsa rilasciata.
    fclose(fp);

    /* --- 5. CHIAMATA ALGORITMO (Logica) --- */
    printf("\n--- Calcolo Elemento Maggioritario ---\n");
    
    int risultato = majority(vet, N);

    /* --- 6. VISUALIZZAZIONE RISULTATI --- */
    if (risultato != -1) {
        printf("Elemento maggioritario trovato: %d\n", risultato);
    } else {
        printf("Nessun elemento maggioritario esiste (-1)\n");
    }

    /* --- 7. RILASCIO MEMORIA (Memory Management) --- */
    // Ogni malloc deve avere una corrispondente free.
    // Se non lo facciamo, creiamo un Memory Leak (perdita di memoria).
    free(vet);
    
    // Buona norma: annullare il puntatore dopo la free per evitare "dangling pointers"
    vet = NULL; 

    return 0;
}