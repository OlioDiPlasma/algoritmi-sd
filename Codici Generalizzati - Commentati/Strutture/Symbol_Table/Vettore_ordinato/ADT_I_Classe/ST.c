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

// Inserisce un nuovo elemento MANTENENDO L'ORDINE (Operazione O(N))
void STinsert(ST st, Item val) {
    // Assegna a 'i' la prima posizione libera (la dimensione attuale, es. 0)
    // e POI incrementa 'size' (es. a 1).
    int i = st->size++;

    // Controlla se (dopo l'incremento) abbiamo superato la capacità
    if (st->size > st->maxN) {
        // È pieno: RADDOPPIA la capacità dell'array
        st->a = realloc(st->a, (2*st->maxN)*sizeof(Item));
        // Controlla se realloc è fallito
        if (st->a == NULL) 
            return;
        // Aggiorna la capacità massima
        st->maxN = 2*st->maxN;
    }
    
    // Esegui l'inserimento ordinato (Insertion Sort)
    // 'KEYcmp(A, B) == -1' (assumiamo) significa 'A < B'
    // Finché 'i' non è la radice (i > 0) E il nuovo 'val' è MINORE
    // dell'elemento alla sua sinistra (a[i-1])...
    while((i > 0) && KEYcmp(KEYget(&val), KEYget(&st->a[i-1])) == -1) {
        // ...sposta l'elemento a sinistra (a[i-1]) a destra (a[i])
        st->a[i] = st->a[i-1];
        // ...torna indietro di una posizione
        i--;
    }
    // Trovata la "buca" giusta (alla posizione 'i'), inserisci il valore
    st->a[i] = val;
}

// Funzione 'searchR' (helper): Ricerca Binaria Ricorsiva (O(log N))
static Item searchR(ST st, int l, int r, Key k) {
    int m; // Variabile per il punto medio (middle)

    // Calcola il punto medio
    m = (l + r)/2;
    
    // Caso base 1: Non trovato (sinistra > destra)
    if (l > r) 
        return ITEMsetvoid();
        
    // Caso base 2: Trovato!
    if (KEYcmp(k, KEYget(&st->a[m])) == 0) 
        return st->a[m];
        
    // (Controllo ridondante, già coperto da 'l > r', ma non fa danni)
    if (l == r) 
        return ITEMsetvoid();
        
    // Passo ricorsivo:
    // Se k < a[m] (KEYcmp == -1), cerca a SINISTRA
    if (KEYcmp(k, KEYget(&st->a[m])) == -1)
        return searchR(st, l, m-1, k);
    else // Altrimenti (k > a[m]), cerca a DESTRA
        return searchR(st, m+1, r, k);
}

// Cerca un elemento data la chiave 'k' (wrapper per O(log N))
Item STsearch(ST st, Key k) {
    // Chiama la ricerca binaria ricorsiva sull'intero array (da 0 a size-1)
    return searchR(st, 0, st->size-1, k) ;
}

// Seleziona l'r-esimo elemento (Operazione O(1))
Item STselect(ST st, int r) { 
    // Dato che l'array è ordinato, l'elemento di rango 'r'
    // si trova esattamente all'indice 'r'.
    return st->a[r]; 
}

// Cancella un elemento data la chiave 'k' (Operazione O(N))
void STdelete(ST st, Key k) {
    int i, j = 0; // j = indice di ricerca, i = indice di scorrimento

    // !!! BUG CRITICO !!!
    // Questa ricerca lineare (O(N)) non controlla 'j < st->size'.
    // Se 'k' non è nell'array, continuerà a leggere memoria
    // non valida finché non crasha (Segmentation Fault).
    // (Inoltre, potrebbe usare la ricerca binaria (O(log N)) per trovare 'j')
    while (KEYcmp(KEYget(&st->a[j]), k) != 0)
        j++; // Cerca l'indice 'j' dell'elemento da cancellare
        
    // (Assumendo che 'j' sia stato trovato)
    // "Compatta" l'array (O(N)): sposta tutti gli elementi a destra di 'j'
    // una posizione a sinistra, sovrascrivendo 'j'.
    for (i = j; i < st->size-1; i++)
        st->a[i] = st->a[i+1];
        
    // Decrementa il numero di elementi
    st->size--;
}

// Stampa tutti gli elementi (O(N))
void STdisplay(ST st){
    int i;

    // Scorre tutti gli elementi (già ordinati)
    for (i = 0; i < st->size; i++)
        ITEMstore(st->a[i]); // 'ITEMstore' (non mostrata) stampa l'Item
}