#include "list.h"
#include <stdlib.h> // Necessaria per malloc e free
#include <stdio.h>  // Necessaria solo per printf (debug)

/* --- DEFINIZIONE STRUTTURE DATI (Nascoste nel .c) --- */

// Il nodo della lista concatenata
typedef struct node_s {
    int val;
    struct node_s *next;
} Node;

// Il wrapper (l'ADT vero e proprio).
// Una LIST non è un nodo, ma una scatola che CONTIENE il puntatore al primo nodo.
// Questo facilita la gestione di liste vuote o modifiche alla testa.
struct list_s {
    Node *head;
    Node *tail; // Ottimizzazione: teniamo traccia dell'ultimo per inserimenti O(1)
};

/* --- FUNZIONI DI UTILITÀ (HELPER) --- */

/*
 * Scopo: Creare una nuova lista vuota.
 * Allocazione: Alloca la struttura wrapper.
 */
LIST LIST_new() {
    LIST l = (LIST)malloc(sizeof(struct list_s));
    if (l != NULL) {
        l->head = NULL;
        l->tail = NULL;
    }
    return l;
}

/*
 * Scopo: Inserire un valore in fondo alla lista.
 * Concetto chiave: Usiamo il puntatore 'tail' per non dover scorrere tutta la lista ogni volta.
 */
void LIST_append(LIST l, int val) {
    // Allocazione del nuovo nodo
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return; // Gestione errore allocazione
    
    new_node->val = val;
    new_node->next = NULL;

    // Se la lista è vuota, testa e coda coincidono
    if (l->head == NULL) {
        l->head = new_node;
        l->tail = new_node;
    } else {
        // Altrimenti attacco al nodo che era l'ultimo
        l->tail->next = new_node;
        // E aggiorno il puntatore tail
        l->tail = new_node;
    }
}

void LIST_print(LIST l) {
    if (l == NULL || l->head == NULL) {
        printf("Empty\n");
        return;
    }
    Node *curr = l->head;
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

void LIST_free(LIST l) {
    if (l == NULL) return;
    Node *curr = l->head;
    while (curr != NULL) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp); // Libero il singolo nodo
    }
    free(l); // Libero il wrapper
}

/* ============================================================================
 * FUNZIONE: split
 * ============================================================================
 * SCOPO:
 * Divide una lista in un vettore di sottomatrici basandosi sulla parità contigua.
 *
 * PARAMETRI:
 * - LIST l: La lista originale (da non modificare).
 * - int *n: Puntatore a intero dove scriveremo la dimensione del vettore risultante.
 *
 * CONCETTI CHIAVE:
 * - Passaggio 1 (Conteggio): Scorriamo la lista per capire quanti "blocchi" di parità
 * diversa ci sono. Questo serve per sapere quanto grande fare la malloc del vettore.
 * - Passaggio 2 (Costruzione): Scorriamo di nuovo e copiamo i valori in nuove liste.
 * - Deep Copy: Non spostiamo i nodi originali (che distruggerebbe la lista 'l'),
 * ma creiamo NUOVI nodi con lo stesso valore (tramite LIST_append).
 * ============================================================================
 */
LIST* split(LIST l, int *n) {
    
    /* --- 1. GESTIONE LISTA VUOTA --- */
    if (l == NULL || l->head == NULL) {
        *n = 0;
        return NULL;
    }

    /* --- 2. LOGICA DI CONTEGGIO (PRIMO PASSAGGIO) --- */
    // Contiamo quante sottoliste serviranno.
    // Partiamo da 1 perché c'è almeno un blocco (la lista non è vuota).
    int count = 1;
    Node *curr = l->head;

    // Scorriamo fino al PENULTIMO nodo (curr->next != NULL)
    while (curr->next != NULL) {
        // Accesso ai valori: curr->val
        // Controllo parità: (a % 2) != (b % 2) rileva se uno è pari e l'altro dispari.
        // Nota: abs() servirebbe per i negativi, ma qui assumiamo interi standard o positivi.
        // Un modo sicuro per la parità è controllare il bit meno significativo, ma % 2 va bene.
        int parita_curr = (curr->val % 2);
        int parita_next = (curr->next->val % 2);

        // Se la parità cambia, abbiamo finito un blocco e ne inizia uno nuovo.
        if (abs(parita_curr) != abs(parita_next)) {
            count++;
        }
        
        // Aritmetica dei puntatori: avanzo al prossimo nodo
        curr = curr->next;
    }

    // Salviamo il risultato nel puntatore fornito dal chiamante
    // Dereferenziazione: scriviamo all'indirizzo puntato da n
    *n = count;

    /* --- 3. ALLOCAZIONE DEL VETTORE DI LISTE --- */
    // Dobbiamo restituire un LIST* (array di LIST).
    // Ogni elemento dell'array è una LIST (che è un puntatore a struct list_s).
    LIST *result_vector = (LIST *)malloc(count * sizeof(LIST));
    if (result_vector == NULL) return NULL; // Errore fatale memoria

    // Inizializziamo subito tutte le liste nel vettore per poter usare LIST_append
    for (int i = 0; i < count; i++) {
        result_vector[i] = LIST_new();
    }

    /* --- 4. POPOLAMENTO (SECONDO PASSAGGIO) --- */
    curr = l->head;
    int current_list_index = 0; // Indice corrente nel vettore 'result_vector'

    while (curr != NULL) {
        // Aggiungiamo il valore corrente alla lista corrente nel vettore.
        // Sintassi: result_vector[current_list_index] è la lista destinazione.
        LIST_append(result_vector[current_list_index], curr->val);

        // Ora decidiamo se dobbiamo cambiare indice per il prossimo giro.
        // Guardiamo avanti (lookahead) solo se non siamo all'ultimo nodo.
        if (curr->next != NULL) {
            int parita_curr = (curr->val % 2);
            int parita_next = (curr->next->val % 2);

            // Se la parità cambia, incrementiamo l'indice del vettore
            // così al prossimo giro scriveremo nella lista successiva.
            if (abs(parita_curr) != abs(parita_next)) {
                current_list_index++;
            }
        }

        curr = curr->next;
    }

    return result_vector;
}