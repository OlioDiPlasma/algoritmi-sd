#include <stdio.h>
#include <stdlib.h>
#include "LIST.h"

/* --- 1. DEFINIZIONE DELLE STRUTTURE DATI --- */

/* * NODE: Struttura classica per lista linkata.
 * Contiene il dato intero e il puntatore al prossimo nodo.
 */
struct node {
    int data;
    struct node *next;
};

/* * LIST (Rappresentazione interna):
 * Perché usiamo questa struttura "wrapper"? 
 * CONCETTO CHIAVE: Se LIST fosse semplicemente un puntatore al primo nodo (struct node*),
 * passando la lista alla funzione 'f' per valore, non potremmo modificare l'indirizzo 
 * di testa (necessario quando cancelliamo l'elemento 0).
 * Usando un wrapper che CONTIENE il puntatore alla testa, possiamo modificare 
 * l->head e la modifica resterà valida anche fuori dalla funzione.
 */
struct list_rep {
    struct node *head;
};

/* --- 2. LOGICA DELLA FUNZIONE RICHIESTA --- */

void f(LIST l) {
    /* Controllo difensivo: se la lista o il wrapper non esistono, usciamo */
    if (l == NULL || l->head == NULL) return;

    struct node *curr = l->head;  // Cursore per scorrere la lista
    struct node *prev = NULL;     // Puntatore al nodo precedente (per ricucire)
    int index = 0;                // Contatore della posizione ORIGINALE

    while (curr != NULL) {
        /* Salviamo il prossimo nodo prima di fare qualsiasi operazione,
         * perché se cancelliamo 'curr', perderemmo il link a 'curr->next'. */
        struct node *next_node = curr->next;

        /* VERIFICA CONDIZIONE: L'indice è divisibile per 3? (0, 3, 6...) */
        if (index % 3 == 0) {
            
            /* --- CANCELLAZIONE DEL NODO --- */
            
            /* Caso A: Stiamo cancellando la TESTA (indice 0) */
            if (prev == NULL) {
                /* Aggiorniamo la testa nel wrapper.
                 * La nuova testa diventa il nodo successivo. */
                l->head = next_node;
            } 
            /* Caso B: Stiamo cancellando un nodo INTERNO o di CODA */
            else {
                /* "Saltiamo" il nodo corrente collegando il precedente al successivo */
                prev->next = next_node;
            }

            /* Liberiamo la memoria del nodo cancellato.
             * NOTA: Non aggiorniamo 'prev' qui! 'prev' deve rimanere dov'era
             * perché il nodo attuale è sparito. */
            free(curr);
        } 
        else {
            /* --- MANTENIMENTO DEL NODO --- */
            /* Se non cancelliamo, il nodo corrente diventa il 'precedente' 
             * per la prossima iterazione */
            prev = curr;
        }

        /* Avanziamo col cursore e incrementiamo l'indice posizionale */
        curr = next_node;
        index++;
    }
}

/* --- 3. IMPLEMENTAZIONE FUNZIONI DI SUPPORTO (Per far funzionare il test) --- */

LIST creaListaVuota() {
    /* Allocazione del wrapper */
    LIST l = (LIST)malloc(sizeof(struct list_rep));
    if (l != NULL) l->head = NULL;
    return l;
}

void aggiungiInCoda(LIST l, int val) {
    if (l == NULL) return;

    /* Creazione nuovo nodo */
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = val;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        struct node *temp = l->head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = new_node;
    }
}

void stampaLista(LIST l) {
    if (l == NULL) return;
    struct node *temp = l->head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void distruggiLista(LIST l) {
    if (l == NULL) return;
    struct node *curr = l->head;
    while (curr != NULL) {
        struct node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l); // Liberiamo anche il wrapper
}