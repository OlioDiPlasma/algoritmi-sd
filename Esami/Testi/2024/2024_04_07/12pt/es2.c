#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. DEFINIZIONE DEI TIPI (Richiesto dalla traccia) --- */

#define MAX_STR 100 // Lunghezza massima per la stringa
#define MAX_H 200   // Altezza massima stimata dell'albero

// Definizione della struttura del Nodo
typedef struct node {
    char key[MAX_STR];     // Chiave/Valore del nodo
    struct node *left;     // Puntatore al sottoalbero sinistro
    struct node *right;    // Puntatore al sottoalbero destro
} Node;

// Definizione del tipo BST (ADT di prima classe)
typedef Node *BST;

/* --- 2. FUNZIONE AUSILIARIA (HELPER) --- */

/* * Funzione ricorsiva per trovare il percorso verso la foglia migliore.
 * - currPath: tiene traccia del percorso corrente durante la discesa.
 * - bestPath: memorizza il percorso vincente trovato finora.
 * - maxDepth: puntatore alla profondità massima trovata finora.
 */
void findDeepestHelper(BST root, int level, char *currPath[], char *bestPath[], int *maxDepth) {
    if (root == NULL) {
        return;
    }

    // Aggiungo il nodo corrente al percorso temporaneo
    currPath[level] = root->key;

    // Controllo se è una foglia (nessun figlio)
    if (root->left == NULL && root->right == NULL) {
        
        // Caso 1: Trovata una foglia a profondità maggiore del record attuale
        if (level > *maxDepth) {
            *maxDepth = level;
            // Copio l'intero percorso corrente nel percorso "migliore"
            for (int i = 0; i <= level; i++) {
                bestPath[i] = currPath[i];
            }
        }
        // Caso 2: Trovata una foglia alla stessa profondità (Spareggio)
        else if (level == *maxDepth) {
            // Seleziono quella con chiave lessicograficamente maggiore
            // Confronto la chiave corrente con quella della foglia vincente attuale
            if (strcmp(root->key, bestPath[level]) > 0) {
                // Aggiorno il percorso migliore
                for (int i = 0; i <= level; i++) {
                    bestPath[i] = currPath[i];
                }
            }
        }
    }

    // Chiamata ricorsiva sui figli, incrementando il livello
    findDeepestHelper(root->left, level + 1, currPath, bestPath, maxDepth);
    findDeepestHelper(root->right, level + 1, currPath, bestPath, maxDepth);
}

/* --- 3. FUNZIONE RICHIESTA (BSTprintDeepest) --- */

void BSTprintDeepest(BST b) {
    if (b == NULL) {
        return; // Albero vuoto, non stampo nulla
    }

    // Array di puntatori a stringhe per memorizzare i percorsi
    // Usiamo puntatori perché le stringhe sono già allocate nei nodi
    char *currPath[MAX_H];
    char *bestPath[MAX_H];
    
    // Inizializzo la profondità massima a -1
    int maxDepth = -1;

    // Avvio la ricorsione
    findDeepestHelper(b, 0, currPath, bestPath, &maxDepth);

    // Stampa richiesta: a ritroso (dalla foglia MAXF alla radice)
    // Il percorso in bestPath è salvato da 0 (radice) a maxDepth (foglia).
    // Quindi scorriamo l'array all'indietro.
    for (int i = maxDepth; i >= 0; i--) {
        printf("%s", bestPath[i]);
        if (i > 0) {
            printf(" "); // Spazio separatore (o " -> " se preferisci, ma spazio è standard)
        }
    }
    printf("\n");
}