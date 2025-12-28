#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE STRUTTURA DATI
 * ============================================================================
 * Definiamo il nodo dell'albero binario di ricerca (BST).
 */
typedef struct nnode {
    int key;                // Valore intero (chiave) del nodo
    struct nnode *left;     // Puntatore al sottoalbero sinistro (chiavi minori)
    struct nnode *right;    // Puntatore al sottoalbero destro (chiavi maggiori)
} nnode_t;

/* ============================================================================
 * FUNZIONE AUSILIARIA: distFromRoot
 * ============================================================================
 * SCOPO:
 * Calcola la distanza (numero di archi) da un nodo specifico 'root' fino
 * a un nodo con chiave 'target'.
 *
 * PARAMETRI:
 * - root:   Il nodo da cui partire (la radice del sottoalbero corrente).
 * - target: La chiave da cercare.
 *
 * NOTE:
 * Assumiamo che la chiave 'target' esista nel sottoalbero, come da prassi
 * negli esercizi teorici semplificati.
 * ============================================================================
 */
int distFromRoot(nnode_t *root, int target) {
    /* --- 1. CASO BASE --- */
    /* Se abbiamo trovato il nodo, la distanza da se stesso è 0. */
    if (root->key == target) {
        return 0;
    }

    /* --- 2. DISCESA RICORSIVA (Sfruttando proprietà BST) --- */
    /* Se il target è minore della chiave corrente, andiamo a sinistra. */
    if (target < root->key) {
        /* Concetto Chiave: 1 + ...
           Ogni volta che scendiamo di un livello, aggiungiamo 1 alla distanza. */
        return 1 + distFromRoot(root->left, target);
    } 
    /* Altrimenti, andiamo a destra. */
    else {
        return 1 + distFromRoot(root->right, target);
    }
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: distance
 * ============================================================================
 * SCOPO:
 * Calcola il numero di archi necessari per andare dal nodo con chiave key1
 * al nodo con chiave key2 (o viceversa).
 *
 * PARAMETRI:
 * - root: Puntatore alla radice dell'albero.
 * - key1: Prima chiave intera.
 * - key2: Seconda chiave intera.
 *
 * CONCETTI CHIAVE:
 * 1. Proprietà BST: In un BST, tutti i nodi a sinistra sono minori della radice,
 * tutti i nodi a destra sono maggiori.
 * 2. LCA (Lowest Common Ancestor): Dobbiamo trovare il nodo in cui i percorsi
 * per key1 e key2 si separano. Fino a quel punto, il percorso è comune e
 * non contribuisce alla distanza "tra" i due nodi.
 * La distanza totale sarà: (distanza LCA -> key1) + (distanza LCA -> key2).
 * ============================================================================
 */
int distance(nnode_t *root, int key1, int key2) {
    
    /* --- 1. CONTROLLI DI SICUREZZA --- */
    if (root == NULL) {
        return 0; // Albero vuoto o chiavi non trovate
    }

    /* --- 2. RICERCA DEL PUNTO DI DIVERGENZA (LCA) --- */
    
    /* CASO A: Entrambe le chiavi sono più piccole della radice corrente.
       Significa che il percorso comune continua nel sottoalbero sinistro.
       Non ci siamo ancora "separati". Ricorsione a sinistra. */
    if (key1 < root->key && key2 < root->key) {
        return distance(root->left, key1, key2);
    }
    
    /* CASO B: Entrambe le chiavi sono più grandi della radice corrente.
       Il percorso comune continua nel sottoalbero destro. Ricorsione a destra. */
    else if (key1 > root->key && key2 > root->key) {
        return distance(root->right, key1, key2);
    }
    
    /* CASO C: PUNTO DI DIVERGENZA TROVATO!
       Siamo qui se:
       1. Una chiave è a sinistra e l'altra a destra.
       2. Oppure una delle due chiavi è PROPRIO la radice corrente.
       
       In questo momento 'root' è il Minimo Antenato Comune (LCA).
       La distanza totale è la somma della distanza dalla radice a key1
       più la distanza dalla radice a key2. */
    else {
        return distFromRoot(root, key1) + distFromRoot(root, key2);
    }
}

/* ============================================================================
 * FUNZIONI PER IL TESTING (Creazione e Deallocazione)
 * ============================================================================
 */

/* Crea un nuovo nodo allocando memoria */
nnode_t* nuovoNodo(int key) {
    /* MALLOC: Allochiamo byte pari alla dimensione della struct. */
    nnode_t* node = (nnode_t*)malloc(sizeof(nnode_t));
    
    /* Assegniamo i valori. -> è l'operatore per accedere ai campi tramite puntatore. */
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Inserimento standard in un BST (per costruire l'albero di prova) */
nnode_t* insert(nnode_t* node, int key) {
    if (node == NULL) return nuovoNodo(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    return node;
}

/* Deallocazione ricorsiva (Post-Order) per evitare memory leaks */
void freeTree(nnode_t* root) {
    if (root == NULL) return;
    freeTree(root->left);  // Libera sottoalbero sinistro
    freeTree(root->right); // Libera sottoalbero destro
    free(root);            // Libera il nodo corrente
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* Creiamo un albero di esempio:
            20
           /  \
         10    30
        /  \
       5   15
    */
    nnode_t *root = NULL;
    root = insert(root, 20);
    insert(root, 10);
    insert(root, 30);
    insert(root, 5);
    insert(root, 15);

    printf("--- TEST FUNZIONE DISTANCE ---\n");
    printf("Albero creato:\n    20\n   /  \\\n 10    30\n /  \\\n5   15\n\n");

    /* TEST 1: Distanza tra fratelli (5 e 15)
       LCA è 10. Distanza 10->5 è 1. Distanza 10->15 è 1. Totale = 2. */
    int k1 = 5, k2 = 15;
    printf("Distanza tra %d e %d: %d (Atteso: 2)\n", k1, k2, distance(root, k1, k2));

    /* TEST 2: Distanza tra nodo e suo nonno (5 e 20)
       LCA è 20. Distanza 20->20 è 0. Distanza 20->5 è 2. Totale = 2. */
    k1 = 5; k2 = 20;
    printf("Distanza tra %d e %d: %d (Atteso: 2)\n", k1, k2, distance(root, k1, k2));

    /* TEST 3: Distanza attraverso la radice principale (5 e 30)
       LCA è 20. Distanza 20->5 è 2. Distanza 20->30 è 1. Totale = 3. */
    k1 = 5; k2 = 30;
    printf("Distanza tra %d e %d: %d (Atteso: 3)\n", k1, k2, distance(root, k1, k2));

    /* Pulizia memoria */
    freeTree(root);
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}