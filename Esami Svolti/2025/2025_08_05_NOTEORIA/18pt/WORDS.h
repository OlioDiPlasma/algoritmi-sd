#ifndef WORDS_H
#define WORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 16 /* Max 15 caratteri + terminatore (Pag. 4) */

/* Struttura per singola parola */
typedef struct word_s {
    char parola[MAXL];
    int lun;
    int valore;
} Word;

/* Struttura wrapper per il vettore di parole */
typedef struct words_s {
    Word *vettW;
    int numW;
} Words;

Words WORDSinit(int n);
Words WORDSfile(FILE *fp);
void WORDSfree(Words w);

#endif