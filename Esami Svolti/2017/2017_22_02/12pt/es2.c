#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE DELLA STRUTTURA DATI
 * ============================================================================
 * Concetti Chiave:
 * - Rappresentazione LCRS (Left-Child Right-Sibling):
 * Ogni nodo ha un puntatore al suo PRIMO figlio (left_child) e un puntatore
 * al suo PROSSIMO fratello (right_sibling).
 *
 * - Nuovi Campi Richiesti:
 * 1. num_figli: intero per contare quanti figli diretti ha il nodo.
 * 2. parent: puntatore per risalire al padre (back-link).
 * ============================================================================
 */
typedef struct node *link; // link è un alias per "puntatore a struct node"

struct node {
    char key;           // Dato contenuto nel nodo (es. 'a', 'b'...)
    link left_child;    // Punta al primo figlio
    link right_sibling; // Punta al fratello successivo
    
    /* CAMPI AGGIUNTIVI RICHIESTI DALL'ESERCIZIO */
    int num_figli;      // Numero totale di figli diretti
    link parent;        // Puntatore al nodo padre
};

/* ============================================================================
 * FUNZIONE HELPER: newNode
 * ============================================================================
 * Scopo:
 * Creare un nuovo nodo in memoria, inizializzando i campi a valori sicuri.
 *
 * Parametri:
 * - key: il carattere da inserire nel nodo.
 *
 * Ritorna:
 * - Un puntatore (link) al nuovo nodo allocato.
 *
 * Gestione Memoria:
 * - Utilizza malloc. È CRUCIALE controllare se malloc restituisce NULL.
 * - sizeof(struct node) calcola esattamente i byte necessari per la struct.
 * ============================================================================
 */
link newNode(char key) {
    /* --- 1. ALLOCAZIONE --- */
    link x = (link)malloc(sizeof(struct node));
    
    if (x == NULL) {
        printf("Errore: Memoria esaurita!\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    x->key = key;
    x->left_child = NULL;
    x->right_sibling = NULL;
    x->num_figli = 0; // Inizializzato a 0, sarà calcolato da processTree
    x->parent = NULL; // Inizializzato a NULL, sarà calcolato da processTree
    
    return x;
}

/* ============================================================================
 * FUNZIONE RICHIESTA: processTree
 * ============================================================================
 * Scopo:
 * Attraversa l'albero ricorsivamente. Per ogni nodo:
 * 1. Conta quanti figli diretti possiede scorrendo la lista dei fratelli del primo figlio.
 * 2. Imposta il campo 'parent' di tutti questi figli puntandolo a se stesso.
 * 3. Aggiorna il proprio campo 'num_figli'.
 *
 * Parametri:
 * - root: puntatore al nodo corrente che stiamo analizzando.
 *
 * Concetti Chiave:
 * - Visita Pre-order (o simile): Elaboriamo il nodo corrente (contiamo i suoi figli)
 * prima di scendere ricorsivamente nei figli.
 * - Iterazione vs Ricorsione: Usiamo un ciclo while per scorrere i figli orizzontalmente
 * (siblings) per contarli, ma usiamo la ricorsione per scendere in profondità nell'albero.
 * ============================================================================
 */
void processTree(link root) {
    /* --- 1. CASO BASE --- */
    if (root == NULL) {
        return; // Se il nodo non esiste, non c'è nulla da processare.
    }

    /* --- 2. LOGICA DI CONTEGGIO E ASSEGNAZIONE PADRE --- */
    /* * Ora agiamo come se 'root' fosse il padre.
     * Dobbiamo guardare il suo primo figlio e scorrere tutti i fratelli di quel figlio.
     */
    int count = 0;
    link child = root->left_child; // Accedo al primo figlio puntato da root

    /* Ciclo per scorrere tutti i figli diretti di 'root' */
    while (child != NULL) {
        count++; // Trovato un figlio, incremento il contatore
        
        /* * Assegno il padre al figlio corrente.
         * "child->parent" accede al campo parent della struct puntata da child.
         * Gli assegniamo l'indirizzo di 'root'.
         */
        child->parent = root; 
        
        /* Passo al prossimo fratello nella lista dei figli */
        child = child->right_sibling; 
    }

    /* Salviamo il conteggio totale nel nodo padre */
    root->num_figli = count;

    /* --- 3. PASSO RICORSIVO --- */
    /*
     * Dobbiamo ripetere il processo per tutto l'albero.
     * La struttura LCRS si visita ricorsivamente andando sul figlio sinistro 
     * E sul fratello destro.
     */
    
    processTree(root->left_child);    // Scendo in profondità (processo i sotto-alberi dei figli)
    processTree(root->right_sibling); // Mi sposto lateralmente (processo i fratelli di root)
}

/* ============================================================================
 * FUNZIONE HELPER: printTreeInfo
 * ============================================================================
 * Scopo:
 * Stampare i risultati per verificare se processTree ha funzionato.
 * Stampa: Chiave, Numero Figli, Chiave del Padre.
 */
void printTreeInfo(link h) {
    if (h == NULL) return;

    printf("Nodo: %c | Figli: %d | Padre: ", h->key, h->num_figli);
    if (h->parent != NULL) {
        printf("%c\n", h->parent->key);
    } else {
        printf("NULL (Radice)\n");
    }

    /* Visita ricorsiva standard per LCRS */
    printTreeInfo(h->left_child);
    printTreeInfo(h->right_sibling);
}

/* ============================================================================
 * FUNZIONE HELPER: freeTree
 * ============================================================================
 * Scopo:
 * Liberare la memoria allocata per evitare memory leaks.
 *
 * Concetti Chiave:
 * - Post-order traversal: Libero prima i figli, poi me stesso.
 */
void freeTree(link h) {
    if (h == NULL) return;
    freeTree(h->left_child);
    freeTree(h->right_sibling);
    free(h); // Libero il blocco di memoria puntato da h
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE DELL'ALBERO (HARDCODED) --- */
    /* * Costruiamo l'albero mostrato nell'immagine dell'esercizio:
     * Radice: a
     * Figli di a: b, c, d
     * Figli di b: e
     * Figli di e: h, i, j
     * Figli di c: nessuno
     * Figli di d: f, g
     */

    printf("--- 1. Creazione Albero ---\n");
    
    // Livello 0
    link a = newNode('a');

    // Livello 1 (figli di a: b -> c -> d)
    link b = newNode('b');
    link c = newNode('c');
    link d = newNode('d');
    
    a->left_child = b;      // b è il primo figlio di a
    b->right_sibling = c;   // c è fratello di b
    c->right_sibling = d;   // d è fratello di c

    // Livello 2 (figlio di b: e)
    link e = newNode('e');
    b->left_child = e;      // e è primo figlio di b

    // Livello 2 (figli di d: f -> g)
    link f = newNode('f');
    link g = newNode('g');
    d->left_child = f;      // f è primo figlio di d
    f->right_sibling = g;   // g è fratello di f

    // Livello 3 (figli di e: h -> i -> j)
    link h = newNode('h');
    link i = newNode('i');
    link j = newNode('j');
    e->left_child = h;      // h è primo figlio di e
    h->right_sibling = i;   // i è fratello di h
    i->right_sibling = j;   // j è fratello di i

    /* --- 2. ESECUZIONE DELLA FUNZIONE --- */
    printf("--- 2. Esecuzione processTree ---\n");
    // Chiamiamo la funzione sulla radice. Nota: 'a' non ha padre, rimarrà NULL.
    processTree(a);

    /* --- 3. VERIFICA RISULTATI --- */
    printf("--- 3. Risultati ---\n");
    printf("Dovremmo vedere:\n");
    printf("a: 3 figli, Padre NULL\n");
    printf("b: 1 figlio (e), Padre a\n");
    printf("c: 0 figli, Padre a\n");
    printf("d: 2 figli (f,g), Padre a\n");
    printf("e: 3 figli (h,i,j), Padre b\n");
    printf("...\n\n");
    
    printTreeInfo(a);

    /* --- 4. PULIZIA MEMORIA --- */
    freeTree(a);
    printf("\nMemoria liberata con successo.\n");

    return 0;
}