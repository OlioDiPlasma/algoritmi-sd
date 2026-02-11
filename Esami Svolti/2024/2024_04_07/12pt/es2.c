#include <stdio.h>
#include <stdlib.h>
#include <string.h>


///////////////// .H //////////////////

/* * Definizione del tipo BST come puntatore a una struct "node" 
 * la cui definizione rimane nascosta nel file .c.
 * Questo garantisce l'incapsulamento (ADT di prima classe).
 */
typedef struct node *BST;

/* Prototipo della funzione richiesta dalla traccia */
void BSTprintDeepest(BST b);

///////////////////////////// .C /////////////////////////77

/* * Definizione reale della struttura. 
 * Solo le funzioni in questo file possono accedere a ->key, ->left e ->right.
 */
struct node {
    char *key;           // Puntatore a stringa (allocazione dinamica consigliata)
    struct node *left;
    struct node *right;
};

/* * FUNZIONE AUSILIARIA: getBSTHeight
 * Scopo: Calcola l'altezza massima dell'albero per allocare i percorsi.
 */
static int getBSTHeight(BST b) {
    if (b == NULL) return 0;
    int lh = getBSTHeight(b->left);
    int rh = getBSTHeight(b->right);
    // L'altezza è 1 (il nodo corrente) + il massimo tra i due rami
    return 1 + (lh > rh ? lh : rh);
}

/* * FUNZIONE AUSILIARIA: findDeepestHelper
 * Scopo: Esplora ricorsivamente l'albero cercando il cammino verso la foglia migliore.
 */
static void findDeepestHelper(BST root, int level, char **currPath, char **bestPath, int *maxDepth) {
    if (root == NULL) return;

    // "DO": Aggiungo il nodo corrente al cammino che sto costruendo
    currPath[level] = root->key;

    // CONTROLLO FOGLIA: Se non ha figli, siamo arrivati alla fine di un ramo
    if (root->left == NULL && root->right == NULL) {
        
        // CONDIZIONE A: Ho trovato una foglia più profonda del record attuale?
        // CONDIZIONE B: Stessa profondità, ma la chiave è alfabeticamente maggiore (strcmp > 0)?
        if (level > *maxDepth || (level == *maxDepth && strcmp(root->key, bestPath[level]) > 0)) {
            
            *maxDepth = level; // Aggiorno il record di profondità
            
            // Copio i puntatori delle stringhe nel cammino "best"
            for (int i = 0; i <= level; i++) {
                bestPath[i] = currPath[i];
            }
        }
    }

    // "RECURSE": Esploro i rami sottostanti
    findDeepestHelper(root->left, level + 1, currPath, bestPath, maxDepth);
    findDeepestHelper(root->right, level + 1, currPath, bestPath, maxDepth);
}

/* * FUNZIONE PRINCIPALE: BSTprintDeepest
 */
void BSTprintDeepest(BST b) {
    if (b == NULL) return; // Difesa: albero vuoto

    // 1. Fase di preparazione: calcolo l'altezza per sapere quanto spazio serve
    int h = getBSTHeight(b);

    // 2. Allocazione dinamica degli array di supporto (vettori di puntatori a char)
    char **currPath = malloc(h * sizeof(char *));
    char **bestPath = malloc(h * sizeof(char *));
    int maxDepth = -1;

    if (currPath == NULL || bestPath == NULL) {
        fprintf(stderr, "Errore di allocazione memoria\n");
        return;
    }

    // 3. Lancio la ricerca ricorsiva
    findDeepestHelper(b, 0, currPath, bestPath, &maxDepth);

    // 4. Stampa a ritroso: dalla foglia (maxDepth) fino alla radice (0)
    for (int i = maxDepth; i >= 0; i--) {
        printf("%s", bestPath[i]);
        // Aggiungo uno spazio solo se non è l'ultimo elemento della stampa
        if (i > 0) printf(" ");
    }
    printf("\n");

    // 5. CLEANUP: Libero la memoria allocata in questa funzione
    // Nota: non liberiamo bestPath[i] perché sono stringhe che appartengono ai nodi!
    free(currPath);
    free(bestPath);
}