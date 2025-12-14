#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE STRUTTURE DATI
 * ======================================================================================
 * Usiamo una lista semplice collegata (Linked List) con un wrapper ADT.
 */

typedef struct node {
    int val;
    struct node *next;
} Node;

// Wrapper per l'ADT List
typedef struct {
    Node *head;
} List;

/*
 * ======================================================================================
 * FUNZIONE: listDoubleItem
 * ======================================================================================
 * Scopo:
 * Cerca 'key', raddoppia il valore e sposta il nodo per mantenere l'ordinamento.
 *
 * Parametri:
 * - List *l: Puntatore alla struttura lista (per poter modificare la testa se necessario).
 * - int key: Il valore da cercare.
 *
 * Concetti Chiave:
 * - Single Pass (Unico passaggio): Non resettiamo mai i puntatori all'inizio.
 * - Detach & Reattach: Stacchiamo il nodo come un vagone di un treno e lo riagganciamo più avanti.
 * - Gestione dei Puntatori: Usiamo 'prev' (precedente) e 'curr' (corrente) per cucire la lista.
 */
bool listDoubleItem(List *l, int key) {
    /* --- 1. CONTROLLI INIZIALI --- */
    if (l == NULL || l->head == NULL) {
        return false; // Lista vuota o inesistente
    }

    Node *prev = NULL;
    Node *curr = l->head;

    /* --- 2. FASE DI RICERCA (SEARCH) --- */
    // Scorriamo finché non troviamo la chiave o finché i valori sono minori della chiave
    // (essendo ordinata, se superiamo 'key' possiamo fermarci).
    while (curr != NULL && curr->val < key) {
        prev = curr;
        curr = curr->next;
    }

    // Se siamo arrivati in fondo o abbiamo superato la chiave, l'elemento non c'è.
    if (curr == NULL || curr->val != key) {
        return false;
    }

    /* --- 3. TROVATO! MODIFICA VALORE --- */
    // A questo punto 'curr' punta al nodo con la chiave.
    // Raddoppiamo il valore in loco.
    curr->val = curr->val * 2;
    int newVal = curr->val;

    // Controllo rapido: se il nodo successivo è NULL o ha un valore >= del nuovo valore,
    // l'ordine è già preservato! Non serve spostare nulla.
    if (curr->next == NULL || curr->next->val >= newVal) {
        return true; 
    }

    /* --- 4. SPOSTAMENTO (DETACH & MOVE) --- */
    // Se siamo qui, l'ordine è violato. Dobbiamo spostare 'curr' più avanti.
    
    Node *target = curr; // Memorizziamo il nodo da spostare

    // A. STACCARE IL NODO (DETACH)
    // Colleghiamo il precedente direttamente al successivo, saltando 'target'.
    if (prev == NULL) {
        // Caso speciale: stiamo spostando la TESTA della lista
        l->head = target->next;
    } else {
        // Caso normale: scavalchiamo 'target'
        prev->next = target->next;
    }

    // Ora 'target' è "in volo", scollegato dalla lista logica.
    // Dobbiamo trovare dove atterrare. 
    // NOTA BENE: Non ripartiamo dall'inizio! Continuiamo da dove eravamo.
    // I puntatori 'prev' e 'curr' devono riposizionarsi per la continuazione della scansione.
    
    // Se abbiamo staccato la testa, il nuovo punto di partenza per 'prev' è NULL.
    // Altrimenti 'prev' rimane dov'è (puntava al nodo PRIMA di target).
    // 'curr' deve diventare il nodo successivo a target (da dove riprende la ricerca).
    curr = target->next; 

    // B. TROVARE NUOVA POSIZIONE (CONTINUE SCAN)
    // Scorriamo in avanti finché non troviamo il punto di inserimento per 'newVal'
    while (curr != NULL && curr->val < newVal) {
        prev = curr;       // Avanza prev
        curr = curr->next; // Avanza curr
    }

    // C. RIATTACCARE IL NODO (REATTACH)
    // Inseriamo 'target' tra 'prev' e 'curr'.
    target->next = curr;
    
    if (prev == NULL) {
        // Teoricamente impossibile se i valori crescono e la lista era ordinata,
        // a meno che la lista non fosse vuota (gestito) o tutti elementi < newVal (gestito sopra).
        // Ma per robustezza sintattica:
        l->head = target;
    } else {
        prev->next = target;
    }

    return true;
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO (PER IL TEST)
 * ======================================================================================
 */

// Inserimento ordinato (per costruire la lista di prova)
void insertSorted(List *l, int v) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->val = v;
    new_node->next = NULL;

    if (l->head == NULL || l->head->val >= v) {
        new_node->next = l->head;
        l->head = new_node;
    } else {
        Node *curr = l->head;
        while (curr->next != NULL && curr->next->val < v) {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
}

// Stampa lista
void printList(List *l) {
    Node *curr = l->head;
    printf("LISTA: ");
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    List mylist = {NULL};

    // Creiamo una lista ordinata: 2 -> 5 -> 8 -> 12 -> 20
    insertSorted(&mylist, 2);
    insertSorted(&mylist, 5);
    insertSorted(&mylist, 8);
    insertSorted(&mylist, 12);
    insertSorted(&mylist, 20);

    printList(&mylist);

    /* --- TEST 1: Modifica con spostamento --- */
    // Raddoppiamo il 5 -> diventa 10.
    // Dovrebbe spostarsi tra 8 e 12.
    printf("\nRaddoppio chiave 5...\n");
    bool res = listDoubleItem(&mylist, 5);
    printf("Risultato: %s\n", res ? "Trovato" : "Non Trovato");
    printList(&mylist); // Atteso: 2 -> 8 -> 10 -> 12 -> 20

    /* --- TEST 2: Modifica senza spostamento --- */
    // Raddoppiamo il 20 -> diventa 40. Rimane in fondo.
    printf("\nRaddoppio chiave 20...\n");
    listDoubleItem(&mylist, 20);
    printList(&mylist); // Atteso: 2 -> 8 -> 10 -> 12 -> 40

    /* --- TEST 3: Elemento non presente --- */
    printf("\nRaddoppio chiave 99...\n");
    res = listDoubleItem(&mylist, 99);
    printf("Risultato: %s\n", res ? "Trovato" : "Non Trovato");

    /* --- CLEANUP (MEMORY MANAGEMENT) --- */
    Node *curr = mylist.head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }

    return 0;
}