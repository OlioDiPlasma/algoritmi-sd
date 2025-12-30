#ifndef AUTOMOTIVE_H
#define AUTOMOTIVE_H

#include <stdio.h>

/* --- DEFINIZIONE STRUTTURE DATI --- */

/* * Usiamo tre strutture collegate gerarchicamente.
 * 1. Accessorio: Elemento di una LISTA (basta una scansione sequenziale).
 * 2. Modello: Nodo di un ALBERO BINARIO DI RICERCA (BST) per ricerca veloce O(log M).
 * 3. Produttore: Nodo di un ALBERO BINARIO DI RICERCA (BST) per ricerca veloce O(log P).
 */

typedef struct accessorio {
    char nome[101];
    float prezzo;
    struct accessorio *next; // Puntatore al prossimo accessorio
} Accessorio;

typedef struct modello {
    char nome[101];
    Accessorio *lista_accessori; // Testa della lista accessori
    struct modello *left, *right; // Figli dell'albero dei modelli
} Modello;

typedef struct produttore {
    char nome[101];
    Modello *albero_modelli;      // Radice dell'albero dei modelli
    struct produttore *left, *right; // Figli dell'albero produttori
} Produttore;

/* --- PROTOTIPI FUNZIONI --- */

/* Caricamento dati da file */
Produttore* carica_database(const char *nome_file_produttori);

/* Funzioni di visualizzazione (Menu) */
void stampa_modelli_produttore(Produttore *root, char *nome_produttore);
void stampa_accessori_modello(Produttore *root, char *nome_modello);

/* Funzioni di cancellazione */
Produttore* cancella_produttore(Produttore *root, char *nome_prod);
void cancella_modello_globale(Produttore *root, char *nome_modello); // Cerca ovunque ed elimina
void cancella_accessorio_globale(Produttore *root, char *nome_accessorio);

/* Funzione di fusione (Merge) */
Produttore* incorpora_produttori(Produttore *root, char *prod1, char *prod2);

/* Gestione memoria in uscita */
void libera_tutto(Produttore *root);

#endif