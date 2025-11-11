// Include l'header file che definisce i tipi (ST, Item, Key)
#include "ST.h"

// --- DEFINIZIONE STRUTTURA ---
struct symbtab {
    Item *a;  // L'array (allocato dinamicamente) che contiene i dati
    int N;    // Numero di elementi *attualmente* presenti
    int M;    // Dimensione *totale* (capacità) dell'array 'a'
}; // Manca un 'typedef' qui, che è probabilmente nel file ST.h

/*
 * ESEMPIO 1: Funzione di mappatura per chiavi 'char' (A-Z)
 * Trasforma la chiave 'A' in indice 0, 'B' in 1, ecc.
 */
int GETindex(Key k) {
    int i;
    i = k - 'A'; // Semplice aritmetica dei caratteri
    return i;
}

/*
 * ESEMPIO 2: Funzione di mappatura per chiavi 'int' (0..M-1)
 * La chiave È l'indice.
 */
int GETindex(Key k) {
    int i;
    i = (int) k; // Casting (se Key non fosse già int)
    return i;
}

/*
 * ESEMPIO 3: Funzione di mappatura per stringhe
 * Tratta la stringa come un numero in base 26 (b=26)
 * "ABC" -> (0*26 + 0 ('A')) -> (0*26 + 1 ('B')) -> (1*26 + 2 ('C')) = 28
 */
int GETindex(Key k) {
    int i = 0, b = 26; // i = accumulatore, b = base
    
    // Scorre la stringa puntata da 'k'
    for ( ; *k != '\0'; k++)
        // Calcola il valore numerico della stringa
        i = (b * i + (*k - ((int) 'A')));

    return i; // Ritorna l'indice calcolato
}

// --- FUNZIONI PUBBLICHE ---

// Inizializza (alloca) la tabella
ST STinit(int maxN) {
    ST st;
    int i;

    // Alloca la struttura wrapper
    st = malloc(sizeof(*st));
    // Alloca l'array 'a' di dimensione maxN
    st->a = malloc(maxN*sizeof(Item));
    
    // Inizializza *tutte* le M celle a "vuoto" (ITEMsetvoid)
    for (i = 0; i < maxN; i++)
        st->a[i] = ITEMsetvoid();
        
    st->M = maxN; // Salva la capacità massima
    st->N= 0;     // All'inizio ci sono 0 elementi

    return st;
}

// Libera tutta la memoria
void STfree(ST st) {
    free(st->a); // 1. Libera l'array
    free(st);    // 2. Libera il wrapper
}

// Ritorna il numero di elementi inseriti
int STcount(ST st) {
    return st->N; // Operazione O(1)
}

// Inserisce un elemento (Operazione O(1))
void STinsert(ST st, Item val) {
    // 1. Calcola l'indice DIRETTO dalla chiave
    int index = GETindex(KEYget(val));

    // 2. Inserisci l'Item in quella posizione
    // (Sovrascrive un elemento precedente se aveva lo stesso indice)
    st->a[index] = val;
    // 3. Incrementa il contatore
    st->N++;
}

// Cerca un elemento (Operazione O(1))
Item STsearch(ST st, Key k) {
    // 1. Calcola l'indice DIRETTO dalla chiave
    int index = GETindex(k);

    // 2. Ritorna l'Item in quella posizione
    // (Sarà ITEMsetvoid() se la chiave non è mai stata inserita)
    return st->a[index];
}

// Seleziona l'r-esimo elemento *inserito* (Operazione O(M))
Item STselect(ST st, int r) {
    int i;

    // Deve scorrere l'INTERO array (M celle)
    for (i = 0; i < st->M; i++)
        // Se la cella NON è vuota...
        if ((ITEMcheckvoid(st->a[i])==0) && (r-- == 0))
            // ...e il contatore 'r' è arrivato a 0...
            return st->a[i]; // ...restituisci l'Item
    
    return NULL; // O ITEMsetvoid(), se r è troppo grande
}

// Cancella un elemento (Operazione O(1))
void STdelete(ST st, Key k) {
    // 1. Calcola l'indice DIRETTO
    // 2. Imposta la cella a "vuoto"
    st->a[GETindex(k)] = ITEMsetvoid();
    // 3. Decrementa il contatore
    st->N--;
}

// Stampa tutti gli elementi *presenti* (Operazione O(M))
void STdisplay(ST st){
    int i;

    // Deve scorrere l'INTERO array (M celle)
    for (i = 0; i < st->M; i++)
        // Se la cella NON è vuota, la stampa
        if (ITEMcheckvoid(st->a[i]) == 0) 
            ITEMstore(st->a[i]);
}