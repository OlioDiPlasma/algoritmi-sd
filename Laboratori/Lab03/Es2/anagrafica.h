/* * File: anagrafica.h
 * Scopo: Definizione dei tipi di dato e prototipi delle funzioni.
 * Concetti Chiave: 
 * - struct: per raggruppare dati eterogenei.
 * - typedef: per creare alias di tipi e scrivere codice piu' pulito.
 */

#ifndef ANAGRAFICA_H
#define ANAGRAFICA_H

#include <stdio.h>

/* --- 1. DEFINIZIONE STRUTTURE DATI --- */

// Struttura per la data (giorno, mese, anno)
typedef struct {
    int g, m, a;
} Data;

// Struttura Item: rappresenta una singola persona
typedef struct {
    char codice[6];      // AXXXX + terminatore
    char nome[51];
    char cognome[51];
    Data data_nascita;
    char via[51];
    char citta[51];
    int cap;
} Item;

// Nodo della lista concatenata
typedef struct nodo {
    Item info;              // Il dato utile
    struct nodo *next;      // Puntatore al prossimo nodo
} Nodo;

// Alias per il puntatore a nodo (facilita la lettura)
typedef Nodo* Lista;

/* --- 2. PROTOTIPI FUNZIONI --- */

// Gestione Item e Input/Output
Item leggiItem(FILE *fp); // Legge un item da file o tastiera
void stampaItem(Item item, FILE *fp); // Stampa un item
int confrontaDate(Data d1, Data d2); // Utile per l'ordinamento
Item creaItemVuoto(); // Per ritornare errori

// Gestione Lista
void inserisciOrdinato(Lista *head, Item val); // Inserimento per anzianita'
Lista ricercaCodice(Lista head, char *codice); // Ricerca semplice
Item estraiPerCodice(Lista *head, char *codice); // Cancellazione puntuale
Item estraiIntervalloDate(Lista *head, Data d1, Data d2); // Estrazione range
void stampaListaSuFile(Lista head, char *nomeFile);
void liberaLista(Lista head); // Pulizia memoria

#endif