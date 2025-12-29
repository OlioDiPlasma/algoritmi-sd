#ifndef RISTORANTE_H
#define RISTORANTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE STRUTTURE DATI --- */

// Rappresenta il singolo piatto letto da file
typedef struct {
    char nome[101];
    char portata[101];
    char tipologia[101];
    float costo;
} Piatto;

// Quasi-ADT per il Menu.
// Contiene gli indici che puntano al vettore principale dei piatti.
// Usiamo gli indici (int) invece di copiare le stringhe per risparmiare RAM.
typedef struct {
    int *indici;    // Vettore dinamico di interi (indici dei piatti scelti)
    int n_piatti;   // Quanti piatti (P)
    float costo_tot;// Prezzo pre-calcolato
} Menu;

// Nodo dell'Albero Binario di Ricerca (BST)
typedef struct bst_node {
    Menu val;                   // Il dato è un Menu completo
    struct bst_node *left;      // Sottoalbero sinistro (minori)
    struct bst_node *right;     // Sottoalbero destro (maggiori)
} BSTNode;

/* --- PROTOTIPI FUNZIONI --- */

// Legge il file e carica il vettore di piatti
Piatto* leggiPiatti(char *filename, int *N);

// Funzione wrapper che prepara la ricorsione
BSTNode* generaMenu(Piatto *elenco, int N, int P);

// Funzione di confronto richiesta dal testo
int MENUcompare(Menu m1, Menu m2, Piatto *elenco);

// Stampa in ordine (visita simmetrica)
void stampaBST(BSTNode *root, Piatto *elenco);

// Libera la memoria dell'albero
void freeBST(BSTNode *root);

#endif