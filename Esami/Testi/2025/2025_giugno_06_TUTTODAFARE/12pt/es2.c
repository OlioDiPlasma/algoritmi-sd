#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================= */
/* DEFINIZIONE ADT                              */
/* ========================================================================= */

/* Immagina che Item sia un dato complesso. Qui usiamo int per semplicità */
typedef int Item;

/* Struttura del Nodo (link è un puntatore a struct node) */
typedef struct node *link;
struct node {
    Item item;
    link l;     /* Puntatore al figlio sinistro */
    link r;     /* Puntatore al figlio destro */
};

/* Struttura Wrapper dell'ADT BST */
struct binarysearchtree {
    link root;  /* Puntatore alla radice dell'albero */
    int size;   /* (Opzionale) Numero di nodi */
};
typedef struct binarysearchtree *BST;

/* * Funzione fornita dal testo (simulata qui).
 * Scopo: Creare una copia di un Item.
 */
Item ItemDup(Item i) {
    /* Se Item fosse una struct con puntatori, qui faremmo malloc.
       Essendo int, ritorniamo il valore diretto. */
    return i; 
}

/* ========================================================================= */
/* IMPLEMENTAZIONE SOLUZIONE                       */
/* ========================================================================= */

/*
 * Funzione: dupR (Helper Ricorsivo)
 * -------------------------------------------------------------------------
 * Scopo:       Duplica ricorsivamente una catena di nodi (sottoalbero).
 * Parametri:   
 * - h: il puntatore al nodo radice del sottoalbero corrente da copiare.
 * Ritorna:     Un puntatore (link) al nuovo nodo duplicato (o NULL).
 * -------------------------------------------------------------------------
 */
static link dupR(link h) {
    /* --- 1. CASO BASE --- */
    /* Se il nodo originale è NULL, non c'è nulla da copiare. */
    if (h == NULL)
        return NULL;

    /* --- 2. ALLOCAZIONE NUOVO NODO --- */
    /*
     * MALLOC: Richiediamo memoria nello heap per un nuovo nodo.
     * sizeof(*new_node): Calcola la dimensione della struttura puntata.
     * È buona norma controllare se malloc ritorna NULL, ma per brevità didattica lo omettiamo.
     */
    link new_node = malloc(sizeof *new_node);

    /* --- 3. COPIA DEL DATO --- */
    /* Usiamo la funzione ItemDup come richiesto per garantire l'indipendenza del dato. 
       Accediamo al campo item usando l'operatore freccia (->) perché h è un puntatore. */
    new_node->item = ItemDup(h->item);

    /* --- 4. RICORSIONE (PASSO INDUTTIVO) --- */
    /*
     * Qui avviene la magia della Deep Copy.
     * Assegniamo al puntatore sinistro (l) del nuovo nodo il risultato 
     * della duplicazione del sottoalbero sinistro originale.
     * Stessa cosa per il destro (r).
     */
    new_node->l = dupR(h->l);
    new_node->r = dupR(h->r);

    /* Ritorniamo l'indirizzo del nodo appena creato al chiamante */
    return new_node;
}

/*
 * Funzione: BstDup (Funzione richiesta dall'esercizio)
 * -------------------------------------------------------------------------
 * Scopo:       Crea un nuovo ADT BST identico a quello passato.
 * Parametri:   
 * - b: il BST originale da copiare.
 * Concetti:    Questa è una funzione "wrapper". Gestisce la struttura esterna
 * e delega la copia dei nodi alla funzione ricorsiva.
 * -------------------------------------------------------------------------
 */
BST BstDup(BST b) {
    /* --- 1. CONTROLLI INIZIALI --- */
    /* Se l'albero in input non esiste, ritorniamo NULL */
    if (b == NULL) 
        return NULL;

    /* --- 2. ALLOCAZIONE STRUTTURA ADT --- */
    /* Allocazione dinamica per il contenitore (wrapper) del nuovo albero */
    BST new_bst = malloc(sizeof *new_bst);
    
    /* --- 3. COPIA DELLA STRUTTURA --- */
    /* Chiamiamo la funzione ricorsiva partendo dalla radice dell'originale */
    new_bst->root = dupR(b->root);
    
    /* Copiamo eventuali metadati (come la dimensione) */
    new_bst->size = b->size;

    return new_bst;
}

/* ========================================================================= */
/* FUNZIONI DI UTILITÀ (TEST)                     */
/* ========================================================================= */

/* Crea un nuovo nodo (per popolare l'albero di test) */
link NEW(Item item, link l, link r) {
    link x = malloc(sizeof *x);
    x->item = item; x->l = l; x->r = r;
    return x;
}

/* Inizializza un BST vuoto */
BST BSTinit() {
    BST b = malloc(sizeof *b);
    b->root = NULL;
    b->size = 0;
    return b;
}

/* Inserimento semplice non bilanciato per creare dati di prova */
void BSTinsert_test(BST b, Item item) {
    /* Nota: questa è una insert semplificata iterativa solo per il test */
    link *p = &(b->root); /* puntatore a puntatore per scorrere */
    while (*p != NULL) {
        if (item < (*p)->item) p = &((*p)->l);
        else p = &((*p)->r);
    }
    *p = NEW(item, NULL, NULL);
    b->size++;
}

/* Stampa in Pre-Order per verificare la struttura */
void treePrint(link h) {
    if (h == NULL) return;
    printf("%d ", h->item);
    treePrint(h->l);
    treePrint(h->r);
}

/* Wrapper per la stampa */
void BSTprint(BST b) {
    if (b == NULL) return;
    treePrint(b->root);
    printf("\n");
}

/* Libera la memoria (importante!) */
void treeFree(link h) {
    if (h == NULL) return;
    treeFree(h->l);
    treeFree(h->r);
    free(h);
}

void BSTfree(BST b) {
    if (b == NULL) return;
    treeFree(b->root);
    free(b);
}

/* ========================================================================= */
/* MAIN DI TEST                            */
/* ========================================================================= */

int main() {
    printf("--- LEZIONE: Duplicazione BST ---\n");

    /* --- 1. CREAZIONE DATI DI PROVA --- */
    /* Creiamo un albero originale e inseriamo dei valori */
    BST originale = BSTinit();
    BSTinsert_test(originale, 20);
    BSTinsert_test(originale, 10);
    BSTinsert_test(originale, 30);
    BSTinsert_test(originale, 5);
    
    printf("Albero Originale (Pre-Order): ");
    BSTprint(originale);

    /* --- 2. CHIAMATA ALLA FUNZIONE DA IMPLEMENTARE --- */
    printf("\n...Duplicazione in corso...\n");
    BST copia = BstDup(originale);

    /* --- 3. VERIFICA RISULTATI --- */
    printf("Albero Copia     (Pre-Order): ");
    BSTprint(copia);

    /* --- 4. VERIFICA DEEP COPY (INDIPENDENZA) --- */
    printf("\n--- Test Indipendenza (Deep Copy) ---\n");
    printf("Modifico la copia (aggiungo 100). L'originale NON deve cambiare.\n");
    
    /* Aggiungiamo un nodo solo alla copia */
    BSTinsert_test(copia, 100);

    printf("Originale dopo modifica copia: ");
    BSTprint(originale); /* Dovrebbe rimanere 20 10 5 30 */
    
    printf("Copia dopo modifica:         ");
    BSTprint(copia);     /* Dovrebbe avere 20 10 5 30 100 */

    /* --- 5. PULIZIA MEMORIA --- */
    BSTfree(originale);
    BSTfree(copia);
    
    printf("\nMemoria liberata. Lezione conclusa.\n");

    return 0;
}