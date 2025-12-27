#include "list.h"
#include <stdlib.h> // Necessaria per malloc/free
#include <stdio.h>  // Necessaria per printf

/* --- DEFINIZIONE STRUTTURE DATI (Nascoste) --- */

// Il singolo Nodo
typedef struct node_s {
    int val;
    struct node_s *next;
} Node;

// Il Wrapper (ADT I Classe)
// Contiene il puntatore alla testa (e opzionalmente alla coda per efficienza)
struct list_s {
    Node *head;
    // Non è strettamente necessario 'tail' per l'esercizio, ma aiuta nell'append
    Node *tail; 
};

/* --- FUNZIONI DI UTILITÀ (HELPER) --- */

LIST LIST_new() {
    LIST l = (LIST)malloc(sizeof(struct list_s));
    if (l != NULL) {
        l->head = NULL;
        l->tail = NULL;
    }
    return l;
}

void LIST_append(LIST l, int val) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return;
    new_node->val = val;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
        l->tail = new_node;
    } else {
        l->tail->next = new_node; // Attacco in coda
        l->tail = new_node;       // Aggiorno il puntatore alla fine
    }
}

void LIST_print(LIST l) {
    if (!l) return;
    Node *curr = l->head;
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

void LIST_free(LIST l) {
    if (!l) return;
    Node *curr = l->head;
    while (curr != NULL) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp); // Libero il nodo
    }
    free(l); // Libero il wrapper
}

/* --- FUNZIONE AUSILIARIA PER LA RICERCA --- */
/*
 * Scopo: Verificare se un valore 'val' esiste nel vettore 'v'.
 * Ritorna: 1 (Vero) se presente, 0 (Falso) se assente.
 * Nota: Senza funzioni di libreria, implementiamo una ricerca lineare O(n).
 */
int is_in_vector(int val, int *v, int n) {
    for (int i = 0; i < n; i++) {
        if (v[i] == val) {
            return 1; // Trovato!
        }
    }
    return 0; // Non trovato
}

/* ============================================================================
 * FUNZIONE: f (Richiesta dall'esercizio)
 * ============================================================================
 * SCOPO:
 * Scorre la lista e CANCELLA fisicamente i nodi che contengono valori
 * non presenti nel vettore 'v'.
 *
 * PARAMETRI:
 * - LIST l: La lista da filtrare (modificata in-place).
 * - int *v: Il vettore "whitelist" (valori ammessi).
 * - int n: Dimensione del vettore.
 *
 * CONCETTI CHIAVE:
 * - Rimozione in lista singola: Per rimuovere un nodo 'curr', dobbiamo modificare
 * il puntatore 'next' del nodo PRECEDENTE ('prev').
 * - Caso speciale TESTA: Se dobbiamo rimuovere la testa, non c'è un 'prev'.
 * Dobbiamo aggiornare direttamente l->head.
 * - Free: È fondamentale chiamare free() sul nodo rimosso per evitare memory leaks.
 * ============================================================================
 */
void f(LIST l, int *v, int n) {
    /* Controllo sicurezza */
    if (l == NULL || l->head == NULL) return;

    Node *curr = l->head;
    Node *prev = NULL;

    while (curr != NULL) {
        /* --- 1. VERIFICA CONDIZIONE --- */
        // Devo tenere il nodo? (Esiste nel vettore v?)
        int keep = is_in_vector(curr->val, v, n);

        if (!keep) {
            /* --- 2. LOGICA DI CANCELLAZIONE --- */
            // Il valore NON è nel vettore -> Eliminare il nodo
            
            Node *to_delete = curr; // Salvo puntatore per fare free dopo

            if (curr == l->head) {
                // CASO A: Stiamo rimuovendo la TESTA
                l->head = curr->next; // La nuova testa è il prossimo
                curr = l->head;       // Avanzo curr
                // prev rimane NULL
            } else {
                // CASO B: Stiamo rimuovendo un nodo INTERNO o CODA
                prev->next = curr->next; // "Salto" il nodo da eliminare
                curr = prev->next;       // Avanzo curr
                // prev rimane lo stesso (perché il nodo attuale è sparito)
                
                // Opzionale: gestione tail (se servisse mantenerla aggiornata)
                if (curr == NULL) { 
                    l->tail = prev; // Se ho cancellato l'ultimo, tail è prev
                }
            }

            /* --- 3. GESTIONE MEMORIA --- */
            free(to_delete); // Importante!
        } else {
            /* --- 4. AVANZAMENTO NORMALE --- */
            // Il nodo va tenuto -> Avanzo entrambi i puntatori
            prev = curr;
            curr = curr->next;
        }
    }
}