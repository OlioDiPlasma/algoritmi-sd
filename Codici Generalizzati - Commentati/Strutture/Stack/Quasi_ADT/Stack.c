// Include il file header che definisce i tipi (STACK, Item) e i prototipi
// (Nota: questa implementazione IGNORA i parametri 'STACK s'
// che dovrebbero essere nei prototipi)
#include "Stack.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct STACKnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO della lista
struct STACKnode {
    Item val;  // Il dato (Item) contenuto nel nodo
    link next; // Puntatore al nodo successivo
};

// !!! ATTENZIONE: VARIABILE GLOBALE !!!
// 'head' è definita come statica a livello globale (privata di questo file).
// Questa singola variabile rappresenta la testa dell'UNICA pila del programma.
static link head;

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
 * @brief Funzione 'STACKinit': inizializza la PILA GLOBALE.
 * (Ignora il parametro 'maxN')
 */
void STACKinit(int maxN) {
    // Imposta la variabile globale 'head' a NULL
    head = NULL;
}

/**
 * @brief Funzione 'STACKempty': controlla se la PILA GLOBALE è vuota (O(1)).
 */
int STACKempty() {
    // Ritorna 1 (vero) se la testa globale è NULL, 0 (falso) altrimenti
    return head == NULL;
}

/**
 * @brief Funzione 'STACKpush': inserisce un elemento in cima alla PILA GLOBALE (O(1)).
 */
void STACKpush(Item val) {
    // Inserimento in testa:
    // 1. NEW crea un nuovo nodo che punta alla vecchia 'head'
    // 2. La 'head' globale viene aggiornata per puntare al nuovo nodo
    head = NEW(val, head);
}

/**
 * @brief Funzione 'STACKpop': rimuove e ritorna l'elemento in cima alla PILA GLOBALE (O(1)).
 * !!! BUG: Non controlla se la pila è vuota !!!
 */
Item STACKpop() {
    // (Controllo mancante: if (STACKempty()) { ... errore ... })
    
    // Salva il valore contenuto nel nodo in testa
    // CRASH se 'head' è NULL
    Item tmp = head->val;
    
    // Salva il puntatore al *secondo* nodo (che diventerà la nuova testa)
    link t = head->next;

    // Libera la memoria del nodo che stiamo rimuovendo (la vecchia testa)
    free(head);
    
    // Aggiorna la testa globale: ora punta al secondo nodo
    head = t;
    
    // Ritorna il valore che avevamo salvato
    return tmp;
}