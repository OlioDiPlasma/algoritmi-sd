#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Per abs()

/*
 * ======================================================================================
 * 1. DEFINIZIONE STRUTTURE DATI (ADT BST)
 * ======================================================================================
 */

typedef int Item;

typedef struct BSTnode {
    Item item;
    struct BSTnode *l; // left child
    struct BSTnode *r; // right child
} BSTnode;

typedef BSTnode* link;

struct binary_search_tree {
    link root;
    // Potremmo avere un campo 'size' globale, ma l'esercizio chiede di
    // verificare il bilanciamento locale dei sottoalberi.
};
typedef struct binary_search_tree *BST;

/*
 * ======================================================================================
 * FUNZIONE AUSILIARIA: checkBalanceAndSize
 * ======================================================================================
 * Scopo:
 * Questa è la funzione "tuttofare". Esegue una visita Post-Order (Sinistra, Destra, Radice).
 * Ritorna la DIMENSIONE del sottoalbero radicato in 'h', ma nel frattempo controlla
 * se il nodo 'h' è bilanciato.
 *
 * Parametri:
 * - link h: Nodo corrente.
 * - int *isBalanced: Puntatore a un flag (booleano). Se troviamo un nodo sbilanciato,
 * impostiamo questo flag a 0 (Falso).
 *
 * Ritorna:
 * - int: Il numero di nodi nel sottoalbero radicato in 'h' (size).
 *
 * Concetti Chiave:
 * - Bottom-Up: Le informazioni (size) fluiscono dalle foglie verso la radice.
 * - Short-circuiting: Se scopriamo che un sottoalbero è sbilanciato, potremmo teoricamente
 * fermarci subito, ma per semplicità didattica completiamo la visita aggiornando il flag.
 */
int checkBalanceAndSize(link h, int *isBalanced) {
    // CASO BASE: Nodo nullo (albero vuoto)
    // Ha dimensione 0 ed è tecnicamente bilanciato.
    if (h == NULL) {
        return 0;
    }

    /* --- 1. VISITA RICORSIVA (Calcolo dimensioni figli) --- */
    // Chiediamo ai figli: "Quanti nodi avete?"
    int size_l = checkBalanceAndSize(h->l, isBalanced);
    int size_r = checkBalanceAndSize(h->r, isBalanced);

    /* --- 2. VERIFICA BILANCIAMENTO LOCALE --- */
    // Ora abbiamo le dimensioni dei due sottoalberi.
    // Verifichiamo la condizione: |size_sx - size_dx| <= 1
    if (abs(size_l - size_r) > 1) {
        *isBalanced = 0; // Trovato uno sbilanciamento!
    }

    /* --- 3. RITORNO DIMENSIONE --- */
    // La dimensione di questo albero è: nodi a sx + nodi a dx + 1 (la radice stessa)
    return size_l + size_r + 1;
}

/*
 * ======================================================================================
 * FUNZIONE PRINCIPALE: BSTisBalanced
 * ======================================================================================
 * Scopo:
 * Interfaccia pubblica. Inizializza il flag e lancia la ricorsione.
 */
int BSTisBalanced(BST b) {
    // Controllo robustezza
    if (b == NULL) return 1; // Un albero inesistente è bilanciato? Diciamo di sì o gestiamo errore.

    int isBalanced = 1; // Assumiamo sia bilanciato (1 = Vero) all'inizio.

    // Lanciamo la funzione ausiliaria.
    // Ignoriamo il valore di ritorno (la dimensione totale) perché ci interessa solo
    // l'effetto collaterale sulla variabile 'isBalanced'.
    checkBalanceAndSize(b->root, &isBalanced);

    return isBalanced;
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO (PER IL TEST)
 * ======================================================================================
 */
link newNode(Item val, link l, link r) {
    link x = malloc(sizeof *x);
    x->item = val;
    x->l = l;
    x->r = r;
    return x;
}

int main() {
    /* --- TEST 1: Albero Bilanciato per dimensioni --- */
    //       10
    //      /  \
    //     5    20
    // Size(L)=1, Size(R)=1 -> Diff=0 (OK)
    BST b1 = malloc(sizeof(*b1));
    b1->root = newNode(10, newNode(5, NULL, NULL), newNode(20, NULL, NULL));

    printf("Test 1 (Bilanciato): %s (Atteso: 1)\n", 
           BSTisBalanced(b1) ? "SI" : "NO");

    /* --- TEST 2: Albero Sbilanciato --- */
    //       10
    //      /  \
    //     5    NULL
    //    /
    //   2
    // Nodo 5: L=1, R=0 -> Diff=1 (OK)
    // Nodo 10: L=2, R=0 -> Diff=2 (NO! Sbilanciato)
    BST b2 = malloc(sizeof(*b2));
    b2->root = newNode(10, 
                       newNode(5, 
                           newNode(2, NULL, NULL), 
                           NULL), 
                       NULL);

    printf("Test 2 (Sbilanciato): %s (Atteso: 0)\n", 
           BSTisBalanced(b2) ? "SI" : "NO");

    /* --- CLEANUP (Omesso per brevità, ma necessario in esame) --- */
    // freeTree(b1->root); free(b1); ...

    return 0;
}