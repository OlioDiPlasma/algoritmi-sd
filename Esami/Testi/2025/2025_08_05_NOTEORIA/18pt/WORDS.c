#include "WORDS.h"

/* * Scopo: Allocare il vettore di strutture Word.
 */
Words WORDSinit(int n) {
    Words w;
    w.numW = n;
    w.vettW = (Word *) malloc(n * sizeof(Word));
    if (w.vettW == NULL) {
        printf("Errore allocazione parole.\n");
        exit(1);
    }
    return w;
}

/* * Scopo: Leggere file a lunghezza ignota.
 * Concetti Chiave: Tecnica "Due Passate" (count -> rewind -> read).
 */
Words WORDSfile(FILE *fp) {
    Words w;
    char buf[MAXL];
    int val, n = 0, i = 0;

    /* --- 1. PRIMA PASSATA (CONTEGGIO) --- */
    while(fscanf(fp, "%s %d", buf, &val) == 2) {
        n++;
    }

    /* --- 2. RESET PUNTATORE FILE --- */
    rewind(fp);

    /* --- 3. ALLOCAZIONE E LETTURA --- */
    w = WORDSinit(n);
    while(fscanf(fp, "%s %d", w.vettW[i].parola, &w.vettW[i].valore) == 2) {
        w.vettW[i].lun = strlen(w.vettW[i].parola);
        i++;
    }

    return w;
}

void WORDSfree(Words w) {
    if (w.vettW != NULL) {
        free(w.vettW);
    }
}