#include <stdio.h>
#include <stdlib.h>

/* Definiamo il tipo 'Key' come intero per semplicità, come suggerito dal contesto */
typedef int Key;

/* Struttura del nodo dell'albero binario */
struct node {
    Key key;
    struct node *left;
    struct node *right;
};

/*
 * ======================================================================================
 * NOME FUNZIONE: KEYcmp
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria per comparare due chiavi.
 * In C è buona norma astrarre il confronto, specialmente se Key fosse una struct complessa.
 *
 * RITORNA:
 * 0 se le chiavi sono uguali.
 * Un valore diverso da 0 se sono diverse.
 * ======================================================================================
 */
int KEYcmp(Key k1, Key k2) {
    return k1 - k2;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: TREEisomorph
 * ======================================================================================
 * SCOPO:
 * Verifica se due alberi binari (t1 e t2) sono isomorfi.
 * L'isomorfismo richiede tre condizioni simultanee:
 * 1. Le radici devono contenere lo stesso valore.
 * 2. Il sottoalbero sinistro di t1 deve essere isomorfo al sottoalbero sinistro di t2.
 * 3. Il sottoalbero destro di t1 deve essere isomorfo al sottoalbero destro di t2.
 *
 * PARAMETRI:
 * * t1 : (struct node *) Puntatore alla radice del primo albero.
 * * t2 : (struct node *) Puntatore alla radice del secondo albero.
 *
 * CONCETTI CHIAVE:
 * 1. Ricorsione in parallelo: Attraversiamo due strutture dati contemporaneamente.
 * 2. Logica Booleana in C: Poiché il C standard (ANSI C) non ha il tipo 'bool' nativo,
 * usiamo gli interi: 1 rappresenta VERO, 0 rappresenta FALSO.
 * 3. Corto circuito (Short-circuit evaluation): Nell'istruzione 'return ... && ...',
 * se la prima chiamata ricorsiva ritorna 0 (Falso), la seconda non viene nemmeno eseguita,
 * risparmiando tempo.
 * ======================================================================================
 */
int TREEisomorph(struct node *t1, struct node *t2) {

    /* --- 1. CASI BASE (Condizioni di terminazione) --- */

    /* CASO A: Entrambi gli alberi sono vuoti (NULL).
     * Due alberi vuoti sono considerati uguali/isomorfi.
     */
    if (t1 == NULL && t2 == NULL) {
        return 1; // Vero
    }

    /* CASO B: Uno è vuoto e l'altro no.
     * Se siamo qui, non sono entrambi NULL (per via dell'if precedente).
     * Quindi se uno dei due è NULL, sono strutturalmente diversi.
     */
    if (t1 == NULL || t2 == NULL) {
        return 0; // Falso
    }

    /* --- 2. CONTROLLO DEI DATI --- */

    /* Se entrambi i nodi esistono, verifichiamo se le chiavi sono uguali.
     * Usiamo la funzione helper KEYcmp come richiesto dal testo.
     * Se restituisce un valore != 0, le chiavi sono diverse.
     */
    if (KEYcmp(t1->key, t2->key) != 0) {
        return 0; // Falso
    }

    /* --- 3. PASSO RICORSIVO --- */

    /*
     * Se siamo arrivati qui:
     * 1. I nodi esistono entrambi.
     * 2. Le chiavi sono uguali.
     *
     * Ora dobbiamo verificare che ANCHE i figli siano identici.
     * La condizione è: (Sinistra t1 == Sinistra t2) E (Destra t1 == Destra t2).
     *
     * DETTAGLIO SINTATTICO:
     * t1->left è equivalente a (*t1).left. Accediamo al puntatore del figlio sinistro.
     */
    return TREEisomorph(t1->left, t2->left) && TREEisomorph(t1->right, t2->right);
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST
 * ======================================================================================
 */

/* Funzione per creare un nuovo nodo (Helper) */
struct node* newNode(int key) {
    /*
     * GESTIONE MEMORIA (MALLOC):
     * Allocazione dinamica nello HEAP.
     * sizeof(struct node) calcola esattamente i byte necessari per:
     * 1 intero + 2 puntatori.
     */
    struct node* node = (struct node*)malloc(sizeof(struct node));

    /* Controllo difensivo: la memoria potrebbe essere piena */
    if (node == NULL) {
        fprintf(stderr, "Errore: Memoria esaurita!\n");
        exit(1);
    }

    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Funzione per liberare la memoria di un albero (Post-Order Traversal) */
void freeTree(struct node* root) {
    if (root == NULL) return;

    /* Prima libero i figli */
    freeTree(root->left);
    freeTree(root->right);

    /*
     * GESTIONE MEMORIA (FREE):
     * Solo ORA posso liberare il padre. Se lo facessi prima,
     * perderei l'accesso ai figli (Dangling Pointer) creando un Memory Leak.
     */
    free(root);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    printf("--- TEST ISOMORFISMO ALBERI ---\n");

    /* --- 1. COSTRUZIONE ALBERO 1 --- */
    /* Costruiamo:
     * 1
     * / \
     * 2   3
     */
    struct node *root1 = newNode(1);
    root1->left = newNode(2);
    root1->right = newNode(3);

    /* --- 2. COSTRUZIONE ALBERO 2 (Identico a 1) --- */
    struct node *root2 = newNode(1);
    root2->left = newNode(2);
    root2->right = newNode(3);

    /* --- 3. COSTRUZIONE ALBERO 3 (Diverso per valore) --- */
    /* Costruiamo:
     * 1
     * / \
     * 5   3  (Il 5 è diverso dal 2)
     */
    struct node *root3 = newNode(1);
    root3->left = newNode(5); // Valore diverso
    root3->right = newNode(3);

    /* --- 4. COSTRUZIONE ALBERO 4 (Diverso per struttura) --- */
    /* Costruiamo:
     * 1
     * /
     * 2      (Manca il figlio destro)
     */
    struct node *root4 = newNode(1);
    root4->left = newNode(2);


    /* --- 5. ESECUZIONE TEST --- */

    printf("Test 1: Confronto due alberi identici (root1 vs root2).\n");
    if (TREEisomorph(root1, root2)) {
        printf("ESITO: OK -> Sono isomorfi (Atteso: SI)\n");
    } else {
        printf("ESITO: ERRORE -> Non sono isomorfi (Atteso: SI)\n");
    }

    printf("\nTest 2: Confronto alberi con valori diversi (root1 vs root3).\n");
    if (TREEisomorph(root1, root3)) {
        printf("ESITO: ERRORE -> Sono isomorfi (Atteso: NO)\n");
    } else {
        printf("ESITO: OK -> Non sono isomorfi (Atteso: NO)\n");
    }

    printf("\nTest 3: Confronto alberi con struttura diversa (root1 vs root4).\n");
    if (TREEisomorph(root1, root4)) {
        printf("ESITO: ERRORE -> Sono isomorfi (Atteso: NO)\n");
    } else {
        printf("ESITO: OK -> Non sono isomorfi (Atteso: NO)\n");
    }

    /* --- 6. PULIZIA MEMORIA --- */
    /* È fondamentale liberare tutta la memoria allocata con malloc */
    freeTree(root1);
    freeTree(root2);
    freeTree(root3);
    freeTree(root4);
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}