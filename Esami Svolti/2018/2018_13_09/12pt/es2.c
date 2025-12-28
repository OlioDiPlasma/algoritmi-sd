#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE DELLE STRUTTURE DATI
 * ============================================================================
 * Abbiamo due tipi di nodi diversi, come richiesto dal testo.
 */

/* 1. Nodo della lista di Input (semplice intero) */
typedef struct node1 {
    int val;
    struct node1 *next;
} Node1, *link1;

/* 2. Nodo della lista di Output (compresso: valore + conteggio) */
typedef struct node2 {
    int val;            // Il numero (es. 3)
    int count;          // Quante volte appare consecutivamente (es. 4)
    struct node2 *next;
} Node2, *link2;

/* ============================================================================
 * FUNZIONE DI UTILITÀ: newNode2
 * ============================================================================
 * Scopo: Allocare un nuovo nodo per la lista compressa.
 * Serve a tenere pulita la funzione principale.
 */
link2 newNode2(int val, int count) {
    link2 n = (link2)malloc(sizeof(Node2));
    if (n == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }
    n->val = val;
    n->count = count;
    n->next = NULL;
    return n;
}

/* ============================================================================
 * FUNZIONE: comprimi
 * ============================================================================
 * SCOPO:
 * Legge una lista di interi e ne crea una NUOVA dove i duplicati consecutivi
 * sono raggruppati.
 *
 * PARAMETRI:
 * - link1 head: Puntatore alla testa della lista originale.
 *
 * RITORNA:
 * - link2: Puntatore alla testa della nuova lista compressa.
 *
 * CONCETTI CHIAVE:
 * 1. Scansione "Lookahead": Usiamo un ciclo per fissare un valore e un
 * sotto-ciclo (o logica interna) per scorrere avanti finché il valore è uguale.
 * 2. Costruzione in coda (Tail Pointer): Per inserire efficientemente nella
 * nuova lista, manteniamo un puntatore all'ultimo nodo inserito (`tail`).
 * Senza `tail`, dovremmo scorrere tutta la nuova lista ogni volta per
 * aggiungere un elemento (costo O(N^2)). Con `tail` è O(N).
 * ============================================================================
 */
link2 comprimi(link1 head) {

    /* --- 1. GESTIONE CASO BASE --- */
    if (head == NULL) {
        return NULL; // Lista vuota produce lista vuota
    }

    /* Puntatori per la nuova lista */
    link2 res_head = NULL; // La testa che ritorneremo
    link2 res_tail = NULL; // La coda per inserimenti veloci

    /* Puntatore per scorrere la lista originale */
    link1 curr = head;

    /* --- 2. CICLO PRINCIPALE --- */
    /* Iteriamo finché ci sono nodi nella lista originale */
    while (curr != NULL) {

        /* Memorizziamo il valore che stiamo analizzando ora */
        int target_val = curr->val;
        int conteggio = 0;

        /* --- 3. CICLO DI CONTEGGIO (LOGICA RLE) --- */
        /*
         * Continuiamo a scorrere 'curr' finché:
         * A) Non finisce la lista (curr != NULL)
         * B) E il valore è ancora uguale a quello corrente (curr->val == target_val)
         */
        while (curr != NULL && curr->val == target_val) {
            conteggio++;        // Abbiamo trovato un altro elemento uguale
            curr = curr->next;  // Avanziamo nella lista originale
        }

        /*
         * A questo punto 'curr' punta al PRIMO elemento diverso (o a NULL).
         * Abbiamo tutti i dati per creare il nodo compresso: (target_val, conteggio).
         */

        /* --- 4. ALLOCAZIONE NUOVO NODO LISTA 2 --- */
        link2 nuovo_nodo = newNode2(target_val, conteggio);

        /* --- 5. INSERIMENTO NELLA LISTA RISULTATO --- */
        if (res_head == NULL) {
            /* È il primo nodo che inseriamo */
            res_head = nuovo_nodo;
            res_tail = nuovo_nodo;
        } else {
            /*
             * DETTAGLIO SINTATTICO:
             * res_tail->next = ... : Agganciamo il nuovo nodo alla fine della lista.
             * res_tail = ...       : Spostiamo il puntatore 'tail' sull'ultimo nodo
             * appena creato, così siamo pronti per il prossimo giro.
             */
            res_tail->next = nuovo_nodo;
            res_tail = nuovo_nodo;
        }
    }

    return res_head;
}

/* ============================================================================
 * FUNZIONI HELPER PER IL MAIN (Creazione e Stampa)
 * ============================================================================
 */
link1 insertTail1(link1 head, int val) {
    link1 n = (link1)malloc(sizeof(Node1));
    n->val = val;
    n->next = NULL;

    if (head == NULL) return n;

    link1 curr = head;
    while (curr->next != NULL) curr = curr->next;
    curr->next = n;
    return head;
}

void printList1(link1 h) {
    while (h != NULL) {
        printf("%d -> ", h->val);
        h = h->next;
    }
    printf("NULL\n");
}

void printList2(link2 h) {
    while (h != NULL) {
        /* Stampiamo nel formato richiesto dall'esempio: (val, conteggio) */
        printf("(%d, %d) ", h->val, h->count);
        h = h->next;
    }
    printf("\n");
}

void freeList1(link1 h) {
    while (h != NULL) {
        link1 tmp = h;
        h = h->next;
        free(tmp);
    }
}

void freeList2(link2 h) {
    while (h != NULL) {
        link2 tmp = h;
        h = h->next;
        free(tmp);
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Ricreare l'esempio esatto: (3, 3, 3, 3, 2, 2, 3, 5, 5, 5)
 * Atteso: ((3, 4), (2, 2), (3, 1), (5, 3))
 * ============================================================================
 */
int main() {
    link1 lista_input = NULL;

    /* --- 1. CREAZIONE DATI HARDCODED --- */
    /* Inseriamo i valori uno a uno */
    int valori[] = {3, 3, 3, 3, 2, 2, 3, 5, 5, 5};
    int n = sizeof(valori) / sizeof(int);

    for (int i = 0; i < n; i++) {
        lista_input = insertTail1(lista_input, valori[i]);
    }

    printf("Lista Originale:\n");
    printList1(lista_input);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    printf("\nCompressione in corso...\n");
    link2 lista_compressa = comprimi(lista_input);

    /* --- 3. VERIFICA RISULTATI --- */
    printf("Lista Compressa (Attesa: (3, 4) (2, 2) (3, 1) (5, 3)):\n");
    printList2(lista_compressa);

    /* --- 4. GESTIONE MEMORIA --- */
    freeList1(lista_input);
    freeList2(lista_compressa);
    printf("\nMemoria liberata.\n");

    return 0;
}