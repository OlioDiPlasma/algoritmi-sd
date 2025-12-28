#include <stdio.h>
#include <stdlib.h> // Necessario solo per malloc/free/NULL
#include "list.h"

/* =================================================================================
 * DEFINIZIONE DEI TIPI (Nascosti all'utente)
 * =================================================================================
 */

/* * QUASI ADT: IL NODO
 * Contiene i due operandi (v1, v2) per riferimento visivo e il loro prodotto
 * per l'ordinamento.
 */
typedef struct node {
    int val1;           // Elemento dal primo vettore
    int val2;           // Elemento dal secondo vettore
    int product;        // Il prodotto (val1 * val2), usato per ordinare
    struct node *next;  // Puntatore al prossimo nodo
} node_t;

/* * ADT DI I CLASSE: IL WRAPPER
 * Contiene il puntatore alla testa della lista.
 */
struct list_dummy {
    node_t *head;
};

/* =================================================================================
 * FUNZIONE AUSILIARIA (Privata): newNode
 * ---------------------------------------------------------------------------------
 * Crea un nuovo nodo allocandolo in memoria.
 * =================================================================================
 */
static node_t *newNode(int a, int b) {
    /* --- 1. ALLOCAZIONE --- */
    // Chiediamo al sistema operativo spazio per un nodo
    node_t *n = (node_t *)malloc(sizeof(node_t));
    if (n == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }
    /* --- 2. INIZIALIZZAZIONE --- */
    n->val1 = a;
    n->val2 = b;
    n->product = a * b; // Pre-calcoliamo il prodotto
    n->next = NULL;
    return n;
}

/* =================================================================================
 * FUNZIONE AUSILIARIA (Privata): insertOrdered
 * ---------------------------------------------------------------------------------
 * SCOPO: Inserisce un nodo nella lista mantenendo l'ordine CRESCENTE del prodotto.
 * =================================================================================
 */
static void insertOrdered(list_t L, int a, int b) {
    node_t *new_node = newNode(a, b);

    /* CASO 1: Lista vuota o inserimento in TESTA */
    // Se la lista è vuota OPPURE il nuovo prodotto è minore del primo elemento:
    if (L->head == NULL || new_node->product < L->head->product) {
        // Il nuovo nodo punta alla vecchia testa
        new_node->next = L->head;
        // La testa della lista diventa il nuovo nodo
        L->head = new_node;
        return;
    }

    /* CASO 2: Inserimento nel mezzo o in coda */
    // Dobbiamo trovare il punto giusto. Usiamo due puntatori o uno con look-ahead.
    // Usiamo 'curr' per scorrere. Ci fermiamo quando il PROSSIMO è più grande.
    node_t *curr = L->head;

    // Scorro finché:
    // 1. Non finisco la lista (curr->next != NULL)
    // 2. E il prodotto del nodo SUCCESSIVO è ancora minore o uguale al mio
    while (curr->next != NULL && curr->next->product <= new_node->product) {
        curr = curr->next; // Avanzo
    }

    /* INSERIMENTO */
    // Ho trovato il punto: inserisco tra 'curr' e 'curr->next'
    new_node->next = curr->next; // Il nuovo punta al successivo
    curr->next = new_node;       // Il corrente punta al nuovo
}

/* =================================================================================
 * FUNZIONE PRINCIPALE: prodCart
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Genera il prodotto cartesiano. Scorre v1 e v2 con due loop annidati e
 * inserisce ogni coppia nella lista ordinata.
 *
 * PARAMETRI:
 * - int *v1, int d1: Primo vettore e dimensione.
 * - int *v2, int d2: Secondo vettore e dimensione.
 * =================================================================================
 */
list_t prodCart(int *v1, int d1, int *v2, int d2) {
    
    /* --- 1. ALLOCAZIONE LISTA (Wrapper) --- */
    list_t L = (list_t)malloc(sizeof(struct list_dummy));
    if (L == NULL) return NULL;
    L->head = NULL; // Importante inizializzare a NULL

    /* --- 2. LOGICA PRODOTTO CARTESIANO --- */
    // Doppio ciclo for per generare tutte le coppie (x, y)
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            
            // Per ogni coppia, chiamiamo la funzione che inserisce ordinato.
            // Passiamo v1[i] e v2[j].
            insertOrdered(L, v1[i], v2[j]);
        }
    }

    return L;
}

/* =================================================================================
 * FUNZIONI DI UTILITÀ (Stampa e Free)
 * =================================================================================
 */
void listPrint(list_t L) {
    if (L == NULL || L->head == NULL) {
        printf("Lista vuota\n");
        return;
    }
    node_t *curr = L->head;
    printf("L = ");
    while (curr != NULL) {
        // Stampiamo formato (v1, v2) [prod]
        printf("(%d,%d)[p=%d] ", curr->val1, curr->val2, curr->product);
        curr = curr->next;
        if(curr != NULL) printf(", ");
    }
    printf("\n");
}

void listFree(list_t L) {
    if (L == NULL) return;
    node_t *curr = L->head;
    node_t *next;
    while (curr != NULL) {
        next = curr->next; // Salvo il riferimento al prossimo
        free(curr);        // Libero il corrente
        curr = next;       // Avanzo
    }
    free(L); // Libero il wrapper
}