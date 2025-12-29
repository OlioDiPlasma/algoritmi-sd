#ifndef ST_H
#define ST_H

#include <stdio.h>

/* Definizione del tipo opaco per la Symbol Table */
typedef struct symboltable *ST;

/* --- FUNZIONI --- */
ST  STinit(int maxN);
void STfree(ST st);
int STinsert(ST st, char *val); /* Inserisce se non c'è, ritorna indice */
int STsearch(ST st, char *val); /* Cerca e ritorna indice, o -1 */
char* STsearchByIndex(ST st, int index); /* Dato indice, ritorna stringa */
int STcount(ST st); /* Ritorna numero elementi attuali */

#endif