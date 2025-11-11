// Include l'header file che definisce i tipi (QUEUE, Item) e i prototipi
#include "Queue.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della struttura Coda
struct queue {
    Item *q;      // Puntatore all'array (allocato dinamicamente)
    int N;        // La capacità della coda (usata per il modulo)
    int head;     // Indice della testa (da dove si estrae)
    int tail;     // Indice della coda (dove si inserisce)
};

// --- FUNZIONI PUBBLICHE ---

// Inizializza (alloca) la Coda
QUEUE QUEUEinit(int maxN) {
    // Alloca la memoria per la struttura "wrapper"
    QUEUE q = malloc(sizeof *q);

    // !!! BUG CRITICO !!!
    // Alloca l'array per 'maxN' elementi (indici 0..maxN-1)
    q->q = malloc(maxN*sizeof(Item));
    // Ma imposta la capacità 'N' a 'maxN+1'
    q->N = maxN+1; 
    
    // Inizializza 'head' a 'N'.
    // Questo (insieme a tail=0) è un modo per far funzionare
    // la condizione di 'QUEUEempty'.
    q->head = q->N;
    // 'tail' parte da 0 (il primo slot libero)
    q->tail = 0;

    return q; // Ritorna il puntatore alla coda
}

// Controlla se la coda è vuota (O(1))
int QUEUEempty(QUEUE q) {
    // La coda è vuota quando (head % N) e 'tail'
    // si "incontrano" sullo stesso indice.
    // All'inizio: ( (maxN+1) % (maxN+1) ) == 0 -> 0 == 0 -> true.
    return (q->head)%(q->N) == q->tail;
}

// Inserisce un elemento in CODA (O(1))
void QUEUEput(QUEUE q, Item val) {
    // Inserisce l'elemento nella posizione 'tail'
    // e POI incrementa 'tail'.
    // (Questo causerà un overflow quando tail raggiunge maxN)
    q->q[q->tail++] = val;
    
    // "Avvolge" l'indice 'tail' se supera N
    q->tail = (q->tail)%(q->N);
}

// Estrae un elemento dalla TESTA (O(1))
Item QUEUEget(QUEUE q) {
    // "Avvolge" l'indice 'head' prima di usarlo.
    // (es. all'inizio, head = maxN+1 -> head = 0)
    q->head = (q->head)%(q->N);

    // Estrae l'elemento dalla posizione 'head'
    // e POI incrementa 'head'.
    return q->q[q->head++];
}