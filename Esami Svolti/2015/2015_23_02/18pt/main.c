#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "st.h"

#define MAX_PATH_LEN 100

/* --- FUNZIONE AUSILIARIA PER PUNTO 2 (INTERSEZIONE) --- */
/*
 * Scopo: Leggere cammini, trovare nodi comuni e decomporre.
 * Parametri: Nome file, Grafo (per tradurre nomi in indici e viceversa).
 */
void processPaths(char *filename, Graph G) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Errore apertura file cammini.\n"); return; }

    int len1, len2, i, j;
    /* Allocazione array per i cammini (indici interi) */
    int *path1, *path2;
    char buff[21];

    /* --- LETTURA CAMMINO 1 --- */
    fscanf(fp, "%d", &len1);
    /* In realtà il numero nel file è "lunghezza" intesa come archi o nodi?
       L'esempio mostra 4 e poi 5 nomi -> 4 archi. 
       Il path ha N nodi. Allocchiamo abbastanza. */
    path1 = malloc((len1 + 1) * sizeof(int)); // +1 per sicurezza
    
    /* Leggo i nomi e converto in indici */
    for(i = 0; i <= len1; i++) {
        fscanf(fp, "%s", buff);
        path1[i] = GRAPHgetIndex(G, buff);
    }

    /* --- LETTURA CAMMINO 2 --- */
    fscanf(fp, "%d", &len2);
    path2 = malloc((len2 + 1) * sizeof(int));
    for(i = 0; i <= len2; i++) {
        fscanf(fp, "%s", buff);
        path2[i] = GRAPHgetIndex(G, buff);
    }
    fclose(fp);

    /* --- TROVARE INTERSEZIONI --- */
    /* Usiamo un array di flag se V è piccolo, oppure doppio loop. 
       Qui doppio loop è ok. */
    printf("\n--- VERIFICA INTERSEZIONI ---\n");
    printf("I vertici in comune sono:\n");
    
    /* Per decomporre, serve sapere QUALI sono comuni per poi stampare i pezzi. */
    /* Segniamo i nodi comuni in un array booleano per semplicità di stampa successiva */
    int *isCommon1 = calloc(len1 + 1, sizeof(int)); /* Flag per path 1 */
    int *isCommon2 = calloc(len2 + 1, sizeof(int)); /* Flag per path 2 */

    for (i = 0; i <= len1; i++) {
        for (j = 0; j <= len2; j++) {
            if (path1[i] == path2[j]) {
                printf("%s\n", GRAPHgetName(G, path1[i]));
                isCommon1[i] = 1;
                isCommon2[j] = 1;
            }
        }
    }

    /* --- DECOMPOSIZIONE CAMMINO 1 --- */
    printf("\nIl cammino 1 si decompone in:\n");
    int subIdx = 1;
    int start = 0;
    for (i = 0; i <= len1; i++) {
        /* Se è un nodo comune (ed è diverso dall'inizio assoluto o abbiamo avanzato)
           oppure siamo alla fine */
        if (isCommon1[i] || i == len1) {
             /* Se trovo un comune all'inizio (i=0), non stampo nulla, avanzo start */
             /* Logica dell'esempio: A0 è comune.
                Sottocammino 1.1: A0, fF, 123 (123 è comune).
                Quindi da Comune a Comune. */
            
            if (i > start) {
                printf("Sottocammino 1.%d: ", subIdx++);
                for (j = start; j <= i; j++) {
                    printf("%s ", GRAPHgetName(G, path1[j]));
                }
                printf("\n");
                start = i; /* Il prossimo sottocammino inizia da qui */
            }
        }
    }

    /* --- DECOMPOSIZIONE CAMMINO 2 --- */
    printf("\nIl cammino 2 si decompone in:\n");
    subIdx = 1;
    start = 0;
    for (i = 0; i <= len2; i++) {
        if (isCommon2[i] || i == len2) {
            if (i > start) {
                printf("Sottocammino 2.%d: ", subIdx++);
                for (j = start; j <= i; j++) {
                    printf("%s ", GRAPHgetName(G, path2[j]));
                }
                printf("\n");
                start = i;
            }
        }
    }

    free(path1); free(path2);
    free(isCommon1); free(isCommon2);
}

/* --- MAIN --- */
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Uso: %s <file_grafo> <file_cammini>\n", argv[0]);
        return 1;
    }

    /* 1. CARICAMENTO GRAFO */
    FILE *f = fopen(argv[1], "r");
    if (!f) return 1;
    
    /* Allocazione ST dinamica (dimensione iniziale arbitraria grande o resizing) 
       Per esame: stima alta (es 100) va bene se non è chiesto resize esplicito */
    ST st = STinit(100); 
    
    Graph G = GRAPHload(f, st);
    fclose(f);
    printf("Grafo caricato! Vertici: %d\n", GRAPHgetV(G));

    /* 2. VERIFICA (Intersezione Cammini) */
    processPaths(argv[2], G);

    /* 3. OTTIMIZZAZIONE (Cammino Massimo con Vincoli) */
    printf("\n--- OTTIMIZZAZIONE ---\n");
    char srcName[21], dstName[21];
    int k, p;

    /* Simulazione input come da esempio PDF */
    strcpy(srcName, "A0");
    strcpy(dstName, "fF");
    
    /* Test 1 */
    k = 1; p = 1;
    printf("\nTest 1: Src=%s, Dst=%s, k=%d, p=%d\n", srcName, dstName, k, p);
    GRAPHpathBest(G, GRAPHgetIndex(G, srcName), GRAPHgetIndex(G, dstName), k, p);

    /* Test 2 */
    k = 6; p = 7;
    printf("\nTest 2: Src=%s, Dst=%s, k=%d, p=%d\n", srcName, dstName, k, p);
    GRAPHpathBest(G, GRAPHgetIndex(G, srcName), GRAPHgetIndex(G, dstName), k, p);

    /* Pulizia */
    GRAPHfree(G);
    STfree(st);

    return 0;
}