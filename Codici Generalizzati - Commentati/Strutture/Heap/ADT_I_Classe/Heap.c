// Include il file header che definisce i tipi (Heap, Item) e i prototipi
#include "Heap.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della struttura Heap
struct heap {
    Item *A;        // Puntatore all'array che conterrà gli elementi
    int heapsize;   // Il numero di elementi *attualmente* nell'array
};

// --- FUNZIONI HELPER (PRIVATE) ---
// 'static' significa che queste funzioni sono visibili solo all'interno di questo file .c

// Calcola l'indice del figlio SINISTRO di 'i' (per array 0-based)
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

// --- FUNZIONI PUBBLICHE (definite in Heap.h) ---

// Inizializza (alloca) una nuova struttura Heap vuota
Heap HEAPinit(int maxN) {
    Heap h; // Dichiara il puntatore all'heap

    // Alloca la memoria per la struttura "wrapper" (il contenitore)
    h = malloc(sizeof(*h));
    // Alloca la memoria per l'array 'A' (che conterrà 'maxN' Item)
    h->A = malloc(maxN*sizeof(Item));
    // Inizializza la dimensione a 0 (l'heap è vuoto)
    h->heapsize = 0;

    // Ritorna il puntatore all'heap inizializzato
    return h;
}

// Libera tutta la memoria usata dall'heap
void HEAPfree(Heap h) {
    free(h->A); // 1. Libera l'array interno
    free(h);    // 2. Libera la struttura wrapper
}

// Riempie l'heap: aggiunge un elemento in fondo all'array
// NOTA: Questa funzione NON mantiene la proprietà di heap.
// Aggiunge semplicemente l'elemento, "rompendo" l'heap.
void HEAPfill(Heap h, Item item) {
    int i; // Indice dove inserire

    // 'i' prende il valore attuale di heapsize (es. 0 se è il primo)
    // poi 'heapsize' viene incrementato (es. diventa 1)
    i = h->heapsize++; 
    
    // Inserisce l'item nella prima posizione libera in fondo all'array
    h->A[i] = item;

    return; // Funzione void
}

// Ordina l'array interno usando l'algoritmo Heapsort
void HEAPsort(Heap h) {
    int i, j; // Variabili di ciclo

    // PASSO 1: Costruisci un Max-Heap
    // Trasforma l'array (riempito in disordine da HEAPfill)
    // in un heap valido, dove A[0] è il massimo.
    HEAPbuild(h);
    
    // Salva la dimensione originale dell'array
    j = h->heapsize; 
    
    // PASSO 2: Ordina (Sort-down)
    // Cicla dall'ultimo elemento (heapsize-1) fino al secondo (1)
    for (i = h->heapsize-1; i > 0; i--) {
        // Scambia il massimo (A[0]) con l'ultimo elemento (A[i])
        // Ora A[i] è nel suo posto definitivo (è ordinato)
        Swap(h, 0, i); // 'Swap' è una funzione helper non mostrata
        
        // "Riduci" l'heap: l'elemento in A[i] è ora "fuori" dall'heap
        h->heapsize--;
        
        // L'elemento scambiato in A[0] è fuori posto.
        // Ripristina la proprietà di heap (facendolo "affondare")
        // sull'heap ridotto.
        HEAPify(h, 0);
    }
    // Alla fine del loop, l'array A è ordinato in modo crescente,
    // ma h->heapsize è 1 (o 0).
    
    // Ripristina la dimensione dell'heap a quella originale
    // in modo che HEAPdisplay possa stampare l'array ordinato.
    h->heapsize = j;
}

// Stampa il contenuto dell'array A
void HEAPdisplay(Heap h) {
    int i;
    
    // Scorre l'array e stampa ogni elemento
    // (Se HEAPsort è stato chiamato, stamperà in ordine)
    for (i = 0; i < h->heapsize; i++)
        ITEMstore(h->A[i]); // 'ITEMstore' (non mostrata) stampa un Item
}

// Ripristina la proprietà di Max-Heap per il sottoalbero in 'i'
// (Facendo "affondare" l'elemento 'i' - "Sink" o "Heap-Down")
// Presuppone che i figli (LEFT(i) e RIGHT(i)) siano già heap validi.
void HEAPify(Heap h, int i) {
    int l, r, largest; // Indici: sinistro, destro, il più grande

    l = LEFT(i); // Indice figlio sinistro
    r = RIGHT(i); // Indice figlio destro
    
    // Cerca il 'largest' tra 'i', 'l', 'r'
    
    // 1. Confronta 'i' (genitore) e 'l' (sinistro)
    // Se 'l' esiste (l < heapsize) E A[l] > A[i] (KEYcmp == 1)
    if ((l < h->heapsize) && KEYcmp(KEYget(h->A[l]), KEYget(h->A[i])) == 1)
        largest = l; // Il sinistro è il più grande
    else
        largest = i; // Il genitore è (per ora) il più grande
        
    // 2. Confronta 'largest' (che sia 'i' o 'l') e 'r' (destro)
    // Se 'r' esiste (r < heapsize) E A[r] > A[largest]
    if ((r < h->heapsize) && KEYcmp(KEYget(h->A[r]), KEYget(h->A[largest])) == 1)
        largest = r; // Il destro è il più grande di tutti
        
    // Se 'i' (il genitore) NON era il più grande...
    if (largest != i) {
        // ...scambia il genitore 'i' con il figlio 'largest'
        Swap(h, i, largest); // 'Swap' non mostrata
        
        // Chiamata ricorsiva:
        // L'elemento (ex 'i') è ora in posizione 'largest'.
        // Continua a far affondare l'elemento da quella nuova posizione.
        HEAPify(h, largest);
    }
    // Se largest == i, la proprietà di heap è rispettata e la ricorsione si ferma.
}

// Costruisce un Max-Heap da un array disordinato (Bottom-Up)
void HEAPbuild(Heap h) {
    int i;

    // Inizia dall'ultimo NODO GENITORE
    // (tutti gli elementi dopo (heapsize/2)-1 sono foglie,
    // e le foglie sono heap validi per definizione).
    for (i = (h->heapsize)/2 - 1; i >= 0; i--)
        // Applica HEAPify a ogni genitore, risalendo fino alla radice (0)
        HEAPify(h, i);
}