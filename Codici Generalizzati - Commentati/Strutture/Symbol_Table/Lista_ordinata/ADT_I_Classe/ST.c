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
    // 'maxN' non è usato, la lista è dinamica
    ST st;
    
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
    link x, t; // x = nodo corrente, t = nodo temporaneo

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
    return st->tab.size; // Ritorna il valore 'size' memorizzato
}

// Inserisce un nuovo elemento MANTENENDO L'ORDINE (operazione O(N))
void STinsert(ST st, Item val) {
    link x, p; // x = corrente, p = precedente

    if (st == NULL) // Controllo di sicurezza
        return;
        
    // 'KEYcmp(A, B) == 1' (assumiamo) significa 'A > B'
    // Caso 1: La lista è vuota O il nuovo 'val' è maggiore della testa
    // (L'ordinamento è DECRESCENTE)
    if ((st->tab.head == NULL) || (KEYcmp(KEYget(&st->tab.head->val), KEYget(&val)) == 1))
        // Inserisci in testa
        st->tab.head = NEW(val, st->tab.head);
    else {
        // Caso 2: Inserimento in mezzo o in coda
        // Scorre la lista finché 'val' è maggiore della chiave di 'x'
        for (x = st->tab.head->next, p = st->tab.head; 
             x != NULL && (KEYcmp(KEYget(&val), KEYget(&x->val)) == 1); 
             p = x, x = x->next);
        // Inserisci il nodo tra 'p' e 'x'
        p->next = NEW(val, x);
    }
    // Incrementa la dimensione
    st->tab.size++;
}

// Cerca un elemento data una chiave 'k' (operazione O(N))
Item STsearch(ST st, Key k) {
    link x; // Puntatore per scorrere

    if (st == NULL)
        return ITEMsetvoid();
    if (st->tab.head == NULL) // Lista vuota
       return ITEMsetvoid();
       
    // Scorre l'intera lista
    for (x = st->tab.head; x != NULL; x = x->next)
        // Se le chiavi sono uguali (KEYcmp == 0)
        if (KEYcmp( KEYget(&x->val), k) == 0)
            return x->val; // ...ritorna l'Item trovato.
    
    // (Nota: la ricerca potrebbe essere ottimizzata fermandosi
    // non appena si trova una chiave minore, dato che la lista è ordinata)
    
    return ITEMsetvoid(); // Non trovato
}

// Seleziona l'elemento alla posizione 'r' (0-based) (operazione O(r))
Item STselect(ST st, int r) {
    int i;
    link x = st->tab.head; // Parti dalla testa

    // Scorre la lista 'r' volte
    for (i = r; i > 0; i--)
        x = x->next; // Avanza al prossimo
        // !!! BUG: Se r >= st->tab.size, x diventerà NULL
        // e il 'return x->val' successivo causerà un crash.
    
    return x->val; // Ritorna il valore del nodo alla r-esima posizione
}

// Cancella un elemento data una chiave 'k' (operazione O(N))
void STdelete(ST st, Key k) {
    link x, p; // x = corrente, p = precedente

    if (st == NULL) 
        return;
    if (st->tab.head == NULL) 
        return;
        
    // Scorre la lista per trovare l'elemento
    for (x = st->tab.head, p = NULL; x != NULL; p = x, x = x->next) {
        
        // Se troviamo la chiave da cancellare...
        if (KEYcmp(k, KEYget(&x->val)) == 0) {
            
            // Caso 1: L'elemento è la testa
            if (x == st->tab.head)
                st->tab.head = x->next;
            // Caso 2: L'elemento è in mezzo o in coda
            else
                p->next = x->next; // "Bypass" del nodo x
                
            free(x); // Libera la memoria del nodo
            
            // Il decremento della dimensione dovrebbe essere QUI
            // st->tab.size--; 
            
            break; // Esci dal loop
        }
    }
    // !!! BUG: 'size' viene decrementato anche se l'elemento non è stato trovato!
    st->tab.size--;
}

// Stampa l'intera tabella (operazione O(N))
void STdisplay(ST st) {
    link x; // Puntatore per scorrere

    // Scorre e stampa ogni elemento (in ordine decrescente)
    for (x = st->tab.head; x != NULL; x = x->next)
        ITEMstore(x->val);
}