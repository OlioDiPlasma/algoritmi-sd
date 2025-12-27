#include <stdio.h>
#include <stdlib.h>
#include "albero.h" /* Includiamo l'header per verificare la coerenza */

#define N 3 /* Grado dell'albero definito qui (dettaglio implementativo) */

/* --- 1. DEFINIZIONE STRUTTURE (Nascoste al Main) --- */

typedef struct node {
    int data;
    struct node *figli[N];
} Node;

struct tree_wrapper {
    Node *root;
};

/* --- 2. FUNZIONI PRIVATE (STATIC) --- */

/* Crea un singolo nodo (Non visibile fuori da questo file) */
static Node* creaNodo(int val) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (n == NULL) { perror("Malloc fallita"); exit(1); }
    n->data = val;
    for(int i=0; i<N; i++) n->figli[i] = NULL;
    return n;
}

/* La funzione ricorsiva vera e propria (WORKER) */
static void conta_nodi_ricorsiva(Node *u, int *c1, int *c2, int *c3) {
    if (u == NULL) return;

    /* Logica di conteggio locale */
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (u->figli[i] != NULL) count++;
    }

    /* Aggiornamento variabili esterne */
    if (count == 1) (*c1)++;
    else if (count == 2) (*c2)++;
    else if (count == 3) (*c3)++;

    /* Chiamate ricorsive sui figli */
    for (int i = 0; i < N; i++) {
        conta_nodi_ricorsiva(u->figli[i], c1, c2, c3);
    }
}

/* Funzione ricorsiva per la free */
static void freeNode(Node *n) {
    if (n == NULL) return;
    for(int i=0; i<N; i++) freeNode(n->figli[i]);
    free(n);
}

/* --- 3. IMPLEMENTAZIONE FUNZIONI PUBBLICHE --- */

nTREE initTree() {
    nTREE t = (nTREE)malloc(sizeof(struct tree_wrapper));
    if (t != NULL) t->root = NULL;
    return t;
}

void freeTree(nTREE t) {
    if (t != NULL) {
        freeNode(t->root);
        free(t);
    }
}

/* Funzione Wrapper richiesta dall'esercizio */
void countIf(nTREE t, int *count1, int *count2, int *count3) {
    if (t == NULL) return;

    /* Reset contatori */
    *count1 = 0; *count2 = 0; *count3 = 0;

    /* Avvio ricorsione */
    if (t->root != NULL) {
        conta_nodi_ricorsiva(t->root, count1, count2, count3);
    }
}

/* * Implementazione della funzione di test.
 * Qui possiamo accedere a t->root perché siamo dentro alberto.c!
 */
void popola_albero_per_test(nTREE t) {
    if (t == NULL) return;
    
    // Ricostruiamo lo stesso albero dell'esempio precedente
    t->root = creaNodo(10); // 3 figli -> count3

    t->root->figli[0] = creaNodo(20); // 1 figlio -> count1
    t->root->figli[1] = creaNodo(30); // 0 figli
    t->root->figli[2] = creaNodo(40); // 2 figli -> count2

    t->root->figli[0]->figli[0] = creaNodo(50);
    
    t->root->figli[2]->figli[0] = creaNodo(60);
    t->root->figli[2]->figli[1] = creaNodo(70);
}