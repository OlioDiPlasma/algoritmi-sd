// Include l'header file che definisce i tipi (QUEUE, Item) e i prototipi
#include "Queue.h"

// --- ATTENZIONE: VARIABILI GLOBALI ---
// L'intera coda è definita da queste variabili statiche globali.
// 'static' le rende private a questo file .c, ma sono comunque globali.
// Questo significa che l'intero programma può avere UNA SOLA CODA.
static Item *q;       // L'array che contiene i dati
static int N;         // La capacità *totale* dell'array (maxN + 1)
static int head;      // L'indice da cui leggere (testa)
static int tail;      // L'indice in cui scrivere (coda)

// Funzione 'QUEUEinit': inizializza la CODA GLOBALE
// Nota: questa funzione NON ritorna un 'QUEUE'
// e ignora il 'maxN' dell'interfaccia (sebbene ne usi uno simile).
void QUEUEinit(int maxN) {
    // Alloca l'array con 'maxN + 1' slot.
    // L'uso di N = maxN + 1 è una tecnica standard
    // per distinguere facilmente lo stato "vuoto" da quello "pieno"
    // (riservando uno slot).
    q = malloc((maxN+1)*sizeof(Item));
    N = maxN + 1; // Salva la capacità totale
    
    // Inizializza 'head' a 'N'.
    // Questo è un trucco: quando 'QUEUEget' è chiamata la prima volta,
    // (head % N) farà 0.
    head = N; 
    
    // 'tail' parte da 0 (il primo slot libero)
    tail = 0;
}

// Controlla se la CODA GLOBALE è vuota (O(1))
int QUEUEempty() {
    // La coda è vuota quando (head % N) e 'tail'
    // si "incontrano" sullo stesso indice.
    // All'inizio: (N % N) == 0 e tail == 0 -> true.
    return head%N == tail;
}

// Inserisce un elemento in CODA (nella CODA GLOBALE) (O(1))
void QUEUEput(Item val) {
    // !!! BUG CRITICO !!!
    // Manca un controllo per la coda piena.
    // if ( (tail+1) % N == head%N ) { ... coda piena ... }
    // Se la coda è piena, questa funzione sovrascrive
    // l'elemento in testa (a 'head').
    
    // Inserisce l'elemento nella posizione 'tail'
    // e POI incrementa 'tail'.
    q[tail++] = val;
    
    // "Avvolge" l'indice 'tail' se supera N
    tail = tail%N;
}

// Estrae un elemento dalla TESTA (dalla CODA GLOBALE) (O(1))
Item QUEUEget() {
    // !!! BUG (Minore) !!!
    // Manca un controllo per la coda vuota (underflow).
    // if (QUEUEempty()) { ... errore ... }
    
    // "Avvolge" l'indice 'head' prima di usarlo.
    // (es. all'inizio, head = N -> head = 0)
    head = head%N;

    // Estrae l'elemento dalla posizione 'head'
    // e POI incrementa 'head'.
    return q[head++];
}