#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* --- 1. DEFINIZIONE STRUTTURE INTERNE --- */

/* * NODO (Quasi ADT)
 * La traccia chiede di definire esplicitamente i nodi.
 */
typedef struct node {
    int val;            // Il dato (intero)
    struct node *next;  // Puntatore al prossimo nodo
} node_t;

/* * WRAPPER (ADT di I Classe)
 * Contiene il puntatore alla testa. Questo ci permette di passare 'L' 
 * per valore ma modificare comunque il contenuto della lista.
 */
struct list_dummy {
    node_t *head;
};

/* --- 2. FUNZIONI DI SUPPORTO (Creazione Nodi) --- */

static node_t *newNode(int val) {
    node_t *n = (node_t *)malloc(sizeof(node_t));
    if (n == NULL) return NULL;
    n->val = val;
    n->next = NULL;
    return n;
}

list LISTinit() {
    // Allocazione del wrapper
    list L = (list)malloc(sizeof(struct list_dummy));
    L->head = NULL;
    return L;
}

void LISTadd(list L, int val) {
    node_t *n = newNode(val);
    if (L->head == NULL) {
        L->head = n;
        return;
    }
    // Scorriamo fino alla fine per inserire in coda
    node_t *curr = L->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = n;
}

void LISTprint(list L) {
    node_t *curr = L->head;
    printf("Lista: ");
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

void LISTfree(list L) {
    node_t *curr = L->head;
    node_t *next;
    while (curr != NULL) {
        next = curr->next;
        free(curr); // Libero il nodo
        curr = next;
    }
    free(L); // Libero il wrapper
}

/* =================================================================================
 * FUNZIONE: swap
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Scambia il valore del nodo in posizione k (dalla testa) con il valore del nodo
 * in posizione k dalla coda.
 *
 * PARAMETRI:
 * - list L: Il puntatore all'ADT lista.
 * - int k: L'indice intero specificato.
 *
 * CONCETTI CHIAVE:
 * 1. Calcolo Lunghezza: Poiché è una lista singola, non sappiamo dov'è la coda
 * senza scorrerla tutta. Dobbiamo calcolare la lunghezza 'N'.
 * 2. Indici:
 * - Nodo A (dalla testa): Indice 'k'.
 * - Nodo B (dalla coda): Indice 'N - 1 - k'.
 * 3. Scambio Valori: Invece di "staccare" i puntatori e riattaccarli (rischio bug),
 * scambiamo semplicemente il campo 'val' dei due nodi trovati.
 * =================================================================================
 */
void swap(list L, int k) {
    
    /* --- 1. CONTROLLO LISTA VUOTA --- */
    if (L == NULL || L->head == NULL) {
        return;
    }

    /* --- 2. CALCOLO LUNGHEZZA (N) --- */
    int N = 0;
    node_t *curr = L->head;
    
    // Scorriamo tutta la lista per contare i nodi
    while (curr != NULL) {
        N++;
        curr = curr->next;
    }

    /* --- 3. VERIFICA VALIDITÀ DI K --- */
    // Se k è fuori range (es. lista lung 5, chiedo k=6), non faccio nulla.
    if (k >= N) {
        return; 
    }

    /* --- 4. CALCOLO INDICI TARGET --- */
    int idx_head = k;            // Indice partendo dall'inizio (0, 1, 2...)
    int idx_tail = N - 1 - k;    // Indice corrispondente dalla fine
    
    // Esempio: N=7, k=2. 
    // idx_head = 2.
    // idx_tail = 7 - 1 - 2 = 4.
    
    // Ottimizzazione: Se gli indici coincidono (es. elemento centrale), inutile scambiare.
    if (idx_head == idx_tail) {
        return;
    }

    /* --- 5. RICERCA DEI PUNTATORI AI DUE NODI --- */
    node_t *nodeA = L->head;
    node_t *nodeB = L->head;

    // Avanziamo fino al nodo A
    for (int i = 0; i < idx_head; i++) {
        nodeA = nodeA->next;
    }

    // Avanziamo fino al nodo B (ripartendo dalla testa o ottimizzando, qui ripartiamo per chiarezza)
    for (int i = 0; i < idx_tail; i++) {
        nodeB = nodeB->next;
    }

    /* --- 6. SCAMBIO DEI VALORI (SWAP) --- */
    // Usiamo una variabile temporanea intera
    int temp = nodeA->val;      // Salvo valore di A
    nodeA->val = nodeB->val;    // Scrivo valore di B in A
    nodeB->val = temp;          // Scrivo vecchio valore di A in B
}