//////////////// LISTA
// --- 1. IL MATTONE (Il nodo nascosto) ---
typedef struct listNode *link;
struct listNode {
    int val;
    link next;
};

// --- 2. IL WRAPPER (L'amministratore) ---
typedef struct list *List;
struct list {
    link head;
    link tail; // Opzionale: comodissimo per l'inserimento in coda!
    int N;     // Contatore dei nodi (così non devi mai fare un while per contarli)
};

// --- 3. INIZIALIZZAZIONE ---
List LISTinit() {
    List L = malloc(sizeof(*L));
    L->head = NULL;
    L->tail = NULL;
    L->N = 0;
    return L;
}


////////////////BST
// --- 1. IL MATTONE (Il nodo dell'albero) ---
typedef struct BSTnode *link;
struct BSTnode {
    int val;
    link l; // Figlio sinistro
    link r; // Figlio destro
};

// --- 2. IL WRAPPER (La radice e la sentinella) ---
typedef struct bst *BST;
struct bst {
    link root; // Punta alla cima dell'albero
    link z;    // Il nodo Sentinella (sostituisce i NULL)
    int N;     // Numero totale di nodi
};

// --- 3. INIZIALIZZAZIONE ---
BST BSTinit() {
    BST b = malloc(sizeof(*b));
    
    // Inizializzo la sentinella (punta a se stessa e non ha valori reali)
    b->z = malloc(sizeof(*b->z));
    b->z->l = b->z; 
    b->z->r = b->z;
    
    // All'inizio, l'albero è vuoto, quindi la radice E' la sentinella
    b->root = b->z;
    b->N = 0;
    return b;
}


////////////////GRAFO
// --- 1. IL MATTONE (L'arco/vicino nella lista) ---
typedef struct edgeNode *linkEdge;
struct edgeNode {
    int v;         // Il nodo di destinazione
    int wt;        // (Opzionale) Il peso dell'arco
    linkEdge next; // Prossimo vicino nella lista
};

// --- 2. IL WRAPPER (Il "Casellario Postale" completo) ---
typedef struct graph *Graph;
struct graph {
    int V;          // Numero di Vertici (Nodi)
    int E;          // Numero di Archi (Edges)
    linkEdge *ladj; // Array di liste (Il pettine!)
};

typedef struct graph *Graph;
struct graph {
    int V;
    int E;
    int **adj; // Un doppio puntatore! Sarà una matrice V x V
};

// --- 3. INIZIALIZZAZIONE ---
Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    
    // Alloco l'array che farà da "asse del pettine"
    G->ladj = malloc(V * sizeof(linkEdge));
    
    // Inizializzo tutte le liste a NULL (all'inizio non ci sono archi)
    for (int i = 0; i < V; i++) {
        G->ladj[i] = NULL;
    }
    
    return G;
}



//////////////////////////////  PQUEUE
// --- IL WRAPPER (Gestisce l'array dinamico) ---
typedef struct pq *PQ;
struct pq {
    int *A;     // L'array che conterrà gli elementi (spesso si parte dall'indice 1)
    int N;      // Quanti elementi ci sono attualmente dentro
    int maxN;   // La capacità massima dell'array (per evitare overflow)
};

// --- INIZIALIZZAZIONE ---
PQ PQinit(int maxN) {
    PQ pq = malloc(sizeof(*pq));
    // Alloco l'array di N+1 elementi (l'indice 0 di solito si lascia vuoto per comodità matematica)
    pq->A = malloc((maxN + 1) * sizeof(int)); 
    pq->N = 0;
    pq->maxN = maxN;
    return pq;
}

///////////////////////////////////77hash table
// --- 1. IL MATTONE (Il nodo della lista per le collisioni) ---
typedef struct STnode *linkST;
struct STnode {
    char *key;   // La stringa da cercare
    int val;     // Il dato associato
    linkST next; 
};

// --- 2. IL WRAPPER (Il vettore di cassetti) ---
typedef struct st *ST;
struct st {
    linkST *heads; // Un array di "teste" di liste
    int M;         // Il numero di cassetti (dimensione dell'array)
    int N;         // Quanti elementi in totale ho inserito finora
};

// --- 3. INIZIALIZZAZIONE ---
ST STinit(int M) {
    ST st = malloc(sizeof(*st));
    st->M = M;
    st->N = 0;
    
    // Alloco l'array di teste e le metto tutte a NULL usando calloc
    st->heads = calloc(M, sizeof(linkST));
    return st;
}


////////////calcolo

 /* ---------------------------------------------------------------------------------
 * SEZIONE 00: Mini calcolo combinatorio
 * --------------------------------------------------------------------------------- */


 // PERMUTAZIONI (es. Anagrammi, Cammino Hamiltoniano) L'ordine conta? SÌ. Posso ripetere? NO
void permuta(int pos, int n, int *sol, int *mark, ... ) {
    if (pos >= n) { /* Soluzione trovata */ return; }

    for (int i = 0; i < n; i++) {
        if (mark[i] == 0) { // NON l'ho ancora usato
            mark[i] = 1;
            sol[pos] = i; // Prendo l'elemento i
            permuta(pos + 1, n, sol, mark, ...);
            mark[i] = 0; // Backtrack
        }
    }
}



// DISPOSIZIONI (es. Numeri in Base B) L'ordine conta? SÌ. Posso ripetere? SÌ
void disposizioni(int pos, int cifreDaRiempire, int Base, int *sol) {
    if (pos >= cifreDaRiempire) { /* Stampo numero */ return; }

    for (int i = 0; i < Base; i++) {
        // Nessun check su mark! Posso riusare 'i' quante volte voglio
        sol[pos] = i;
        disposizioni(pos + 1, cifreDaRiempire, Base, sol);
    }
}

// COMBINAZIONI (es. Sottoinsiemi, Subset Sum) L'ordine conta? NO. Posso ripetere? NO
// start: indice da cui iniziare a pescare nel vettore originale
void combinazioni(int pos, int start, int n, int *sol, int k) {
    if (pos >= k) { /* Ho scelto k elementi */ return; }

    for (int i = start; i < n; i++) {
        sol[pos] = i;
        // Passo i + 1 alla ricorsione per prendere solo i successivi
        combinazioni(pos + 1, i + 1, n, sol, k);
    }
}


// PARTIZIONI (es. Minibus, Assegnamento) Devo dividere TUTTI gli oggetti in GRUPPI?
// pos: indice del passeggero corrente
void partizioni(int pos, int n_passeggeri, int k_bus, int *assegnamenti) {
    if (pos >= n_passeggeri) { /* Tutti assegnati */ return; }

    for (int bus = 0; bus < k_bus; bus++) {
        assegnamenti[pos] = bus; // Metto passeggero 'pos' sul 'bus'
        partizioni(pos + 1, n_passeggeri, k_bus, assegnamenti);
    }
}
