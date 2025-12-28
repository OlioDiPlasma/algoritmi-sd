#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> // Serve per FLT_MAX (massimo valore float)

/* ============================================================================
 * DEFINIZIONE DEI TIPI DI DATO (come richiesto dalla traccia)
 * ============================================================================
 */

/* 1. Lista interna dei concessionari (list1_t) */
typedef struct list1_node {
    char *nome_concessionario;
    float prezzo;
    struct list1_node *next;
} list1_t;

/* 2. Nodo dell'albero BST (bst_t) */
typedef struct bst_node {
    char *nome_modello;   // Chiave di ordinamento del BST
    list1_t *concessionari; // Testa della lista dei concessionari
    struct bst_node *left;
    struct bst_node *right;
} bst_t;

/* 3. Lista di output (list2_t) */
typedef struct list2_node {
    char *nome_modello;
    char *nome_concessionario_min;
    float prezzo_min;
    struct list2_node *next;
} list2_t;

/* ============================================================================
 * FUNZIONE HELPER: crea_nodo_lista2
 * ============================================================================
 * Scopo:
 * Alloca e inizializza un nuovo nodo per la lista finale.
 * Copia le stringhe per evitare problemi di condivisione di memoria.
 */
list2_t *crea_nodo_lista2(char *modello, char *concessionario, float prezzo) {
    /* --- ALLOCAZIONE --- */
    list2_t *new_node = (list2_t *)malloc(sizeof(list2_t));
    if (new_node == NULL) exit(1); // Errore critico di memoria

    /* --- COPIA DEI DATI --- */
    /*
     * Usiamo malloc + strcpy per le stringhe.
     * strlen(s) + 1 serve per includere il terminatore '\0'.
     */
    new_node->nome_modello = (char *)malloc(strlen(modello) + 1);
    strcpy(new_node->nome_modello, modello);

    new_node->nome_concessionario_min = (char *)malloc(strlen(concessionario) + 1);
    strcpy(new_node->nome_concessionario_min, concessionario);

    new_node->prezzo_min = prezzo;
    new_node->next = NULL;

    return new_node;
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: bst2list
 * ============================================================================
 * Scopo:
 * Visita il BST in ordine alfabetico (In-Order) e costruisce una lista lineare
 * contenente, per ogni modello, il concessionario col prezzo più basso.
 *
 * Parametri:
 * - bst: puntatore alla radice dell'albero corrente.
 * - head: doppio puntatore alla testa della lista risultato (per inizializzarla).
 * - tail: doppio puntatore all'ultimo elemento della lista risultato (per inserimento O(1)).
 *
 * Concetti Chiave:
 * - Visita Simmetrica (In-Order): Andando a sx, processando il nodo, poi andando a dx,
 * garantiamo che la lista risultante sia ordinata alfabeticamente (A-Z).
 * - Doppio Puntatore Tail: Manteniamo un puntatore all'ultimo nodo inserito.
 * Così per aggiungere in coda non dobbiamo scorrere tutta la lista ogni volta.
 * ============================================================================
 */
void bst2list(bst_t *bst, list2_t **head, list2_t **tail) {
    /* --- 1. CASO BASE --- */
    if (bst == NULL) {
        return; // Albero vuoto o foglia raggiunta, torniamo su.
    }

    /* --- 2. RICORSIONE SINISTRA (Visita sottoalbero con nomi "minori") --- */
    bst2list(bst->left, head, tail);

    /* --- 3. ELABORAZIONE NODO CORRENTE (ROOT) --- */
    
    /* A. Trovare il prezzo minimo nella lista dei concessionari del nodo corrente */
    list1_t *curr = bst->concessionari;
    float min_p = FLT_MAX; // Inizializzo a valore altissimo
    char *best_dealer = NULL;

    /* Scorro la lista dei concessionari */
    while (curr != NULL) {
        /* Controllo se il prezzo attuale è minore del minimo trovato finora */
        if (curr->prezzo < min_p) {
            min_p = curr->prezzo;
            best_dealer = curr->nome_concessionario;
        }
        /* Avanzamento puntatore: vado al prossimo concessionario */
        curr = curr->next;
    }

    /* B. Se abbiamo trovato un venditore (la lista non era vuota), creiamo il nodo */
    if (best_dealer != NULL) {
        list2_t *new_node = crea_nodo_lista2(bst->nome_modello, best_dealer, min_p);

        /* C. Inserimento in coda nella lista finale */
        if (*head == NULL) {
            /* Caso lista vuota: il nuovo nodo è sia testa che coda */
            *head = new_node; // Modifico il puntatore nel chiamante
            *tail = new_node;
        } else {
            /* Caso generico: collego l'attuale ultimo al nuovo nodo */
            /* (*tail) accede al nodo che è attualmente l'ultimo.
             * (*tail)->next accede al suo campo next. */
            (*tail)->next = new_node;
            
            /* Aggiorno il puntatore tail per puntare al nuovo ultimo */
            *tail = new_node;
        }
    }

    /* --- 4. RICORSIONE DESTRA (Visita sottoalbero con nomi "maggiori") --- */
    bst2list(bst->right, head, tail);
}

/* ============================================================================
 * HELPER FUNCTIONS PER IL TEST (Creazione e Stampa)
 * ============================================================================
 */

/* Helper per aggiungere un concessionario a un modello */
void add_dealer(bst_t *node, char *name, float price) {
    list1_t *new_dealer = (list1_t *)malloc(sizeof(list1_t));
    new_dealer->nome_concessionario = strdup(name); // strdup duplica la stringa
    new_dealer->prezzo = price;
    new_dealer->next = node->concessionari; // Inserimento in testa
    node->concessionari = new_dealer;
}

/* Helper per creare un nodo BST */
bst_t *create_bst_node(char *model) {
    bst_t *node = (bst_t *)malloc(sizeof(bst_t));
    node->nome_modello = strdup(model);
    node->concessionari = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Helper per liberare la lista risultato */
void free_list2(list2_t *head) {
    while (head != NULL) {
        list2_t *temp = head;
        head = head->next;
        free(temp->nome_modello);
        free(temp->nome_concessionario_min);
        free(temp);
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE MANUALE ALBERO (HARDCODED) --- */
    /*
     * Struttura Albero (ordinato per modello):
     * [Fiat Panda]
     * /           \
     * [Alfa Giulia]    [Lancia Y]
     */
    
    bst_t *root = create_bst_node("Fiat Panda");
    add_dealer(root, "Concessionario A", 12000.0);
    add_dealer(root, "Concessionario B", 11500.0); // Prezzo migliore

    root->left = create_bst_node("Alfa Giulia");
    add_dealer(root->left, "Sport Cars", 45000.0); // Unico prezzo
    
    root->right = create_bst_node("Lancia Y");
    add_dealer(root->right, "City Motors", 14000.0);
    add_dealer(root->right, "Best Auto", 13500.0); // Prezzo migliore

    /* --- 2. PREPARAZIONE VARIABILI --- */
    list2_t *head = NULL;
    list2_t *tail = NULL;

    printf("--- INIZIO ELABORAZIONE ---\n");

    /* --- 3. CHIAMATA FUNZIONE --- */
    /* Passiamo l'indirizzo di head e tail (&head, &tail) perché la funzione deve modificarli */
    bst2list(root, &head, &tail);

    /* --- 4. STAMPA RISULTATI --- */
    printf("Lista Risultante (dovrebbe essere ordinata per Modello):\n");
    list2_t *curr = head;
    while (curr != NULL) {
        printf("Modello: %-12s | Miglior Dealer: %-15s | Prezzo: %.2f\n", 
               curr->nome_modello, curr->nome_concessionario_min, curr->prezzo_min);
        curr = curr->next;
    }

    /* --- 5. PULIZIA MEMORIA --- */
    free_list2(head);
    // Nota: bisognerebbe liberare anche l'albero BST e le sue liste interne, 
    // ma per brevità dell'esempio ci limitiamo alla lista risultato.
    
    return 0;
}