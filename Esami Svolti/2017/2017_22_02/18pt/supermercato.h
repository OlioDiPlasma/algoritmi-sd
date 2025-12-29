#ifndef SUPERMERCATO_H
#define SUPERMERCATO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_STR 31

/* --- STRUTTURE DATI --- */

/*
 * Concetto Chiave: Riferimenti incrociati.
 * Un'offerta punta ai prodotti (tramite indice).
 * Un prodotto deve sapere in quali offerte si trova (richiesta n. 83).
 */

// Struttura per un elemento dentro un'offerta (es. "2 pezzi di Latte")
typedef struct {
    int id_prod_index; // Indice nel vettore prodotti globale
    int quantity;
} OfferItem;

// Struttura Offerta
typedef struct {
    char id_code[MAX_STR];
    int price;
    int num_items;     // Quanti prodotti diversi ci sono nell'offerta
    OfferItem *items;  // Vettore dinamico dei prodotti inclusi
} Offer;

// Struttura Prodotto
typedef struct {
    char id_name[MAX_STR];
    int unit_price;
    // Lista inversa: indici delle offerte che contengono questo prodotto
    int *offer_refs;   
    int num_offer_refs;
} Product;

// Struttura Catalogo (Contenitore Globale)
typedef struct {
    Product *products;
    int n_prod;
    Offer *offers;
    int n_off;
} Catalog;

// Struttura Spesa
typedef struct {
    char id_name[MAX_STR];
    int quantity_needed;
    int prod_index; // Collegamento rapido al catalogo
} ShoppingItem;

/* --- PROTOTIPI DI FUNZIONE --- */

// Gestione Dati
Catalog* carica_catalogo(const char *filename);
int trova_indice_prodotto(Catalog *C, char *name);
void collega_prodotti_offerte(Catalog *C); // Punto cruciale richiesta 83
void libera_catalogo(Catalog *C);

// Funzioni Richieste dal Testo
void stampa_offerte_per_prodotto(Catalog *C, char *prod_name); // Richiesta 84
void prodotti_in_comune(Catalog *C, char *off1_name, char *off2_name); // Richiesta 85

// Algoritmo di Ottimizzazione
void calcola_spesa_ottima(Catalog *C, char *filename_spesa);

#endif