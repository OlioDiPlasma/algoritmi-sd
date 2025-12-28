#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONI TIPI E STRUTTURE
 * ============================================================================
 * Definiamo le strutture base per l'albero binario come si fa tipicamente
 * nei corsi di algoritmi.
 */

typedef int KEY; // Assumiamo che le chiavi siano interi per semplicità

typedef struct node {
    KEY key;
    struct node *left;
    struct node *right;
} node_t, *link;

/*
 * Macro per il confronto (come richiesto dal testo: KEYcmp).
 * Restituisce 0 se uguali, diverso da 0 se diverse.
 */
#define KEYcmp(k1, k2) ((k1) != (k2))

/* ============================================================================
 * FUNZIONE HELPER: areIdentical
 * ============================================================================
 * SCOPO:
 * Questa è una funzione "ausiliaria". Verifica se due alberi (o sottoalberi)
 * sono ESATTAMENTE identici: stessa radice, stesso sottoalbero sinistro,
 * stesso sottoalbero destro.
 *
 * PARAMETRI:
 * - link t1: Puntatore alla radice del primo albero da confrontare.
 * - link t2: Puntatore alla radice del secondo albero da confrontare.
 *
 * CONCETTI CHIAVE:
 * La ricorsione strutturale. Due alberi sono uguali se:
 * 1. Entrambi sono NULL (alberi vuoti sono uguali).
 * 2. Le loro radici hanno la stessa chiave E i figli sinistri sono uguali E
 * i figli destri sono uguali.
 *
 * BLOCCHI LOGICI:
 * 1. Casi Base (t1 o t2 sono NULL).
 * 2. Confronto Chiavi.
 * 3. Ricorsione (AND logico).
 */
int areIdentical(link t1, link t2) {
    /* --- 1. CASI BASE (NULL) --- */
    /* Se entrambi sono vuoti, sono identici. */
    if (t1 == NULL && t2 == NULL)
        return 1;

    /* Se uno è vuoto e l'altro no, sono diversi. */
    if (t1 == NULL || t2 == NULL)
        return 0;

    /* --- 2. CONFRONTO CHIAVI E 3. RICORSIONE --- */
    /*
     * DETTAGLIO SINTATTICO:
     * KEYcmp(t1->key, t2->key) == 0 : Controlla se le chiavi sono uguali.
     * && : Operatore logico AND. Tutte le condizioni devono essere vere.
     * areIdentical(...) : Chiamata ricorsiva sui figli.
     */
    return (KEYcmp(t1->key, t2->key) == 0 &&
            areIdentical(t1->left, t2->left) &&
            areIdentical(t1->right, t2->right));
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: subtree
 * ============================================================================
 * SCOPO:
 * Determina se l'albero 'root2' è contenuto all'interno dell'albero 'root1'.
 *
 * PARAMETRI:
 * - link root1: L'albero "principale" (il contenitore).
 * - link root2: L'albero "pattern" (quello da cercare).
 *
 * CONCETTI CHIAVE:
 * Qui non stiamo solo confrontando, stiamo **visitando** root1.
 * Per ogni nodo di root1, ci chiediamo: "Il sottoalbero che parte da qui
 * è identico a root2?".
 * Se sì -> Trovato.
 * Se no -> Prova a cercare nel figlio sinistro o nel figlio destro.
 */
int subtree(link root1, link root2) {

    /* --- 1. CASI BASE --- */

    /* Se root2 è vuoto, è tecnicamente un sottoalbero di qualsiasi albero. */
    if (root2 == NULL)
        return 1;

    /* Se root1 è vuoto (e root2 no, visto l'if sopra), non possiamo trovare nulla. */
    if (root1 == NULL)
        return 0;

    /* --- 2. VERIFICA CORRISPONDENZA CORRENTE --- */
    /*
     * Controlliamo se l'albero che inizia ESATTAMENTE qui in root1
     * è identico a root2. Usiamo la funzione helper.
     */
    if (areIdentical(root1, root2))
        return 1;

    /* --- 3. RICORSIONE DI RICERCA --- */
    /*
     * Se non abbiamo trovato una corrispondenza qui, cerchiamo
     * nel sottoalbero sinistro OPPURE nel sottoalbero destro.
     *
     * DETTAGLIO SINTATTICO:
     * || (OR logico): Basta che una delle due chiamate ritorni 1 (true)
     * perché il risultato finale sia 1. È "short-circuit": se trova a sinistra,
     * non esegue nemmeno la chiamata a destra.
     */
    return subtree(root1->left, root2) ||
           subtree(root1->right, root2);
}

/* ============================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST
 * ============================================================================
 * newNode: Crea un nuovo nodo (gestione memoria malloc).
 * freeTree: Libera la memoria (gestione memoria free).
 */
link newNode(int key, link left, link right) {
    /* --- ALLOCAZIONE --- */
    link n = (link)malloc(sizeof(struct node));
    /* Verifica malloc */
    if (n == NULL) {
        printf("Errore allocazione memoria.\n");
        exit(1);
    }
    n->key = key;
    n->left = left;
    n->right = right;
    return n;
}

void freeTree(link root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root); /* Libera il nodo corrente solo dopo aver liberato i figli */
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Ricostruire l'esempio dell'immagine (T1, T2, T3) e verificare
 * il funzionamento della funzione subtree.
 */
int main() {
    /*
     * COSTRUZIONE ALBERI (Hardcoded come da esempio nell'immagine)
     * Costruiamo dal basso verso l'alto (foglie -> radice) per semplicità.
     */

    /* --- COSTRUZIONE T1 --- */
    /*
          3
         / \
        7   1
       / \  /
      2  2 0
         /
        9
    */
    /* Foglie e nodi bassi di T1 */
    link t1_node9 = newNode(9, NULL, NULL);
    link t1_node2a = newNode(2, NULL, NULL);        // Il 2 a sinistra
    link t1_node2b = newNode(2, t1_node9, NULL);    // Il 2 centrale con figlio 9
    link t1_node0  = newNode(0, NULL, NULL);

    /* Livello intermedio T1 */
    link t1_node7 = newNode(7, t1_node2a, t1_node2b);
    link t1_node1 = newNode(1, t1_node0, NULL);

    /* Radice T1 */
    link root1 = newNode(3, t1_node7, t1_node1);


    /* --- COSTRUZIONE T2 (Dovrebbe essere un sottoalbero) --- */
    /*
          7
         / \
        2   2
           /
          9
    */
    link t2_node9 = newNode(9, NULL, NULL);
    link t2_node2a = newNode(2, NULL, NULL);
    link t2_node2b = newNode(2, t2_node9, NULL);
    link root2 = newNode(7, t2_node2a, t2_node2b);


    /* --- COSTRUZIONE T3 (NON dovrebbe essere un sottoalbero) --- */
    /*
     * T3 sembra uguale alla parte alta di T1, MA:
     * il nodo 7 in T1 ha dei figli.
     * il nodo 7 in T3 ha figli diversi (o nulli).
     * Nell'immagine T3 è:
          3
         / \
        7   1
    */
    link t3_node7 = newNode(7, NULL, NULL); // Foglia in T3
    link t3_node1 = newNode(1, NULL, NULL); // Foglia in T3
    link root3 = newNode(3, t3_node7, t3_node1);

    /* --- TEST ED ESECUZIONE --- */
    printf("--- Test Funzione Subtree ---\n");

    /* Test 1: T2 dentro T1? */
    if (subtree(root1, root2)) {
        printf("[OK] T2 e' sottoalbero di T1 (Atteso: SI)\n");
    } else {
        printf("[ERR] T2 non e' stato trovato in T1 (Atteso: SI)\n");
    }

    /* Test 2: T3 dentro T1? */
    /* Questo è il caso sottile: i valori sono uguali, ma la struttura sotto il 7 no */
    if (subtree(root1, root3)) {
        printf("[ERR] T3 e' sottoalbero di T1 (Atteso: NO)\n");
    } else {
        printf("[OK] T3 NON e' sottoalbero di T1 (Atteso: NO)\n");
    }

    /* --- PULIZIA MEMORIA --- */
    freeTree(root1);
    freeTree(root2);
    freeTree(root3);

    return 0;
}