#include "ST.h" // Include i prototipi e le definizioni dei tipi (ST, Item, Key)

// --- FUNZIONI DI HASHING (PRIVATE) ---

/*
 * Funzione hash1 (Posizione Iniziale)
 * Calcola l'indice iniziale 'i' usando il metodo di Horner.
 */
static int hash1(Key k, int M) {
    int h = 0, base = 127; // h = accumulatore, base = numero primo
    
    // Scorre la stringa
    for ( ; *k != '\0'; k++) 
        // Calcola l'hash polinomiale (modulo M ad ogni passo)
        h = (base * h + *k) % M;
    
    return h; // Ritorna l'indice (0 <= h < M)
}

/*
 * Funzione hash2 (Dimensione del Passo)
 * Calcola lo "step" 'j' per la scansione (probing).
 */
static int hash2(Key k, int M) {
    int h = 0, base = 127; // h = accumulatore

    // 1. Calcola un hash "completo" (senza modulo M)
    for ( ; *k != '\0'; k++) 
        h = (base * h + *k);
        
    // 2. Trasforma l'hash in uno "step" > 0
    // (h % 97) -> un numero 0-96
    // + 1 -> un numero 1-97
    // % M -> un numero 0-(M-1)
    // L'uso di 97 (primo) e +1 è una tecnica per
    // generare uno step > 0 e (si spera) primo rispetto a M.
    h = ((h % 97) + 1) % M; 
    
    // 3. Controllo di sicurezza CRITICO:
    // Lo step 'j' NON DEVE MAI essere 0,
    // altrimenti (i+j)%M rimarrebbe bloccato su 'i'.
    if (h == 0) 
        h = 1; // Se il risultato è 0, forzalo a 1.
    
    return h; // Ritorna lo step (1 <= h < M)
}

// --- FUNZIONI PUBBLICHE ---

/*
 * Inserimento con Hashing Doppio (Open Addressing)
 */
void STinsert(ST st, Item item) {
    // 1. Calcola posizione iniziale 'i' e step 'j'
    int i = hash1(KEYget(&item), st->M); // KEYget non mostrato
    int j = hash2(KEYget(&item), st->M);

    // 2. Scansione (Probing)
    // Cicla finché la cella 'i' è piena ('full' non mostrata)
    while (full(st, i))
        // Calcola la prossima posizione
        i = (i+j) % st->M; 
        
    // 3. Inserimento
    // Trovata una cella vuota 'i', inserisci l'item
    st->a[i] = item; 
    // Incrementa il contatore degli elementi
    st->N++; 
}

/*
 * Ricerca con Hashing Doppio
 */
Item STsearch(ST st, Key k) {
    // 1. Calcola posizione iniziale 'i' e step 'j'
    // (Devono essere *identici* a quelli di STinsert)
    int i = hash1(k, st->M);
    int j = hash2(k, st->M);

    // 2. Scansione (Probing)
    // Cicla finché la cella 'i' è piena
    while (full(st, i))
        // Controlla se l'elemento in questa cella è quello giusto
        if (KEYcmp(k, KEYget(&st->a[i])) == 0) // KEYcmp non mostrato
            return st->a[i]; // Trovato! Ritorna l'item.
        else
            // Non è lui, vai alla prossima cella
            i = (i+j) % st->M;

    // 3. Non Trovato
    // Se 'while(full(st, i))' diventa falso, significa che abbiamo
    // colpito una cella VUOTA. L'elemento non può essere
    // in nessun'altra posizione, quindi non c'è.
    return ITEMsetNull(); // ITEMsetNull non mostrato
}

/*
 * Cancellazione con Hashing Doppio
 * (Questa è l'operazione più complessa in Open Addressing)
 */
void STdelete(ST st, Key k) {
    // !!! BUG: Manca st->M nella chiamata a hash2()
    int i = hash1(k, st->M), j = hash2(k, st->M); // CORRETTO
    Item tmp; // Variabile per la re-insertion
    
    // --- FASE 1: Trova l'elemento da cancellare ---
    while (full(st, i)) // Finché la cella è piena
        if (KEYcmp(k, KEYget(&st->a[i])) == 0) // Trovato
            break; // Esci dal loop, 'i' è la posizione da cancellare
        else 
            i = (i+j) % st->M; // Prova la prossima
            
    // Se il loop si è fermato perché la cella è vuota (non trovata)
    if (ITEMcheckNull(st->a[i])) // ITEMcheckNull (o !full)
        return; // Elemento non trovato, non c'è nulla da cancellare

    // --- FASE 2: Cancella l'elemento (lascia un "buco") ---
    st->a[i] = ITEMsetNull(); // Imposta la cella come "vuota"
    st->N--; // Decrementa il contatore

    // --- FASE 3: Ricostruisci il "cluster" (chain) ---
    // Bisogna re-inserire tutti gli elementi successivi nella
    // stessa catena di probing, altrimenti STsearch si fermerà
    // al "buco" che abbiamo appena creato.
    
    i = (i+j) % st->M; // Passa alla cella *successiva* a quella cancellata
    
    // Cicla finché ci sono elementi in questa catena
    while(full(st, i)) {
        // "Raccogli" l'elemento
        tmp = st->a[i]; 
        // "Svuota" la sua vecchia cella
        st->a[i] = ITEMsetNull(); 
        st->N--; // Decrementa il contatore (verrà ri-aumentato da STinsert)
        
        // Re-inserisci l'elemento.
        // STinsert troverà un nuovo posto (o 'i' o uno successivo)
        STinsert(st, tmp); 
        
        // Passa al prossimo elemento della catena originale
        i = (i+j) % st->M;
    }
}