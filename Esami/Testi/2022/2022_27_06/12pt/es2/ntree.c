#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntree.h"

/* ============================================================================
 * STRUTTURE DATI
 * ============================================================================
 */

/* * Struttura del nodo come definita dal testo dell'esercizio.
 */
struct node {
    char *key;                 // Stringa contenente il dato
    struct node *children[N];  // Array statico di puntatori ai figli
};

/* * Struttura Wrapper (il contenitore).
 * L'ADT nTREE è un puntatore a questa struttura.
 * Serve per mantenere un riferimento alla radice dell'albero.
 */
struct n_tree_s {
    struct node *root;
};

/* ============================================================================
 * FUNZIONI DI SUPPORTO (Private)
 * ============================================================================
 */

/* --- CALCOLO GRADO DEL NODO --- */
/* Scopo: Conta quanti figli non-NULL ha un nodo specifico. */
int get_degree(struct node *n) {
    if (n == NULL) return 0;
    int count = 0;
    /* Ciclo sull'array dei figli. Se il puntatore non è NULL, c'è un figlio. */
    for (int i = 0; i < N; i++) {
        if (n->children[i] != NULL) {
            count++;
        }
    }
    return count;
}

/* --- FUNZIONE RICORSIVA (CORE LOGIC) --- */
/* * Scopo: Visita l'albero e conta i nodi che soddisfano la condizione.
 * Parametri:
 * - curr: puntatore al nodo corrente che stiamo analizzando.
 * - parent_degree: intero che rappresenta il grado del padre di 'curr'.
 */
int countIf_recursive(struct node *curr, int parent_degree) {
    /* 1. CASO BASE */
    /* Se il nodo è NULL, non c'è nulla da contare. Torniamo 0. */
    if (curr == NULL) return 0;

    /* 2. CALCOLO GRADO CORRENTE */
    int current_degree = get_degree(curr);
    
    /* 3. VERIFICA CONDIZIONE LOCALE */
    /* Variabile accumulatore per il nodo corrente.
     * Vale 1 se il grado corrente è strettamente maggiore del grado del padre.
     * Altrimenti vale 0.
     */
    int match = (current_degree > parent_degree) ? 1 : 0;

    /* 4. PASSO RICORSIVO (Discesa) */
    int total_from_children = 0;
    for (int i = 0; i < N; i++) {
        /*
         * Chiamata ricorsiva:
         * Passiamo 'curr->children[i]' come nuovo nodo da analizzare.
         * Passiamo 'current_degree' perché 'curr' è il padre dei suoi figli!
         */
        total_from_children += countIf_recursive(curr->children[i], current_degree);
    }

    /* 5. RITORNO SOMMA */
    /* Risultato = (1 se io soddisfo la condizione) + (quanti figli la soddisfano) */
    return match + total_from_children;
}

/* ============================================================================
 * IMPLEMENTAZIONE FUNZIONI DELL'INTERFACCIA
 * ============================================================================
 */

/* --- WRAPPER COUNTIF --- */
/* * Scopo: Funzione pubblica che innesca la ricorsione.
 * Concetti Chiave: Gestione del caso speciale della Radice.
 */
int countIf(nTREE t) {
    /* Controllo sicurezza: se l'albero non esiste o è vuoto */
    if (t == NULL || t->root == NULL) return 0;

    /*
     * STRATEGIA PER LA RADICE:
     * Il testo dice: "Il nodo radice, che non ha un padre, conta 1 per default".
     *
     * Opzione A (Didattica): Contiamo 1 manualmente, poi lanciamo la ricorsione sui figli.
     */
    
    /* Grado della radice */
    int root_degree = get_degree(t->root);
    
    /* Contatore totale: parte da 1 perché la radice conta sempre */
    int total = 1; 

    /* Lanciamo la ricorsione su tutti i figli della radice.
     * Per questi figli, il "parent_degree" è proprio "root_degree".
     */
    for (int i = 0; i < N; i++) {
        total += countIf_recursive(t->root->children[i], root_degree);
    }

    return total;
}

/* --- FUNZIONI DI UTILITÀ PER CREARE L'ALBERO (Non richieste esplicitamente ma necessarie) --- */

nTREE ntree_init() {
    /* Allocazione della struttura wrapper "struct n_tree_s" */
    nTREE t = (nTREE)malloc(sizeof(struct n_tree_s));
    if (t != NULL) t->root = NULL;
    return t;
}

/* Funzione helper interna per allocare un nodo */
struct node* create_node(char *key) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->key = strdup(key); // Duplica la stringa
    for(int i=0; i<N; i++) new_node->children[i] = NULL;
    return new_node;
}

void ntree_insert_root(nTREE t, char *key) {
    if (t->root == NULL) t->root = create_node(key);
}

/* Inserimento brutale (cerca il padre per chiave) - solo per test */
struct node* find_node(struct node* r, char *key) {
    if(r == NULL) return NULL;
    if(strcmp(r->key, key) == 0) return r;
    for(int i=0; i<N; i++) {
        struct node* res = find_node(r->children[i], key);
        if(res != NULL) return res;
    }
    return NULL;
}

void ntree_insert_child(nTREE t, char *parent_key, char *child_key) {
    struct node *parent = find_node(t->root, parent_key);
    if(parent != NULL) {
        for(int i=0; i<N; i++) {
            if(parent->children[i] == NULL) {
                parent->children[i] = create_node(child_key);
                break;
            }
        }
    }
}

/* --- FREE MEMORY --- */
void free_recursive(struct node *n) {
    if(n == NULL) return;
    for(int i=0; i<N; i++) free_recursive(n->children[i]);
    free(n->key); // Libera la stringa
    free(n);      // Libera il nodo
}

void ntree_free(nTREE t) {
    if(t != NULL) {
        free_recursive(t->root);
        free(t); // Libera il wrapper
    }
}