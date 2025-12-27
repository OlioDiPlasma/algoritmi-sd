#include <stdio.h>
#include <stdlib.h>

/* =================================================================================
 * DEFINIZIONE STRUTTURE DATI
 * =================================================================================
 * Come da traccia, definiamo il nodo e il puntatore 'link'.
 */
typedef struct node *link;

struct node {
    int item;
    link left;
    link right;
};

/* =================================================================================
 * FUNZIONE: NEW
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Crea un nuovo nodo in memoria dinamica (Heap) e lo inizializza.
 *
 * PARAMETRI:
 * - int chiave: Il valore intero da memorizzare.
 * - link left: Puntatore al figlio sinistro.
 * - link right: Puntatore al figlio destro.
 *
 * GESTIONE MEMORIA:
 * Utilizziamo malloc per allocare esattamente lo spazio necessario per una struct node.
 * È fondamentale controllare se malloc ritorna NULL (memoria esaurita), anche se
 * negli esercizi accademici spesso si omette per brevità.
 * =================================================================================
 */
link NEW(int chiave, link left, link right) {
    link x = malloc(sizeof(struct node)); 
    if (x == NULL) {
        printf("Errore di allocazione memoria.\n");
        exit(1);
    }
    x->item = chiave;
    x->left = left;
    x->right = right;
    return x;
}

/* =================================================================================
 * FUNZIONE: buildTree
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Ricostruisce ricorsivamente un albero binario dati i vettori delle visite
 * Inorder e Preorder.
 *
 * PARAMETRI:
 * - int *inorder: Puntatore all'array contenente la visita simmetrica.
 * - int *preorder: Puntatore all'array contenente la visita anticipata.
 * - int N: Numero di nodi da processare in questa chiamata ricorsiva.
 *
 * CONCETTI CHIAVE:
 * 1. Ruolo di Preorder: Il PRIMO elemento di un array preorder è SEMPRE la radice
 * del sottoalbero corrente.
 * 2. Ruolo di Inorder: Una volta trovata la radice (dal preorder), la cerchiamo
 * nell'array inorder. La sua posizione fa da "perno" (pivot):
 * - Tutto ciò che sta a SINISTRA del perno in Inorder appartiene al sottoalbero sinistro.
 * - Tutto ciò che sta a DESTRA del perno in Inorder appartiene al sottoalbero destro.
 * 3. Aritmetica dei Puntatori: Passiamo alle chiamate ricorsive i puntatori spostati
 * (es. preorder + 1) per lavorare su sottomatrici degli array originali senza copiarli.
 *
 * RITORNA:
 * Il puntatore (link) alla radice dell'albero (o sottoalbero) costruito.
 * =================================================================================
 */
link buildTree(int *inorder, int *preorder, int N) {
    
    /* --- 1. CASO BASE --- */
    // Se N è 0, non ci sono nodi da costruire. Ritorniamo puntatore nullo.
    if (N == 0) {
        return NULL;
    }

    /* --- 2. IDENTIFICAZIONE RADICE --- */
    // In Preorder, la radice è sempre il primo elemento.
    int radiceVal = preorder[0];
    
    // Creiamo il nodo radice. I figli li collegheremo dopo la ricorsione.
    // Usiamo la funzione NEW fornita dalla traccia.
    link root = NEW(radiceVal, NULL, NULL);

    /* --- 3. RICERCA DELLA RADICE IN INORDER (Pivot) --- */
    // Dobbiamo trovare dove si trova questa radice nel vettore Inorder
    // per capire quanti nodi stanno a sinistra e quanti a destra.
    int i;
    for (i = 0; i < N; i++) {
        if (inorder[i] == radiceVal) {
            break; // Trovato! L'indice 'i' rappresenta la posizione della radice.
        }
    }
    
    // Ora sappiamo che:
    // - Ci sono 'i' nodi nel sottoalbero sinistro.
    // - Ci sono 'N - 1 - i' nodi nel sottoalbero destro.

    /* --- 4. COSTRUZIONE RICORSIVA SOTTOALBERI --- */
    
    /* Costruzione Sinistra:
     * - Inorder: parte dall'inizio del vettore corrente (inorder).
     * - Preorder: parte da (preorder + 1), saltando la radice appena usata.
     * - Lunghezza: 'i' (numero nodi trovati a sinistra del pivot).
     */
    root->left = buildTree(inorder, preorder + 1, i);

    /* Costruzione Destra:
     * - Inorder: parte da (inorder + i + 1), saltando la parte sinistra e la radice.
     * - Preorder: parte da (preorder + 1 + i). Dobbiamo saltare la radice (+1) 
     * E tutti i nodi che abbiamo assegnato al sottoalbero sinistro (+i).
     * - Lunghezza: N - 1 - i (i rimanenti).
     */
    root->right = buildTree(inorder + i + 1, preorder + 1 + i, N - i - 1);

    return root;
}

/* =================================================================================
 * FUNZIONE AUSILIARIA: treeFree
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Liberare la memoria allocata per l'albero (Post-order traversal).
 * È fondamentale visitare prima i figli e poi il padre, altrimenti perdiamo i puntatori.
 * =================================================================================
 */
void treeFree(link h) {
    if (h == NULL) return;
    treeFree(h->left);
    treeFree(h->right);
    free(h); // Libero il nodo corrente solo dopo aver liberato i figli
}

/* =================================================================================
 * FUNZIONE AUSILIARIA: printInOrder
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Stampa l'albero in visita simmetrica per verificare la correttezza nel main.
 * =================================================================================
 */
void printInOrder(link h) {
    if (h == NULL) return;
    printInOrder(h->left);
    printf("%d ", h->item);
    printInOrder(h->right);
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Testare la funzione con un albero noto e verificare che l'output sia coerente.
 *
 * Albero di prova:
 * 1
 * / \
 * 2   3
 * / \
 * 4   5
 *
 * Preorder atteso: 1 2 4 5 3
 * Inorder atteso:  4 2 5 1 3
 * =================================================================================
 */
int main() {
    printf("--- ESERCIZIO RICOSTRUZIONE ALBERO ---\n\n");

    /* 1. DATI DI PROVA HARDCODED */
    int N = 5;
    // Visita anticipata (Radice - Sinistra - Destra)
    int preorder[] = {1, 2, 4, 5, 3};
    // Visita simmetrica (Sinistra - Radice - Destra)
    int inorder[]  = {4, 2, 5, 1, 3};

    printf("Preorder Input: ");
    for(int k=0; k<N; k++) printf("%d ", preorder[k]);
    printf("\n");

    printf("Inorder Input : ");
    for(int k=0; k<N; k++) printf("%d ", inorder[k]);
    printf("\n\n");

    /* 2. CHIAMATA ALLA FUNZIONE */
    printf("Costruzione albero in corso...\n");
    link root = buildTree(inorder, preorder, N);

    /* 3. VERIFICA */
    // Se l'albero è corretto, stampando la sua Inorder dobbiamo riottenere il vettore di input.
    printf("Verifica Inorder dell'albero costruito: ");
    printInOrder(root);
    printf("\n");

    /* 4. PULIZIA MEMORIA */
    treeFree(root);
    printf("Memoria liberata correttamente.\n");

    return 0;
}