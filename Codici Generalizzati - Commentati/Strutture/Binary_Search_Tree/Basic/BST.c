// Includiamo l'header file che contiene le definizioni dei tipi Item, Key, BST
// e probabilmente i prototipi delle funzioni pubbliche.
#include "BST.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

// 'link' è un alias (un soprannome) per un puntatore a 'struct BSTnode'.
typedef struct BSTnode *link;

// Questa è la struttura che definisce il singolo NODO dell'albero.
struct BSTnode {
    Item item; // Il dato contenuto nel nodo (es. intero, struct, ...)
    link l;    // Puntatore al figlio sinistro
    link r;    // Puntatore al figlio destro
};

// Questa è la struttura "wrapper" (contenitore) dell'intero albero.
// È l'ADT di prima classe (un puntatore a questa struct è un 'BST').
struct binarysearchtree {
    link root; // Puntatore al nodo radice dell'albero.
    link z;    // Puntatore a un NODO SENTINELLA (dummy node).
};

// Definiamo un tipo 'enum' per scegliere la strategia di visita/stampa.
typedef enum {
    PREORDER, INORDER, POSTORDER
} TraversalStrategy; // Nome del tipo (non presente nel codice, ma implicito)

// --- FUNZIONI PRIVATE (static) ---
// 'static' significa che queste funzioni sono "private" di questo file .c
// e non possono essere chiamate dall'esterno (es. dal main).

/*
 * Funzione 'NEW' (helper)
 * Alloca e inizializza un nuovo nodo.
 */
static link NEW(Item item, link l, link r) {
    // Alloca memoria per un nuovo nodo (grande quanto 'struct BSTnode').
    link x = malloc(sizeof *x); // *x si riferisce alla struttura puntata da x

    // Imposta i campi del nodo con i valori passati come argomenti.
    x->item = item; // Il dato
    x->l = l;       // Figlio sinistro
    x->r = r;       // Figlio destro

    // Ritorna il puntatore al nodo appena creato.
    return x;
}

// --- FUNZIONI PUBBLICHE (interfaccia dell'ADT) ---

/*
 * Funzione 'BSTinit'
 * Inizializza un albero binario di ricerca vuoto.
 */
BST BSTinit( ) {
    // Alloca memoria per la struttura "contenitore" (il wrapper).
    BST bst = malloc(sizeof *bst); // *bst è 'struct binarysearchtree'

    // Crea il nodo sentinella 'z'.
    // È un nodo "finto" che rappresenta il NULL.
    // Tutti i puntatori 'l' e 'r' che dovrebbero essere NULL, punteranno a 'z'.
    // Anche bst->root all'inizio punta a 'z' per indicare che l'albero è vuoto.
    bst->root = (bst->z = NEW(ITEMsetNull(), NULL, NULL)); // ITEMsetNull() ritorna un Item "vuoto".

    // Ritorna il puntatore all'ADT inizializzato.
    return bst;
}

/*
 * Funzione 'BSTfree'
 * Libera tutta la memoria allocata dall'albero (nodi e wrapper).
 */
void BSTfree(BST bst) {
    // Controllo di sicurezza: se l'albero è già NULL, non fare nulla.
    if (bst == NULL)
        return;
    // Chiama la funzione ricorsiva 'treeFree' per liberare tutti i nodi.
    treeFree(bst->root, bst->z);
    // Libera il nodo sentinella (che era stato allocato a parte).
    free(bst->z);
    // Libera la struttura wrapper.
    free(bst);
}

/*
 * Funzione 'treeFree' (helper ricorsivo per BSTfree)
 * Libera (in post-ordine) tutti i nodi di un sottoalbero.
 */
static void treeFree(link h, link z) {
    // Caso base: se il nodo è la sentinella (NULL), fermati.
    if (h == z)
        return;
    // Libera ricorsivamente il sottoalbero sinistro.
    treeFree(h->l, z);
    // Libera ricorsivamente il sottoalbero destro.
    treeFree(h->r, z);
    // Libera il nodo corrente 'h' (dopo aver liberato i suoi figli).
    free(h);
}

/*
 * Funzione 'countR' (helper ricorsivo per BSTcount)
 * Conta il numero di nodi (non sentinella) in un sottoalbero.
 */
static int countR(link h, link z) {
    // Caso base: se il nodo è la sentinella, conta 0.
    if (h == z)
        return 0;
    // Il totale è: (nodi a sinistra) + (nodi a destra) + 1 (il nodo corrente 'h').
    return countR(h->l, z) + countR(h->r, z) + 1;
}

/*
 * Funzione 'BSTcount'
 * Ritorna il numero totale di nodi nell'albero.
 */
int BSTcount(BST bst) {
    // Chiama la funzione ricorsiva partendo dalla radice.
    return countR(bst->root, bst->z);
}

/*
 * Funzione 'BSTempty'
 * Controlla se l'albero è vuoto.
 */
int BSTempty(BST bst) {
    // L'albero è vuoto se il suo conteggio di nodi è 0.
    if (BSTcount(bst) == 0)
        return 1; // 1 = vero (è vuoto)
    return 0; // 0 = falso (non è vuoto)
    // Alternativa: return (bst->root == bst->z);
}

/*
 * Funzione 'BSTinsert_leafI'
 * Inserisce un nuovo Item 'x' come foglia (Metodo Iterativo).
 */
void BSTinsert_leafI(BST bst, Item x) {
    // 'p' (parent) tiene traccia del genitore di 'h'.
    // 'h' (head) scorre l'albero per trovare la posizione.
    link p = bst->root, h = p;

    // Caso 1: L'albero è vuoto.
    if (bst->root == bst->z) {
        // Il nuovo nodo diventa la radice. Punta a 'z' (sentinella).
        bst->root = NEW(x, bst->z, bst->z);
        return; // Finito.
    }
    
    // Caso 2: Albero non vuoto.
    // Cicla finché 'h' non diventa la sentinella 'z'.
    while (h != bst->z) {
        p = h; // Aggiorna il genitore ('p' insegue 'h').
        // 'KEYcmp' confronta le chiavi (es. -1 se k1 < k2, 0 se k1 == k2, 1 se k1 > k2).
        // Se la nuova chiave è < di quella di 'h', vai a sinistra.
        h = (KEYcmp(KEYget(x), KEYget(h->item)) == -1) ? h->l : h->r;
    }
    
    // 'h' ora è 'z'. 'p' è l'ultimo nodo valido (la foglia genitore).
    // Creiamo il nuovo nodo che punta alla sentinella 'z'.
    h = NEW(x, bst->z, bst->z);
    
    // Collega il nuovo nodo 'h' al genitore 'p'.
    // Se la nuova chiave è < di quella di 'p', diventa figlio sinistro.
    if (KEYcmp(KEYget(x), KEYget(p->item)) == -1)
        p->l = h;
    else // Altrimenti (se >=), diventa figlio destro.
        p->r = h;
}

/*
 * Funzione 'insertR' (helper ricorsivo per BSTinsert_leafR)
 * Inserisce 'x' nel sottoalbero 'h' e ritorna il puntatore al sottoalbero (modificato).
 */
static link insertR(link h, Item x, link z) {
    // Caso base: Trovata la posizione (un puntatore alla sentinella).
    if (h == z)
        return NEW(x, z, z); // Ritorna il nuovo nodo.
    
    // Passo ricorsivo:
    // Se la nuova chiave è < di quella di 'h', scendi a sinistra.
    if (KEYcmp(KEYget(x), KEYget(h->item)) == -1)
        // Aggiorna il puntatore sinistro di 'h' con il risultato della ricorsione.
        h->l = insertR(h->l, x, z);
    else // Altrimenti (se >=), scendi a destra.
        // Aggiorna il puntatore destro di 'h'.
        h->r = insertR(h->r, x, z);
    
    // Ritorna il puntatore al nodo 'h' (che non è cambiato).
    return h;
}

/*
 * Funzione 'BSTinsert_leafR'
 * Inserisce un nuovo Item 'x' come foglia (Metodo Ricorsivo).
 */
void BSTinsert_leafR(BST bst, Item x) {
    // Chiama la funzione ricorsiva sulla radice e ri-assegna la radice
    // (serve se l'albero era vuoto).
    bst->root = insertR(bst->root, x, bst->z);
}

/*
 * Funzione 'insertT' (helper ricorsivo per BSTinsert_root)
 * Inserimento in radice (BST non bilanciato, usa rotazioni).
 */
static link insertT(link h, Item x, link z) {
    // Caso base: Trovata la posizione (sentinella).
    if (h == z)
        return NEW(x, z, z); // Ritorna il nuovo nodo.
    
    // Inserisci ricorsivamente (come in 'insertR').
    if (KEYcmp(KEYget(x), KEYget(h->item)) == -1) {
        h->l = insertT(h->l, x, z); // Inserisci a sinistra
        h = rotR(h); // Esegui una ROTAZIONE A DESTRA per portare 'x' su.
    } else {
        h->r = insertT(h->r, x, z); // Inserisci a destra
        h = rotL(h); // Esegui una ROTAZIONE A SINISTRA per portare 'x' su.
    }

    // Ritorna il puntatore al nodo 'h' (che ora è il nuovo nodo inserito,
    // o un nodo ruotato).
    return h;
}

/*
 * Funzione 'BSTinsert_root'
 * Inserisce un nuovo Item 'x' e lo rende la nuova RADICE dell'albero.
 */
void BSTinsert_root(BST bst, Item x) {
    // Chiama la funzione ricorsiva con rotazioni.
    bst->root = insertT(bst->root, x, bst->z);
}

/*
 * Funzione 'searchR' (helper ricorsivo per BSTsearch)
 * Cerca un Item data una chiave 'k' nel sottoalbero 'h'.
 */
Item searchR(link h, Key k, link z) {
    int cmp; // Variabile per il risultato del confronto.

    // Caso base 1: Nodo è sentinella (non trovato).
    if (h == z)
        return ITEMsetNull(); // Ritorna l'Item "vuoto".
    
    // Confronta la chiave 'k' con la chiave del nodo corrente 'h'.
    cmp = KEYcmp(k, KEYget(h->item));
    
    // Caso base 2: Trovato! (chiavi uguali).
    if (cmp == 0)
        return h->item; // Ritorna l'Item del nodo.
    
    // Passo ricorsivo 1: Se k è MINORE, cerca a sinistra.
    if (cmp == -1)
        return searchR(h->l, k, z);
    
    // Passo ricorsivo 2: (Se k è MAGGIORE), cerca a destra.
    return searchR(h->r, k, z);
}

/*
 * Funzione 'BSTsearch'
 * Cerca un Item data una chiave 'k'.
 */
Item BSTsearch(BST bst, Key k) {
    // Chiama la funzione ricorsiva partendo dalla radice.
    return searchR(bst->root, k, bst->z);
}

/*
 * Funzione 'minR' (helper ricorsivo per BSTmin)
 * Trova l'Item con chiave minima nel sottoalbero 'h'.
 */
Item minR(link h, link z) {
    // Caso base 1: Albero vuoto.
    if (h == z)
        return ITEMsetNull();
    // Caso base 2: Trovato il minimo.
    // Il minimo è il nodo più a sinistra, cioè quello
    // che non ha un figlio sinistro (ha 'z' come figlio sinistro).
    if (h->l == z)
        return (h->item);
    
    // Passo ricorsivo: Continua a scendere a sinistra.
    return minR(h->l, z);
}

/*
 * Funzione 'BSTmin'
 * Trova l'Item con chiave minima nell'intero albero.
 */
Item BSTmin(BST bst) {
    return minR(bst->root, bst->z);
}

/*
 * Funzione 'maxR' (helper ricorsivo per BSTmax)
 * Trova l'Item con chiave massima nel sottoalbero 'h'.
 */
Item maxR(link h, link z) {
    // Caso base 1: Albero vuoto.
    if (h == z)
        return ITEMsetNull();
    // Caso base 2: Trovato il massimo.
    // Il massimo è il nodo più a destra (quello con 'z' come figlio destro).
    if (h->r == z)
        return (h->item);
    
    // Passo ricorsivo: Continua a scendere a destra.
    return maxR(h->r, z);
}

/*
 * Funzione 'BSTmax'
 * Trova l'Item con chiave massima nell'intero albero.
 */
Item BSTmax(BST bst) {
    return maxR(bst->root, bst->z);
}

/*
 * Funzione 'treePrintR' (helper ricorsivo per BSTvisit)
 * Visita (attraversa) l'albero e "visita" (ITEMstore) i nodi
 * secondo la strategia (PREORDER, INORDER, POSTORDER).
 */
static void treePrintR(link h, link z, int strategy) {
    // Caso base: Se è sentinella, fermati.
    if (h == z)
        return;
    
    // PREORDINE: Visita il NODO, poi Sinistra, poi Destra.
    if (strategy == PREORDER)
        ITEMstore(h->item); // ITEMstore presumibilmente stampa o salva l'item.
    
    // Visita ricorsiva del sottoalbero sinistro.
    treePrintR(h->l, z, strategy);
    
    // INORDINE: Visita Sinistra, poi il NODO, poi Destra.
    // (Per un BST, questo stampa i nodi in ordine di chiave).
    if (strategy == INORDER)
        ITEMstore(h->item);
    
    // Visita ricorsiva del sottoalboro destro.
    treePrintR(h->r, z, strategy);
    
    // POSTORDINE: Visita Sinistra, poi Destra, poi il NODO.
    if (strategy == POSTORDER)
        ITEMstore(h->item);
}

/*
 * Funzione 'BSTvisit'
 * Esegue una visita completa dell'albero.
 */
void BSTvisit(BST bst, int strategy) {
    // Se l'albero è vuoto, non fare nulla.
    if (BSTempty(bst))
        return;
    // Chiama la funzione ricorsiva di visita/stampa.
    treePrintR(bst->root, bst->z, strategy);
}

/*
 * Funzione 'rotL' (Rotazione a Sinistra)
 * Esegue una rotazione a sinistra sul nodo 'h'.
 */
link rotL(link h) {
    // 'x' è il figlio destro di 'h' (che diventerà il nuovo genitore).
    link x = h->r;

    // Il figlio sinistro di 'x' (sottoalbero "in mezzo")
    // diventa il nuovo figlio destro di 'h'.
    h->r = x->l;
    
    // 'h' (il vecchio genitore) diventa il figlio sinistro di 'x'.
    x->l = h;

    // Ritorna 'x', che è il nuovo nodo radice di questo sottoalbero.
    return x;
}

/*
 * Funzione 'rotR' (Rotazione a Destra)
 * Esegue una rotazione a destra sul nodo 'h'.
 */
link rotR(link h) {
    // 'x' è il figlio sinistro di 'h' (che diventerà il nuovo genitore).
    link x = h->l;

    // Il figlio destro di 'x' (sottoalbero "in mezzo")
    // diventa il nuovo figlio sinistro di 'h'.
    h->l = x->r;
    
    // 'h' (il vecchio genitore) diventa il figlio destro di 'x'.
    x->r = h;

    // Ritorna 'x', che è il nuovo nodo radice di questo sottoalbero.
    return x;
}