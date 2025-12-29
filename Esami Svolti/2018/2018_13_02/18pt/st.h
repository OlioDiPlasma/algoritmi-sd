#ifndef ST_H
#define ST_H

#include <stdio.h>

/* --- DEFINIZIONE TIPO OPACO --- */
/*
    Scopo: Nascondiamo i dettagli della struttura all'utente.
    Usiamo un puntatore a ST (Symbol Table).
*/
typedef struct symboltable *ST;

/* --- PROTOTIPI --- */

/*
    Scopo: Inizializza una tabella di simboli vuota.
    Parametri:
      - maxN: numero massimo di elementi previsti.
*/
ST STinit(int maxN);

/*
    Scopo: Dealloca completamente la memoria usata dalla ST.
*/
void STfree(ST st);

/*
    Scopo: Inserisce un nome nella tabella se non esiste, restituisce il suo indice.
    Parametri:
      - st: la tabella.
      - label: stringa da inserire.
*/
int STinsert(ST st, char *label);

/*
    Scopo: Cerca un nome e ritorna l'indice. Restituisce -1 se non trovato.
*/
int STsearch(ST st, char *label);

/*
    Scopo: Dato un indice, restituisce il nome (stringa) associato.
    Utile per stampare i risultati finali.
*/
char *STsearchByIndex(ST st, int index);

#endif