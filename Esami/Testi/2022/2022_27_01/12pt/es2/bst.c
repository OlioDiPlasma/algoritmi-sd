#include <stdio.h>
#include <stdlib.h> // Solo per malloc/free e NULL
#include "bst.h"

/* --- 1. DEFINIZIONE STRUTTURE (QUASI-ADT e ADT) --- */

/*
 * QUASI-ADT: Il Nodo.
 * Definito solo qui, invisibile all'esterno.
 */
typedef struct node {
    int val;            // Valore intero
    struct node *l;     // Figlio sinistro
    struct node *r;     // Figlio destro
} *link; // link è un puntatore a struct node

/*
 * ADT DI PRIMA CLASSE: Il Wrapper.
 * Questa è la struttura puntata dal tipo BT definito nell'.h
 */
struct bt_wrapper {
    link root; 
};

/* --- 2. FUNZIONE RICORSIVA (IL CUORE LOGICO) --- */

/*
 * Scopo: Verifica ricorsiva con passaggio di vincoli (min e max).
 * * Parametri:
 * - link x: Il nodo corrente.
 * - int *min: Puntatore al valore minimo ammissibile (NULL = -infinito).
 * - int *max: Puntatore al valore massimo ammissibile (NULL = +infinito).
 * * Concetti Chiave (Puntatori come Limiti):
 * Perché usiamo int *min invece di int min?
 * Per gestire l'infinito. Se min è NULL, non c'è limite inferiore.
 * Se min punta a un intero, il valore del nodo deve essere > *min.
 * Questo evita l'uso di INT_MIN/INT_MAX che sono in limits.h (vietato).
 */
int is_bst_util(link x, int *min, int *max) {
    
    // Caso Base: Un albero vuoto è un BST valido.
    if (x == NULL) {
        return 1;
    }

    /* --- CONTROLLO VIOLAZIONE MINIMO --- */
    // Se esiste un limite inferiore (min != NULL) E il valore attuale
    // è minore o uguale a quel limite, abbiamo rotto la regola del BST.
    if (min != NULL && x->val <= *min) {
        return 0;
    }

    /* --- CONTROLLO VIOLAZIONE MASSIMO --- */
    // Se esiste un limite superiore (max != NULL) E il valore attuale
    // è maggiore o uguale a quel limite, abbiamo rotto la regola.
    if (max != NULL && x->val >= *max) {
        return 0;
    }

    /* --- PASSO RICORSIVO --- */
    // 1. Controllo a sinistra: Il nuovo massimo diventa il valore corrente.
    //    Il minimo rimane quello ereditato dal padre.
    // 2. Controllo a destra: Il nuovo minimo diventa il valore corrente.
    //    Il massimo rimane quello ereditato dal padre.
    return is_bst_util(x->l, min, &x->val) && 
           is_bst_util(x->r, &x->val, max);
}

/* --- 3. FUNZIONE WRAPPER RICHIESTA --- */

/*
 * Scopo: Interfaccia pubblica che nasconde la complessità della ricorsione.
 */
int f(BT t) {
    // Controllo sicurezza puntatore
    if (t == NULL) return 1; 

    // Chiamiamo la funzione ricorsiva passando NULL come limiti iniziali.
    // Significato: la radice può avere qualsiasi valore (-inf, +inf).
    return is_bst_util(t->root, NULL, NULL);
}

/* ==========================================================
 * FUNZIONI DI SUPPORTO (Solo per far funzionare il test)
 * ========================================================== */

// Helper per creare nodi
link new_node(int v, link l, link r) {
    link x = malloc(sizeof(struct node));
    x->val = v;
    x->l = l;
    x->r = r;
    return x;
}

// Crea un BST valido:
//      10
//     /  \
//    5    15
BT crea_albero_valido() {
    BT t = malloc(sizeof(struct bt_wrapper));
    t->root = new_node(10, new_node(5, NULL, NULL), new_node(15, NULL, NULL));
    return t;
}

// Crea un albero NON valido:
//      10
//     /  \
//    5    2 (Errore! 2 è a destra di 10, dovrebbe essere > 10)
BT crea_albero_invalido() {
    BT t = malloc(sizeof(struct bt_wrapper));
    t->root = new_node(10, new_node(5, NULL, NULL), new_node(2, NULL, NULL));
    return t;
}

void free_r(link x) {
    if (x == NULL) return;
    free_r(x->l);
    free_r(x->r);
    free(x);
}

void libera_albero(BT t) {
    if (t != NULL) {
        free_r(t->root);
        free(t);
    }
}