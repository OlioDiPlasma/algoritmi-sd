#include "gestione_articoli.h"

/* --- STRUTTURA NASCOSTA --- */
struct tabella_articoli {
    Articolo *vett; // Vettore dinamico di Articoli
    int n_articoli; // Dimensione vettore
};

Articoli carica_articoli(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    /* 1. ALLOCAZIONE STRUTTURA PRINCIPALE */
    Articoli tab = (Articoli)malloc(sizeof(struct tabella_articoli));
    
    /* Legge il numero di articoli dalla prima riga [cite: 43] */
    fscanf(fp, "%d", &tab->n_articoli);

    /* 2. ALLOCAZIONE VETTORE DINAMICO */
    tab->vett = (Articolo *)malloc(tab->n_articoli * sizeof(Articolo));

    /* 3. POPOLAMENTO */
    for (int i = 0; i < tab->n_articoli; i++) {
        tab->vett[i] = leggi_articolo(fp, i);
    }

    fclose(fp);
    return tab;
}

int get_num_articoli(Articoli a) {
    return a->n_articoli;
}

Articolo get_articolo_by_index(Articoli a, int index) {
    return a->vett[index];
}

void free_articoli(Articoli a) {
    for (int i = 0; i < a->n_articoli; i++) {
        free_articolo(a->vett[i]); // Libera le stringhe interne
    }
    free(a->vett); // Libera il vettore
    free(a);       // Libera la struttura wrapper
}