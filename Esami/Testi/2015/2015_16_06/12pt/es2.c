#include <stdio.h>
#include <stdlib.h>

/*
 * Definizione della struttura del nodo come richiesto.
 * Contiene una chiave intera e due puntatori ai figli.
 */
struct node {
    int key;
    struct node *left;
    struct node *right;
};

/*
 * ======================================================================================
 * NOME FUNZIONE: doubleTree
 * ======================================================================================
 * SCOPO:
 * Per ogni nodo dell'albero, crea un nuovo nodo duplicato (stessa chiave) e lo inserisce
 * come figlio sinistro del nodo originale. Il vecchio figlio sinistro diventa il
 * figlio sinistro del nuovo duplicato.
 *
 * Esempio Visivo:
 * 2            Diventa:        2
 * /                            /
 * 1                            2 (nuovo)
 * /
 * 1
 * /
 * 1 (nuovo)
 *
 * PARAMETRI:
 * * root : (struct node *) Puntatore alla radice dell'albero (o sottoalbero) corrente.
 *
 * CONCETTI CHIAVE:
 * 1. Ordine di Visita (Post-Order): È fondamentale elaborare prima i figli e poi la radice.
 * Se inserissimo subito il duplicato e poi scendessimo ricorsivamente a sinistra,
 * rischieremmo di processare all'infinito i nuovi nodi appena creati (loop infinito).
 * Usando la post-order (sinistra -> destra -> radice), quando arriviamo a processare
 * la radice, il sottoalbero sinistro è già stato "raddoppiato" e siamo sicuri di non
 * visitarlo di nuovo erroneamente in questo ciclo.
 * 2. Manipolazione Puntatori: Dobbiamo salvare il puntatore al vecchio figlio sinistro
 * in una variabile temporanea prima di sovrascriverlo, altrimenti perderemmo per sempre
 * tutto il ramo sinistro originale.
 * ======================================================================================
 */
void doubleTree(struct node *root) {

    /* --- 1. CASO BASE --- */
    /* Se il nodo è NULL, non c'è nulla da duplicare. Terminiamo la ricorsione. */
    if (root == NULL) {
        return;
    }

    /* --- 2. PASSO RICORSIVO (POST-ORDER) --- */
    /*
     * Chiamiamo la funzione ricorsivamente PRIMA di fare le modifiche al nodo corrente.
     * In questo modo, "sistemiamo" prima il fondo dell'albero.
     */
    doubleTree(root->left);
    doubleTree(root->right);

    /* --- 3. ALLOCAZIONE E CREAZIONE DUPLICATO --- */
    
    /* Salviamo il vecchio figlio sinistro (che ora è già stato raddoppiato internamente) */
    struct node *oldLeft = root->left;

    /*
     * GESTIONE MEMORIA (MALLOC):
     * Creiamo il nuovo nodo nello Heap.
     * sizeof(struct node) ci dà la dimensione esatta per contenere int e puntatori.
     */
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    
    /* Controllo robustezza: se la malloc fallisce, usciamo */
    if (newNode == NULL) {
        perror("Errore malloc");
        exit(1);
    }

    /* Copiamo la chiave del nodo corrente nel nuovo nodo */
    newNode->key = root->key;

    /* --- 4. RICABLAGGIO DEI PUNTATORI (La parte logica cruciale) --- */

    /*
     * Il testo dice: "inserisca tale nodo come figlio sinistro del nodo originale".
     * E il vecchio figlio sinistro? Diventa figlio del nuovo nodo.
     */
    
    /* Passo A: Il nuovo nodo punta al vecchio figlio sinistro */
    newNode->left = oldLeft;
    
    /* Passo B: Il nuovo nodo non ha figlio destro (di solito i duplicati in questi esercizi sono foglie a destra) */
    newNode->right = NULL;

    /* Passo C: Il nodo originale ora punta al nuovo nodo come suo figlio sinistro */
    root->left = newNode;
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST
 * ======================================================================================
 */

/* Helper per creare nodi rapidamente nel main */
struct node* newNode(int key) {
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
 * Stampa l'albero (Visita In-Order: Sx -> Centro -> Dx).
 * Utile perché se l'albero è duplicato correttamente, vedremo coppie di numeri.
 * Es: 1 1 2 2 3 3
 */
void printInOrder(struct node* node) {
    if (node == NULL) return;
    
    printInOrder(node->left);
    printf("%d ", node->key);
    printInOrder(node->right);
}

/* Libera la memoria per evitare Memory Leaks */
void freeTree(struct node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Ricostruire l'albero dell'esempio e verificare la trasformazione.
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE ALBERO ESEMPIO --- */
    /*
     * Costruiamo l'albero mostrato nell'immagine originale:
     * 2
     * / \
     * 1   3
     */
    struct node *root = newNode(2);
    root->left = newNode(1);
    root->right = newNode(3);

    printf("--- TEST ESERCIZIO doubleTree ---\n");
    printf("Albero Iniziale (In-Order): ");
    printInOrder(root); /* Output atteso: 1 2 3 */
    printf("\n");

    /* --- 2. ESECUZIONE FUNZIONE --- */
    doubleTree(root);

    /* --- 3. VERIFICA RISULTATI --- */
    /*
     * Dopo la duplicazione, l'albero dovrebbe essere logicamente:
     * 2
     * / \
     * 2   3
     * /   /
     * 1   3
     * /
     * 1
     *
     * Se stampiamo in-order (visita simmetrica), dovremmo vedere ogni numero due volte.
     * Ordine atteso: (1 -> 1) -> 2 -> 2 -> (3 -> 3)
     * Output: 1 1 2 2 3 3
     */
    printf("Albero Raddoppiato (In-Order): ");
    printInOrder(root);
    printf("\n");
    printf("Output Atteso: 1 1 2 2 3 3\n");

    /* --- 4. PULIZIA MEMORIA --- */
    /*
     * GESTIONE MEMORIA (FREE):
     * Ora l'albero ha il doppio dei nodi. La funzione freeTree li libererà tutti
     * ricorsivamente, sia gli originali che i duplicati allocati da doubleTree.
     */
    freeTree(root);
    printf("Memoria liberata correttamente.\n");

    return 0;
}