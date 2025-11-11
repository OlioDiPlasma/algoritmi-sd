// Include il file header che definisce i tipi (PQ, Item) e i prototipi.
#include "PQ.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

// 'link' è un alias per un puntatore al nodo della lista
typedef struct PQnode *link;

// Struttura del NODO della lista
struct PQnode{ 
    Item val;     // L'Item (con la sua priorità)
    link next;    // Puntatore al nodo successivo
};

// Struttura "wrapper" (contenitore) della coda
// Contiene solo il puntatore alla testa della lista
struct pqueue { 
    link head; 
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo della lista
link NEW(Item val, link next) {
    // Alloca memoria per un 'struct PQnode'
    link x = malloc(sizeof(*x));
    
    // Inizializza i campi del nodo
    x->val = val;   // Imposta il dato
    x->next = next; // Imposta il puntatore al nodo successivo
    
    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'PQinit': inizializza una nuova coda vuota
PQ PQinit(int maxN) {
    // 'maxN' (dimensione massima) non è usato in questa implementazione
    // a lista, ma è presente per compatibilità con l'interfaccia PQ.h
    
    // Alloca la struttura wrapper
    PQ pq = malloc(sizeof(*pq)) ;

    // Inizializza la testa a NULL (la lista è vuota)
    pq->head = NULL;
    
    return pq; // Ritorna il puntatore alla coda
}

// --- FUNZIONI PUBBLICHE ---

// Controlla se la coda è vuota
int PQempty(PQ pq) {
    // La coda è vuota se il puntatore alla testa è NULL
    return pq->head == NULL;
}

// Inserisce un nuovo elemento 'val' mantenendo l'ordine (Max -> Min)
void PQinsert(PQ pq, Item val) {
    link x, p; // x = nodo corrente, p = nodo precedente
    Key k = KEYget(val); // Estrae la chiave (priorità) dal nuovo Item

    // Caso 1: La lista è vuota OPPURE il nuovo elemento ha priorità
    // più alta della testa (e deve diventare la nuova testa).
    // 'KEYless(head->val, k)' significa 'head->val < k'
    if (pq->head == NULL || KEYless(KEYget(pq->head->val), k)) {
        // Inserisci in testa
        pq->head = NEW(val, pq->head);
        return; // Finito
    }
    
    // Caso 2: Inserimento in mezzo o in coda.
    // Cerca il punto giusto dove inserire.
    // 'p' (precedente) parte dalla testa, 'x' (corrente) parte dal secondo.
    // Cicla finché:
    // 1. 'x' non è NULL (non siamo alla fine)
    // 2. La nuova chiave 'k' è MINORE della chiave di 'x'
    //    (cioè stiamo ancora cercando il punto giusto)
    for (x = pq->head->next, p = pq->head; 
         x != NULL && KEYless(k, KEYget(x->val)); 
         p = x, x = x->next);
    
    // Usciti dal loop, 'p' è l'ultimo nodo con priorità > k
    // e 'x' è il primo nodo con priorità <= k (o NULL).
    // Inseriamo il new nodo TRA 'p' e 'x'.
    p->next = NEW(val, x);
    
    return; // Funzione void
}

// Estrae (rimuove e ritorna) l'elemento con priorità massima
Item PQextractMax(PQ pq) {
    Item tmp; // Variabile per salvare l'Item da restituire
    link t;   // Puntatore temporaneo al secondo nodo

    // Controlla se la coda è vuota (underflow)
    if (PQempty(pq)) {
        printf("PQ empty\n");
        return ITEMsetvoid(); // Ritorna un Item nullo
    }
    
    // L'elemento massimo è sempre la testa (lista ordinata)
    tmp = pq->head->val; // Salva il valore
    
    // Aggiorna la testa:
    t = pq->head->next; // Salva il puntatore al secondo nodo
    free(pq->head);     // Libera la memoria della vecchia testa
    pq->head = t;       // La testa ora è il secondo nodo (o NULL)

    // Ritorna l'Item che era in testa
    return tmp;
}

// Ritorna l'elemento massimo *senza* estrarlo
Item PQshowMax(PQ pq) {
    // Il massimo è sempre in testa
    return pq->head->val;
}

// Stampa tutti gli elementi (in ordine di priorità)
void PQdisplay(PQ pq) {
    link x; // Puntatore per scorrere la lista

    // Ciclo 'for' standard per scorrere una lista
    for (x = pq->head; x != NULL; x = x->next)
        ITEMdisplay(x->val); // 'ITEMdisplay' (non mostrata) stampa un Item
    
    return; // Funzione void
}

// Cambia la priorità di un elemento (implementazione O(N))
void PQchange(PQ pq, Item val) {
    link x, p; // x = corrente, p = precedente

    if (PQempty(pq)) {
        printf("PQ empty\n"); 
        return;
    }
    
    // 1. TROVA E CANCELLA il vecchio elemento
    // (Si assume che 'val' contenga un ID e 'ITEMeq' confronti l'ID,
    // anche se 'val' contiene una *nuova* priorità)
    for(x = pq->head, p = NULL; x != NULL; p = x, x = x->next) {
        
        // Cerca l'elemento da aggiornare
        if (ITEMeq(x->val, val)) {
            // Trovato: ora rimuovilo (logica standard 'listDelKey')
            if (x == pq->head) // Se è la testa
                pq->head = x->next;
            else // Se è in mezzo o in coda
                p->next = x->next;
            
            free(x); // Libera il nodo rimosso
            break;   // Esci dal loop di ricerca
        }
    }
    
    // 2. INSERISCI l'elemento (con la nuova priorità)
    // 'PQinsert' lo posizionerà nel punto corretto O(N)
    PQinsert(pq, val);

    return; // Funzione void
}