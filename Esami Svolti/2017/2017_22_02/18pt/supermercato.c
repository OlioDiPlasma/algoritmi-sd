#include "supermercato.h"

/* * Scopo: Trovare l'indice numerico di un prodotto dato il suo nome stringa.
 * Parametri:
 * - C: puntatore al catalogo.
 * - name: stringa da cercare.
 * Ritorna: Indice nell'array (0..N-1) o -1 se non trovato.
 */
int trova_indice_prodotto(Catalog *C, char *name) {
    for (int i = 0; i < C->n_prod; i++) {
        if (strcmp(C->products[i].id_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* * Scopo: Trovare l'indice di un'offerta dato il nome.
 */
int trova_indice_offerta(Catalog *C, char *name) {
    for (int i = 0; i < C->n_off; i++) {
        if (strcmp(C->offers[i].id_code, name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * Scopo: Caricare il file catalogo.txt nelle strutture dati dinamiche.
 * Concetti Chiave:
 * - Allocazione dinamica doppia (vettore di struct, e vettori dentro le struct).
 * - Gestione della lettura file con fscanf.
 */
Catalog* carica_catalogo(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        exit(1);
    }

    Catalog *C = (Catalog*)malloc(sizeof(Catalog));
    
    /* --- 1. LETTURA DIMENSIONI --- */
    fscanf(fp, "%d %d", &C->n_prod, &C->n_off);

    /* --- 2. ALLOCAZIONE VETTORE PRODOTTI --- */
    // Malloc: Allocazione blocco contiguo per N prodotti.
    C->products = (Product*)malloc(C->n_prod * sizeof(Product));
    
    for (int i = 0; i < C->n_prod; i++) {
        fscanf(fp, "%s %d", C->products[i].id_name, &C->products[i].unit_price);
        // Inizializziamo i riferimenti alle offerte a NULL/0 per ora
        C->products[i].offer_refs = NULL;
        C->products[i].num_offer_refs = 0;
    }

    /* --- 3. ALLOCAZIONE VETTORE OFFERTE --- */
    C->offers = (Offer*)malloc(C->n_off * sizeof(Offer));

    for (int i = 0; i < C->n_off; i++) {
        fscanf(fp, "%s %d %d", C->offers[i].id_code, &C->offers[i].price, &C->offers[i].num_items);
        
        // Allocazione dinamica per gli elementi DENTRO questa specifica offerta
        C->offers[i].items = (OfferItem*)malloc(C->offers[i].num_items * sizeof(OfferItem));

        for (int j = 0; j < C->offers[i].num_items; j++) {
            char temp_name[MAX_STR];
            int qty;
            fscanf(fp, "%s %d", temp_name, &qty);
            
            // Convertiamo subito il nome in indice per efficienza
            int idx = trova_indice_prodotto(C, temp_name);
            C->offers[i].items[j].id_prod_index = idx;
            C->offers[i].items[j].quantity = qty;
        }
    }

    fclose(fp);
    
    /* --- 4. COSTRUZIONE LISTE INVERSE (Richiesta 81/83) --- */
    // Ora che abbiamo tutte le offerte, aggiorniamo i prodotti dicendo "Hey, tu sei nell'offerta X"
    collega_prodotti_offerte(C);

    return C;
}

/*
 * Scopo: Popolare il campo offer_refs di ogni prodotto (Richiesta 83).
 * Logica: Scorriamo tutte le offerte e, per ogni prodotto contenuto, 
 * aggiungiamo l'indice dell'offerta alla lista del prodotto.
 */
void collega_prodotti_offerte(Catalog *C) {
    // Passo 1: Contare quante volte appare ogni prodotto nelle offerte (per allocare giusto)
    // Usiamo un vettore di conteggio temporaneo allocato con calloc (inizializza a 0)
    int *counts = (int*)calloc(C->n_prod, sizeof(int));
    
    for (int i = 0; i < C->n_off; i++) { // Per ogni offerta
        for (int j = 0; j < C->offers[i].num_items; j++) { // Per ogni oggetto nell'offerta
            int prod_idx = C->offers[i].items[j].id_prod_index;
            if(prod_idx != -1) counts[prod_idx]++;
        }
    }

    // Passo 2: Allocare la memoria per ogni prodotto
    for (int i = 0; i < C->n_prod; i++) {
        if (counts[i] > 0) {
            C->products[i].offer_refs = (int*)malloc(counts[i] * sizeof(int));
        } else {
            C->products[i].offer_refs = NULL;
        }
        // Resettiamo il contatore interno per usarlo come indice di inserimento nel prossimo ciclo
        C->products[i].num_offer_refs = 0; 
    }

    // Passo 3: Riempire i riferimenti
    for (int i = 0; i < C->n_off; i++) {
        for (int j = 0; j < C->offers[i].num_items; j++) {
            int prod_idx = C->offers[i].items[j].id_prod_index;
            if (prod_idx != -1) {
                // Sintassi C: Accediamo al prodotto -> al suo array -> alla posizione corrente
                int current_pos = C->products[prod_idx].num_offer_refs;
                C->products[prod_idx].offer_refs[current_pos] = i; // Salviamo l'indice dell'offerta
                C->products[prod_idx].num_offer_refs++;
            }
        }
    }
    
    free(counts);
}

/*
 * Scopo: Implementazione Richiesta 84
 * Dato un prodotto, stampa le offerte.
 */
void stampa_offerte_per_prodotto(Catalog *C, char *prod_name) {
    int idx = trova_indice_prodotto(C, prod_name);
    printf("--- Offerte per %s ---\n", prod_name);
    if (idx == -1) {
        printf("Prodotto non trovato.\n");
        return;
    }

    Product *p = &C->products[idx];
    if (p->num_offer_refs == 0) {
        printf("Nessuna offerta include questo prodotto.\n");
    } else {
        for (int i = 0; i < p->num_offer_refs; i++) {
            int off_idx = p->offer_refs[i];
            printf("- Offerta: %s (Prezzo: %d)\n", C->offers[off_idx].id_code, C->offers[off_idx].price);
        }
    }
    printf("\n");
}

/*
 * Scopo: Implementazione Richiesta 85
 * Date due offerte, elenca prodotti comuni.
 */
void prodotti_in_comune(Catalog *C, char *off1_name, char *off2_name) {
    int idx1 = trova_indice_offerta(C, off1_name);
    int idx2 = trova_indice_offerta(C, off2_name);
    
    printf("--- Prodotti comuni tra %s e %s ---\n", off1_name, off2_name);
    if (idx1 == -1 || idx2 == -1) {
        printf("Una delle offerte non esiste.\n");
        return;
    }

    Offer *o1 = &C->offers[idx1];
    Offer *o2 = &C->offers[idx2];
    int found = 0;

    // Doppio ciclo annidato (Join). Poiché le liste sono piccolissime (pochi prodotti per offerta), è efficiente.
    for (int i = 0; i < o1->num_items; i++) {
        for (int j = 0; j < o2->num_items; j++) {
            if (o1->items[i].id_prod_index == o2->items[j].id_prod_index) {
                // Recuperiamo il nome dal catalogo globale usando l'indice
                printf("- %s\n", C->products[o1->items[i].id_prod_index].id_name);
                found = 1;
            }
        }
    }

    if (!found) printf("Nessun prodotto in comune.\n");
    printf("\n");
}

/*
 * Scopo: Liberare tutta la memoria allocata.
 * Importante: Free deve essere fatto in ordine inverso rispetto alla malloc (deep to shallow).
 */
void libera_catalogo(Catalog *C) {
    if (C == NULL) return;

    // 1. Libera dentro le offerte
    for (int i = 0; i < C->n_off; i++) {
        free(C->offers[i].items);
    }
    free(C->offers);

    // 2. Libera dentro i prodotti
    for (int i = 0; i < C->n_prod; i++) {
        if(C->products[i].offer_refs != NULL)
            free(C->products[i].offer_refs);
    }
    free(C->products);

    // 3. Libera il contenitore
    free(C);
}