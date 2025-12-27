#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* =========================================================================
 * STRUTTURE DATI (Nascoste nel .c)
 * =========================================================================
 */

/* 1. Il Nodo della lista doppia */
typedef struct node {
    int val;
    struct node *next; // Puntatore al prossimo elemento
    struct node *prev; // Puntatore all'elemento precedente
} Node;

/* 2. Il Wrapper (la "maniglia" della lista) */
struct list_wrapper {
    Node *head; // Punta al primo nodo della lista
    /* Potremmo avere anche *tail o int size, ma non sono strettamente necessari qui */
};

/* =========================================================================
 * FUNZIONI DI UTILITÀ (Costruzione e Gestione)
 * =========================================================================
 */

/* Inizializza una lista vuota */
LIST LISTinit() {
    LIST l = malloc(sizeof(struct list_wrapper));
    l->head = NULL;
    return l;
}

/* Aggiunge in testa (utile per popolare la lista nel main) */
void LISTaddHead(LIST l, int val) {
    Node *new_node = malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = l->head;
    new_node->prev = NULL; // È la nuova testa, non ha precedenti

    /* Se la lista non era vuota, il vecchio primo nodo deve puntare indietro al nuovo */
    if (l->head != NULL) {
        l->head->prev = new_node;
    }
    
    /* Aggiorniamo il puntatore di testa nel wrapper */
    l->head = new_node;
}

/* Stampa la lista per verificare i risultati */
void LISTprint(LIST l) {
    Node *x = l->head;
    printf("L = ");
    while (x != NULL) {
        printf("%d ", x->val);
        if (x->next != NULL) printf("<-> "); // Freccia doppia per indicare lista doppia
        x = x->next;
    }
    printf("\n");
}

/* Libera tutta la memoria */
void LISTdestroy(LIST l) {
    Node *curr = l->head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l); // Libero anche il wrapper
}

/* =========================================================================
 * FUNZIONE PURGE (Cuore dell'esercizio)
 * =========================================================================
 * SCOPO:
 * Rimuove dalla lista tutti i nodi il cui valore NON è divisibile per 'div'.
 *
 * PARAMETRI:
 * - l: Il puntatore al wrapper della lista.
 * - div: Il divisore (intero).
 *
 * CONCETTI CHIAVE:
 * 1. Attraversamento sicuro: Quando cancelliamo un nodo (free), perdiamo
 * i suoi puntatori 'next' e 'prev'. Dobbiamo salvare il 'next' PRIMA
 * di cancellare il nodo corrente per poter continuare il ciclo.
 * 2. Ricucitura (Rewiring): Cancellare un nodo in una lista doppia richiede
 * di aggiornare il 'next' del nodo precedente E il 'prev' del nodo successivo.
 * 3. Caso Testa: Se cancelliamo il primo nodo, dobbiamo aggiornare l->head.
 *
 * BLOCCCHI LOGICI:
 * 1. Inizializzazione puntatori.
 * 2. Ciclo di scansione.
 * 3. Logica di rimozione (con gestione casi speciali: testa, centro, coda).
 * ========================================================================= */
void purge(LIST l, int div) {
    /* --- 1. INIZIALIZZAZIONE --- */
    if (l == NULL || l->head == NULL) return; // Lista vuota o inesistente

    Node *curr = l->head; // Partiamo dalla testa
    Node *next_save;      // Variabile temporanea per non perdere il filo

    /* --- 2. CICLO DI SCANSIONE --- */
    while (curr != NULL) {
        
        /* Salviamo SUBITO il riferimento al prossimo nodo.
         * Perché? Se decidiamo di fare free(curr), non potremo più fare curr->next. */
        next_save = curr->next;

        /* --- 3. CONTROLLO CONDIZIONE --- */
        /* La traccia dice: cancella se NON divisibile (val % div != 0) */
        if (curr->val % div != 0) {
            
            /* Dobbiamo rimuovere 'curr'. Gestiamo i collegamenti. */

            /* PASSO A: Gestione del predecessore (Indietro) */
            if (curr->prev != NULL) {
                /* Caso nodo centrale o finale:
                 * Il 'next' del nodo precedente deve saltare 'curr' e puntare al successivo. */
                curr->prev->next = curr->next;
            } else {
                /* Caso nodo di TESTA:
                 * Se non c'è un precedente, stiamo cancellando la testa.
                 * Il wrapper deve ora puntare al secondo nodo. */
                l->head = curr->next;
            }

            /* PASSO B: Gestione del successore (Avanti) */
            if (curr->next != NULL) {
                /* Il 'prev' del nodo successivo deve saltare 'curr' e puntare all'indietro
                 * verso il nodo precedente di 'curr'. */
                curr->next->prev = curr->prev;
            }

            /* PASSO C: Gestione Memoria */
            /* Ora che la lista è "ricucita" attorno a curr, possiamo liberarlo. */
            free(curr);
        }
        
        /* --- 4. AVANZAMENTO --- */
        /* Passiamo al nodo successivo salvato all'inizio (che potrebbe essere NULL) */
        curr = next_save;
    }
}