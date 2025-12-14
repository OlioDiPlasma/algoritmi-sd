#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE STRUTTURE DATI (ADT BST)
 * ======================================================================================
 */

typedef int Item;

typedef struct BSTnode {
    Item item;
    struct BSTnode *l; // left child
    struct BSTnode *r; // right child
} BSTnode;

typedef BSTnode* link;

// Wrapper dell'ADT
struct binary_search_tree {
    link root;
    int size; // Non strettamente necessario per questo esercizio, ma fa parte dell'ADT
};
typedef struct binary_search_tree *BST;

/*
 * ======================================================================================
 * FUNZIONE AUSILIARIA: checkOrderRecursive
 * ======================================================================================
 * Scopo:
 * Esegue una visita in-order (simmetrica) tenendo traccia dell'ultimo valore visitato.
 * Se incontriamo un valore che non è maggiore del precedente, la proprietà BST è violata.
 *
 * Parametri:
 * - link h: Il nodo corrente.
 * - Item *last_val: Puntatore all'ultimo valore valido visitato.
 * - int *has_seen_first: Flag per gestire il primo nodo (che non ha un "precedente").
 *
 * Ritorna:
 * - 1 se il sottoalbero è valido, 0 se è invalido.
 */
int checkOrderRecursive(link h, Item *last_val, int *has_seen_first) {
    // CASO BASE: Albero vuoto è un BST valido
    if (h == NULL) {
        return 1;
    }

    /* --- 1. VISITA SINISTRA (Ricorsione) --- */
    // Se il sottoalbero sinistro è invalido, tutto l'albero è invalido.
    if (!checkOrderRecursive(h->l, last_val, has_seen_first)) {
        return 0;
    }

    /* --- 2. ANALISI NODO CORRENTE --- */
    // Se non è il primissimo nodo che visitiamo...
    if (*has_seen_first) {
        // ...controlliamo che il valore attuale sia MAGGIORE del precedente.
        // Se è minore o uguale, la sequenza non è crescente -> NON È UN BST.
        if (h->item <= *last_val) {
            return 0;
        }
    }
    
    // Aggiorniamo il "passato": il nodo corrente diventa l'ultimo visitato
    *last_val = h->item;
    *has_seen_first = 1; // Da ora in poi abbiamo un "precedente" con cui confrontarci

    /* --- 3. VISITA DESTRA (Ricorsione) --- */
    // Controlliamo il sottoalbero destro
    if (!checkOrderRecursive(h->r, last_val, has_seen_first)) {
        return 0;
    }

    // Se siamo arrivati qui, è tutto ok
    return 1;
}

/*
 * ======================================================================================
 * FUNZIONE PRINCIPALE: BSTcheckOrder
 * ======================================================================================
 * Scopo:
 * Interfaccia pubblica che prepara le variabili di stato e lancia la ricorsione.
 *
 * Concetti Chiave:
 * - Passaggio per indirizzo (&last_val): La funzione ricorsiva deve aggiornare la variabile
 * 'last_val' in modo che le modifiche siano visibili alle chiamate successive.
 */
int BSTcheckOrder(BST b) {
    // Controllo robustezza: se il puntatore all'ADT è NULL
    if (b == NULL) return 0; 
    
    // Variabili di stato per la ricorsione
    Item last_val = 0; // Valore dummy iniziale
    int has_seen_first = 0; // Flag: "abbiamo già visto almeno un nodo?"

    return checkOrderRecursive(b->root, &last_val, &has_seen_first);
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO (PER IL TEST)
 * ======================================================================================
 */
link newNode(Item val, link l, link r) {
    link x = malloc(sizeof *x);
    x->item = val; 
    x->l = l; 
    x->r = r;
    return x;
}

int main() {
    /* --- TEST 1: COSTRUZIONE DI UN VERO BST --- */
    //       10
    //      /  \
    //     5    20
    //         /  \
    //        15   30
    link rootValid = newNode(10, 
                        newNode(5, NULL, NULL), 
                        newNode(20, 
                            newNode(15, NULL, NULL), 
                            newNode(30, NULL, NULL)
                        )
                     );
    
    BST bValid = malloc(sizeof *bValid);
    bValid->root = rootValid;

    printf("Test BST Valido: %s\n", BSTcheckOrder(bValid) ? "OK (1)" : "ERRORE (0)");

    /* --- TEST 2: COSTRUZIONE DI UN FALSO BST (Violazione profonda) --- */
    // L'errore comune: 6 è minore di 20 (ok localmente), ma è nel sottoalbero DESTRO di 10!
    //       10
    //      /  \
    //     5    20
    //         /
    //        6  <-- VIOLAZIONE! (6 < 10, non dovrebbe stare a destra del 10)
    link rootInvalid = newNode(10, 
                        newNode(5, NULL, NULL), 
                        newNode(20, 
                            newNode(6, NULL, NULL), // Il 6 rompe la proprietà rispetto al 10
                            NULL
                        )
                       );

    BST bInvalid = malloc(sizeof *bInvalid);
    bInvalid->root = rootInvalid;

    printf("Test BST Invalido: %s\n", BSTcheckOrder(bInvalid) ? "ERRORE (1)" : "OK, Rilevato (0)");

    // Pulizia memoria (omessa per brevità ma necessaria in produzione)
    free(bValid);
    free(bInvalid);
    
    return 0;
}