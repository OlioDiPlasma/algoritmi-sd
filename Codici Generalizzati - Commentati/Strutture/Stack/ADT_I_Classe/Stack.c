// Include il file header che definisce i tipi (STACK, Item) e i prototipi
#include "Stack.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

// 'link' è un alias (un soprannome) per un puntatore a 'struct STACKnode'
typedef struct STACKnode *link;

// Struttura del NODO della lista
struct STACKnode {
    Item val;  // Il dato (Item) contenuto nel nodo
    link next; // Puntatore al nodo successivo
};

// Struttura "wrapper" (contenitore) della Pila
struct stack {
    // Basta un solo puntatore (alla testa),
    // perché tutte le operazioni (LIFO) avvengono lì
    link head; 
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

/**
 * @brief Funzione 'NEW' (helper, statica/privata)
 * Alloca e inizializza un nuovo nodo della lista.
 */
static link NEW(Item val, link next) {
    // Alloca memoria sull'heap grande quanto un nodo
    // (il cast (link) è opzionale in C)
    link x = (link) malloc(sizeof(*x));

    // Inizializza i campi del nodo appena creato
    x->val = val;   // Imposta il dato
    x->next = next; // Imposta il puntatore al nodo successivo

    // Ritorna il puntatore al nuovo nodo
    return x;
}

/**
 * @brief Funzione 'STACKinit': inizializza una nuova Pila vuota.
 */
STACK STACKinit(int maxN) {
    // 'maxN' (dimensione massima) non è usato in questa implementazione
    // perché una lista linkata è dinamica (cresce quanto serve).
    
    // Alloca memoria per la struttura "wrapper" (il contenitore)
    STACK s = malloc(sizeof(*s));

    // Inizializza la testa a NULL, che significa "pila vuota"
    s->head = NULL;

    // Ritorna il puntatore alla nuova pila
    return s;
}

/**
 * @brief Funzione 'STACKempty': controlla se la Pila è vuota (O(1)).
 */
int STACKempty(STACK s) {
    // Ritorna 1 (vero) se la testa è NULL, 0 (falso) altrimenti
    return s->head == NULL;
}

/**
 * @brief Funzione 'STACKpush': inserisce un elemento in cima (in testa) (O(1)).
 */
void STACKpush(STACK s, Item val) {
    // Questa è la classica operazione "inserimento in testa":
    // 1. NEW crea un nuovo nodo (passandogli 'val')
    // 2. Il 'next' del nuovo nodo viene impostato alla *vecchia* testa (s->head)
    // 3. 's->head' viene aggiornato per puntare al nuovo nodo.
    s->head = NEW(val, s->head); 
}

/**
 * @brief Funzione 'STACKpop': rimuove e ritorna l'elemento in cima (O(1)).
 * !!! BUG: Non controlla se la pila è vuota !!!
 */
Item STACKpop (STACK s) {
    // (Controllo mancante: if (STACKempty(s)) { ... errore ... })
    
    // Salva il valore contenuto nel nodo in testa
    Item tmp = s->head->val;
    
    // Salva il puntatore al *secondo* nodo (che diventerà la nuova testa)
    link t = s->head->next;

    // Libera la memoria del nodo che stiamo rimuovendo (la vecchia testa)
    free(s->head);
    
    // Aggiorna la testa della pila: ora punta al secondo nodo
    s->head = t;
    
    // Ritorna il valore che avevamo salvato
    return tmp;
}