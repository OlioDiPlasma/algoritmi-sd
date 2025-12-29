#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONI COSTANTI --- */
#define MAX_STR 21 /* 20 caratteri + terminatore \0 */

/* --- STRUTTURE DATI --- */

/* Nodo della lista di adiacenza (per ogni vertice, lista dei vicini) */
typedef struct node {
    int v;              /* Indice del vertice adiacente nella Tabella Simboli */
    struct node *next;  /* Puntatore al prossimo vicino */
} Node;

/* Tabella di Simboli: Mappa stringa -> intero */
typedef struct {
    char **nomi;    /* Array dinamico di stringhe (i nomi dei vertici) */
    int size;       /* Numero attuale di vertici */
    int max_size;   /* Dimensione allocata (opzionale, qui useremo realloc precisa) */
} SymbolTable;

/* Struttura principale del Grafo */
typedef struct {
    Node **ladj;    /* Array di puntatori a liste (Liste di Adiacenza) */
    int V;          /* Numero di vertici */
    int E;          /* Numero di archi */
    SymbolTable st; /* Tabella di simboli interna al grafo */
} Graph;

/* --- PROTOTIPI DI FUNZIONE --- */

/* Gestione Grafo e Caricamento */
Graph* initGraph();
int getIndex(SymbolTable *st, char *str); /* Cerca o aggiunge un nome */
void loadGraph(Graph *G, FILE *fp);
void printGraph(Graph *G);
void freeGraph(Graph *G);

/* --- PARTE 1: VERIFICA INSIEME INDIPENDENTE --- */
/* Verifica se la lista di vertici nel file 'fp' forma un Insieme Indipendente */
int verificaIndipendente(Graph *G, FILE *fp);

/* --- PARTE 2: OTTIMIZZAZIONE (Insieme Massimale) --- */
/* Trova un Insieme Indipendente Massimale, lo scrive su file e ritorna la cardinalita' */
int trovaInsiemeMassimale(Graph *G, char *outputFilename);

#endif