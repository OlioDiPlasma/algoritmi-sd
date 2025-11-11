// Include l'header file che definisce i tipi (ST, Item, Key) e i prototipi
#include "ST.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della Symbol Table
struct symbtab {
    Item *a;    // Puntatore all'array (allocato dinamicamente)
    int maxN;   // La capacità *massima* (dimensione allocata) dell'array
    int size;   // Il numero di elementi *attualmente* presenti
};

// --- FUNZIONI PUBBLICHE ---

// Inizializza (alloca) la Symbol Table
ST STinit(int maxN) {
    ST st; 
    int i;
    
    // Alloca la memoria per la struttura "wrapper"
    st = malloc(sizeof(*st));
    // Alloca la memoria per l'array interno di 'maxN' elementi
    st->a = malloc(maxN * sizeof(Item));
    
    // (Operazione non strettamente necessaria per questa implementazione,
    // ma usata in altre, come le hash table)
    // Inizializza tutte le celle dell'array a "vuoto"
    for (i = 0; i < maxN; i++)
        st->a[i] = ITEMsetvoid(); // ITEMsetvoid() non mostrata
        
    // Salva la capacità massima
    st->maxN = maxN;
    // Inizializza il numero di elementi a 0
    st->size = 0;

    return st; // Ritorna il puntatore alla ST
}

// Libera tutta la memoria usata dalla ST
void STfree(ST st) {
    free(st->a); // 1. Libera l'array interno
    free(st);    // 2. Libera la struttura wrapper
}

// Ritorna il numero di elementi (operazione O(1))
int STcount(ST st) {
    return st->size; 
}

// Inserisce un nuovo elemento in coda (Array non ordinato)
// Operazione O(1) ammortizzata (O(N) quando rialloca)
void STinsert(ST st, Item val) {
    // 'i' è l'indice della prima cella libera (cioè la fine)
    int i = st->size;

    // Controlla se l'array è pieno
    if (st->size >= st->maxN) {
        // È pieno: RADDOPPIA la capacità dell'array
        st->a = realloc(st->a, (2*st->maxN)*sizeof(Item));
        // Controlla se realloc è fallito (es. memoria esaurita)
        if (st->a == NULL) return; 
        // Aggiorna la capacità massima
        st->maxN = 2*st->maxN;
    }
    
    // Inserisce l'elemento in coda (nella posizione 'i')
    st->a[i] = val; 
    // Incrementa il contatore degli elementi
    st->size++;
}

// Cerca un elemento data la chiave 'k' (Ricerca Lineare O(N))
Item STsearch(ST st, Key k) {
    int i; // Indice per scorrere

    // Se la tabella è vuota, non cercare
    if (st->size == 0) 
        return ITEMsetvoid();
        
    // Scorre tutti gli elementi *validi* (da 0 a size-1)
    for (i = 0; i < st->size; i++)
        // 'KEYcmp' (non mostrata) confronta le chiavi
        if (KEYcmp(k, KEYget(&st->a[i])) == 0) // Se le chiavi sono uguali
            return st->a[i]; // Ritorna l'Item trovato
    
    // Se il loop finisce, l'elemento non è stato trovato
    return ITEMsetvoid();
}

// Cancella un elemento data la chiave 'k' (Operazione O(N))
void STdelete(ST st, Key k) {
    int i, j = 0; // j = indice di ricerca, i = indice di scorrimento

    // !!! BUG CRITICO !!!
    // Questa ricerca non controlla 'j < st->size'.
    // Se 'k' non è nell'array, continuerà a leggere memoria
    // non valida finché non crasha (Segmentation Fault).
    while (KEYcmp(KEYget(&st->a[j]), k) != 0)
        j++; // Cerca l'indice 'j' dell'elemento da cancellare
        
    // (Assumendo che 'j' sia stato trovato correttamente)
    // "Compatta" l'array: sposta tutti gli elementi a destra di 'j'
    // una posizione a sinistra, sovrascrivendo 'j'.
    for (i = j; i < st->size-1; i++)
        st->a[i] = st->a[i+1];
        
    // Decrementa il numero di elementi
    st->size--;
}

// Stampa tutti gli elementi (O(N))
void STdisplay(ST st){
    int i;

    // Scorre tutti gli elementi *validi*
    for (i = 0; i < st->size; i++)
        ITEMstore(st->a[i]); // 'ITEMstore' (non mostrata) stampa l'Item
}