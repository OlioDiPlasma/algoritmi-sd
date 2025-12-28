#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* * DEFINIZIONE STRUTTURE (Nascoste all'utente)
 */

/* * Nodo (Quasi ADT all'interno del modulo)
 * Contiene valore, contatore occorrenze e puntatore al prossimo.
 */
typedef struct node {
    int val;
    int occorrenze;     // Contatore richiesto dalla traccia
    struct node *next;
} node_t;

/* * Struttura Wrapper per la Lista
 * Mantiene il puntatore alla testa. 
 * Utile per gestire la lista vuota o il cambio di testa internamente.
 */
struct list_dummy {
    node_t *head;
    int size; // Opzionale, ma comodo per gestire le posizioni
};

/* --- 1. FUNZIONI DI UTILITÀ (Creazione Nodi) --- */

/*
 * Scopo: Allocare un nuovo nodo e inizializzarlo.
 * Parametri: Intero v (valore).
 * Ritorna: Puntatore al nuovo nodo.
 */
static node_t *newNode(int v) {
    node_t *n = (node_t *)malloc(sizeof(node_t));
    if (n == NULL) {
        printf("Errore allocazione memoria.\n");
        exit(1);
    }
    n->val = v;
    n->occorrenze = 1; // Prima volta che lo vediamo
    n->next = NULL;
    return n;
}

/* --- 2. INIZIALIZZAZIONE --- */
list_t LISTinit() {
    list_t L = (list_t)malloc(sizeof(struct list_dummy));
    L->head = NULL;
    L->size = 0;
    return L;
}

/* --- 3. LOGICA PRINCIPALE: LISTinsert --- */
/*
 * Scopo: Inserisce un elemento o incrementa il contatore se esiste.
 * Parametri:
 * - list_t L: Il puntatore alla lista.
 * - int posizione: Indice dove inserire se non presente (0 = testa).
 * - int val: Il valore da inserire.
 */
void LISTinsert(list_t L, int posizione, int val) {
    node_t *curr, *prev;
    
    /* CASO A: Lista Vuota
     * Se la lista è vuota, l'inserimento è banale, ma dobbiamo renderla circolare
     * (punta a se stesso).
     */
    if (L->head == NULL) {
        L->head = newNode(val);
        L->head->next = L->head; // Circolarità: punta a se stesso
        L->size++;
        return;
    }

    /* CASO B: Ricerca dell'elemento (Aggiornamento occorrenze)
     * Dobbiamo scorrere la lista. Poiché è circolare, il ciclo si ferma
     * quando torniamo alla testa.
     */
    curr = L->head;
    int found = 0;
    do {
        if (curr->val == val) {
            curr->occorrenze++; // Trovato! Incremento
            found = 1;
            break;
        }
        curr = curr->next;
    } while (curr != L->head); // Condizione di stop per liste circolari

    if (found) return; // Se trovato e incrementato, abbiamo finito.

    /* CASO C: Inserimento nuovo nodo (Elemento non presente) */
    
    node_t *n = newNode(val);
    
    // Sottocaso C1: Inserimento in Testa (Posizione 0)
    // In una lista circolare, inserire in testa richiede di aggiornare l'ultimo elemento!
    if (posizione == 0) {
        // 1. Trovo l'ultimo nodo (tail)
        node_t *tail = L->head;
        while (tail->next != L->head) {
            tail = tail->next;
        }
        
        // 2. Aggancio i puntatori
        n->next = L->head; // Il nuovo nodo punta alla vecchia testa
        L->head = n;       // Il wrapper ora punta al nuovo nodo (nuova testa)
        tail->next = L->head; // L'ultimo nodo chiude il cerchio sulla nuova testa
    }
    // Sottocaso C2: Inserimento in posizione generica
    else {
        // Scorro fino alla posizione desiderata o fine lista
        // Nota: partiamo dalla testa e ci muoviamo 'posizione-1' volte
        prev = L->head;
        int i = 0;
        
        // Ci fermiamo se finiamo il giro o arriviamo alla pos corretta
        while (i < posizione - 1 && prev->next != L->head) {
            prev = prev->next;
            i++;
        }
        
        // Inserimento standard tra prev e prev->next
        n->next = prev->next;
        prev->next = n;
    }
    
    L->size++;
}

/* --- 4. STAMPA E PULIZIA --- */

void LISTprint(list_t L) {
    if (L->head == NULL) {
        printf("Lista Vuota\n");
        return;
    }
    node_t *curr = L->head;
    printf("Lista: ");
    do {
        // Stampiamo valore e occorrenze
        printf("[%d|x%d] -> ", curr->val, curr->occorrenze);
        curr = curr->next;
    } while (curr != L->head);
    printf("(ritorno a testa %d)\n", L->head->val);
}

void LISTfree(list_t L) {
    if (L->head == NULL) {
        free(L);
        return;
    }
    node_t *curr = L->head;
    node_t *next;
    
    // Per liberare dobbiamo rompere il cerchio o usare un flag
    // Un modo sicuro è salvare la testa e fermarsi quando ci torniamo
    node_t *start = L->head;
    do {
        next = curr->next;
        free(curr);
        curr = next;
    } while (curr != start);
    
    free(L); // Libero il wrapper
}