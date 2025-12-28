#include <stdio.h>
#include <stdlib.h>

/* Definizione della struttura del nodo come da testo */
struct node {
    int key;
    struct node *left;
    struct node *right;
};

/*
 * ======================================================================================
 * NOME FUNZIONE: printPaths
 * ======================================================================================
 * SCOPO:
 * Funzione ricorsiva che attraversa l'albero dalla radice verso le foglie.
 * Memorizza il percorso corrente in un array e, quando incontra una foglia (nodo senza figli),
 * stampa tutto il contenuto dell'array accumulato fino a quel momento.
 *
 * PARAMETRI:
 * * root    : (struct node *) Puntatore al nodo che stiamo visitando attualmente.
 * * path    : (int *) Array di interi che funge da "buffer" o memoria storica del cammino.
 * Contiene le chiavi dei nodi visitati dalla radice fino al nodo corrente.
 * pathLen   : (int) Indice che indica la profondità corrente (o quanti nodi abbiamo nel path).
 *
 * NOTA SUI PARAMETRI:
 * Il testo diceva "è possibile aggiungere parametri". Abbiamo aggiunto 'path' e 'pathLen'
 * perché senza di essi la ricorsione perderebbe la memoria di "da dove veniamo".
 * L'altezza 'h' menzionata nel testo serve nel main per dimensionare l'array 'path'.
 *
 * CONCETTI CHIAVE:
 * 1. Accumulo nello Stack: Ogni chiamata ricorsiva rappresenta un passo in giù nell'albero.
 * L'array 'path' viene riempito progressivamente.
 * 2. Backtracking Implicito: Quando la funzione ricorsiva ritorna (finisce un ramo),
 * l'indice 'pathLen' torna automaticamente al valore precedente per il ramo successivo.
 * Non dobbiamo cancellare manualmente l'array, i nuovi valori sovrascriveranno i vecchi.
 * ======================================================================================
 */
void printPaths(struct node *root, int path[], int pathLen) {

    /* --- 1. CASO BASE (NODO NULLO) --- */
    /* Se il nodo non esiste, torniamo indietro. */
    if (root == NULL) {
        return;
    }

    /* --- 2. LOGICA DI ACCUMULO --- */
    /*
     * DETTAGLIO SINTATTICO:
     * path[pathLen] accede alla cella dell'array all'indice corrente.
     * root->key accede al valore del nodo (deferenziazione puntatore).
     * Salviamo il valore del nodo corrente nel percorso.
     */
    path[pathLen] = root->key;
    
    /* Incrementiamo la lunghezza del percorso per le prossime chiamate */
    pathLen++;

    /* --- 3. CONTROLLO FOGLIA (CASO DI STAMPA) --- */
    /*
     * Un nodo è una foglia se entrambi i figli (sinistro e destro) sono NULL.
     * Se siamo su una foglia, abbiamo completato un cammino: stampiamolo.
     */
    if (root->left == NULL && root->right == NULL) {
        printf("Cammino trovato: ");
        for (int i = 0; i < pathLen; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        /* --- 4. PASSO RICORSIVO --- */
        /*
         * Se non è una foglia, continuiamo a scendere.
         * Passiamo lo stesso array 'path'.
         * Passiamo 'pathLen' che è stato incrementato sopra.
         */
        printPaths(root->left, path, pathLen);
        printPaths(root->right, path, pathLen);
    }
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST
 * ======================================================================================
 */

/* Helper per creare un nuovo nodo */
struct node* newNode(int key) {
    /*
     * GESTIONE MEMORIA (MALLOC):
     * Allocazione dinamica nello HEAP.
     * sizeof(struct node) calcola i byte necessari per: 1 int + 2 puntatori.
     */
    struct node* node = (struct node*)malloc(sizeof(struct node));
    
    /* Verifica allocazione */
    if (node == NULL) {
        fprintf(stderr, "Errore: Memoria esaurita\n");
        exit(1);
    }

    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Helper per liberare la memoria dell'albero (Post-Order) */
void freeTree(struct node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    /*
     * GESTIONE MEMORIA (FREE):
     * Importante: liberiamo il nodo corrente SOLO DOPO aver liberato i figli.
     * Altrimenti perderemmo i puntatori ai sottoalberi (dangling pointers).
     */
    free(root);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Costruire l'albero, preparare il buffer 'path' e lanciare la funzione.
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE DATI DI PROVA --- */
    /*
     * Costruiamo un albero di altezza 3:
     * 1
     * /   \
     * 2     3
     * / \   /
     * 4   5 8
     */
    struct node *root = newNode(1);
    root->left        = newNode(2);
    root->right       = newNode(3);
    root->left->left  = newNode(4);
    root->left->right = newNode(5);
    root->right->left = newNode(8);

    /* --- 2. PREPARAZIONE BUFFER --- */
    /*
     * Il testo parla di un albero di altezza 'h'.
     * Dobbiamo allocare un array capace di contenere il cammino più lungo possibile.
     * Supponiamo h=10 per sicurezza in questo test.
     */
    int h = 10;
    
    /*
     * GESTIONE MEMORIA (ARRAY DINAMICO):
     * Allochiamo l'array path. Potremmo usarne uno statico (int path[100]),
     * ma usare malloc è più professionale se 'h' è variabile.
     */
    int *path = (int *)malloc(h * sizeof(int));
    if (path == NULL) return 1;

    printf("--- STAMPA DI TUTTI I CAMMINI RADICE-FOGLIE ---\n");
    
    /* --- 3. ESECUZIONE --- */
    /* Partiamo con lunghezza percorso 0 */
    printPaths(root, path, 0);

    /*
     * Output Atteso:
     * Cammino trovato: 1 2 4
     * Cammino trovato: 1 2 5
     * Cammino trovato: 1 3 8
     */

    /* --- 4. PULIZIA MEMORIA --- */
    free(path);     // Liberiamo il buffer del percorso
    freeTree(root); // Liberiamo tutti i nodi dell'albero

    printf("Memoria liberata correttamente.\n");

    return 0;
}