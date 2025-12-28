#include <stdio.h>
#include <stdlib.h> // Necessario SOLO per malloc e free (esplicitamente richieste per allocazione dinamica)

/* ----------------------------------------------------- */
/* --- DEFINIZIONE DELLE STRUTTURE (Richieste) --- */
/* ----------------------------------------------------- */

// Struttura del Nodo con intero
typedef struct node {
    int val;               // Valore intero
    struct node *left;     // Sottoalbero sx (valori minori)
    struct node *right;    // Sottoalbero dx (valori maggiori)
} Node;

// Definizione del tipo BST (ADT di prima classe)
typedef Node *BST;

/*
 * Helper 1: Conta i nodi.
 * Necessario perché il testo chiede allocazione "senza riallocazione".
 * Dobbiamo sapere la dimensione esatta PRIMA di creare il vettore.
 */
int countNodes(BST root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

/*
 * Helper 2: Calcola l'altezza dell'albero.
 * Ci serve per sapere quanti livelli dobbiamo esplorare nel ciclo principale.
 */
int treeHeight(BST root) {
    if (root == NULL) return -1; // Altezza albero vuoto è -1 (o 0, dipende dalle convenzioni, qui usiamo -1 per coerenza con depth 0)
    
    int hLeft = treeHeight(root->left);
    int hRight = treeHeight(root->right);
    
    // Implementazione manuale di max()
    if (hLeft > hRight) return 1 + hLeft;
    else return 1 + hRight;
}

/*
 * Helper 3: "Raccoglitore" per un livello specifico.
 * Scopo: Attraversa l'albero e, quando trova un nodo alla 'target_depth',
 * lo inserisce nell'array risultato.
 *
 * Concetto Chiave: Ordine di Visita
 * Visitando prima Left e poi Right, garantiamo che a parità di livello
 * i nodi siano inseriti in ordine crescente di valore (proprietà BST).
 *
 * Parametri:
 * - BST root: nodo corrente
 * - int current_depth: profondità attuale nella ricorsione
 * - int target_depth: il livello che stiamo cercando di raccogliere
 * - Node **resultArray: il vettore destinazione
 * - int *index: puntatore all'indice corrente del vettore (passato per riferimento per avanzare)
 */
void getNodesAtLevel(BST root, int current_depth, int target_depth, Node **resultArray, int *index) {
    if (root == NULL) return;

    // Se siamo al livello giusto, salviamo il PUNTATORE AL NODO
    if (current_depth == target_depth) {
        // resultArray è un array di (Node*), quindi resultArray[*index] è un (Node*)
        resultArray[*index] = root;
        (*index)++; // Incremento l'indice per il prossimo inserimento
        return;     // Non serve scendere oltre
    }

    // Se non siamo ancora al livello, scendiamo.
    // IMPORTANTE: Prima sinistra, poi destra per mantenere l'ordinamento dei valori.
    getNodesAtLevel(root->left, current_depth + 1, target_depth, resultArray, index);
    getNodesAtLevel(root->right, current_depth + 1, target_depth, resultArray, index);
}

/*
 * Scopo: Genera un vettore di puntatori ai nodi ordinato per profondità crescente
 * e, secondariamente, per valore crescente.
 *
 * Parametri:
 * - BST b: L'albero in input.
 * - int *n: Puntatore a intero dove scrivere la dimensione finale del vettore.
 *
 * Ritorno:
 * - Node**: Un puntatore a un array di puntatori a Node.
 */
Node **BSTlevelizedNodes(BST b, int *n) {
    
    /* --- 1. CALCOLO DIMENSIONE E ALLOCAZIONE --- */
    // Calcoliamo il numero totale di nodi per allocare esattamente lo spazio necessario.
    int numNodes = countNodes(b);
    
    // Salviamo il numero di nodi nel parametro di output
    *n = numNodes;

    // Se l'albero è vuoto, ritorniamo NULL
    if (numNodes == 0) return NULL;

    // Allocazione dinamica del vettore di puntatori (Node *)
    // sizeof(Node *) perché ogni cella contiene un indirizzo di memoria.
    Node **pnodes = (Node **)malloc(numNodes * sizeof(Node *));
    if (pnodes == NULL) {
        *n = 0;
        return NULL; // Errore allocazione
    }

    /* --- 2. LOGICA DI RIEMPIMENTO (Senza QSort) --- */
    // Calcoliamo l'altezza massima per sapere quante iterazioni fare
    int h = treeHeight(b);
    
    // Indice progressivo per riempire l'array 'pnodes'
    int currentIndex = 0;

    // Ciclo per ogni livello (Profondità crescente: 0, 1, 2, ... h)
    for (int i = 0; i <= h; i++) {
        // Per ogni livello 'i', raccogliamo tutti i nodi e li mettiamo nell'array
        // L'indice 'currentIndex' viene aggiornato automaticamente dalla funzione helper
        getNodesAtLevel(b, 0, i, pnodes, &currentIndex);
    }

    // A questo punto 'pnodes' è ordinato per profondità (grazie al ciclo for esterno)
    // e per valori (grazie alla visita sinistra-destra della helper su un BST).

    return pnodes;
}

/* ==========================================
   MAIN DI TEST
   ========================================== */

// Helper per inserire nodi nel BST (solo per costruire il test)
BST insert(BST root, int val) {
    if (root == NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->val = val;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (val < root->val) root->left = insert(root->left, val);
    else if (val > root->val) root->right = insert(root->right, val);
    return root;
}

void freeTree(BST root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    BST root = NULL;

    /*
       Costruiamo questo albero:
             10        (Liv 0)
            /  \
           5    15     (Liv 1)
          / \     \
         2   7    20   (Liv 2)
    */
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 2);
    root = insert(root, 7);
    root = insert(root, 20);

    int n = 0;
    printf("Chiamata a BSTlevelizedNodes...\n");
    Node **result = BSTlevelizedNodes(root, &n);

    printf("Numero nodi trovati: %d\n", n);
    printf("Risultato (Atteso: 10 -> 5 -> 15 -> 2 -> 7 -> 20):\n");
    
    for (int i = 0; i < n; i++) {
        // result[i] è un puntatore al nodo, quindi accediamo a ->val
        printf("%d ", result[i]->val);
    }
    printf("\n");

    // Pulizia finale
    if (result != NULL) free(result); // Libero l'array dei puntatori
    freeTree(root); // Libero l'albero

    return 0;
}