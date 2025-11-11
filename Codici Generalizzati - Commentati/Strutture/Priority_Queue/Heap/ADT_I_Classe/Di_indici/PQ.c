// Include il file header che definisce i tipi (PQ, Item) e i prototipi.
// (In questo file, Item è 'heapItem', ma l'interfaccia PQ.h
// potrebbe nasconderlo).
#include "PQ.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

// Struttura interna per l'elemento dell'heap.
// Non memorizza l'Item completo, ma solo il suo ID ('index')
// e la sua priorità ('prio').
typedef struct {
    int index; // L'identificatore univoco dell'elemento
    int prio;  // Il valore di priorità (per un Max-Heap)
} heapItem;

// Struttura "wrapper" (contenitore) della coda
struct pqueue {
    heapItem *A;  // L'array che memorizza l'heap
    int heapsize; // Numero di elementi attuali
    int *qp;      // L'array "inverso" o "di posizione"
};

// --- FUNZIONI HELPER (statiche, non mostrate in PQ.h) ---
// (Assumiamo che LEFT, RIGHT, PARENT siano definite altrove o tramite macro)
// static int LEFT(int i) { return (i*2 + 1); }
// static int RIGHT(int i) { return (i*2 + 2); }
// static int PARENT(int i) { return ((i-1)/2); }


// --- FUNZIONI PUBBLICHE ---

// Inizializza (alloca) una nuova coda vuota
PQ PQinit(int maxN) {
    int i; 

    // Alloca la struttura wrapper
    PQ pq = malloc(sizeof(*pq));

    // Alloca l'array per l'heap (di 'heapItem')
    pq->A = malloc(maxN*sizeof(heapItem));
    // Alloca l'array inverso per le posizioni
    pq->qp = malloc(maxN*sizeof(int));
    
    // Inizializza l'array inverso
    for (i = 0; i < maxN; i++){
        // Inizializza l'heap (non strettamente necessario, ma pulito)
        pq->A[i].index = -1; 
        // Inizializza qp: -1 significa "questo elemento (i) non è nell'heap"
        pq->qp[i] = -1; 
    }
    // L'heap è vuoto
    pq->heapsize = 0;

    return pq;
}

// Libera tutta la memoria
void PQfree(PQ pq) {
    free(pq->qp); // Libera l'array inverso
    free(pq->A);  // Libera l'array heap
    free(pq);     // Libera il wrapper
}

// Controlla se la coda è vuota
int PQempty(PQ pq) { 
    return pq->heapsize == 0; 
}

// Ritorna il numero di elementi
int PQsize(PQ pq) { 
    return pq->heapsize; 
}

// Inserisce un elemento (identificato da 'index') con priorità 'prio'
void PQinsert(PQ pq, int index, int prio) {
    int i; // Indice della posizione corrente (dove inserire)

    // Prendi la prima posizione libera (in fondo) e incrementa la dimensione
    i = pq->heapsize++; 
    
    // Loop "Swim" (o "Heap-Up"): fa "galleggiare" il nuovo elemento
    // Finché non siamo alla radice (i >= 1) E
    // la priorità del genitore è MINORE di quella nuova...
    while((i >= 1) && (pq->A[PARENT(i)].prio < prio)) {
        // ...sposta il genitore in basso (alla posizione 'i')
        pq->A[i] = pq->A[PARENT(i)];
        
        // AGGIORNAMENTO CHIAVE:
        // Di' all'array 'qp' che l'elemento che era genitore
        // (pq->A[i].index) ora si trova alla posizione 'i'.
        pq->qp[pq->A[i].index] = i;
        
        // Sali alla posizione del genitore per il prossimo ciclo
        i = PARENT(i);
    }
    
    // Trovata la posizione corretta 'i', inserisci il nuovo elemento
    pq->A[i].index = index;
    pq->A[i].prio = prio;
    
    // AGGIORNAMENTO CHIAVE:
    // Di' all'array 'qp' che l'elemento 'index' si trova ora alla posizione 'i'
    pq->qp[index] = i;
}

// Funzione 'Swap' (helper) - scambia due elementi nell'heap
static void Swap(PQ pq, int pos1, int pos2) {
    heapItem temp; // Variabile temporanea per lo scambio
    int index1, index2; // Variabili per gli ID degli elementi

    // Scambia gli 'heapItem' nell'array A
    temp = pq->A[pos1];
    pq->A[pos1] = pq->A[pos2];
    pq->A[pos2] = temp;
    
    // Estrai gli ID degli elementi che ora sono in pos1 e pos2
    index1 = pq->A[pos1].index;
    index2 = pq->A[pos2].index;
    
    // AGGIORNAMENTO CHIAVE 'qp':
    // Di' a 'qp' che l'elemento 'index1' ora è in 'pos1'
    pq->qp[index1] = pos1;
    // Di' a 'qp' che l'elemento 'index2' ora è in 'pos2'
    pq->qp[index2] = pos2;
}

// Funzione 'HEAPify' (helper) - ripristina la proprietà di heap (Max-Heap)
// Facendo "affondare" (sink) l'elemento alla posizione 'i'
static void HEAPify(PQ pq, int i) {
    int l, r, largest; // Indici: sinistro, destro, massimo

    l = LEFT(i); // Calcola figlio sinistro
    r = RIGHT(i); // Calcola figlio destro
    
    // Controlla se il figlio sinistro esiste (l < heapsize) E
    // se ha priorità maggiore del nodo corrente 'i'
    if (l < pq->heapsize && (pq->A[l].prio > pq->A[i].prio))
        largest = l; // Il sinistro è (per ora) il massimo
    else
        largest = i; // 'i' è (per ora) il massimo
        
    // Controlla se il figlio destro esiste (r < heapsize) E
    // se ha priorità maggiore del *massimo trovato finora* ('largest')
    if (r < pq->heapsize && (pq->A[r].prio > pq->A[largest].prio))
        largest = r; // Il destro è il nuovo massimo
        
    // Se 'i' non è il massimo (cioè uno dei figli è più grande)
    if (largest != i) {
        // Scambia 'i' con il figlio 'largest'
        // (Swap aggiorna automaticamente anche 'qp')
        Swap(pq, i, largest);
        
        // Chiamata ricorsiva: continua a far affondare l'elemento
        // (che ora si trova alla posizione 'largest')
        HEAPify(pq, largest);
    }
}

// Estrae (rimuove e ritorna) l'ID ('index') dell'elemento con priorità massima
int PQextractMax(PQ pq) {
    int res; // Variabile per salvare l'ID (index) del massimo

    // Scambia la radice (max, pos 0) con l'ultimo elemento (pos heapsize-1)
    Swap(pq, 0, pq->heapsize-1);
    
    // Salva l'ID dell'elemento massimo (che ora è in fondo)
    res = pq->A[pq->heapsize-1].index;
    
    // AGGIORNAMENTO CHIAVE 'qp':
    // L'elemento 'res' è stato rimosso. Segnalo in 'qp' che
    // non si trova più da nessuna parte nell'heap (-1).
    pq->qp[res] = -1;
    
    // Riduci la dimensione (l'elemento è "virtualmente" rimosso)
    pq->heapsize--;
    
    // Pulisci l'ID nell'array 'A' (buona prassi, non essenziale)
    pq->A[pq->heapsize].index = -1;
    
    // Ripristina la proprietà di heap: fai "affondare" il
    // (ex ultimo) elemento che ora è in radice (pos 0)
    HEAPify(pq, 0);

    // Ritorna l'ID dell'elemento che era il massimo
    return res;
}

// Cambia la priorità di un elemento (identificato da 'index')
// Questa è l'operazione (spesso chiamata 'decrease-key' o 'increase-key')
// che beneficia maggiormente dell'array 'qp'.
void PQchange(PQ pq, int index, int prio) {
    // TROVA L'ELEMENTO IN O(1):
    // Chiedi a 'qp' in quale posizione 'pos' si trova l'elemento 'index'
    int pos = pq->qp[index]; 
    
    // Crea una copia dell'elemento (non strettamente necessario, ma pulito)
    heapItem temp = pq->A[pos];
    
    // Aggiorna la priorità nella copia
    temp.prio = prio;
    
    // CASO 1: La priorità è AUMENTATA
    // Prova a far "galleggiare" (Swim) l'elemento verso l'alto
    // NOTA: c'è un bug nel codice, confronta .index con prio.
    // La riga CORRETTA dovrebbe essere:
    // while ((pos >= 1) && (pq->A[PARENT(pos)].prio < prio)) {
    while ((pos >= 1) && (pq->A[PARENT(pos)].prio < prio)) { // Correggo il bug
        // ...sposta il genitore in basso
        pq->A[pos] = pq->A[PARENT(pos)];
        // ...aggiorna 'qp' per il genitore spostato
        pq->qp[pq->A[pos].index] = pos;
        // ...sali di un livello
        pos = PARENT(pos);
    }
    
    // Inserisci l'elemento (con la nuova priorità) nella posizione 'pos' trovata
    pq->A[pos] = temp;
    // Aggiorna 'qp' per l'elemento 'index'
    pq->qp[index] = pos;
    
    // CASO 2: La priorità è DIMINUITA (o rimasta uguale)
    // Se il loop 'while' non è stato eseguito (perché la priorità
    // è diminuita), 'pos' è ancora la posizione originale.
    // 'HEAPify' (sink) farà "affondare" l'elemento fino alla posizione corretta.
    // Se il loop 'while' *è* stato eseguito, 'pos' è una nuova
    // posizione (più vicina alla radice) e HEAPify(pq, pos)
    // non farà nulla (perché l'elemento è già > dei figli), il che è corretto.
    HEAPify(pq, pos);
}