#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT I Categoria)
 * ============================================================================
 * Concetto Chiave: Wrapper
 * L'esercizio richiede un ADT di I categoria. Questo significa che non passiamo
 * in giro direttamente il puntatore al primo nodo (node_t *), ma passiamo una
 * struttura "contenitore" (lista_t) che al suo interno ha il puntatore alla testa.
 */

/* 1. Il Nodo (Il vagone del treno) */
typedef struct node {
    int val;
    struct node *next;
} node_t;

/* 2. Il Wrapper (L'intestazione del treno) */
typedef struct {
    node_t *head;
} lista_t;

/* ============================================================================
 * FUNZIONE AUSILIARIA: append_node
 * ============================================================================
 * SCOPO:
 * Aggiunge un nuovo valore in fondo a una lista mantenendo l'ordine.
 * Usa un "doppio puntatore" per il puntatore 'tail' (coda) per rendere
 * l'inserimento efficiente O(1) invece di scorrere la lista ogni volta O(N).
 *
 * PARAMETRI:
 * - head_ref: Indirizzo del puntatore alla testa (per il caso lista vuota).
 * - tail_ref: Indirizzo del puntatore all'ultimo nodo (per aggiornare la coda).
 * - val:      Il valore da inserire.
 * ============================================================================
 */
void append_node(node_t **head_ref, node_t **tail_ref, int val) {
    /* Allocazione nuovo nodo */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("Errore memoria\n");
        exit(1);
    }
    new_node->val = val;
    new_node->next = NULL;

    /* Se la lista è vuota, il nuovo nodo è sia testa che coda */
    if (*head_ref == NULL) {
        *head_ref = new_node;
        *tail_ref = new_node;
    } else {
        /* Altrimenti, lo attacchiamo dopo l'attuale coda */
        (*tail_ref)->next = new_node;
        /* E aggiorniamo il puntatore di coda */
        *tail_ref = new_node;
    }
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: split_list
 * ============================================================================
 * SCOPO:
 * Divide la lista L0 in due nuove liste L1 e L2 basandosi sulla soglia 'n'.
 * - L1: contiene valori < n
 * - L2: contiene valori >= n
 * Mantiene l'ordine relativo degli elementi.
 *
 * PARAMETRI:
 * - n:  Intero soglia.
 * - L0: Puntatore al wrapper della lista originale (input).
 * - L2: Doppio puntatore al wrapper della seconda lista (output).
 * Perché **L2? Perché il main ci passa l'indirizzo di un puntatore (&L2)
 * e noi dobbiamo allocare la struttura lista_t e far puntare quella variabile
 * alla nuova memoria.
 *
 * RITORNA:
 * - lista_t *: Puntatore alla struttura della prima lista (L1).
 * ============================================================================
 */
lista_t *split_list(int n, lista_t *L0, lista_t **L2) {

    /* --- 1. ALLOCAZIONE WRAPPER (Le "scatole" delle liste) --- */
    
    /* Allocazione wrapper per L1 (che ritorneremo col return) */
    lista_t *L1 = (lista_t *)malloc(sizeof(lista_t));
    L1->head = NULL;

    /* Allocazione wrapper per L2 (che ritorneremo per riferimento) */
    /* Dettaglio Sintattico: *L2 accede alla variabile nel main. 
       Stiamo dicendo: "La variabile nel main ora punta a questa nuova malloc" */
    *L2 = (lista_t *)malloc(sizeof(lista_t));
    (*L2)->head = NULL;

    /* Se L0 è NULL o vuota, abbiamo finito (restituiamo liste vuote) */
    if (L0 == NULL || L0->head == NULL) {
        return L1;
    }

    /* --- 2. PREPARAZIONE VARIABILI DI SCANSIONE --- */
    /* Usiamo dei puntatori "tail" (coda) per L1 e L2.
       Perché? Per mantenere l'ordine relativo, dobbiamo inserire in coda.
       Se ogni volta ripartissimo dalla testa, l'algoritmo sarebbe lentissimo.
       Tenendo traccia dell'ultimo elemento, l'inserimento è istantaneo. */
    node_t *tailL1 = NULL;
    node_t *tailL2 = NULL;

    /* Puntatore per scorrere la lista originale */
    node_t *curr = L0->head;

    /* --- 3. CICLO DI SCANSIONE E DIVISIONE --- */
    while (curr != NULL) {
        
        /* Logica di confronto con la soglia */
        if (curr->val < n) {
            /* Caso A: Minore della soglia -> Va in L1 */
            /* Passiamo l'indirizzo della testa (L1->head) perché se è il primo
               elemento, append_node deve aggiornare la testa della lista.
               Passiamo l'indirizzo di tailL1 per aggiornare la coda. */
            append_node(&(L1->head), &tailL1, curr->val);
        } 
        else {
            /* Caso B: Maggiore o uguale -> Va in L2 */
            /* Nota sintattica complessa: (*L2)->head 
               1. *L2 dereferenzia il doppio puntatore (ottengo il puntatore al wrapper L2).
               2. ->head accede al campo head del wrapper.
               3. & prende l'indirizzo di questo campo head. */
            append_node(&((*L2)->head), &tailL2, curr->val);
        }

        /* Avanziamo nella lista originale */
        curr = curr->next;
    }

    return L1;
}

/* ============================================================================
 * FUNZIONI PER IL MAIN (Creazione, Stampa, Free)
 * ============================================================================
 */

/* Crea un wrapper lista vuoto */
lista_t *crea_lista() {
    lista_t *l = (lista_t *)malloc(sizeof(lista_t));
    l->head = NULL;
    return l;
}

/* Inserimento semplice in coda per popolare L0 nel main */
void inserisci(lista_t *l, int val) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->val = val;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        node_t *curr = l->head;
        while (curr->next != NULL) curr = curr->next;
        curr->next = new_node;
    }
}

void stampa_lista(lista_t *l, char *nome) {
    printf("%s: ", nome);
    if (l == NULL || l->head == NULL) {
        printf("VUOTA\n");
        return;
    }
    node_t *curr = l->head;
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

/* Fondamentale: Liberare la memoria */
void free_lista(lista_t *l) {
    if (l == NULL) return;
    node_t *curr = l->head;
    while (curr != NULL) {
        node_t *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l); // Libera anche il wrapper
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* --- 1. PREPARAZIONE DATI --- */
    /* Soglia richiesta dall'esempio */
    int n = 18;
    
    /* Lista L0, L1, L2 come puntatori a wrapper */
    lista_t *L0 = crea_lista();
    lista_t *L1 = NULL; // Sarà ritornato dalla funzione
    lista_t *L2 = NULL; // Sarà modificato "by reference"

    /* Popoliamo L0 con i dati dell'esempio: 7, 8, 25, 2, 9, -5, 10, 37 */
    inserisci(L0, 7);
    inserisci(L0, 8);
    inserisci(L0, 25);
    inserisci(L0, 2);
    inserisci(L0, 9);
    inserisci(L0, -5);
    inserisci(L0, 10);
    inserisci(L0, 37);

    stampa_lista(L0, "Lista L0 (Originale)");
    printf("Soglia di divisione: %d\n", n);
    printf("--------------------------------------------------\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /* Sintassi Chiave: &L2
       Passiamo l'indirizzo del puntatore L2. In questo modo la funzione
       può allocare memoria e assegnarla a L2. */
    L1 = split_list(n, L0, &L2);

    /* --- 3. VERIFICA RISULTATI --- */
    /* L1 dovrebbe contenere: 7, 8, 2, 9, -5, 10 */
    stampa_lista(L1, "Lista L1 (Minori di 18)");
    
    /* L2 dovrebbe contenere: 25, 37 */
    stampa_lista(L2, "Lista L2 (Maggiori o uguali a 18)");

    /* --- 4. PULIZIA MEMORIA --- */
    free_lista(L0);
    free_lista(L1);
    free_lista(L2);
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}