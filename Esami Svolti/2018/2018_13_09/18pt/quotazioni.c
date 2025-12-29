/* file: quotazioni.c */
#include <stdlib.h>
#include <stdio.h>
#include <float.h> // Per FLT_MAX
#include "quotazioni.h"

/* --- 1. STRUTTURE DATI --- */

/* Contenuto del nodo: La quotazione di UN giorno */
typedef struct {
    Data data;
    double numeratore;   // Somma (valore * quantita)
    int denominatore;    // Somma (quantita)
    float media;         // Quota effettiva (num/den)
} QuotazioneGiornaliera;

/* Nodo dell'albero BST */
struct bst_quotazioni {
    QuotazioneGiornaliera contenuto;
    struct bst_quotazioni *left;
    struct bst_quotazioni *right;
};

/* --- 2. FUNZIONI DI SUPPORTO (Private/Statiche) --- */

/* Crea un nuovo nodo */
static struct bst_quotazioni *crea_nodo(Data data, float valore, int quantita) {
    struct bst_quotazioni *nodo = malloc(sizeof(struct bst_quotazioni));
    if (nodo == NULL) {
        printf("Errore malloc nodo quotazione\n");
        exit(1);
    }
    nodo->contenuto.data = data;
    nodo->contenuto.numeratore = valore * quantita;
    nodo->contenuto.denominatore = quantita;
    nodo->contenuto.media = (float)(nodo->contenuto.numeratore / nodo->contenuto.denominatore);
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
}

/* --- 3. IMPLEMENTAZIONE INTERFACCIA --- */

QUOTAZIONI quotazioni_init() {
    return NULL; // Un albero vuoto è un puntatore a NULL
}

/*
 * Concetti Chiave: Ricorsione su BST
 * Complessità: O(log M) dove M è il numero di giorni quotati
 */
void quotazioni_aggiorna(QUOTAZIONI *root_ptr, Data data, float valore, int quantita) {
    /* Caso Base: Albero vuoto o foglia raggiunta -> Inserimento */
    if (*root_ptr == NULL) {
        *root_ptr = crea_nodo(data, valore, quantita);
        return;
    }

    struct bst_quotazioni *nodo = *root_ptr;
    int cmp = data_confronta(data, nodo->contenuto.data);

    if (cmp == 0) {
        /* La data esiste già: AGGIORNIAMO LA MEDIA PESATA */
        nodo->contenuto.numeratore += (valore * quantita);
        nodo->contenuto.denominatore += quantita;
        /* Ricalcolo media */
        nodo->contenuto.media = (float)(nodo->contenuto.numeratore / nodo->contenuto.denominatore);
    } else if (cmp < 0) {
        /* Vai a sinistra */
        quotazioni_aggiorna(&(nodo->left), data, valore, quantita);
    } else {
        /* Vai a destra */
        quotazioni_aggiorna(&(nodo->right), data, valore, quantita);
    }
}

float quotazioni_cerca_data(QUOTAZIONI q, Data data) {
    if (q == NULL) return -1.0; // Non trovato

    int cmp = data_confronta(data, q->contenuto.data);
    if (cmp == 0) return q->contenuto.media;
    if (cmp < 0) return quotazioni_cerca_data(q->left, data);
    return quotazioni_cerca_data(q->right, data);
}

/* Funzione ricorsiva per visita intervallo (Punti 4 e 5) */
static void r_minmax(QUOTAZIONI q, Data d1, Data d2, float *min, float *max, int check_range) {
    if (q == NULL) return;

    /* Logica di pruning (taglio rami inutili):
       Se stiamo cercando in un range, e il nodo corrente è fuori range in modo "definitivo", non scendiamo. */
    
    Data cur = q->contenuto.data;
    
    /* Verifica se il nodo corrente è nel range (o se non c'è range) */
    int in_range = 1;
    if (check_range) {
        if (data_confronta(cur, d1) < 0 || data_confronta(cur, d2) > 0)
            in_range = 0;
    }

    if (in_range) {
        if (q->contenuto.media < *min) *min = q->contenuto.media;
        if (q->contenuto.media > *max) *max = q->contenuto.media;
    }

    /* Visita Sinistra: solo se potrebbe contenere date utili (>= d1) */
    if (!check_range || data_confronta(cur, d1) >= 0)
        r_minmax(q->left, d1, d2, min, max, check_range);

    /* Visita Destra: solo se potrebbe contenere date utili (<= d2) */
    if (!check_range || data_confronta(cur, d2) <= 0)
        r_minmax(q->right, d1, d2, min, max, check_range);
}

void quotazioni_minmax_intervallo(QUOTAZIONI q, Data d1, Data d2, float *min, float *max) {
    *min = FLT_MAX;
    *max = -1.0;
    
    /* Trucco: Se data_confronta(d1, d2) > 0 assumiamo "tutto il periodo" (punto 5) */
    int check_range = 1;
    if (data_confronta(d1, d2) > 0) check_range = 0;

    r_minmax(q, d1, d2, min, max, check_range);
}

void quotazioni_free(QUOTAZIONI q) {
    if (q == NULL) return;
    quotazioni_free(q->left);
    quotazioni_free(q->right);
    free(q);
}