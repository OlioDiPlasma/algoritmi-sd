#include "IBST.h" // Include le definizioni dei tipi (Item, Key, IBST)

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct IBSTnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO
struct IBSTnode {
    Item item; // Il dato (che è un intervallo, es. [low, high])
    link l, r; // Figlio sinistro e destro
    int N;     // Dimensione del sottoalbero (come nel BST precedente)
    int max;   // CAMPO CHIAVE: il valore MASSIMO tra tutti gli estremi
               // superiori (high) degli intervalli in questo sottoalbero.
};

// Struttura "wrapper" (contenitore) dell'albero
struct intervalbinarysearchtree {
    link root; // Puntatore alla radice
    int size;  // Dimensione totale dell'albero (memorizzata qui per O(1))
    link z;    // Puntatore al nodo sentinella (per il NULL)
};

// Enum per le strategie di visita (non usata nel codice, ma definita)
typedef enum {
    PREORDER, INORDER, POSTORDER
} TraversalStrategy; // Nome del tipo (non presente nel codice, ma implicito)

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo
static link NEW(Item item, link l, link r, int N, int max) {
    link x = malloc(sizeof *x); // Alloca memoria

    x->item = item; // Imposta l'intervallo
    x->l = l;       // Figlio sinistro
    x->r = r;       // Figlio destro
    x->N = N;       // Dimensione
    x->max = max;   // Valore massimo dell'estremo superiore

    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'NODEshow' (helper): stampa un nodo e il suo 'max'
static void NODEshow(link x) {
    // ITEMstore (non mostrata) stampa l'intervallo (es. [3, 10])
    ITEMstore(x->item); 
    // Stampa anche il valore 'max' memorizzato nel nodo
    printf("max = %d\n", x->max);
}

// Funzione 'IBSTinit': inizializza un albero vuoto
IBST IBSTinit( ) {
    IBST ibst = malloc(sizeof *ibst) ; // Alloca il wrapper
    ibst->size = 0; // La dimensione all'inizio è 0
    // Crea il nodo sentinella 'z'.
    // Il suo 'max' è -1 (o un valore molto piccolo) per non interferire
    // con i calcoli del 'max' reale. N = 0.
    ibst->root = (ibst->z = NEW(ITEMsetNull(), NULL, NULL, 0, -1));
    return ibst;
}

// --- GESTIONE MEMORIA E CONTEGGIO ---

// Funzione 'treeFree' (helper ricorsivo): libera i nodi
static void treeFree(link h, link z) {
    if (h == z) // Caso base: se è la sentinella, fermati
        return;
    treeFree(h->l, z); // Libera a sinistra
    treeFree(h->r, z); // Libera a destra
    free(h);           // Libera il nodo corrente (post-ordine)
}

// Funzione 'IBSTfree': libera l'intero albero (wrapper + nodi)
void IBSTfree(IBST ibst) {
    if (ibst == NULL) 
        return;
    treeFree(ibst->root, ibst->z); // Libera tutti i nodi
    free(ibst->z); // Libera la sentinella
    free(ibst);    // Libera il wrapper
}

// Funzione 'IBSTcount': ritorna il numero di nodi
int IBSTcount(IBST ibst) { 
    // Operazione O(1) perché il contatore è nel wrapper
    return ibst->size; 
}

// Funzione 'IBSTempty': controlla se l'albero è vuoto
int IBSTempty(IBST ibst) {
    if (IBSTcount(ibst) == 0) // Controlla la dimensione
        return 1; // 1 = vero
    return 0; // 0 = falso
}

// --- FUNZIONI DI INSERIMENTO ---

// Funzione 'insertR' (helper ricorsivo)
// L'albero è ordinato in base all'estremo INFERIORE (low) dell'intervallo.
link insertR(link h, Item item, link z) {
    // Caso base: trovato il punto di inserimento (un puntatore 'z')
    if (h == z)
        // Crea un nuovo nodo. N=1. 
        // Il suo 'max' iniziale è l'estremo superiore (high) dell'item stesso.
        return NEW(item, z, z, 1, ITEMhigh(item)); // ITEMhigh (non mostrata)
    
    // Passo ricorsivo:
    // ITEMlt (non mostrata) confronta gli estremi inferiori (low)
    if (ITEMlt(item, h->item)) { // Se item.low < h.item.low
        h->l = insertR(h->l, item, z); // Inserisci a sinistra
        // AGGIORNA MAX: il 'max' di 'h' è il massimo tra:
        // 1. Il suo vecchio max
        // 2. Il max del figlio sinistro (appena aggiornato)
        // 3. Il max del figlio destro
        h->max = max(h->max, h->l->max, h->r->max); // 'max' (non mostrata)
    } else {
        h->r = insertR(h->r, item, z); // Inserisci a destra
        // Stesso aggiornamento del max
        h->max = max(h->max, h->l->max, h->r->max);
    }
    
    (h->N)++; // Incrementa la dimensione del sottoalbero
    return h; // Ritorna il puntatore al nodo (modificato)
}

// Funzione 'IBSTinsert': inserisce un nuovo intervallo
void IBSTinsert(IBST ibst, Item item) {
    // Chiama la funzione ricorsiva e aggiorna la radice
    ibst->root = insertR(ibst->root, item, ibst->z);
    // Aggiorna il contatore globale nel wrapper
    ibst->size++;
}

// --- ROTAZIONI E BILANCIAMENTO ---

// Rotazione a Sinistra (aggiornata per 'N' e 'max')
link rotL(link h) {
    link x = h->r; // 'x' è il figlio destro (nuova radice)
    h->r = x->l;  // Figlio sx di 'x' diventa figlio dx di 'h'
    x->l = h;     // 'h' (vecchia radice) diventa figlio sx di 'x'
    
    // Aggiorna N
    x->N = h->N;
    h->N = h->l->N + h->r->N + 1;
    
    // AGGIORNA MAX (IMPORTANTE!)
    // Bisogna aggiornare PRIMA 'h' (che ora è figlio)
    h->max = max(ITEMhigh(h->item), h->l->max, h->r->max);
    // E POI 'x' (la nuova radice)
    x->max = max(ITEMhigh(x->item), x->l->max, x->r->max);

    return x; // Ritorna la nuova radice
}

// Rotazione a Destra (aggiornata per 'N' e 'max')
link rotR(link h) {
    link x = h->l; // 'x' è il figlio sinistro (nuova radice)
    h->l = x->r;  // Figlio dx di 'x' diventa figlio sx di 'h'
    x->r = h;     // 'h' (vecchia radice) diventa figlio dx di 'x'
    
    // Aggiorna N
    x->N = h->N;
    h->N = h->r->N + h->l->N + 1;
    
    // AGGIORNA MAX (prima 'h' poi 'x')
    h->max = max(ITEMhigh(h->item), h->l->max, h->r->max);
    x->max = max(ITEMhigh(x->item), x->l->max, x->r->max);

    return x; // Ritorna la nuova radice
}

// Partizionamento (Ricorsivo - helper)
// Porta il nodo di rango 'r' alla radice del sottoalbero 'h'
link partR(link h, int r) {
    int t = h->l->N; // Dimensione del sottoalbero sinistro

    if (t > r) { // Se 'r' è a sinistra
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if (t < r) { // Se 'r' è a destra
        h->r = partR(h->r, r-t-1); // Cerca con rango aggiornato
        h = rotL(h);
    }
    // Se t == r, 'h' è già il nodo giusto
    return h;
}

// Unione (Join) di due sottoalberi 'a' (sinistro) e 'b' (destro)
link joinLR(link a, link b, link z) {
    if (b == z) // Se 'b' è vuoto, ritorna 'a'
        return a;
    b = partR(b, 0); // Porta il minimo di 'b' (rango 0) in radice
    b->l = a; // Aggancia 'a' come figlio sinistro
    
    // Aggiorna N e max di 'b'
    b->N = a->N + b->r->N + 1;
    b->max = max(ITEMhigh(b->item), a->max, b->r->max);
    
    return b;
}

// --- CANCELLAZIONE E RICERCA ---

// Cancellazione (Ricorsiva - helper)
link deleteR(link h, Item item, link z) {
    link x;

    if (h == z) // Caso base: non trovato
        return z;
        
    // Cerca il nodo (ordinato per 'low' - ITEMlt)
    if (ITEMlt(item, h->item)) {
        h->l = deleteR(h->l, item, z);
        // Aggiorna 'max' al ritorno
        h->max = max(ITEMhigh(h->item), h->l->max, h->r->max);
    }
    if (ITEMlt(h->item, item)) { // item > h->item
        h->r = deleteR(h->r, item, z);
        // Aggiorna 'max' al ritorno
        h->max = max(ITEMhigh(h->item), h->l->max, h->r->max);
    }
    
    (h->N)--; // Decrementa N
    
    // Trovato il nodo (ITEMeq, non mostrata, controlla uguaglianza)
    if (ITEMeq(item, h->item)) { 
        x = h; // Salva il nodo da liberare
        h = joinLR(h->l, h->r, z); // Unisci i due sottoalberi
        free(x); // Libera il nodo
    }

    return h; // Ritorna il puntatore al (nuovo) nodo 'h'
}

// Cancellazione (wrapper)
void IBSTdelete(IBST ibst, Item item) {
    ibst->root = deleteR(ibst->root, item, ibst->z);
    ibst->size--; // Aggiorna la dimensione totale
}

// Ricerca di INTERSEZIONE (Ricorsiva - helper)
// Cerca un *qualsiasi* intervallo che si sovrappone con 'item'.
Item searchR(link h, Item item, link z) {
    if (h == z) // Caso base: non trovato
        return ITEMsetNull();
        
    // 1. Controlla il nodo corrente
    // ITEMoverlap (non mostrata) controlla se item e h->item si sovrappongono
    if (ITEMoverlap(item, h->item))
        return h->item; // Trovato!
    
    // 2. Decide dove cercare (PRUNING)
    // ITEMlt_int(item, h->l->max) -> (non mostrata)
    // Controlla se l'estremo *inferiore* (low) di 'item'
    // è minore del 'max' del sottoalbero sinistro.
    if (ITEMlt_int(item, h->l->max))
        // Se SÌ: un overlap è POSSIBILE a sinistra. Cerca a sinistra.
        return searchR(h->l, item, z);
    else
        // Se NO: (item.low >= h->l->max), nessun intervallo a sinistra
        // può sovrapporsi. Cerca *solo* a destra.
        return searchR(h->r, item, z);
}

// Ricerca di INTERSEZIONE (wrapper)
Item IBSTsearch(IBST ibst, Item item) {
    return searchR(ibst->root, item, ibst->z);
}

// --- VISITA E STAMPA ---

// Visita (Ricorsiva - helper)
static void treePrintR(link h, link z, int strategy) {
    if (h == z) 
        return;
    if (strategy == PREORDER) 
        NODEshow(h); // Stampa il nodo (inclusa la info 'max')
    treePrintR(h->l, z, strategy);
    if (strategy == INORDER)
        NODEshow(h);
    treePrintR(h->r, z, strategy);
    if (strategy == POSTORDER)
        NODEshow(h);
}

// Visita (wrapper)
void IBSTvisit(IBST ibst, int strategy) {
    if (IBSTempty(ibst)) 
        return;
    treePrintR(ibst->root, ibst->z, strategy);
}