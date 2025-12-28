#include <stdio.h>
#include <stdlib.h>

/* ========================================================================= */
/* 1. DEFINIZIONE ADT BST (Simulazione)                                      */
/* ========================================================================= */

/* Definiamo Key come int per semplicità, come spesso accade negli esami */
typedef int Key;

/* Struttura del Nodo */
typedef struct node *link;
struct node {
    Key key;
    link l;     /* Figlio Sinistro */
    link r;     /* Figlio Destro */
};

/* Wrapper del BST (contiene la radice) */
struct binarysearchtree {
    link root;
};
typedef struct binarysearchtree *BST;

/* ========================================================================= */
/* 2. FUNZIONI AUSILIARIE (PRIVATE)                                          */
/* ========================================================================= */

/*
 * Funzione: pathLen
 * -------------------------------------------------------------------------
 * Scopo:       Calcola la distanza (numero di archi) da un nodo 'h' a una
 * chiave target 'k'. Verifica anche l'esistenza della chiave.
 * Parametri:
 * - h: nodo di partenza (sottoalbero).
 * - k: chiave da cercare.
 * Ritorna:     La distanza >= 0 se trovata, -1 se la chiave non esiste.
 * -------------------------------------------------------------------------
 */
int pathLen(link h, Key k) {
    /* --- CASO BASE: ALBERO VUOTO --- */
    if (h == NULL) {
        return -1; /* Chiave non trovata, fine del ramo */
    }

    /* --- CASO BASE: TROVATO --- */
    if (h->key == k) {
        return 0; /* Distanza da se stesso è 0 */
    }

    /* --- RICORSIONE (PASSO INDUTTIVO) --- */
    /* Sfruttiamo la proprietà del BST:
       - Se k < chiave corrente, andiamo a sinistra.
       - Se k > chiave corrente, andiamo a destra.
    */
    if (k < h->key) {
        int d = pathLen(h->l, k);
        /* Se d è -1, significa che non l'abbiamo trovato giù di lì,
           quindi ritorniamo -1. Altrimenti aggiungiamo 1 (questo arco). */
        return (d == -1) ? -1 : 1 + d;
    } 
    else {
        int d = pathLen(h->r, k);
        return (d == -1) ? -1 : 1 + d;
    }
}

/*
 * Funzione: findLCA
 * -------------------------------------------------------------------------
 * Scopo:       Trova il "Lowest Common Ancestor" (Antenato Comune più Basso).
 * È il nodo dove i percorsi per k1 e k2 divergono.
 * Parametri:
 * - h: nodo radice del sottoalbero corrente.
 * - k1, k2: le due chiavi.
 * Concetto:    In un BST, se entrambe le chiavi sono minori del nodo corrente,
 * l'LCA è a sinistra. Se entrambe maggiori, è a destra.
 * Altrimenti, SIAMO sull'LCA.
 * -------------------------------------------------------------------------
 */
link findLCA(link h, Key k1, Key k2) {
    if (h == NULL) return NULL;

    /* Se entrambe le chiavi sono più piccole, l'LCA sta a sinistra */
    if (k1 < h->key && k2 < h->key) {
        return findLCA(h->l, k1, k2);
    }
    
    /* Se entrambe le chiavi sono più grandi, l'LCA sta a destra */
    if (k1 > h->key && k2 > h->key) {
        return findLCA(h->r, k1, k2);
    }

    /*
     * Se siamo qui, significa che o abbiamo trovato uno dei nodi (h->key == k1 o k2),
     * oppure k1 < h->key < k2 (punto di divergenza).
     * In entrambi i casi, 'h' è il candidato LCA.
     */
    return h;
}

/* ========================================================================= */
/* 3. FUNZIONE PRINCIPALE (RICHIESTA DALL'ESERCIZIO)                         */
/* ========================================================================= */

/*
 * Funzione: BSTdist
 * -------------------------------------------------------------------------
 * Scopo:       Calcola la distanza tra due nodi k1 e k2.
 * Parametri:
 * - b: il puntatore all'ADT BST.
 * - k1, k2: le chiavi target.
 * Ritorna:     Distanza intera, oppure -1 se una o entrambe mancano.
 * -------------------------------------------------------------------------
 */
int BSTdist(BST b, Key k1, Key k2) {
    /* --- 1. CONTROLLI DI SICUREZZA --- */
    if (b == NULL || b->root == NULL) {
        return -1;
    }

    /* --- 2. LOGICA LCA (Lowest Common Ancestor) --- */
    /*
     * Troviamo il nodo da cui i percorsi verso k1 e k2 si separano.
     * Nota: Questo trova un POTENZIALE antenato. Non garantisce che k1 e k2
     * esistano effettivamente nell'albero (potrebbero mancare più in basso).
     */
    link lca = findLCA(b->root, k1, k2);
    
    if (lca == NULL) return -1; /* Albero vuoto o logica fallita */

    /* --- 3. CALCOLO DISTANZE PARZIALI --- */
    /*
     * Calcoliamo la distanza dall'LCA a k1 e dall'LCA a k2.
     * La funzione pathLen ritorna -1 se la chiave non esiste nel sottoalbero.
     */
    int d1 = pathLen(lca, k1);
    int d2 = pathLen(lca, k2);

    /* --- 4. VERIFICA ESISTENZA --- */
    /* Se anche solo una delle due chiavi non è stata trovata... */
    if (d1 == -1 || d2 == -1) {
        return -1;
    }

    /* --- 5. RISULTATO --- */
    /* La distanza totale è la somma dei due segmenti */
    return d1 + d2;
}

/* ========================================================================= */
/* 4. MAIN DI TEST E FUNZIONI DI SUPPORTO                                    */
/* ========================================================================= */

/* Helper per creare nodi (malloc e inizializzazione) */
link NEW(Key k, link l, link r) {
    /* MALLOC: Chiedo memoria per un singolo nodo */
    link x = malloc(sizeof *x); 
    x->key = k; 
    x->l = l; 
    x->r = r;
    return x;
}

/* Helper per inizializzare BST */
BST BSTinit() {
    BST b = malloc(sizeof *b);
    b->root = NULL;
    return b;
}

/* Inserimento semplice per popolare l'albero di prova */
void BSTinsert_simple(BST b, Key k) {
    if (b->root == NULL) {
        b->root = NEW(k, NULL, NULL);
        return;
    }
    link p = b->root;
    while (1) {
        if (k < p->key) {
            if (p->l == NULL) { p->l = NEW(k, NULL, NULL); break; }
            p = p->l;
        } else {
            if (p->r == NULL) { p->r = NEW(k, NULL, NULL); break; }
            p = p->r;
        }
    }
}

/* Funzione per liberare la memoria (Post-Order Traversal) */
void nodeFree(link h) {
    if (h == NULL) return;
    nodeFree(h->l);
    nodeFree(h->r);
    free(h); /* Libera il nodo corrente solo dopo aver liberato i figli */
}

void BSTfree(BST b) {
    if (b == NULL) return;
    nodeFree(b->root);
    free(b); /* Libera il wrapper */
}

int main() {
    printf("--- LEZIONE: Calcolo Distanza BST ---\n");

    /*
     * Costruiamo il seguente albero BST:
     * 20
     * /  \
     * 10    30
     * /  \     \
     * 5   15    40
     */
    BST b = BSTinit();
    BSTinsert_simple(b, 20);
    BSTinsert_simple(b, 10);
    BSTinsert_simple(b, 30);
    BSTinsert_simple(b, 5);
    BSTinsert_simple(b, 15);
    BSTinsert_simple(b, 40);

    /* --- TEST 1: Nodi in sottoalberi diversi (Passano per la radice) --- */
    /* Distanza tra 5 e 40. Percorso: 5->10->20->30->40. Archi: 4 */
    printf("Test 1 (5 e 40). Atteso: 4. Risultato: %d\n", BSTdist(b, 5, 40));

    /* --- TEST 2: Nodi nello stesso sottoalbero (LCA non è la radice) --- */
    /* Distanza tra 5 e 15. LCA è 10. Percorso: 5->10->15. Archi: 2 */
    printf("Test 2 (5 e 15). Atteso: 2. Risultato: %d\n", BSTdist(b, 5, 15));

    /* --- TEST 3: Uno dei nodi è l'antenato dell'altro --- */
    /* Distanza tra 10 e 5. Percorso: 10->5. Archi: 1 */
    printf("Test 3 (10 e 5). Atteso: 1. Risultato: %d\n", BSTdist(b, 10, 5));

    /* --- TEST 4: Nodo inesistente --- */
    /* Cerco 99. Deve ritornare -1 */
    printf("Test 4 (5 e 99). Atteso: -1. Risultato: %d\n", BSTdist(b, 5, 99));

    /* --- TEST 5: Stesso nodo --- */
    /* Distanza tra 20 e 20. Atteso: 0 */
    printf("Test 5 (20 e 20). Atteso: 0. Risultato: %d\n", BSTdist(b, 20, 20));

    /* Pulizia Memoria */
    BSTfree(b);
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}