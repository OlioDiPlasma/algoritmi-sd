// Include il file header che definisce i tipi (PQ, Item)
// e i prototipi delle funzioni pubbliche.
#include "PQ.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della struttura Priority Queue
struct pqueue { 
    Item *A;        // Puntatore a un array (allocato dinamicamente) che conterrà gli Item
    int heapsize;   // Numero di elementi *attualmente* presenti nell'heap
};

// --- FUNZIONI HELPER (PRIVATE) ---
// 'static' significa che queste funzioni sono visibili solo all'interno di questo file .c

// Calcola l'indice del figlio SINISTRO di 'i'
// Usa la formula per heap con indice 0 (0-based)
static int LEFT(int i) { 
    return (i*2 + 1); 
}

// Calcola l'indice del figlio DESTRO di 'i'
static int RIGHT(int i) { 
    return (i*2 + 2); 
}

// Calcola l'indice del GENITORE di 'i'
static int PARENT(int i) { 
    return ((i-1)/2); 
}

// --- FUNZIONI PUBBLICHE (definite in PQ.h) ---

// Inizializza (alloca) una nuova coda con priorità vuota
// 'maxN' è la capacità massima della coda
PQ PQinit(int maxN) {
    // Alloca memoria per la struttura "wrapper" (il contenitore)
    PQ pq = malloc(sizeof(*pq));
    // Alloca memoria per l'array 'A' che conterrà gli elementi
    pq->A = (Item*) malloc(maxN*sizeof(Item));
    // All'inizio, la coda è vuota (dimensione 0)
    pq->heapsize = 0;
    // Ritorna il puntatore alla coda appena creata
    return pq;
}

// Libera tutta la memoria usata dalla coda
void PQfree(PQ pq) {
    // Prima libera l'array interno
    free(pq->A);
    // Poi libera la struttura wrapper
    free(pq);
}

// Controlla se la coda è vuota
int PQempty(PQ pq) { 
    // Ritorna 1 (vero) se la dimensione è 0, altrimenti 0 (falso)
    return pq->heapsize == 0; 
}

// Ritorna il numero di elementi presenti nella coda
int PQsize(PQ pq) { 
    return pq->heapsize; 
}

// Inserisce un nuovo elemento 'val' nella coda, mantenendo la proprietà di max-heap
void PQinsert(PQ pq, Item val) {
    int i; // Indice del nuovo elemento

    // 'i' parte dalla prima posizione libera (la fine dell'array)
    // pq->heapsize++ prima incrementa 'heapsize', poi usa il vecchio valore
    // NO, ATTENZIONE: i = pq->heapsize++ (post-incremento)
    // 1. Assegna 'i' al valore *attuale* di heapsize
    // 2. Incrementa heapsize
    i = pq->heapsize++; 
    
    // Loop "Swim" o "Heap-Up": fa "galleggiare" il nuovo elemento
    // Finché 'i' non è la radice (i >= 1) E il genitore è PIÙ PICCOLO del nuovo valore...
    // (KEYcmp ritorna -1 se il primo < del secondo -> Max-Heap)
    while((i >= 1) && (KEYcmp(KEYget(pq->A[PARENT(i)]), KEYget(val)) == -1)) {
        // ...sposta il genitore *giù* nella posizione di 'i'
        pq->A[i] = pq->A[PARENT(i)];
        // 'i' si sposta *su* alla posizione del genitore
        i = (i-1)/2; // (Equivalente a i = PARENT(i))
    }
    // Trovata la posizione corretta, inserisci il 'val'
    pq->A[i] = val;
    
    return; // Funzione void
}

// Estrae (rimuove e ritorna) l'elemento con priorità massima (la radice)
Item PQextractMax(PQ pq) {
    Item val; // Variabile per salvare l'elemento massimo
    
    // 'Swap' (funzione non mostrata) scambia la radice (indice 0)
    // con l'ultimo elemento (indice heapsize-1)
    Swap(pq, 0, pq->heapsize-1);
    
    // Salva l'elemento massimo (che ora è alla fine)
    val = pq->A[pq->heapsize-1];
    
    // "Elimina" l'elemento riducendo la dimensione dell'heap
    pq->heapsize--;
    
    // 'HEAPify' (funzione non mostrata, "Sink" o "Heap-Down")
    // Ripristina la proprietà di heap partendo dalla radice (0),
    // facendo "affondare" l'elemento (ex ultimo) che è stato messo in cima.
    HEAPify(pq, 0);
    
    // Ritorna l'elemento massimo che avevamo salvato
    return val;
}

// Ritorna l'elemento massimo *senza* estrarlo
Item PQshowMax(PQ pq) { 
    // In un max-heap, l'elemento massimo è sempre la radice (A[0])
    return pq->A[0]; 
}

// Stampa tutti gli elementi nell'array (per debug)
// NOTA: Questo NON stampa in ordine di priorità, ma solo l'array
void PQdisplay(PQ pq) {
    int i;
    // Scorre tutti gli elementi *validi* dell'heap
    for (i = 0; i < pq->heapsize; i++)
        // 'ITEMstore' (non mostrata) stampa il singolo Item
        ITEMstore(pq->A[i]);
}

// Cambia la priorità di un elemento (identificato da 'val')
// Questa è un'implementazione (delle due nel .h)
void PQchange(PQ pq, Item val) {
    int i, found = 0, pos; // Variabili per la ricerca

    // 1. Trova l'elemento: Ricerca Lineare (O(N))
    // (Presuppone che l'Item 'val' abbia un NOME/ID)
    // 'NAMEcmp' (non mostrata) confronta l'identità, non la priorità
    for (i = 0; i < pq->heapsize && found == 0; i++)
        if (NAMEcmp(NAMEget(&(pq->A[i])), NAMEget(&val)) == 0) {
            found = 1;
            pos = i; // Trovato l'elemento alla posizione 'pos'
        }
        
    if (found == 1) {
        // 2. Aggiorna la priorità (caso: priorità AUMENTATA)
        // 'val' contiene la *nuova* priorità
        // 'PRIOget' (non mostrata) estrae il valore di priorità
        
        // Loop "Swim" (come in PQinsert):
        // Se la nuova priorità 'val' è maggiore del genitore...
        while(pos >= 1 && PRIOget(pq->A[PARENT(pos)]) < PRIOget(val)) {
            // ...sposta il genitore in basso
            pq->A[pos] = pq->A[PARENT(pos)];
            pos = (pos-1)/2; // e sali
        }
        // Inserisci il nuovo 'val' nella posizione 'pos' trovata
        pq->A[pos] = val;
        
        // 3. Aggiorna la priorità (caso: priorità DIMINUITA)
        // 'Heapify' (non mostrata) fa "affondare" (sink) l'elemento.
        // Se il loop "Swim" *non* è stato eseguito (perché la priorità
        // è diminuita o rimasta uguale), 'pos' è la posizione originale
        // e 'Heapify' correggerà l'ordine verso il basso.
        // Se il loop "Swim" *è* stato eseguito, 'pos' è in alto
        // e 'Heapify' non farà nulla (corretto).
        Heapify(pq, pos);
    } else
        // L'elemento da aggiornare non è stato trovato nell'heap
        printf("key not found!\n");
    
    return;
}