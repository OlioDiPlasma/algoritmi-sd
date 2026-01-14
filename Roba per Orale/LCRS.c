#include <stdio.h>
#include <stdlib.h>

// --- STRUTTURA DATI (LCRS) ---
typedef struct Node {
    int data;
    struct Node *firstChild;  // Punta al PRIMO figlio
    struct Node *nextSibling; // Punta al PROSSIMO fratello
    
    // Campo richiesto dalla domanda: "riempire un campo che indica il numero di figli"
    int numChildren; 
} Node;

// --- FUNZIONI DI SUPPORTO ---

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->numChildren = 0; // Inizialmente 0
    return newNode;
}

// Aggiunge un figlio al nodo 'parent'
void addChild(struct Node* parent, struct Node* child) {
    if (parent->firstChild == NULL) {
        // Se non ha figli, questo diventa il primo
        parent->firstChild = child;
    } else {
        // Se ha già figli, scorri i fratelli del primo figlio fino alla fine
        struct Node* temp = parent->firstChild;
        while (temp->nextSibling != NULL) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = child; // Appendi in coda alla lista dei fratelli
    }
}

// --- 1. CONTA NODI TOTALI ---
// La logica ricorsiva deve visitare sia il ramo 'child' che il ramo 'sibling'
int countTotalNodes(struct Node* root) {
    if (root == NULL) return 0;
    
    // 1 (me stesso) + tutto ciò che sta sotto (child) + tutto ciò che sta a destra (sibling)
    // Nota: Il sibling NON è mio figlio, ma fa parte dell'albero totale.
    return 1 + countTotalNodes(root->firstChild) + countTotalNodes(root->nextSibling);
}

// --- 2. CONTA NODI AL LIVELLO K ---
// Qui bisogna fare attenzione: 
// - Andare a firstChild AUMENTA il livello (scendo).
// - Andare a nextSibling MANTIENE il livello (sono fratelli).
int countNodesAtLevel(struct Node* root, int k) {
    if (root == NULL) return 0;
    
    int count = 0;
    
    // Se k == 0, ho trovato un nodo al livello giusto
    if (k == 0) {
        count = 1;
    }
    
    // Ricorsione:
    // 1. Scendo dal figlio: livello diventa k-1
    // 2. Vado dal fratello: livello resta k (il fratello è al mio stesso livello)
    
    // IMPORTANTE: Se k=0, non scendo nei figli (sarebbero livello -1), 
    // ma DEVO visitare i fratelli perché sono anche loro a livello 0!
    
    if (k == 0) {
        return 1 + countNodesAtLevel(root->nextSibling, 0);
    } else {
        return countNodesAtLevel(root->firstChild, k - 1) + 
               countNodesAtLevel(root->nextSibling, k);
    }
}

// --- 3. CALCOLO GRADI E RIEMPIMENTO CAMPO ---
// Riempie il campo numChildren e restituisce il grado massimo dell'albero
int calculateDegreesAndGetMax(struct Node* root) {
    if (root == NULL) return 0;
    
    // 1. Calcola i figli diretti di QUESTO nodo
    int currentDegree = 0;
    struct Node* child = root->firstChild;
    while (child != NULL) {
        currentDegree++;
        child = child->nextSibling;
    }
    
    // Salva nel campo della struct
    root->numChildren = currentDegree;
    
    // 2. Calcola ricorsivamente il max nei sottoalberi
    int maxDeg = currentDegree;
    
    // Visita primo figlio (e tutto il suo sottoalbero)
    int maxChild = calculateDegreesAndGetMax(root->firstChild);
    if (maxChild > maxDeg) maxDeg = maxChild;
    
    // Visita fratello (e tutto il suo sottoalbero)
    int maxSibling = calculateDegreesAndGetMax(root->nextSibling);
    if (maxSibling > maxDeg) maxDeg = maxSibling;
    
    return maxDeg;
}

// --- 4. CAMMINI DI LUNGHEZZA DISPARI (Radice -> Foglia) ---
// Restituisce il numero di cammini dalla radice alle foglie con lunghezza dispari.
// Lunghezza = numero di archi.
// currentLength = archi attraversati finora dalla radice 'reale' dell'albero.
int countOddPaths(struct Node* root, int currentLength) {
    if (root == NULL) return 0;

    int count = 0;

    // Verifica se è una foglia (in LCRS: non ha firstChild)
    if (root->firstChild == NULL) {
        if (currentLength % 2 != 0) {
            count = 1; // Trovato un cammino dispari
        }
    } else {
        // Se non è foglia, scendi nel figlio (lunghezza aumenta)
        count += countOddPaths(root->firstChild, currentLength + 1);
    }

    // Visita i fratelli
    // IMPORTANTE: Quando passo al fratello, la lunghezza del cammino DALLA RADICE 
    // non cambia, perché il fratello condivide lo stesso genitore.
    // Quindi passo 'currentLength' invariato.
    count += countOddPaths(root->nextSibling, currentLength);

    return count;
}

// --- MAIN ---
int main() {
    /* Costruiamo questo albero N-ario:
           1
         / | \
        2  3  4
       / \    |
      5   6   7
    */

    struct Node* root = createNode(1);
    
    struct Node* n2 = createNode(2);
    struct Node* n3 = createNode(3);
    struct Node* n4 = createNode(4);
    
    struct Node* n5 = createNode(5);
    struct Node* n6 = createNode(6);
    struct Node* n7 = createNode(7);

    // Colleghiamo livello 1 (figli di 1)
    addChild(root, n2);
    addChild(root, n3);
    addChild(root, n4);

    // Colleghiamo livello 2
    addChild(n2, n5); // Figli di 2
    addChild(n2, n6);
    addChild(n4, n7); // Figli di 4

    printf("--- Analisi Albero N-ario (LCRS) ---\n");

    // 1. Totale Nodi
    printf("Totale nodi: %d (Atteso: 7)\n", countTotalNodes(root));

    // 2. Nodi a livello K
    // Livello 0: {1} -> 1
    // Livello 1: {2, 3, 4} -> 3
    // Livello 2: {5, 6, 7} -> 3
    int k = 1;
    printf("Nodi al livello %d: %d (Atteso: 3)\n", k, countNodesAtLevel(root, k));

    // 3. Grado Massimo e riempimento
    int maxD = calculateDegreesAndGetMax(root);
    printf("Grado massimo dell'albero: %d (Atteso: 3, nodo radice)\n", maxD);
    printf("Grado del nodo 2: %d (Atteso: 2)\n", n2->numChildren);
    printf("Grado del nodo 3: %d (Atteso: 0)\n", n3->numChildren);

    // 4. Cammini Dispari
    // Percorsi (archi):
    // 1->3 (L=1, Dispari) -> OK
    // 1->2->5 (L=2, Pari)
    // 1->2->6 (L=2, Pari)
    // 1->4->7 (L=2, Pari)
    // Totale atteso: 1
    printf("Numero cammini radice-foglia dispari: %d\n", countOddPaths(root, 0));

    return 0;
}