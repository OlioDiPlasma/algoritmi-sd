/*
 * File: grafo.h
 * Scopo: Definizione delle strutture dati e prototipi delle funzioni.
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE COSTANTI --- */
#define MAXC 21     /* Lunghezza massima stringa ID (20 + \0) */

/* Enumerazione per i colori per rendere il codice leggibile */
typedef enum {
    ROSSO,
    NERO,
    NON_DEFINITO
} Colore;

/* --- STRUTTURE DATI --- */

/* 1. Nodo della lista di adiacenza (l'arco) */
typedef struct node {
    int v_index;        /* Indice del vertice di destinazione nella Tabella Simboli */
    int peso;           /* Peso dell'arco */
    struct node *next;  /* Puntatore al prossimo arco */
} NodoArco;

/* 2. Vertice del grafo (nella Tabella Simboli) */
typedef struct {
    char id[MAXC];      /* Nome del vertice (es. "A", "Milano") */
    Colore col;         /* Colore del vertice */
    NodoArco *head;     /* Testa della lista di adiacenza per questo vertice */
} Vertice;

/* 3. Struttura Grafo (Wrapper principale) */
typedef struct {
    Vertice *vett;      /* Array dinamico di vertici */
    int nV;             /* Numero attuale di vertici */
    int nE;             /* Numero di archi (non strettamente necessario per l'algo ma utile) */
    int capacity;       /* Per la reallocazione dinamica della tabella simboli */
} Grafo;

/* --- PROTOTIPI FUNZIONI --- */

/* Carica il grafo da file e VERIFICA LA CONGRUENZA DEI COLORI */
Grafo* caricaGrafo(const char *filename);

/* Funzione principale per trovare il cammino massimo (OTTIMIZZAZIONE) */
void cercaCamminoMassimo(Grafo *g);

/* Libera tutta la memoria */
void liberaGrafo(Grafo *g);

#endif