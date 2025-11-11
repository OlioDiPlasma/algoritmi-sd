#include "BST.h" // Include le definizioni dei tipi (Item, Key, BST)

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct BSTnode* link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO: ora include puntatore al genitore e dimensione sottoalbero
struct BSTnode {
    Item item;  // Il dato
    link p;     // Puntatore al genitore (NUOVO)
    link l;     // Figlio sinistro
    link r;     // Figlio destro
    int N;      // Numero di nodi nel sottoalbero radicato qui (NUOVO)
};

// Struttura "wrapper" (contenitore) dell'albero
struct binarysearchtree { 
    link root; // Puntatore alla radice
    link z;    // Puntatore al nodo sentinella (per il NULL)
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo
static link NEW(Item item, link p, link l, link r, int N){
    link x = malloc(sizeof *x); // Alloca memoria

    x->item = item; // Imposta il dato
    x->p = p;       // Imposta il genitore
    x->l = l;       // Imposta il figlio sinistro
    x->r = r;       // Imposta il figlio destro
    x->N = N;       // Imposta la dimensione del sottoalbero (di solito 1 per nuovi nodi)

    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'BSTinit': inizializza un albero vuoto
BST BSTinit( ) {
    BST bst = malloc(sizeof *bst) ; // Alloca il wrapper

    // Crea il nodo sentinella 'z'. La radice (root) punta a 'z'.
    // 'z' ha 0 nodi (N=0) e punta a NULL (o a se stesso, ma NULL è più sicuro qui).
    bst->root = (bst->z = NEW(ITEMsetNull(), NULL, NULL, NULL, 0));

    return bst; // Ritorna il puntatore all'albero
}

// --- FUNZIONI DI INSERIMENTO ---

// Inserimento in foglia (Iterativo)
void BSTinsert_leafI(BST bst, Item x) {
    link p = bst->root, h = p; // 'p' (genitore), 'h' (nodo corrente)

    // Caso albero vuoto
    if (bst->root == bst->z) {
        // La radice diventa il nuovo nodo (N=1, genitore 'z')
        bst->root = NEW(x, bst->z, bst->z, bst->z, 1);
        return;
    }
    
    // Scende nell'albero per trovare la posizione
    while (h != bst->z) {
        p = h;       // 'p' insegue 'h'
        h->N++;      // IMPORTANTE: incrementa il contatore N di ogni nodo attraversato
        h = (KEYcmp(KEYget(x),KEYget(h->item)) == -1) ? h->l : h->r;
    }
    
    // 'h' è ora 'z', 'p' è il genitore a cui agganciare il nuovo nodo
    h = NEW(x, p, bst->z, bst->z, 1); // Nuovo nodo (N=1, genitore 'p')
    
    // Aggancia il nuovo nodo a 'p'
    if (KEYcmp(KEYget(x), KEYget(p->item)) == -1)
        p->l = h;
    else
        p->r = h;
}

// Inserimento in foglia (Ricorsivo - helper)
link insertR(link h, Item x, link z) {
    // Caso base: trovata posizione (un puntatore 'z')
    if (h == z)
        return NEW(x, z, z, z, 1); // Ritorna il nuovo nodo (N=1)
    
    // Passo ricorsivo
    if (KEYcmp(KEYget(x), KEYget(h->item)) == -1) {
        h->l = insertR(h->l, x, z); // Inserisci a sinistra
        h->l->p = h;                // Imposta il puntatore al genitore del figlio
    }
    else {
        h->r = insertR(h->r, x, z); // Inserisci a destra
        h->r->p = h;                // Imposta il puntatore al genitore del figlio
    }
    
    (h->N)++; // Aggiorna la dimensione del nodo corrente
    
    return h; // Ritorna il puntatore al nodo (modificato)
}

// Inserimento in foglia (Ricorsivo - wrapper)
void BSTinsert_leafR(BST bst, Item x) {
    bst->root = insertR(bst->root, x, bst->z);
}

// Inserimento in Radice (Ricorsivo - helper)
// Usa le rotazioni per portare il nuovo nodo in radice
link insertT(link h, Item x, link z) {
    // Caso base
    if (h == z)
        return NEW(x, z, z, z, 1);
        
    if (KEYcmp(KEYget(x), KEYget(h->item)) == -1) {
        h->l = insertT(h->l, x, z); // Inserisci a sinistra
        h = rotR(h);                // Ruota a destra (porta su il nodo sinistro)
        h->N++;                     // Aggiorna N (la rotazione aggiorna i figli)
    }
    else {
        h->r = insertT(h->r, x, z); // Inserisci a destra
        h = rotL(h);                // Ruota a sinistra (porta su il nodo destro)
        h->N++;                     // Aggiorna N
    }
    return h;
}

// Inserimento in Radice (wrapper)
void BSTinsert_root(BST bst, Item x) {
    bst->root = insertT(bst->root, x, bst->z);
}

// --- FUNZIONI DI CANCELLAZIONE ---

// Cancellazione (Ricorsiva - helper)
link deleteR(link h, Key k, link z) {
    link y, p; // y = nodo da liberare, p = genitore

    // Caso base: chiave non trovata
    if (h == z) 
        return z;
        
    // Cerca il nodo da eliminare
    if (KEYcmp(k, KEYget(h->item)) == -1)
        h->l = deleteR(h->l, k, z);
    if (KEYcmp(k, KEYget(h->item)) == 1)
        h->r = deleteR(h->r, k, z);
        
    (h->N)--; // Decrementa N *prima* di trovare il nodo
    
    // Trovato il nodo (chiavi uguali)
    if (KEYcmp(k, KEYget(h->item)) == 0) {
        y = h;       // Salva il puntatore al nodo da liberare
        p = h->p;    // Salva il puntatore al genitore
        h = joinLR(h->l, h->r, z); // Unisce i due sottoalberi (sinistro e destro)
        h->p = p;    // Ricollega il genitore al nuovo sottoalbero unito
        free(y);     // Libera la memoria del nodo
    }

    return h; // Ritorna il puntatore al (possibile nuovo) nodo 'h'
}

// Cancellazione (wrapper)
void BSTdelete(BST bst, Key k) {
    bst->root = deleteR(bst->root, k, bst->z);
}

// --- FUNZIONI BASATE SU 'N' (Dimensione) ---

// Selezione (Ricorsiva - helper): trova l'k-esimo elemento
// 'r' è il rango (rank) cercato (da 0 a N-1)
Item selectR(link h, int r, link z) {
    int t; // Dimensione del sottoalbero sinistro

    // Caso base: non trovato
    if (h == z)
        return ITEMsetNull();
        
    // 't' è il numero di nodi nel sottoalbero sinistro
    t = h->l->N; 
    
    // Se r è minore di t, l'elemento è nel sottoalbero sinistro
    if (t > r)
        return selectR(h->l, r, z);
        
    // Se r è maggiore di t, l'elemento è nel sottoalbero destro
    if (t < r)
        // Cerca a destra, ma con un rango aggiornato (r-t-1)
        // (togli i nodi di sinistra 't' e il nodo corrente '1')
        return selectR(h->r, r-t-1, z);
    
    // Se t == r, allora il nodo corrente 'h' è l'elemento cercato
    return h->item;
}

// Selezione (wrapper)
Item BSTselect(BST bst, int r) {
    return selectR(bst->root, r, bst->z);
}

// --- FUNZIONI BASATE SU 'p' (Genitore) ---

// Cerca il SUCCESSORE di un nodo (il più piccolo dei più grandi)
Item searchSucc(link h, Key k, link z) {
    link p; // Puntatore al genitore

    // Caso base: non trovato
    if (h == z) 
        return ITEMsetNull();
        
    // Trovato il nodo con chiave 'k'
    if (KEYcmp(k, KEYget(h->item)) == 0) {
        // Caso 1: Ha un sottoalbero destro
        if (h->r != z) 
            // Il successore è il minimo nel sottoalbero destro
            return minR(h->r, z); // minR (non mostrata) fa la discesa a sinistra
        // Caso 2: Non ha un sottoalbero destro
        else {
            p = h->p; // Parti dal genitore
            // Risali finché non sei più un figlio destro
            while (p != z && h == p->r) {
                h = p; 
                p = p->p;
            }
            // Il successore è il primo genitore "da cui si è svoltato a sinistra"
            return p->item;
        }
    }
    // Continua la ricerca...
    if (KEYcmp(k, KEYget(h->item)) == -1)
        return searchSucc(h->l, k, z);
    
    return searchSucc(h->r, k, z);
}

// Successore (wrapper)
Item BSTsucc(BST bst, Key k) {
    return searchSucc(bst->root, k, bst->z);
}

// Cerca il PREDECESSORE (il più grande dei più piccoli)
Item searchPred(link h, Key k, link z) {
    link p;

    if (h == z) 
        return ITEMsetNull();
        
    // Trovato il nodo con chiave 'k'
    if (KEYcmp(k, KEYget(h->item)) == 0) {
        // Caso 1: Ha un sottoalbero sinistro
        if (h->l != z) 
            // Il predecessore è il massimo nel sottoalbero sinistro
            return maxR(h->l, z); // maxR (non mostrata) fa la discesa a destra
        // Caso 2: Non ha un sottoalbero sinistro
        else {
            p = h->p; // Parti dal genitore
            // Risali finché non sei più un figlio sinistro
            while (p != z && h == p->l) {
                h = p; 
                p = p->p;
            }
            // Il predecessore è il primo genitore "da cui si è svoltato a destra"
            return p->item;
        }
    }
    // Continua la ricerca...
    if (KEYcmp(k, KEYget(h->item)) == -1)
        return searchPred(h->l, k, z);
    return searchPred(h->r, k, z);
}

// Predecessore (wrapper)
Item BSTpred(BST bst, Key k) {
    return searchPred(bst->root, k, bst->z);
}

// --- ROTAZIONI E BILANCIAMENTO ---

// Rotazione a Sinistra (aggiornata per 'p' e 'N')
link rotL(link h) {
    link x = h->r; // 'x' è il figlio destro (nuova radice)

    h->r = x->l;  // Il sottoalbero "in mezzo" (x->l) diventa figlio destro di 'h'
    x->l->p = h;  // Aggiorna il genitore del sottoalbero "in mezzo"
    x->l = h;     // 'h' (vecchia radice) diventa figlio sinistro di 'x'
    x->p = h->p;  // Il genitore di 'x' è il vecchio genitore di 'h'
    h->p = x;     // 'h' ora ha 'x' come genitore
    
    // Aggiorna le dimensioni
    x->N = h->N; // 'x' prende la dimensione totale del sottoalbero (che 'h' aveva)
    // Ricalcola la dimensione di 'h' (ora è un figlio)
    h->N = 1; // 'h' stesso
    h->N += (h->l) ? h->l->N : 0; // Figlio sinistro di h
    h->N += (h->r) ? h->r->N : 0; // Figlio destro di h

    return x; // Ritorna la nuova radice del sottoalbero
}

// Rotazione a Destra (aggiornata per 'p' e 'N')
link rotR(link h) {
    link x = h->l; // 'x' è il figlio sinistro (nuova radice)

    h->l = x->r;  // Il sottoalbero "in mezzo" (x->r) diventa figlio sinistro di 'h'
    x->r->p = h;  // Aggiorna il genitore del sottoalbero "in mezzo"
    x->r = h;     // 'h' (vecchia radice) diventa figlio destro di 'x'
    x->p = h->p;  // Il genitore di 'x' è il vecchio genitore di 'h'
    h->p = x;     // 'h' ora ha 'x' come genitore
    
    // Aggiorna le dimensioni
    x->N = h->N; // 'x' prende la dimensione totale
    // Ricalcola la dimensione di 'h'
    h->N = 1; 
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;

    return x; // Ritorna la nuova radice
}

// Partizionamento (Ricorsivo - helper)
// Ruota il nodo con rango 'r' portandolo alla radice del sottoalbero 'h'
link partR(link h, int r) {
    int t = h->l->N; // Dimensione del sottoalbero sinistro

    // Se r è nel sottoalbero sinistro
    if (t > r) {
        h->l = partR(h->l, r); // Partiziona a sinistra
        h = rotR(h);           // Ruota a destra per portare l'elemento su
    }
    // Se r è nel sottoalbero destro
    if (t < r) {
        h->r = partR(h->r, r-t-1); // Partiziona a destra (con rango aggiornato)
        h = rotL(h);             // Ruota a sinistra per portare l'elemento su
    }
    // (Se t == r, 'h' è già il nodo giusto, non fa nulla)
    
    return h;
}

// Unione (Join) di due sottoalberi 'a' (sinistro) e 'b' (destro)
link joinLR(link a, link b, link z) {
    // Se il sottoalbero destro è vuoto, ritorna solo 'a'
    if (b == z)
        return a;
        
    // Trova l'elemento minimo (rango 0) nel sottoalbero 'b'
    // e portalo in radice di 'b'
    b = partR(b, 0); 
    
    // 'a' (che contiene tutti elementi < di 'b') 
    // diventa il figlio sinistro della nuova radice di 'b'
    b->l = a; 
    
    // Aggiorna il genitore di 'a'
    a->p = b;
    
    // Aggiorna la dimensione di 'b'
    b->N = a->N + b->r->N + 1;
    
    return b; // Ritorna la radice del nuovo albero unito
}

// Bilanciamento (Ricorsivo - helper)
// Rende l'albero 'h' (quasi) bilanciato
static link balanceR(link h, link z) {
    int r;

    if (h == z)
        return z;
        
    // 'r' è il rango della mediana (elemento centrale)
    r = (h->N+1)/2 - 1; 
    
    // Porta la mediana 'r' alla radice di 'h'
    h = partR(h, r); 
    
    // Bilancia ricorsivamente il sottoalbero sinistro
    h->l = balanceR(h->l, z);
    
    // Bilancia ricorsivamente il sottoalbero destro
    h->r = balanceR(h->r, z);
    
    return h; // Ritorna la radice bilanciata
}

// Bilanciamento (wrapper)
void BSTbalance(BST bst) {
    bst->root = balanceR(bst->root, bst->z);
}