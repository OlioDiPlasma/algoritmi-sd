#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT)
 * ============================================================================
 * 1. Definizione del Nodo (nodo_t):
 * Deve contenere il dato (intero) e DUE puntatori: uno al prossimo e uno al precedente.
 *
 * 2. Definizione dell'ADT Lista (list_t):
 * Come richiesto dal testo, contiene i puntatori alla testa e alla coda.
 */

typedef struct nodo {
    int val;              // Dato
    struct nodo *next;    // Puntatore al nodo successivo
    struct nodo *prev;    // Puntatore al nodo precedente (Cruciale per liste bi-linkate)
} nodo_t;

typedef struct {
    nodo_t *head;         // Inizio della lista
    nodo_t *tail;         // Fine della lista
} list_t;

/* ============================================================================
 * FUNZIONE HELPER: list_init
 * ============================================================================
 * SCOPO:
 * Inizializza la struttura della lista impostando i puntatori a NULL.
 * Evita di lavorare con memoria sporca.
 */
void list_init(list_t *l) {
    if (l != NULL) {
        l->head = NULL;
        l->tail = NULL;
    }
}

/* ============================================================================
 * FUNZIONE: list_insert
 * ============================================================================
 * SCOPO:
 * Inserisce un nuovo elemento nella lista.
 * Gestisce due modalità in base al parametro 'estremo':
 * - 0: Inserimento in TESTA (Head).
 * - 1: Inserimento in CODA (Tail).
 *
 * PARAMETRI:
 * - list_t *l:   Puntatore all'ADT lista.
 * - int chiave:  Il valore intero da inserire.
 * - int estremo: Flag per decidere la direzione (0=Testa, 1=Coda).
 *
 * CONCETTI CHIAVE:
 * 1. Aggiornamento Bidirezionale:
 * Quando colleghiamo due nodi A e B, dobbiamo dire "A->next = B" MA ANCHE "B->prev = A".
 *
 * 2. Caso Lista Vuota:
 * Se la lista è vuota, il nuovo nodo diventa SIA testa CHE coda. Questo è un
 * caso speciale che va gestito separatamente per evitare crash.
 * ============================================================================
 */
void list_insert(list_t *l, int chiave, int estremo) {
    if (l == NULL) return;

    /* --- 1. ALLOCAZIONE NUOVO NODO --- */
    nodo_t *new_node = (nodo_t *)malloc(sizeof(nodo_t));
    if (new_node == NULL) {
        printf("Errore malloc\n");
        return;
    }

    /* Inizializzazione base del nodo */
    new_node->val = chiave;
    new_node->next = NULL;
    new_node->prev = NULL;

    /* --- 2. CASO LISTA VUOTA --- */
    /* Se head è NULL, la lista è vuota.
       Il nuovo nodo diventa l'unico elemento (sia head che tail).
       La logica è identica indipendentemente da 'estremo'. */
    if (l->head == NULL) {
        l->head = new_node;
        l->tail = new_node;
        return; // Abbiamo finito
    }

    /* --- 3. INSERIMENTO IN TESTA (estremo == 0) --- */
    if (estremo == 0) {
        /*
         * Logica dei puntatori:
         * 1. Il 'next' del nuovo nodo punta alla vecchia testa.
         * 2. Il 'prev' della vecchia testa deve puntare al nuovo nodo (back-link).
         * 3. Aggiorniamo il puntatore 'head' dell'ADT.
         */
        new_node->next = l->head;   // 1
        l->head->prev = new_node;   // 2 (Accedo al campo prev del nodo puntato da head)
        l->head = new_node;         // 3
    }

    /* --- 4. INSERIMENTO IN CODA (estremo == 1) --- */
    else if (estremo == 1) {
        /*
         * Logica dei puntatori:
         * 1. Il 'prev' del nuovo nodo punta alla vecchia coda.
         * 2. Il 'next' della vecchia coda deve puntare al nuovo nodo.
         * 3. Aggiorniamo il puntatore 'tail' dell'ADT.
         */
        new_node->prev = l->tail;   // 1
        l->tail->next = new_node;   // 2
        l->tail = new_node;         // 3
    }
}

/* ============================================================================
 * FUNZIONE: list_display
 * ============================================================================
 * SCOPO:
 * Stampa il contenuto della lista.
 * - modo 0: Dalla Testa alla Coda (Forward).
 * - modo 1: Dalla Coda alla Testa (Backward).
 *
 * PARAMETRI:
 * - list_t *l: Puntatore alla lista.
 * - int modo:  Direzione di stampa.
 * ============================================================================
 */
void list_display(list_t *l, int modo) {
    if (l == NULL) return;

    printf("Lista [ ");

    /* --- MODO 0: HEAD -> TAIL --- */
    if (modo == 0) {
        nodo_t *curr = l->head; // Partiamo dall'inizio
        while (curr != NULL) {
            printf("%d ", curr->val);
            curr = curr->next;  // Avanziamo col 'next'
        }
    }
    /* --- MODO 1: TAIL -> HEAD --- */
    else if (modo == 1) {
        nodo_t *curr = l->tail; // Partiamo dalla fine!
        while (curr != NULL) {
            printf("%d ", curr->val);
            curr = curr->prev;  // Torniamo indietro col 'prev'
        }
    }
    
    printf("]\n");
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Testare inserimenti misti e le due direzioni di stampa.
 * ============================================================================
 */
int main() {
    /* Creazione istanza ADT */
    list_t miaLista;
    list_init(&miaLista);

    /* --- 1. POPOLAMENTO LISTA --- */
    printf("Inserisco 10 in Testa...\n");
    list_insert(&miaLista, 10, 0); // Lista: 10

    printf("Inserisco 20 in Testa...\n");
    list_insert(&miaLista, 20, 0); // Lista: 20 <-> 10

    printf("Inserisco 99 in Coda...\n");
    list_insert(&miaLista, 99, 1); // Lista: 20 <-> 10 <-> 99

    printf("Inserisco 5 in Testa...\n");
    list_insert(&miaLista, 5, 0);  // Lista: 5 <-> 20 <-> 10 <-> 99

    /* --- 2. VERIFICA STAMPA --- */
    printf("\nStampa Forward (Modo 0 - Atteso: 5 20 10 99):\n");
    list_display(&miaLista, 0);

    printf("Stampa Backward (Modo 1 - Atteso: 99 10 20 5):\n");
    list_display(&miaLista, 1);

    /* --- 3. PULIZIA MEMORIA (FREE) --- */
    /*
     * Dobbiamo scorrere la lista e liberare ogni nodo.
     * Attenzione: salvare il puntatore 'next' prima di fare free(curr).
     */
    nodo_t *curr = miaLista.head;
    nodo_t *temp;
    
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
    /* Reset dei puntatori dell'ADT per sicurezza */
    miaLista.head = NULL;
    miaLista.tail = NULL;
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}