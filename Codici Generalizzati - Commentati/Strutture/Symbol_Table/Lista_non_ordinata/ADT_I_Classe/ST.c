#include "ST.h" // Include i prototipi e le definizioni dei tipi (ST, Item, Key)

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct STnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO della lista
struct STnode {
    Item val;  // Il dato (Item) contenuto nel nodo
    link next; // Puntatore al nodo successivo
};

// Struttura "wrapper" (contenitore) per la lista
typedef struct {
    link head; // Puntatore al primo nodo (testa)
    int size;  // Numero di elementi presenti
} list;

// Struttura "wrapper" principale per la Symbol Table
struct symbtab {
    list tab; // La Symbol Table è implementata come UNA singola lista
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo della lista
static link NEW(Item val, link next) {
    // Alloca memoria per un 'struct STnode'
    link x = malloc(sizeof(*x));

    // Controllo di sicurezza: se malloc fallisce, ritorna NULL
    if (x == NULL)
        return NULL;
        
    // Inizializza i campi del nodo
    x->val = val;   // Imposta il dato
    x->next = next; // Imposta il puntatore al nodo successivo
    
    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'STinit': inizializza una nuova Symbol Table vuota
ST STinit(int maxN) {
    // 'maxN' (dimensione massima) non è usato in questa implementazione
    // perché una lista linkata è una struttura dati dinamica.
    
    ST st; // Dichiara il puntatore al wrapper ST
    
    // Alloca la memoria per la struttura 'symbtab'
    st = malloc(sizeof(*st));
    if(st == NULL) // Controllo fallimento malloc
        return NULL;
        
    // Inizializza la lista interna come vuota
    st->tab.size = 0;   // 0 elementi
    st->tab.head = NULL; // Nessuna testa
    
    return st; // Ritorna il puntatore alla ST
}

// Libera tutta la memoria usata dalla ST
void STfree(ST st) {
    link x, t; // x = nodo corrente, t = nodo temporaneo (per salvare il successivo)

    // Scorre l'intera lista per liberare ogni nodo
    for (x = st->tab.head; x != NULL; x = t) {
        t = x->next; // 1. Salva il puntatore al prossimo nodo
        free(x);     // 2. Libera il nodo corrente
    }
    
    // Infine, libera la struttura "wrapper"
    free(st);
}

// Ritorna il numero di elementi (operazione O(1))
int STcount(ST st) {
    // Ritorna il valore 'size' memorizzato nel wrapper
    return st->tab.size;
}

// Inserisce un nuovo elemento (sempre in testa - O(1))
void STinsert(ST st, Item val) {
    // Controllo di sicurezza
    if (st == NULL)
        return;
        
    // Chiama NEW per creare un nuovo nodo.
    // Il nuovo nodo punterà alla vecchia testa (st->tab.head).
    // La testa della lista viene aggiornata per puntare al nuovo nodo.
    st->tab.head = NEW(val, st->tab.head);
    
    // Incrementa il contatore della dimensione
    st->tab.size++;
}

// Cerca un elemento data una chiave 'k' (operazione O(N))
Item STsearch(ST st, Key k) {
    link x; // Puntatore per scorrere la lista

    // Controlli di sicurezza
    if (st == NULL)
        return ITEMsetvoid();
    if (st->tab.head == NULL) // Lista vuota
       return ITEMsetvoid();
       
    // Scorre l'intera lista dalla testa (head) alla fine (NULL)
    for (x = st->tab.head; x != NULL; x = x->next)
        // 'KEYcmp' (non mostrata) confronta due chiavi.
        // Se le chiavi sono uguali (ritorna 0)...
        if (KEYcmp( KEYget(&x->val), k) == 0)
            return x->val; // ...ritorna l'Item trovato.
    
    // Se il ciclo finisce, l'elemento non è stato trovato.
    return ITEMsetvoid(); // Ritorna un Item "vuoto"
}

// Cancella un elemento data una chiave 'k' (operazione O(N))
void STdelete(ST st, Key k) {
    link x, p; // x = nodo corrente, p = nodo precedente

    // Controlli di sicurezza
    if (st == NULL) 
        return;
    if (st->tab.head == NULL) 
        return;
        
    // Scorre la lista usando 'x' (corrente) e 'p' (precedente)
    for (x = st->tab.head, p = NULL; x != NULL; p = x, x = x->next) {
        
        // Se troviamo la chiave da cancellare...
        if (KEYcmp(k, KEYget(&x->val)) == 0) {
            
            // Caso 1: L'elemento è la testa (x == testa)
            if (x == st->tab.head)
                // La nuova testa diventa il nodo successivo
                st->tab.head = x->next;
            // Caso 2: L'elemento è in mezzo o in coda
            else
                // "Bypass" il nodo 'x': il precedente ('p')
                // punta ora al successivo di 'x'.
                p->next = x->next;
                
            free(x); // Libera la memoria del nodo 'x'
            
            // --- BUG DOVREBBE ESSERE QUI ---
            // st->tab.size--; 
            
            break; // Esci dal loop (cancellata la prima occorrenza)
        }
    }
    
    // !!! BUG !!!
    // Questa istruzione viene eseguita SEMPRE,
    // anche se l'elemento non è stato trovato.
    st->tab.size--;
}

// Stampa l'intera tabella (operazione O(N))
void STdisplay(ST st) {
    link x; // Puntatore per scorrere

    // Scorre l'intera lista
    for (x = st->tab.head; x != NULL; x = x->next)
        // 'ITEMstore' (non mostrata) stampa il singolo Item
        ITEMstore(x->val);
}