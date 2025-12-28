#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE TIPI DI DATO (ADT DI PRIMA CLASSE)
 * ======================================================================================
 * Concetti Chiave:
 * - ADT di Prima Classe: L'utente maneggia solo un puntatore (HEAP o BT), nascondendo
 * i dettagli interni (information hiding).
 * - Isomorfismo: La struttura dell'albero deve rispecchiare esattamente quella dello heap.
 * Ricorda: In uno heap (array), il figlio sinistro di i è 2*i+1, il destro 2*i+2.
 */

// Definiamo un generico Item (qui un int per semplicità, come spesso accade agli esami)
typedef int Item;

// STRUTTURA HEAP (Rappresentazione vettoriale)
struct heap_struct {
    Item *array; // Vettore dinamico degli elementi
    int size;    // Numero attuale di elementi
    int max_dim; // Dimensione massima allocata (non strettamente necessaria per l'esercizio, ma buona norma)
};
typedef struct heap_struct *HEAP;

// STRUTTURA NODO ALBERO BINARIO
typedef struct node {
    Item item;
    struct node *left;
    struct node *right;
} Node;

// STRUTTURA WRAPPER ALBERO BINARIO (ADT)
struct bt_struct {
    Node *root;  // Puntatore alla radice
    int size;    // Numero di nodi (fondamentale per trovare l'ultima foglia!)
};
typedef struct bt_struct *BT;

/*
 * ======================================================================================
 * 2. FUNZIONI AUSILIARIE (CREAZIONE E STAMPA)
 * ======================================================================================
 * Servono per far funzionare l'esempio, anche se non strettamente richieste dalla traccia.
 */

// Crea un nodo dell'albero
Node* new_node(Item val) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->item = val;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Stampa in-order per verificare l'albero
void printTree(Node *n) {
    if (n == NULL) return;
    printTree(n->left);
    printf("%d ", n->item);
    printTree(n->right);
}

/*
 * ======================================================================================
 * 3. HEAPtoBT: DA ARRAY A ALBERO
 * ======================================================================================
 * Scopo: Costruire un albero binario completo che rispecchi l'array dello heap.
 *
 * Logica:
 * Usiamo una funzione ricorsiva ausiliaria 'buildRecursive'.
 * Dato un indice 'i' nell'array heap:
 * - Se 'i' è fuori dai bordi (>= size), ritorniamo NULL (foglia raggiunta).
 * - Altrimenti, creiamo un nodo con heap->array[i].
 * - Il figlio sinistro sarà costruito ricorsivamente con indice 2*i + 1.
 * - Il figlio destro sarà costruito ricorsivamente con indice 2*i + 2.
 */

// Funzione ricorsiva interna (non visibile all'utente dell'ADT teoricamente)
Node* buildRecursive(Item *array, int index, int size) {
    // Caso Base: Indice fuori dal vettore heap -> Nessun nodo qui
    if (index >= size) {
        return NULL;
    }

    // 1. Alloco il nodo corrente
    Node *curr = new_node(array[index]);

    // 2. Costruisco ricorsivamente i sottoalberi
    // Formula standard Heap: Left = 2*i + 1, Right = 2*i + 2
    curr->left = buildRecursive(array, 2 * index + 1, size);
    curr->right = buildRecursive(array, 2 * index + 2, size);

    return curr;
}

BT HEAPtoBT(HEAP h) {
    // 1. Allocazione del wrapper (ADT di prima classe)
    BT bt = (BT)malloc(sizeof(struct bt_struct));
    
    // 2. Copia della dimensione
    bt->size = h->size;

    // 3. Avvio della costruzione ricorsiva partendo dall'indice 0 (radice)
    if (h->size > 0) {
        bt->root = buildRecursive(h->array, 0, h->size);
    } else {
        bt->root = NULL;
    }

    return bt;
}

/*
 * ======================================================================================
 * 4. BTextractLast: RIMOZIONE DELL'ULTIMA FOGLIA
 * ======================================================================================
 * Scopo: Rimuovere il nodo che corrisponde all'ultimo elemento dell'array heap (indice size-1).
 * * Difficoltà:
 * In un albero fatto di puntatori, non possiamo accedere direttamente a "indice X".
 * Dobbiamo navigare dalla radice fino al genitore dell'ultimo nodo.
 * * Strategia "Backtracking dell'Indice":
 * L'ultimo nodo ha indice N = size-1.
 * Il suo genitore ha indice P = (N-1)/2.
 * Possiamo risalire matematicamente da N fino a 0 per capire il percorso, 
 * memorizzare le mosse, e poi applicarle scendendo nell'albero.
 * * Esempio: Target indice 5.
 * 5 è dispari -> è figlio Sinistro di (5-1)/2 = 2.
 * 2 è pari -> è figlio Destro di (2-1)/2 = 0.
 * Percorso dalla radice (0): Destra -> Sinistra.
 */

Item BTextractLast(BT bt) {
    if (bt->root == NULL || bt->size == 0) {
        // Gestione errore (ritorniamo un valore dummy o gestiamo exit)
        printf("Errore: Albero vuoto\n");
        return -1; 
    }

    // CASO SPECIALE: L'albero ha solo la radice (size = 1)
    if (bt->size == 1) {
        Item val = bt->root->item;
        free(bt->root);
        bt->root = NULL;
        bt->size--;
        return val;
    }

    /* --- FASE 1: CALCOLO DEL PERCORSO --- */
    // L'indice del nodo da rimuovere è l'ultimo (size - 1)
    int target = bt->size - 1;
    
    // Array per memorizzare il percorso. La profondità massima è log2(size).
    // Usiamo un array statico sufficientemente grande per l'esame.
    int moves[100]; 
    int depth = 0;

    // Risaliamo dall'indice target alla radice (0)
    int curr_idx = target;
    while (curr_idx > 0) {
        // Se indice è dispari (es. 1, 3, 5) -> è figlio SINISTRO
        // Se indice è pari (es. 2, 4, 6) -> è figlio DESTRO
        if (curr_idx % 2 != 0) {
            moves[depth++] = 0; // 0 rappresenta SINISTRA
        } else {
            moves[depth++] = 1; // 1 rappresenta DESTRA
        }
        
        // Saliamo al genitore: (i-1)/2
        curr_idx = (curr_idx - 1) / 2;
    }

    /* --- FASE 2: NAVIGAZIONE FINO AL GENITORE --- */
    // Dobbiamo fermarci AL GENITORE del nodo da cancellare.
    // L'ultima mossa nell'array 'moves' ci porta al nodo da cancellare, 
    // quindi iteriamo fino a depth-1.
    // Attenzione: l'array moves è stato riempito dal basso verso l'alto (stack),
    // dobbiamo leggerlo al contrario!
    
    Node *parent = bt->root;
    
    // Scendiamo nell'albero leggendo le mosse dall'ultima inserita (depth-1) fino alla penultima (1)
    // L'indice 0 di 'moves' contiene l'ultimo passo (figlio da cancellare).
    for (int i = depth - 1; i > 0; i--) {
        if (moves[i] == 0) {
            parent = parent->left;
        } else {
            parent = parent->right;
        }
    }

    /* --- FASE 3: RIMOZIONE --- */
    // Ora 'parent' è il genitore del nodo da rimuovere.
    // L'ultima mossa (moves[0]) ci dice se rimuovere il sinistro o il destro.
    Node *to_delete = NULL;
    Item val;

    if (moves[0] == 0) { // Il nodo da cancellare è a SINISTRA
        to_delete = parent->left;
        parent->left = NULL; // Scolleghiamo il puntatore
    } else {             // Il nodo da cancellare è a DESTRA
        to_delete = parent->right;
        parent->right = NULL; // Scolleghiamo il puntatore
    }

    val = to_delete->item; // Salviamo il dato
    free(to_delete);       // Liberiamo la memoria
    bt->size--;            // Decrementiamo la dimensione dell'ADT

    return val;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. CREAZIONE HEAP FITTIZIO --- */
    // Creiamo un heap manualmente simulando un array
    int size = 6;
    Item arr[] = {10, 5, 3, 1, 4, 2}; 
    // Struttura logica attesa:
    //      10
    //    /    \
    //   5      3
    //  / \    /
    // 1   4  2
    
    // Allocazione struct HEAP
    HEAP h = (HEAP)malloc(sizeof(struct heap_struct));
    h->array = arr; // Puntiamo direttamente all'array statico per test
    h->size = size;

    printf("--- TEST 1: HEAPtoBT ---\n");
    BT albero = HEAPtoBT(h);
    
    printf("Visita In-Order dell'albero creato (atteso sorted se fosse BST, ma è un HEAP generico):\n");
    printTree(albero->root); // Output atteso (in-order): 1 5 4 10 2 3
    printf("\nSize albero: %d\n", albero->size);

    printf("\n--- TEST 2: BTextractLast ---\n");
    // L'ultimo elemento inserito (indice 5) è il valore '2'.
    // È figlio sinistro di 3.
    Item removed = BTextractLast(albero);
    printf("Elemento rimosso: %d (Atteso: 2)\n", removed);
    printf("Size albero post-rimozione: %d\n", albero->size);
    
    printf("Visita In-Order dopo rimozione:\n");
    printTree(albero->root); 
    printf("\n");

    // Pulizia finale (solo BT, l'heap usava array statico)
    // Nota: servirebbe una funzione freeTree ricorsiva per fare le cose per bene.
    free(albero->root->left->right); // 4
    free(albero->root->left->left);  // 1
    free(albero->root->left);        // 5
    free(albero->root->right);       // 3 (il 2 è già stato liberato)
    free(albero->root);              // 10
    free(albero);
    free(h);

    return 0;
}