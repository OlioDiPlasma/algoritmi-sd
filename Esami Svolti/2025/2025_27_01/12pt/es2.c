#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE STRUTTURE DATI (BST di Stringhe)
 * ======================================================================================
 */

// L'Item è una stringa
typedef char *Item;

// Nodo del BST
typedef struct BSTnode {
    Item item;              // La chiave (stringa)
    struct BSTnode *l;      // Figlio sinistro
    struct BSTnode *r;      // Figlio destro
} BSTnode;

typedef BSTnode *link;

// Wrapper (ADT)
typedef struct binary_search_tree {
    link root;
    link z; // Nodo sentinella (opzionale, qui useremo NULL per semplicità standard)
} *BST;

/*
 * ======================================================================================
 * FUNZIONE AUSILIARIA: findMax1R (FASE 1 - RICERCA)
 * ======================================================================================
 * Scopo:
 * Attraversa l'albero per trovare il cammino che massimizza il numero di nodi con 2 figli.
 *
 * Parametri:
 * - link h: Nodo corrente.
 * - char **pathLeafP: (OUTPUT) Puntatore a stringa dove salveremo la chiave della foglia vincente.
 *
 * Ritorna:
 * - int: Il "punteggio" del cammino migliore nel sottoalbero radicato in h.
 * (Punteggio = numero di nodi con 2 figli lungo il cammino).
 *
 * Logica:
 * È una visita Post-Order (Bottom-Up).
 * 1. Se siamo una foglia, ritorniamo 0 (nessun nodo con 2 figli qui). Salviamo la nostra chiave come candidata.
 * 2. Se siamo un nodo interno, chiediamo ai figli sinistro e destro i loro punteggi migliori.
 * 3. Scegliamo il figlio che offre il punteggio più alto.
 * 4. Aggiungiamo 1 al punteggio se IL NODO CORRENTE ha 2 figli.
 */
int findMax2Recursive(link h, char **pathLeafP) {
    // Caso Base: Albero vuoto (non dovrebbe accadere se chiamato correttamente)
    if (h == NULL) return -1;

    // Caso Base: Foglia
    if (h->l == NULL && h->r == NULL) {
        *pathLeafP = h->item; // "Io sono il termine di questo cammino"
        return 0; // Una foglia non ha figli, quindi contribuisce 0 al conteggio
    }

    int score_left = -1;
    int score_right = -1;
    char *leaf_left = NULL;
    char *leaf_right = NULL;

    // Ricorsione a sinistra
    if (h->l != NULL) {
        score_left = findMax2Recursive(h->l, &leaf_left);
    }

    // Ricorsione a destra
    if (h->r != NULL) {
        score_right = findMax2Recursive(h->r, &leaf_right);
    }

    // Calcolo contributo del nodo corrente (1 se ha entrambi i figli, 0 altrimenti)
    int current_bonus = (h->l != NULL && h->r != NULL) ? 1 : 0;

    // Confronto e Risalita
    // Se il cammino a destra è migliore (o uguale, scelta arbitraria)
    if (score_right > score_left) {
        *pathLeafP = leaf_right; // La foglia vincente viene da destra
        return score_right + current_bonus;
    } else {
        *pathLeafP = leaf_left;  // La foglia vincente viene da sinistra
        return score_left + current_bonus;
    }
}

/*
 * ======================================================================================
 * FUNZIONE AUSILIARIA: printPathToLeaf (FASE 2 - STAMPA)
 * ======================================================================================
 * Scopo:
 * Dato che è un BST, se conosciamo la chiave di destinazione, esiste UN SOLO cammino
 * per arrivarci. Lo ripercorriamo stampando i nodi.
 *
 * Ritorna:
 * - int: La lunghezza del cammino (numero di nodi).
 */
int printPathToLeaf(link h, char *targetKey) {
    if (h == NULL) return 0;

    printf("%s ", h->item); // Stampa il nodo corrente

    int len = 1; // Contiamo questo nodo

    int cmp = strcmp(targetKey, h->item); //val negativo minore sin, val positivo maggiore dx
    if (cmp < 0) {
        // La destinazione è minore -> andiamo a sinistra
        len += printPathToLeaf(h->l, targetKey);
    } else if (cmp > 0) {
        // La destinazione è maggiore -> andiamo a destra
        len += printPathToLeaf(h->r, targetKey);
    }
    // Se cmp == 0, siamo arrivati alla foglia target.

    return len;
}

/*
 * ======================================================================================
 * FUNZIONE PRINCIPALE: BSTprintMax2
 * ======================================================================================
 */
int BSTprintMax2(BST b) {
    if (b == NULL || b->root == NULL) return 0;

    char *bestLeaf = NULL;

    // FASE 1: Trovare la foglia target e il numero di nodi con 2 figli (maxScore)
    // Nota: 'maxScore' non è la lunghezza del cammino, ma i nodi che hanno 2 figli lungo un cammino
    int maxScore = findMax2Recursive(b->root, &bestLeaf);

    printf("Cammino Ottimo (Nodi con 2 figli: %d): ", maxScore);

    // FASE 2: Stampare il cammino dalla radice a 'bestLeaf' e calcolarne la lunghezza reale
    int pathLength = printPathToLeaf(b->root, bestLeaf);
    printf("\n");

    return pathLength; // Il testo chiede di ritornare la lunghezza del cammino
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
link newNode(char *val, link l, link r) {
    link x = malloc(sizeof *x);
    x->item = strdup(val); // Copia la stringa!
    x->l = l;
    x->r = r;
    return x;
}

int main() {
    /*
     * Costruiamo questo albero:
     * "mela" (2 figli)
     * /            \
     * "banana"    "pera" (2 figli)
     *              /    \
     *          "pesca"  "susina"
     *
     * Cammini possibili:
     * 1. mela -> banana (Nodi con 2 figli nel percorso: solo mela = 1)
     * 2. mela -> pera -> pesca (Nodi con 2 figli: mela, pera = 2) -> VINCENTE
     * 3. mela -> pera -> susina (Nodi con 2 figli: mela, pera = 2) -> VINCENTE (pari merito)
     */

    link root = newNode("mela",
                    newNode("banana", NULL, NULL),
                    newNode("pera",
                        newNode("pesca", NULL, NULL),
                        newNode("susina", NULL, NULL)
                    )
                );

    BST b = malloc(sizeof(*b));
    b->root = root;
    b->z = NULL;

    printf("--- Test BSTprintMax2 ---\n");
    int len = BSTprintMax2(b);
    printf("Lunghezza totale del cammino ritornata: %d\n", len);

    // Cleanup (omesso per brevità)
    return 0;
}