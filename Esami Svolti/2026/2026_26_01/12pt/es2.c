#include <stdio.h>
#include <stdlib.h>

// --- Definizioni di supporto per far funzionare il codice dell'immagine ---
typedef int Key;

typedef struct {
    Key key;
    // altri campi...
} Item;

typedef struct node {
    Item item;
    struct node *l, *r;
} *link;

typedef struct bst {
    link root;
    link z; // nodo sentinella
} *BST;

// Macro/Funzioni assunte dall'esercizio
Key KEYget(Item *item) {
    return item->key;
}

// Ritorna <0 se k1 < k2, 0 se uguali, >0 se k1 > k2
int KEYcmp(Key k1, Key k2) {
    return k1 - k2;
}
// ------------------------------------------------------------------------

/*
 * CODICE COPIATO DALLA SOLUZIONE NELL'IMMAGINE
 */

static int countBetweenR(link h, link z, Key k1, Key k2, int *cP) {
    int cmp1, cmp2;
    if (h == z) {
        return 0;
    }
    
    // Nell'immagine si usa &h->item
    cmp1 = KEYcmp(k1, KEYget(&h->item)); 
    cmp2 = KEYcmp(k2, KEYget(&h->item));

    if (cmp1==0) (*cP)++;
    if (cmp2==0) (*cP)++;

    if (cmp1>=0) { // entrambe le chiavi a destra - non ricorre a sinistra
        // Se k1 >= item, allora item <= k1. Poiché cerchiamo nodi > k1,
        // tutto ciò che è a sinistra (che è < item) è sicuramente < k1. Inutile visitare sinistra.
        return countBetweenR(h->r, z, k1, k2, cP);
    }
    
    if (cmp2<=0) { // entrambe le chiavi a sinistra - non ricorre a destra
        // Se k2 <= item, allora item >= k2. Poiché cerchiamo nodi < k2,
        // tutto ciò che è a destra (che è > item) è sicuramente > k2. Inutile visitare destra.
        return countBetweenR(h->l, z, k1, k2, cP);
    }

    // la chiave è in mezzo - contala e ricorri da entrambe le parti
    return 1 + countBetweenR(h->l, z, k1, k2, cP) + countBetweenR(h->r, z, k1, k2, cP);
}

int BSTcountBetween(BST b, Key k1, Key k2) {
    int foundCnt=0;
    if (KEYcmp(k1, k2)>0) { // forza k1<k2 (non è richiesto ma gestito)
        Key t=k1; k1=k2; k2=t;
    }
    
    int ret = countBetweenR(b->root, b->z, k1, k2, &foundCnt);
    
    if (foundCnt<2) return -1;
    else return ret;
}

// ------------------------------------------------------------------------
// MAIN DI TEST (Per verificare che il codice dell'immagine funzioni)
// ------------------------------------------------------------------------

link newNode(Key k, link z) {
    link x = malloc(sizeof *x);
    x->item.key = k;
    x->l = z; 
    x->r = z;
    return x;
}

int main() {
    // Setup di un piccolo albero per testare la funzione trascritta
    BST b = malloc(sizeof *b);
    b->z = malloc(sizeof(struct node)); // Sentinella
    b->root = newNode(10, b->z);
    
    b->root->l = newNode(5, b->z);
    b->root->r = newNode(15, b->z);
    
    b->root->l->l = newNode(2, b->z);
    b->root->l->r = newNode(8, b->z);
    
    b->root->r->l = newNode(12, b->z);
    b->root->r->r = newNode(18, b->z);

    // Test: Chiavi tra 5 e 15 (escluse). Nodi attesi: 8, 10, 12 -> Totale 3.
    // Le chiavi 5 e 15 esistono, quindi non deve ritornare -1.
    printf("Risultato (atteso 3): %d\n", BSTcountBetween(b, 5, 15));

    // Test: Una chiave non esiste (es. 100) -> deve ritornare -1
    printf("Risultato con chiave mancante (atteso -1): %d\n", BSTcountBetween(b, 5, 100));

    return 0;
}