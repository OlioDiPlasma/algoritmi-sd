#include "ST.h" // Include i prototipi e le definizioni dei tipi

// Costanti per il Quadratic Probing
#define c1 1
#define c2 1

/*
 * Inserimento con Quadratic Probing
 */
void STinsert(ST st, Item item) {
    // i = contatore dei tentativi (distanza quadratica)
    // start = posizione hash iniziale
    // index = posizione attuale da controllare
    int i = 0, start = hash(KEYget(&item), st->M), index = start;

    // 1. Scansione (Probing)
    // Finché la cella 'index' è piena...
    // (!!! BUG: potenziale loop infinito se la tabella è piena)
    while (full(st, index)) {
        // ...incrementa il contatore dei tentativi...
        i++;
        // ...e calcola la *nuova* posizione quadratica.
        index = (start + c1*i + c2*i*i) % st->M;
    }
    
    // 2. Inserimento
    // Trovata una cella vuota 'index', inserisci l'item
    st->a[index] = item;
    // Incrementa il contatore degli elementi
    st->N++;
}

/*
 * Ricerca con Quadratic Probing
 */
Item STsearch(ST st, Key k) {
    // 1. Calcola i valori iniziali (DEVONO essere identici a STinsert)
    int i = 0, start = hash(k, st->M), index = start;

    // 2. Scansione (Probing)
    // Finché la cella 'index' è piena...
    // (!!! BUG: potenziale loop infinito)
    while (full(st, index))
        // Controlla se l'elemento in questa cella è quello giusto
        if (KEYcmp(k, KEYget(&st->a[index])) == 0) // KEYcmp non mostrato
            return st->a[index]; // Trovato!
        else {
            // Non è lui: passa alla prossima posizione quadratica
            i++;
            index = (start + c1*i + c2*i*i) % st->M;
        }
    
    // 3. Non Trovato
    // Se 'while(full(st, index))' diventa falso, significa che
    // abbiamo colpito una cella VUOTA. L'elemento non c'è.
    return ITEMsetNull(); // ITEMsetNull non mostrato
}

/*
 * Cancellazione con Quadratic Probing
 * (Logica "delete-and-reinsert")
 */
void STdelete(ST st, Key k) {
    // 1. Calcola i valori iniziali
    int i=0, start = hash(k, st->M), index = start;
    Item tmp; // Per la re-insertion

    // --- FASE 1: Trova l'elemento da cancellare ---
    // (!!! BUG: potenziale loop infinito)
    while (full(st, index))
        if (KEYcmp(k, KEYget(&st->a[index])) == 0) // Trovato
            break; // Esci, 'index' è la posizione da cancellare
        else { 
            // Prova la prossima posizione quadratica
            i++;  
            index = (start + c1*i + c2*i*i) % st->M;
        }
        
    // Se il loop si è fermato perché la cella è vuota (non trovata)
    if (ITEMcheckNull(st->a[index])) // ITEMcheckNull non mostrato
        return; // Elemento non trovato, non c'è nulla da cancellare

    // --- FASE 2: Cancella l'elemento (crea un "buco") ---
    st->a[index] = ITEMsetNull(); // Imposta la cella come "vuota"
    st->N--; // Decrementa il contatore

    // --- FASE 3: Ricostruisci il "cluster" (chain) ---
    // Dobbiamo re-inserire tutti gli elementi successivi
    // che appartengono a questa stessa catena di scansione.
    
    // Calcola la posizione *successiva* nella catena
    i++;
    index = (start + c1*i + c2*i*i) % st->M;
    
    // Cicla finché ci sono elementi in questa catena
    while(full(st, index)) {
        // "Raccogli" l'elemento
        tmp = st->a[index];
        // "Svuota" la sua vecchia cella
        st->a[index] = ITEMsetNull();
        // Decrementa N (verrà ri-aumentato da STinsert)
        st->N--; 
        
        // Re-inserisci l'elemento (STinsert farà il suo probing)
        STinsert(st, tmp); 
        
        // Passa al prossimo elemento della catena originale
        i++;
        index = (start + c1*i + c2*i*i) % st->M;
    }
}