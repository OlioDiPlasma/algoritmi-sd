// Include l'header file che definisce i tipi (QUEUE, Item) e i prototipi
#include "Queue.h"

// --- DEFINIZIONE TIPI E STRUTTURE ---

typedef struct QUEUEnode *link; // 'link' è un alias per un puntatore al nodo

// Struttura del NODO della lista
struct QUEUEnode { 
    Item val;  // Il dato
    link next; // Puntatore al nodo successivo
};

// Struttura "wrapper" (contenitore) della coda
struct queue { 
    link head; // Puntatore al primo elemento (da cui si estrae)
    link tail; // Puntatore all'ultimo elemento (dove si inserisce)
};

// --- FUNZIONI HELPER e di INIZIALIZZAZIONE ---

// Funzione 'NEW' (helper): alloca un nuovo nodo della lista
link NEW(Item val, link next) {
    link x = malloc(sizeof(*x)); // Alloca memoria

    x->val = val;   // Imposta il dato
    x->next = next; // Imposta il puntatore al successivo
    
    return x; // Ritorna il puntatore al nuovo nodo
}

// Funzione 'QUEUEinit': inizializza una nuova coda vuota
QUEUE QUEUEinit(int maxN) {
    // 'maxN' (dimensione massima) non è usato, la lista è dinamica
    QUEUE q = malloc(sizeof(*q)); // Alloca il wrapper

    q->head = NULL; // Inizializza la testa a NULL
    
    // !!! BUG CRITICO 1 !!!
    // Manca l'inizializzazione della coda.
    // q->tail non viene impostato a NULL.
    // La riga corretta è: q->tail = NULL;

    return q; // Ritorna il puntatore alla coda (con 'tail' non inizializzato)
}

// Controlla se la coda è vuota (operazione O(1))
int QUEUEempty(QUEUE q) {
    // Il controllo sulla testa è corretto e sufficiente
    return q->head == NULL; 
}

// Inserisce un elemento in CODA (operazione O(1))
void QUEUEput (QUEUE q, Item val) {
    
    // Caso 1: La coda è vuota
    if (q->head == NULL) {
        // Crea il nuovo nodo, che sarà sia testa che coda
        // q->head è NULL, quindi NEW(val, NULL)
        q->tail = NEW(val, q->head) ; 
        // Sia 'head' che 'tail' puntano all'unico nodo
        q->head = q->tail;
        return;
    }
    
    // Caso 2: La coda ha già elementi
    
    // Aggancia il nuovo nodo in fondo alla lista
    // (q->tail->next è sempre NULL, quindi questo è come NEW(val, NULL))
    q->tail->next = NEW(val, q->tail->next); 
    
    // Sposta il puntatore 'tail' all'elemento appena inserito
    q->tail = q->tail->next;
}

// Estrae un elemento dalla TESTA (operazione O(1))
Item QUEUEget(QUEUE q) {
    // !!! BUG CRITICO 2 (TYPO) !!!
    // Non esiste 'q->head->tmp'. Dovrebbe essere 'q->head->val'.
    // Item tmp = q->head->tmp; 
    Item tmp = q->head->val; // Versione corretta
    
    // Salva il puntatore al *secondo* elemento (che diventerà la nuova testa)
    link t = q->head->next;
    
    // Libera la memoria del nodo che stiamo rimuovendo (la vecchia testa)
    free(q->head); 
    
    // Aggiorna la testa della coda
    q->head = t;
    
    // !!! BUG CRITICO 3 (LOGICA) !!!
    // Se 't' è NULL (cioè abbiamo appena rimosso l'ultimo elemento),
    // 'q->head' è ora NULL, ma 'q->tail' punta ancora alla
    // memoria appena liberata (dangling pointer).
    // Manca il controllo:
    // if (q->head == NULL)
    //     q->tail = NULL;
    
    // Ritorna il valore che era in testa
    return tmp;
}