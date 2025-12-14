#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * DEFINIZIONE TIPI DI DATO (ADT)
 * ============================================================================
 * Il problema richiede la definizione del tipo BST come ADT di prima classe
 * e del tipo per il nodo.
 */

/* * Struttura del NODO.
 * Contiene la stringa (chiave), e i puntatori ai sottoalberi sinistro e destro.
 */
typedef struct node {
    char *val;          // Stringa allocata dinamicamente
    struct node *left;
    struct node *right;
} node_t;

/* * Struttura del BST (Wrapper).
 * È buona norma incapsulare la radice in una struttura. Questo è l'ADT di prima classe.
 * In questo modo, passiamo 'BST' per valore (che è un puntatore alla struct wrapper),
 * ma possiamo modificare la radice se necessario (non richiesto qui, ma buona prassi).
 */
typedef struct binary_search_tree {
    node_t *root;
} *BST; 
/* Nota: Qui definiamo BST come puntatore a struct binary_search_tree. 
   Quindi quando scriveremo 'BST b', 'b' sarà un puntatore. */

/* ============================================================================
 * FUNZIONI DI SUPPORTO (Non richieste esplicitamente ma necessarie)
 * ============================================================================
 */

/* Creazione di un nuovo nodo */
node_t* nuovoNodo(char *str) {
    node_t *n = (node_t*)malloc(sizeof(node_t));
    /* strdup: alloca memoria per la stringa e copia i caratteri. 
       Equivale a malloc + strcpy */
    n->val = strdup(str); 
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* Inserimento classico in un BST (usato solo per costruire l'albero di test) */
void insert(node_t **n, char *str) {
    if (*n == NULL) {
        *n = nuovoNodo(str);
        return;
    }
    if (strcmp(str, (*n)->val) < 0)
        insert(&((*n)->left), str);
    else
        insert(&((*n)->right), str);
}

/* ============================================================================
 * LOGICA DEL PROBLEMA
 * ============================================================================
 * Strategia:
 * 1. Dobbiamo identificare QUALE è la foglia target. Per farlo serve una visita
 * completa dell'albero per trovare quella con profondità massima (e chiave max).
 * 2. Una volta identificata la chiave della foglia, dobbiamo stampare il percorso.
 * Poiché dobbiamo stampare da FOGLIA a RADICE, sfrutteremo la risalita 
 * della ricorsione (post-order logico).
 */

/* --- HELPER 1: TROVA LA FOGLIA TARGET --- 
 * Scopo: Visita l'albero e aggiorna la chiave migliore trovata finora.
 * Parametri:
 * - root: nodo corrente
 * - depth: profondità del nodo corrente
 * - maxDepth: puntatore al valore massimo di profondità trovato finora (input/output)
 * - bestKey: puntatore alla stringa della foglia migliore (input/output)
 */
void trovaFogliaDeepest(node_t *root, int depth, int *maxDepth, char **bestKey) {
    if (root == NULL) return;

    /* Verifica se è una foglia */
    if (root->left == NULL && root->right == NULL) {
        /*
         * CONDIZIONE 1: Nuova profondità massima trovata.
         * Aggiorniamo incondizionatamente.
         */
        if (depth > *maxDepth) {
            *maxDepth = depth;
            *bestKey = root->val; // Salviamo il puntatore alla stringa
        }
        /*
         * CONDIZIONE 2: Stessa profondità massima (Pareggio).
         * Dobbiamo scegliere la chiave lessicograficamente maggiore (strcmp > 0).
         */
        else if (depth == *maxDepth) {
            if (strcmp(root->val, *bestKey) > 0) {
                *bestKey = root->val;
            }
        }
    }

    /* Ricorsione sui figli aumentando la profondità */
    trovaFogliaDeepest(root->left, depth + 1, maxDepth, bestKey);
    trovaFogliaDeepest(root->right, depth + 1, maxDepth, bestKey);
}

/* --- HELPER 2: STAMPA PERCORSO A RITROSO --- 
 * Scopo: Cerca il nodo target. Quando lo trova, inizia a stampare tornando su.
 * Ritorna: 1 se il target è stato trovato in questo sottoalbero, 0 altrimenti.
 */
int stampaPercorsoInverso(node_t *root, char *targetKey) {
    if (root == NULL) return 0;

    /* * Logica di Ricerca BST standard per trovare il nodo.
     * Dato che è un BST, sappiamo esattamente dove andare (O(h)).
     */
    int found = 0;
    
    if (strcmp(targetKey, root->val) == 0) {
        found = 1; /* Trovato! */
    } else if (strcmp(targetKey, root->val) < 0) {
        found = stampaPercorsoInverso(root->left, targetKey);
    } else {
        found = stampaPercorsoInverso(root->right, targetKey);
    }

    /* * CONCETTO CHIAVE: Srotolamento della Ricorsione.
     * Se 'found' è 1, significa che questo nodo fa parte del cammino
     * che porta alla foglia target.
     * Stampiamo il nodo ORA (mentre la funzione sta per terminare e ritornare al padre).
     * Questo garantisce l'ordine Foglia -> ... -> Radice.
     */
    if (found) {
        printf("%s ", root->val);
    }

    return found;
}

/* ============================================================================
 * FUNZIONE RICHIESTA: BSTprintDeepest
 * ============================================================================
 */
void BSTprintDeepest(BST b) {
    /* Controllo sicurezza albero vuoto */
    if (b == NULL || b->root == NULL) {
        printf("Albero vuoto.\n");
        return;
    }

    /* --- FASE 1: Identificazione --- */
    int currentMaxDepth = -1;
    char *targetKey = NULL;

    /* Passiamo gli indirizzi di maxDepth e targetKey perché la funzione
       helper deve modificarli */
    trovaFogliaDeepest(b->root, 0, &currentMaxDepth, &targetKey);

    printf("Foglia scelta: %s (Profondita': %d)\n", targetKey, currentMaxDepth);
    printf("Percorso inverso: ");

    /* --- FASE 2: Stampa --- */
    if (targetKey != NULL) {
        stampaPercorsoInverso(b->root, targetKey);
    }
    printf("\n");
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
/* Helper per deallocare la memoria (non richiesto ma educato) */
void freeTree(node_t *n) {
    if (n == NULL) return;
    freeTree(n->left);
    freeTree(n->right);
    free(n->val); // Importante: liberare la stringa allocata con strdup
    free(n);
}

int main() {
    /* * Creazione dell'ADT BST.
     * Allochiamo la struttura wrapper.
     */
    BST albero = (BST)malloc(sizeof(struct binary_search_tree));
    albero->root = NULL;

    /* * Costruzione scenario di test.
     * Immaginiamo questo albero:
     * "mela" (0)
     * /      \
     * "bana"      "pera" (1)
     * /  \        \
     * "ananas" "kiwi"  "uva" (2)
     * \
     * "mango" (3) -> Foglia profonda 1
     * * Aggiungiamo un ramo per creare competizione:
     * ... "pera" ...
     * \
     * "zuppa" (2)
     * \
     * "zurigo" (3) -> Foglia profonda 2 (Stessa prof, ma Z > M)
     */

    /* Inserimento dati */
    insert(&(albero->root), "mela");
    insert(&(albero->root), "bana");
    insert(&(albero->root), "pera");
    insert(&(albero->root), "ananas"); // prof 2
    insert(&(albero->root), "kiwi");   // prof 2
    insert(&(albero->root), "mango");  // prof 3 (Target potenziale 1)
    insert(&(albero->root), "uva");    
    insert(&(albero->root), "zuppa");
    insert(&(albero->root), "zurigo"); // prof 3 (Target VINCITORE perché 'z' > 'm')

    printf("--- TEST BSTprintDeepest ---\n");
    /*
     * Risultato atteso: 
     * Deve scegliere "zurigo".
     * Stampa inversa: zurigo -> zuppa -> pera -> mela
     */
    BSTprintDeepest(albero);

    /* Pulizia */
    freeTree(albero->root);
    free(albero);
    
    return 0;
}