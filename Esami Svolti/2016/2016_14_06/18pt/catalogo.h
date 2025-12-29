/* catalogo.h */
#ifndef CATALOGO_H
#define CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE COSTANTI --- */
#define MAX_STR 256        // Dimensione per il nome prodotto
#define COD_LEN 20         // 20 caratteri + terminatore '\0'
#define MAX_CAT 100        // Numero massimo di categorie

/* --- 1. STRUTTURA PRODOTTO --- */
/* Contiene i dati effettivi del singolo oggetto */
typedef struct {
    char id[COD_LEN];
    char nome[MAX_STR];
    float prezzo;
    int disponibilita;
    char categoria[COD_LEN]; // Utile tenerlo anche qui per riferimenti
} Prodotto;

/* --- 2. STRUTTURA NODO ALBERO (BST) --- */
/* Necessaria per la complessita' logaritmica.
   Ogni nodo contiene un prodotto e puntatori ai figli. */
typedef struct NodoProd {
    Prodotto data;
    struct NodoProd *left;
    struct NodoProd *right;
} NodoProd;

/* --- 3. STRUTTURA CATEGORIA --- */
/* Ogni categoria ha un codice e la radice del suo albero di prodotti */
typedef struct {
    char id_cat[COD_LEN];
    NodoProd *root;       // Puntatore alla radice dell'albero dei prodotti di questa categoria
} Categoria;

/* --- 4. STRUTTURA NEGOZIO (WRAPPER) --- */
/* Contiene l'array statico di categorie e il contatore di quante ce ne sono */
typedef struct {
    Categoria elenco[MAX_CAT];
    int num_categorie;
} Negozio;

/* --- PROTOTIPI DELLE FUNZIONI --- */

/* Inizializzazione */
void init_negozio(Negozio *n);

/* Punto 1: Inserimento */
NodoProd* inserisci_in_bst(NodoProd *root, Prodotto p);
void aggiungi_prodotto(Negozio *n, char *id, char *nome, float prezzo, int disp, char *cat);
void carica_da_file(Negozio *n, char *filename);

/* Punto 2 e 3: Ricerca */
Prodotto* cerca_in_bst(NodoProd *root, char *id);
void ricerca_prodotto_globale(Negozio *n, char *id); // Punto 2
void ricerca_prodotto_mirata(Negozio *n, char *cat, char *id); // Punto 3

/* Punto 4: Stampa ordinata */
void stampa_categoria(Negozio *n, char *cat, int ordina_per_nome);

/* Punto 5: Ricerca parziale */
void cerca_per_nome_parziale(Negozio *n, char *pattern);

/* Punto 6: Gestione Ordine */
void valuta_ordine(Negozio *n, char *filename);

/* Gestione Memoria */
void libera_albero(NodoProd *root);
void libera_tutto(Negozio *n);

#endif