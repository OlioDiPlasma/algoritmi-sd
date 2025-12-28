#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/*
 * ======================================================================================
 * NOME FUNZIONE: f (compatta lista)
 * ======================================================================================
 * SCOPO:
 * Data una lista ordinata, rimuove i duplicati consecutivi mantenendo solo la prima
 * occorrenza di ogni valore.
 *
 * PARAMETRI:
 * - LIST l: Il puntatore al primo nodo della lista.
 *
 * LOGICA:
 * Poiché la lista è ordinata, i duplicati sono sempre adiacenti.
 * Confrontiamo nodo corrente (curr) con il successivo (curr->next).
 * - Se sono UGUALI: Isoliamo il successivo, ricuciamo la lista saltandolo, e lo liberiamo.
 * - Se sono DIVERSI: Avanziamo con il puntatore corrente.
 *
 * NOTA SUL PUNTATORE 'LIST l':
 * Poiché manteniamo sempre la PRIMA occorrenza di ogni gruppo (es: 1 -> 1 -> 1 diventa 1),
 * la testa della lista (il primo nodo assoluto) non cambierà mai (a meno che la lista non sia vuota).
 * Quindi non serve passare un doppio puntatore (LIST *l) o ritornare la nuova testa.
 * ======================================================================================
 */
void f(LIST l) {
    /* --- 1. CONTROLLO LISTA VUOTA O SINGOLA --- */
    // Se la lista è vuota (NULL) o ha un solo elemento, non ci possono essere duplicati.
    if (l == NULL || l->next == NULL) {
        return;
    }

    /* --- 2. INIZIALIZZAZIONE SCORRIMENTO --- */
    struct node *curr = l; // 'curr' è il nostro cursore

    /* --- 3. CICLO DI SCANSIONE --- */
    // Continuiamo finché esiste un nodo successivo con cui confrontarci.
    while (curr->next != NULL) {
        
        // Confronto il valore del nodo corrente con quello del successivo
        if (curr->val == curr->next->val) {
            
            /* --- A. RILEVATO DUPLICATO --- */
            
            // 1. Salviamo il puntatore al nodo da cancellare in una variabile temporanea
            struct node *temp = curr->next;
            
            // 2. "Saltiamo" il nodo duplicato.
            // Il 'next' del corrente diventa il 'next' del nodo che stiamo per cancellare.
            // Esempio: [1] -> [1] -> [2]   diventa   [1] -> [2]
            curr->next = temp->next;
            
            // 3. Liberiamo la memoria del duplicato (Importante!)
            free(temp);
            
            /* * NOTA CRUCIALE: 
             * Qui NON facciamo 'curr = curr->next'.
             * Perché? Se avessimo 1 -> 1 -> 1, dopo aver cancellato il secondo 1,
             * ci troviamo nella situazione 1 -> 1. Dobbiamo ricontrollare ancora
             * il nodo corrente contro il "nuovo" successivo.
             */

        } else {
            /* --- B. NESSUN DUPLICATO --- */
            // Possiamo avanzare sicuri al prossimo nodo
            curr = curr->next;
        }
    }
}

/* --- FUNZIONI DI SUPPORTO (BOILERPLATE) --- */

LIST crea_nodo(int valore) {
    struct node *nuovo = (struct node *)malloc(sizeof(struct node));
    if (nuovo == NULL) exit(1);
    nuovo->val = valore;
    nuovo->next = NULL;
    return nuovo;
}

void stampa_lista(LIST l) {
    while (l != NULL) {
        printf("%d -> ", l->val);
        l = l->next;
    }
    printf("NULL\n");
}

void libera_lista(LIST l) {
    while (l != NULL) {
        struct node *temp = l;
        l = l->next;
        free(temp);
    }
}