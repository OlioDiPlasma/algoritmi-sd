// Include l'header file che definisce i tipi (QUEUE, Item) e i prototipi
// Nota: l'interfaccia si aspetta 'QUEUE q' come parametro,
// ma questa implementazione lo ignora!
#include "Queue.h"  

IMPLEMENTAZIONE PERICOLOSA

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct QUEUEnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO della lista
struct QUEUEnode {
    Item val;  // Il dato
    link next; // Puntatore al nodo successivo
};

// !!! ATTENZIONE: VARIABILI GLOBALI !!!
// 'head' e 'tail' sono definite come statiche.
// 'static' a livello globale significa "private di questo file .c".
// Poiché non sono dentro una struct, ci può essere solo UN 'head' e UN 'tail'.
// L'intero programma può avere UNA SOLA coda.
static link head, tail;

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo della lista
link NEW (Item val, link next) {
    link x = malloc(sizeof *x); // Alloca memoria

    x->val = val;   // Imposta il dato
    x->next = next; // Imposta il puntatore al successivo
    
    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'QUEUEinit': inizializza la CODA GLOBALE
void QUEUEinit(int maxN) {
    // 'maxN' (dimensione massima) non è usato, la lista è dinamica
    // Inizializza le *variabili globali* a NULL
    head = tail = NULL;
}

// Controlla se la CODA GLOBALE è vuota
int QUEUEempty() {
    // Ritorna vero (1) se la testa globale è NULL
    return head == NULL;
}

// Inserisce un elemento in CODA (nella CODA GLOBALE) - O(1)
void QUEUEput(Item val) {
    // Caso 1: La coda è vuota
    if (head == NULL) {
        // Crea il new nodo (che punta a NULL)
        // e fa puntare SIA 'head' CHE 'tail' a questo nuovo nodo
        head = (tail = NEW(val, head)); // 'head' è NULL qui
        return;
    }
    
    // Caso 2: La coda ha già elementi
    // Aggancia il new nodo in fondo (tail->next era NULL)
    // (tail->next è NULL, quindi NEW(val, NULL))
    tail->next = NEW(val, tail->next); 
    
    // Sposta il puntatore 'tail' all'elemento appena inserito
    tail = tail->next;
}

// Estrae un elemento dalla TESTA (dalla CODA GLOBALE) - O(1)
Item QUEUEget() {
    // Salva il valore della testa
    Item tmp = head->val;
    // Salva il puntatore al *secondo* elemento
    link t = head->next;
    
    // Libera la memoria del nodo che stiamo rimuovendo (la vecchia testa)
    free(head); 
    
    // Aggiorna la testa (ora è il secondo elemento, o NULL se era l'ultimo)
    head = t;
    
    // !!! BUG CRITICO !!!
    // Se 't' è NULL (abbiamo rimosso l'ultimo elemento),
    // 'head' è ora NULL, ma 'tail' PUNTA ANCORA alla
    // memoria liberata (dangling pointer)!
    // Manca:
    // if (head == NULL)
    //     tail = NULL;
    
    // Ritorna il valore che era in testa
    return tmp;
}