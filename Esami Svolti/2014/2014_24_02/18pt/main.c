#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    Graph G;
    char vertice[21]; /* Buffer input utente */

    /* Controllo argomenti riga di comando */
    if (argc != 2) {
        printf("Uso: %s <nome_file_grafo>\n", argv[0]);
        return 1;
    }

    /* Apertura File */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", argv[1]);
        return 1;
    }

    /* --- 1. CARICAMENTO --- */
    printf("--- CARICAMENTO GRAFO ---\n");
    G = GRAPHload(fp);
    fclose(fp);
    printf("Grafo caricato correttamente.\n");

    /* --- 2. VERIFICA REGOLARITA' --- */
    printf("\n--- VERIFICA REGOLARITA' ---\n");
    GRAPHcheckRegular(G);

    /* --- 3. CICLO INTERATTIVO --- */
    printf("\n--- RICERCA CAMMINI ---\n");
    while (1) {
        printf("Inserisci ID vertice sorgente (o 'fine' per terminare): ");
        scanf("%s", vertice);

        if (strcmp(vertice, "fine") == 0) {
            break;
        }

        /* Chiamata alla funzione del punto C */
        GRAPHpathMaxWeight(G, vertice);
        printf("\n");
    }

    /* --- 4. PULIZIA FINALE --- */
    GRAPHfree(G);
    printf("Programma terminato.\n");

    return 0;
}