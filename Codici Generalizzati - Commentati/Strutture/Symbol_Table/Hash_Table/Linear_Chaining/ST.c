#include "ST.h" // Include i prototipi e le definizioni dei tipi (ST, Item, Key)

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct STnode* link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO della lista
struct STnode { 
    Item item; // Il dato
    link next; // Puntatore al nodo successivo
};

// Struttura "wrapper" (contenitore) della Symbol Table
struct symbtab { 
    link *heads; // Puntatore a un ARRAY di PUNTATORI (l'array delle teste delle liste)
    int N;       // Numero *totale* di elementi in *tutte* le liste
    int M;       // Dimensione dell'array 'heads' (numero di liste)
    link z;      // Un nodo SENTINELLA (usato al posto di NULL)
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo della lista
static link NEW( Item item, link next) {
    link x = malloc(sizeof(*x)); // Alloca memoria

    x->item = item; // Imposta il dato
    x->next = next; // Imposta il puntatore al successivo

    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'STinit': inizializza una nuova Tabella di Hash
ST STinit(int maxN, float r) { // maxN e r servono per calcolare M
    int i;
    ST st; // Dichiara il puntatore al wrapper ST

    st = malloc(sizeof(*st)); // Alloca la memoria per la struttura wrapper
    st->N = 0; // Inizializza il numero totale di elementi a 0
    
    // 'STsizeSet' (non mostrata) calcola la dimensione M
    // (di solito un numero primo) in base a maxN
    st->M = STsizeSet(maxN, r); 
    
    // Alloca l'array 'heads' che conterrà M puntatori a nodo
    st->heads = malloc(st->M*sizeof(link));
    
    // Alloca il nodo sentinella 'z' (che rappresenta la fine di una lista)
    st->z = NEW(ITEMsetNull(), NULL); // ITEMsetNull() non mostrata
    
    // Inizializza tutte le M liste come vuote (facendole puntare a 'z')
    for (i=0; i < st->M; i++)
        st->heads[i] = st->z;
    
    return st; // Ritorna il puntatore alla ST
}

// Libera tutta la memoria usata dalla ST
void STfree(ST st) {
    int i;
    link t, u; // t = corrente, u = successivo

    // 1. Libera tutte le liste (tutti i nodi)
    for(i = 0; i < st->M; i++) { // Scorre l'array 'heads'
        // Scorre la singola lista all'indice 'i'
        for (t = st->heads[i]; t != st->z; t = u) {
            u = t->next; // Salva il puntatore al prossimo nodo
            free(t);     // Libera il nodo corrente
        }
    }
    
    // 2. Libera le strutture ausiliarie
    free(st->z);      // Libera il nodo sentinella
    free(st->heads);  // Libera l'array delle teste
    free(st);         // Libera il wrapper
}

// Ritorna il numero totale di elementi (O(1))
int STcount(ST st) {
    return st->N;
}

// Controlla se la tabella è vuota (O(1))
int STempty(ST st) {
    if (STcount(st) == 0)
        return 1; // 1 = vero (è vuota)
    return 0; // 0 = falso (non è vuota)
}

// --- FUNZIONI DI HASHING (PRIVATE) ---

// Funzione Hash (Metodo di Horner, base 127)
static int hash(Key v, int M) { // v = chiave (stringa), M = dimensione tabella
    int h = 0, base = 127; // h = accumulatore, base = numero primo

    // Scorre la stringa
    for ( ; *v != '\0'; v++) 
        // Calcola l'hash polinomiale
        h = (base * h + *v) % M;

    return h; // Ritorna l'indice (0 <= h < M)
}

// Funzione Hash (Metodo Universale - alternativa)
static int hashU(Key v, int M) {
    int h, a = 31415, b = 27183; // Costanti "magiche"

    for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
        h = (a*h + *v) % M;

    return h;
}

// --- FUNZIONI PUBBLICHE (Operazioni ST) ---

// Inserisce un elemento (O(1) in media)
void STinsert(ST st, Item val) {
    int i; // Indice calcolato dall'hash

    // 1. Calcola l'indice 'i' in cui inserire
    i = hash(KEYget(&val), st->M); // KEYget non mostrato
    
    // 2. Inserisci il nuovo nodo IN TESTA alla lista 'i'
    // Il nuovo nodo punta alla vecchia testa (st->heads[i])
    // La testa ora punta al nuovo nodo
    st->heads[i] = NEW(val, st->heads[i]);
    
    // !!! BUG: Manca 'st->N++;'
}

// Funzione 'searchR' (helper): ricerca ricorsiva in UNA lista
static Item searchR(link t, Key k, link z) {
    // Caso base 1: Fine lista (sentinella 'z') -> Non trovato
    if (t == z) 
        return ITEMsetNull();
        
    // Caso base 2: Trovato!
    if ((KEYcmp(KEYget(&t->item), k)) == 0) // KEYcmp non mostrato
        return t->item;
        
    // Passo ricorsivo: Cerca nel resto della lista
    return searchR(t->next, k, z);
}

// Cerca un elemento data la chiave 'k' (O(1) in media, O(N) peggiore)
Item STsearch(ST st, Key k) {
    // 1. Calcola l'indice 'i' della lista in cui cercare
    int i = hash(k, st->M);
    
    // 2. Chiama la ricerca ricorsiva SOLO su quella lista (st->heads[i])
    return searchR(st->heads[i], k, st->z);
}

// Funzione 'deleteR' (helper): cancellazione ricorsiva da UNA lista
static link deleteR(link x, Key k) {
    // !!! INCOERENZA/BUG: Questa funzione usa NULL, ma il resto
    // del codice usa 'st->z' come sentinella.
    if (x == NULL) // Dovrebbe essere 'if (x == st->z)'
        return NULL; // Dovrebbe essere 'return st->z'
        
    // Trovato il nodo da cancellare
    if ((KEYcmp(KEYget(&x->item), k)) == 0) {
        link t = x->next; // Salva il puntatore al successivo
        free(x);          // Libera il nodo corrente
        return t;         // Ritorna il successivo (che si aggancerà al genitore)
    }
    
    // Passo ricorsivo: Cerca nel resto della lista
    x->next = deleteR(x->next, k);
    
    return x; // Ritorna il nodo corrente (non modificato)
}

// Cancella un elemento data la chiave 'k'
void STdelete(ST st, Key k) {
    // 1. Calcola l'indice 'i' della lista da cui cancellare
    int i = hash(k, st->M);

    // 2. Chiama la cancellazione ricorsiva SOLO su quella lista
    st->heads[i] = deleteR(st->heads[i], k);
    
    // !!! BUG: Manca 'st->N--;' (da fare solo se deleteR ha successo)
}

// --- STAMPA E VISUALIZZAZIONE ---

// Funzione 'visitR' (helper): stampa (visita) UNA lista
static void visitR(link h, link z) {
    // Caso base: Fine lista (sentinella 'z')
    if (h == z) 
        return;
    ITEMstore(h->item); // ITEMstore (non mostrata) stampa l'Item
    visitR(h->next, z); // Vai al prossimo
}

// Stampa l'intera tabella di hash
void STdisplay(ST st)  {
    int i;

    // Scorre l'array 'heads' (le M liste)
    for (i = 0; i < st->M; i++) {
        // Stampa l'intestazione per la lista 'i'
        printf("st->heads[%d] = ", i);
        
        // Stampa tutti gli elementi in quella lista (chain)
        visitR(st->heads[i], st->z);
        
        printf("\n"); // Vai a capo per la prossima lista
    }
}