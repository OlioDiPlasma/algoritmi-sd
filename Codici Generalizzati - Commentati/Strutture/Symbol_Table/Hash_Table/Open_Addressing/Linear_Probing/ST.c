// Include l'header file che definisce i tipi (ST, Item, Key) e i prototipi
#include "ST.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della Symbol Table
struct symboltable { 
    Item *a;    // Puntatore all'array (allocato dinamicamente)
    int N;      // Numero di elementi *attualmente* presenti
    int M;      // Dimensione *totale* (capacità) dell'array 'a'
};

// --- FUNZIONI PUBBLICHE ---

// Inizializza (alloca) la Symbol Table
ST STinit(int maxN, float alpha) { // alpha è il fattore di carico (load factor) N/M
    int i;
    ST st = malloc(sizeof(*st)); // Alloca la struttura wrapper

    st->N = 0; // Inizializza il numero di elementi a 0
    
    // 'STsizeSet' (non mostrata) calcola una dimensione 'M'
    // (di solito un numero primo) tale che N (maxN) / M <= alpha
    st->M = STsizeSet(maxN, alpha);
    
    // Se STsizeSet fallisce (es. alpha troppo piccolo), ritorna NULL
    if (st->M == -1)
        st = NULL;
    else {
        // Alloca l'array 'a' della dimensione M calcolata
        st->a = malloc(st->M * sizeof(Item));
        // Inizializza tutte le M celle a "vuoto"
        for (i = 0; i < st->M; i++)
            st->a[i] = ITEMsetNull(); // ITEMsetNull() non mostrata
    }
    return st; // Ritorna il puntatore alla ST
}

// Inserisce un elemento (Linear Probing)
void STinsert(ST st, Item item) {
    // 1. Calcola l'indice hash iniziale
    // 'hash' (non mostrata) calcola la posizione base
    int i = hash(KEYget(&item), st->M); // KEYget() non mostrata

    // 2. Scansione Lineare
    // Finché la cella 'i' è piena ('full' vedi sotto)...
    // (!!! BUG: se st->N == st->M, questo è un loop infinito)
    while (full(st, i))
        // ...passa alla cella successiva (con "wrap-around" modulo M)
        i = (i+1)%st->M;
        
    // 3. Inserimento
    // Trovata una cella vuota 'i', inserisci l'item
    st->a[i] = item;
    // Incrementa il contatore degli elementi
    st->N++;
}

// Funzione 'full' (helper)
// Controlla se una cella è occupata
int full(ST st, int i) {
    // 'ITEMcheckNull' (non mostrata) ritorna 1 se la cella è vuota
    if (ITEMcheckNull(st->a[i])) 
        return 0; // 0 = non piena (vuota)
    return 1; // 1 = piena (occupata)
}

// Cerca un elemento data la chiave 'k' (Linear Probing)
Item STsearch(ST st, Key k) {
    // 1. Calcola l'indice hash iniziale (DEVE essere uguale a STinsert)
    int i = hash(k, st->M);

    // 2. Scansione Lineare
    // Finché la cella 'i' è piena...
    // (!!! BUG: potenziale loop infinito se la tabella è piena e k non c'è)
    while (full(st, i))
        // Controlla se l'elemento in questa cella è quello giusto
        if (KEYcmp(k, KEYget(&st->a[i])) == 0) // KEYcmp non mostrato
            return st->a[i]; // Trovato! Ritorna l'item.
        else
            // Non è lui, passa alla cella successiva
            i = (i+1)%st->M;

    // 3. Non Trovato
    // Se 'while(full(st, i))' diventa falso, significa che abbiamo
    // colpito una cella VUOTA ('ITEMsetNull'). La scansione si ferma
    // perché l'elemento non può essere oltre una cella vuota.
    return ITEMsetNull(); // Ritorna un Item "vuoto"
}

// Cancella un elemento data la chiave 'k' (Linear Probing)
void STdelete(ST st, Key k) {
    int j, i = hash(k, st->M); // i = indice di scansione, j = per la re-insertion
    Item tmp; // Variabile per salvare l'Item durante la re-insertion

    // --- FASE 1: Trova l'elemento da cancellare ---
    // (!!! BUG: potenziale loop infinito)
    while (full(st, i))
        if (KEYcmp(k, KEYget(&st->a[i])) == 0) // Trovato
            break; // Esci, 'i' è la posizione da cancellare
        else
            i = (i+1) % st->M; // Prova la prossima
            
    // Se il loop si è fermato perché la cella è vuota (non trovata)
    if (ITEMcheckNull(st->a[i])) // ITEMcheckNull (o !full)
        return; // Elemento non trovato, non c'è nulla da cancellare

    // --- FASE 2: Cancella l'elemento (crea un "buco") ---
    st->a[i] = ITEMsetNull(); // Imposta la cella come "vuota"
    st->N--; // Decrementa il contatore

    // --- FASE 3: Ricostruisci il "cluster" (chain) ---
    // Bisogna re-inserire tutti gli elementi successivi nella
    // stessa catena di scansione, altrimenti STsearch si fermerà
    // prematuramente al "buco" che abbiamo appena creato.
    
    // Inizia la scansione da 'j' (la cella *dopo* quella cancellata)
    for (j = i+1; full(st, j); j = (j+1)%st->M) { // (Usa j=i+1, non j=(i+1)%M)
        // (La riga corretta è j = (i+1)%st->M;)
        // (Il loop corretto è for (j = (i+1)%st->M; full(st, j); j = (j+1)%st->M) { )
        
        // "Raccogli" l'elemento
        tmp = st->a[j];
        // "Svuota" la sua vecchia cella
        st->a[j] = ITEMsetNull();
        // Decrementa N (verrà ri-incrementato da STinsert)
        st->N--; 
        
        // Re-inserisci l'elemento.
        // STinsert (che usa Linear Probing) troverà
        // il posto giusto per 'tmp' (o il "buco" in 'i' o un'altra cella).
        STinsert(st, tmp); 
    }
}