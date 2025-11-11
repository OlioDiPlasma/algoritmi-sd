// Include il file header che definisce i tipi (SET, Item, Key) e i prototipi.
#include "Set.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct SETnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura "wrapper" (contenitore) per l'ADT Set
struct set {
    link head; // Puntatore al primo nodo (testa) della lista
    int N;     // Numero di elementi (dimensione) del set
};

// Struttura del NODO della lista
// (NOTA: 'struct setNode' è diverso da 'struct SETnode' nella typedef)
struct setNode {
    Item val;  // Il dato (Item) contenuto nel nodo
    link next; // Puntatore al nodo successivo
};

// --- FUNZIONI PUBBLICHE ---

/**
 * @brief Inizializza (alloca) un nuovo set vuoto.
 * 'maxN' non viene utilizzato perché la lista è dinamica.
 */
SET SETinit(int maxN) {
    // Alloca memoria per la struttura 'set' (il wrapper)
    SET s = malloc(sizeof *s);
    
    // Inizializza la lista come vuota
    s->head = NULL; // La testa punta a NULL
    s->N = 0;       // La dimensione è 0
    
    // Ritorna il puntatore al nuovo set
    return s;
}

/**
 * @brief Libera tutta la memoria allocata per il set 's'.
 */
void SETfree(SET s) {
    link x, t; // x = nodo corrente, t = nodo temporaneo (per salvare il successivo)

    // Scorre l'intera lista per liberare ogni singolo nodo
    for (x = s->head; x != NULL; x = t) {
        t = x->next; // 1. Salva il puntatore al prossimo nodo
        free(x);     // 2. Libera il nodo corrente
    }
    // Infine, libera la struttura 'set' (il wrapper)
    free(s);
}

/**
 * @brief Cerca una chiave 'k' nel set 's'.
 * Implementa una ricerca lineare (O(N)).
 * Ritorna 1 (vero) se trovato, 0 (falso) se non trovato.
 */
int SETsearch(SET s, Key k) {
    link x; // Puntatore per scorrere la lista

    x = s->head; // Parti dalla testa
    // Cicla finché la lista non è finita
    while (x != NULL) {
        // 'KEYeq' (non mostrata) confronta la chiave 'k'
        // con la chiave dell'elemento corrente 'x->val'
        if (KEYeq(key(x->val), k)) // (presumendo 'key' sia 'KEYget')
            return 1; // Trovato!
        x = x->next; // Passa al prossimo nodo
    }
    return 0; // Fine della lista, non trovato
}

/**
 * @brief Crea un nuovo set 's' che è l'UNIONE di s1 e s2.
 * (Logica O(N*M))
 */
SET SETunion(SET s1, SET s2) {
    link x1, x2; // Puntatori per scorrere s1 e s2
    int founds2, counts2 = 0; // Flag e contatore
    
    // Crea un nuovo set vuoto 's'
    SET s = SETinit(s1->N + s2->N);
    
    // 1. Copia tutti gli elementi di s1 nel nuovo set s
    x1 = s1->head;
    while (x1 != NULL) {
        // 'SETfill' (non mostrata) presumibilmente inserisce l'elemento
        SETfill(s, x1->val); 
        x1 = x1->next;
    }
    
    // 2. Scorre s2 e aggiunge solo gli elementi non già presenti
    for (x2 = s2->head; x2 != NULL; x2 = x2->next) {
        x1 = s1->head; // Riavvia il puntatore di s1
        founds2 = 0; // Resetta il flag
        
        // Cerca l'elemento x2->val dentro s1
        while (x1 != NULL) {
            // 'ITEMeq' (non mostrata) confronta due Item
            if (ITEMeq(x1->val, x2->val)) 
                founds2 = 1; // Trovato in s1
            x1 = x1->next;
        }
        
        // Se x2->val NON è stato trovato in s1...
        if (founds2 == 0) {
            SETfill(s, x2->val); // ...inseriscilo nel nuovo set 's'
            counts2++; // Incrementa il contatore degli elementi aggiunti da s2
        }
    }
    // Aggiorna la dimensione totale del nuovo set
    s->N = s1->N + counts2;

    return s; // Ritorna il set unione
}

/**
 * @brief Crea un nuovo set 's' che è l'INTERSEZIONE di s1 e s2.
 * (Logica O(N*M))
 */
SET SETintersection(SET s1, SET s2) { 
    link x1, x2; // Puntatori per scorrere s1 e s2
    int counts=0; // Contatore per la dimensione del nuovo set
    SET s;
    
    // Crea un nuovo set vuoto 's'
    s = SETinit(s1->N + s2->N);
    
    // Scorre il primo set (s1)
    x1 = s1->head;
    while (x1 != NULL) {
        // Scorre il secondo set (s2)
        x2 = s2->head;
        while (x2 != NULL) {
            // Se l'elemento di s1 è uguale all'elemento di s2...
            if (ITEMeq(x1->val, x2->val)) {
                // ...inserisci l'elemento nel nuovo set 's'
                SETfill(s, x1->val); 
                counts++; // Incrementa il contatore
                // Interrompi il ciclo *interno* (break)
                // (Abbiamo trovato l'intersezione per x1,
                // passa al prossimo x1)
                break;
            }
            x2 = x2->next; // Prossimo elemento di s2
        }
        x1 = x1->next; // Prossimo elemento di s1
    }
    // Aggiorna la dimensione del set intersezione
    s->N = counts;

    return s; // Ritorna il set intersezione
}