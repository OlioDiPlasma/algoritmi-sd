/* file: data.h */
#ifndef DATA_H
#define DATA_H

#include <stdio.h>

/* --- DEFINIZIONE STRUTTURA --- */
typedef struct {
    int anno;
    int mese;
    int giorno;
} Data;

/* --- FUNZIONI STATIC --- 
 * L'aggiunta di 'static' risolve l'errore di Multiple Definition.
 * Rende queste funzioni "private" per ogni file .c che include questo header.
 */

/* Scopo: Confronta due date */
static int data_confronta(Data d1, Data d2) {
    if (d1.anno != d2.anno) return (d1.anno - d2.anno);
    if (d1.mese != d2.mese) return (d1.mese - d2.mese);
    return (d1.giorno - d2.giorno);
}

/* Scopo: Legge una data da file aperto */
static Data data_leggi(FILE *fp) {
    Data d;
    fscanf(fp, "%d/%d/%d", &d.anno, &d.mese, &d.giorno);
    return d;
}

/* Scopo: Stampa una data su stdout */
static void data_stampa(Data d) {
    printf("%04d/%02d/%02d", d.anno, d.mese, d.giorno);
}

#endif