/* file: titoli.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "titoli.h"

#define MAX_STR 21 // 20 chars + \0

/* --- 1. STRUTTURE --- */

struct bst_titoli {
    char codice[MAX_STR];
    QUOTAZIONI quotazioni; // Radice dell'albero di livello 2
    struct bst_titoli *left;
    struct bst_titoli *right;
};

/* --- 2. GESTIONE BST --- */

/* Cerca un nodo Titolo e lo ritorna. Se non esiste e create_flag=1, lo crea */
static struct bst_titoli *cerca_o_crea(struct bst_titoli **root, char *codice) {
    if (*root == NULL) {
        /* Creazione nuovo nodo Titolo */
        struct bst_titoli *nuovo = malloc(sizeof(struct bst_titoli));
        strcpy(nuovo->codice, codice);
        nuovo->quotazioni = quotazioni_init(); // Init albero livello 2 vuoto
        nuovo->left = NULL;
        nuovo->right = NULL;
        *root = nuovo;
        return nuovo;
    }

    int cmp = strcmp(codice, (*root)->codice);
    if (cmp == 0) return *root;
    if (cmp < 0) return cerca_o_crea(&((*root)->left), codice);
    return cerca_o_crea(&((*root)->right), codice);
}

/* Ricerca pura (senza creazione) */
static struct bst_titoli *cerca_nodo(struct bst_titoli *root, char *codice) {
    if (root == NULL) return NULL;
    int cmp = strcmp(codice, root->codice);
    if (cmp == 0) return root;
    if (cmp < 0) return cerca_nodo(root->left, codice);
    return cerca_nodo(root->right, codice);
}

/* --- 3. IMPLEMENTAZIONE INTERFACCIA --- */

TITOLI titoli_init() {
    return NULL;
}

void titoli_leggi_file(TITOLI *t, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        return;
    }

    int num_titoli_file;
    fscanf(fp, "%d", &num_titoli_file);

    /* Ciclo per ogni blocco titolo nel file */
    for (int i = 0; i < num_titoli_file; i++) {
        char codice[MAX_STR];
        int num_transazioni;
        fscanf(fp, "%s %d", codice, &num_transazioni);

        /* * Otteniamo il puntatore al nodo Titolo.
         * Se non esisteva, viene creato e inserito nel BST titoli.
         */
        struct bst_titoli *nodo_titolo = cerca_o_crea(t, codice);

        /* Leggiamo le transazioni */
        for (int j = 0; j < num_transazioni; j++) {
            Data d;
            int hh, mm, qty;
            float valore;
            
            d = data_leggi(fp);
            fscanf(fp, "%d:%d %f %d", &hh, &mm, &valore, &qty);

            /* Inseriamo nel BST di livello 2 (quotazioni) di QUESTO titolo */
            quotazioni_aggiorna(&(nodo_titolo->quotazioni), d, valore, qty);
        }
    }
    fclose(fp);
    printf("File %s acquisito con successo.\n", filename);
}

QUOTAZIONI titoli_cerca(TITOLI t, char *codice) {
    struct bst_titoli *nodo = cerca_nodo(t, codice);
    if (nodo == NULL) return NULL;
    return nodo->quotazioni;
}

void titoli_free(TITOLI t) {
    if (t == NULL) return;
    titoli_free(t->left);
    titoli_free(t->right);
    /* Prima di liberare il nodo titolo, libero il suo sotto-albero di quotazioni */
    quotazioni_free(t->quotazioni);
    free(t);
}