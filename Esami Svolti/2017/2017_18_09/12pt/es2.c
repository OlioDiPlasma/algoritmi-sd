#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * DEFINIZIONE TIPI DI DATO (STRUCT)
 * ======================================================================================
 * Come richiesto, definiamo esplicitamente i nodi senza usare ADT opachi.
 *
 * node1_t: Nodo della lista PRINCIPALE (Verticale).
 * Nel database originale contiene l'Agricoltore.
 * Nel database invertito conterrà il Prodotto.
 *
 * node2_t: Nodo delle liste SECONDARIE (Orizzontali).
 * Nel database originale contiene il Prodotto.
 * Nel database invertito conterrà l'Agricoltore.
 */

typedef struct node2_s {
    char name[21];          // Stringa statica max 20 char + terminatore
    struct node2_s *next;   // Puntatore al prossimo elemento della sottolista
} node2_t;

typedef struct node1_s {
    char name[21];          // Nome (Agricoltore nell'input, Prodotto nell'output)
    node2_t *sublist;       // Testa della lista secondaria
    struct node1_s *next;   // Puntatore al prossimo nodo della lista principale
} node1_t;

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ (HELPER)
 * ======================================================================================
 * Scopo:
 * Queste funzioni servono per allocare nodi e gestire inserimenti in testa.
 * Rendono il codice della funzione principale più pulito e leggibile.
 */

// Crea un nuovo nodo per la lista secondaria
node2_t *new_node2(char *name) {
    // 1. Allocazione memoria nello Heap
    node2_t *n = (node2_t *)malloc(sizeof(node2_t));
    if (n == NULL) exit(1); // Controllo errore allocazione

    // 2. Copia sicura della stringa
    strcpy(n->name, name);
    
    // 3. Inizializzazione puntatore
    n->next = NULL;
    return n;
}

// Crea un nuovo nodo per la lista principale
node1_t *new_node1(char *name) {
    node1_t *n = (node1_t *)malloc(sizeof(node1_t));
    if (n == NULL) exit(1);
    
    strcpy(n->name, name);
    n->sublist = NULL; // All'inizio la sottolista è vuota
    n->next = NULL;
    return n;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: list_of_list_invert
 * ======================================================================================
 * SCOPO:
 * Trasforma una lista di liste (Agricoltori -> Prodotti) in una nuova lista di liste
 * invertita (Prodotti -> Agricoltori).
 *
 * PARAMETRI:
 * - head1: (node1_t *) Puntatore alla testa della lista di input (Agricoltori).
 *
 * RITORNA:
 * - (node1_t *) Puntatore alla testa della NUOVA struttura dati creata (Prodotti).
 *
 * CONCETTI CHIAVE:
 * 1. Deep Copy e Riorganizzazione:
 * Non stiamo solo spostando puntatori. Stiamo creando una struttura completamente
 * nuova. Se l'agricoltore "Rossi" produce "Mele" e "Pere", nella nuova lista
 * avremo il nodo principale "Mele" con sottomesso "Rossi", e il nodo "Pere"
 * con sottomesso "Rossi".
 *
 * 2. Ricerca Lineare nella Lista Nuova:
 * Per ogni prodotto trovato nella lista vecchia, dobbiamo controllare se esiste
 * già nella lista nuova.
 * - Se esiste: aggiungiamo l'agricoltore alla sua sottolista.
 * - Se non esiste: creiamo il nodo prodotto e poi aggiungiamo l'agricoltore.
 * ======================================================================================
 */
node1_t *list_of_list_invert(node1_t *head1) {
    
    /* --- 1. INIZIALIZZAZIONE --- */
    node1_t *newHead = NULL; // Testa della nuova lista (Prodotti)
    
    node1_t *currFarmer;     // Iteratore per la lista input principale (Agricoltori)
    node2_t *currProduct;    // Iteratore per la lista input secondaria (Prodotti)

    /* --- 2. CICLO ESTERNO: SCANSIONE AGRICOLTORI --- */
    // Scorriamo tutta la lista principale di input
    for (currFarmer = head1; currFarmer != NULL; currFarmer = currFarmer->next) {
        
        // Per ogni agricoltore, prendiamo il suo nome
        char *farmerName = currFarmer->name;

        /* --- 3. CICLO INTERNO: SCANSIONE PRODOTTI DELL'AGRICOLTORE --- */
        // Scorriamo la sottolista dei prodotti di questo agricoltore
        for (currProduct = currFarmer->sublist; currProduct != NULL; currProduct = currProduct->next) {
            
            char *productName = currProduct->name;

            /* --- 4. LOGICA DI INVERSIONE --- */
            // Ora abbiamo la coppia (Agricoltore, Prodotto).
            // Dobbiamo inserirla nella struttura (Prodotto -> Lista Agricoltori).

            // Passo A: Cerco se il 'productName' esiste già nella 'newHead'
            node1_t *foundProductNode = NULL;
            node1_t *scan = newHead;
            
            while (scan != NULL) {
                if (strcmp(scan->name, productName) == 0) {
                    foundProductNode = scan;
                    break; // Trovato!
                }
                scan = scan->next;
            }

            // Passo B: Se non esiste, lo creo e lo inserisco in testa a newHead
            if (foundProductNode == NULL) {
                foundProductNode = new_node1(productName);
                
                // Inserimento in testa alla lista principale dei prodotti
                // (L'ordine non importa secondo il testo)
                foundProductNode->next = newHead;
                newHead = foundProductNode;
            }

            // Passo C: Aggiungo l'agricoltore alla sottolista di foundProductNode
            // Creiamo un nuovo nodo per la sottolista (che ora contiene nomi agricoltori)
            node2_t *newFarmerNode = new_node2(farmerName);

            // Inserimento in testa alla sottolista
            newFarmerNode->next = foundProductNode->sublist;
            foundProductNode->sublist = newFarmerNode;
        }
    }

    /* --- 5. RITORNO --- */
    return newHead;
}

/*
 * ======================================================================================
 * FUNZIONI PER IL TESTING (STAMPA E FREE)
 * ======================================================================================
 */

void print_list(node1_t *head, char *labelMain, char *labelSub) {
    if (head == NULL) {
        printf("Lista vuota.\n");
        return;
    }
    while (head != NULL) {
        printf("[%s %s] -> produce/è prodotto da: ", labelMain, head->name);
        node2_t *sub = head->sublist;
        while (sub != NULL) {
            printf("%s (%s) ", sub->name, labelSub);
            sub = sub->next;
            if (sub != NULL) printf("- ");
        }
        printf("\n|\n");
        head = head->next;
    }
    printf("NULL\n");
}

// Funzione ricorsiva o iterativa per liberare TUTTA la memoria
void free_all(node1_t *head) {
    while (head != NULL) {
        // 1. Libero la sottolista
        node2_t *sub = head->sublist;
        while (sub != NULL) {
            node2_t *tempSub = sub;
            sub = sub->next;
            free(tempSub); // Libero nodo secondario
        }
        
        // 2. Libero il nodo principale
        node1_t *tempHead = head;
        head = head->next;
        free(tempHead); // Libero nodo principale
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare i dati come nell'immagine "head1" e verificare l'inversione "head2".
 * ======================================================================================
 */
int main() {
    /* --- 1. CREAZIONE DATI HARDCODED (Farmer -> Products) --- */
    
    // Creiamo la struttura come nell'immagine di esempio (head1)
    // Farmer1: prod5, prod4, prod2
    node1_t *f1 = new_node1("farmer1");
    f1->sublist = new_node2("product5");
    f1->sublist->next = new_node2("product4");
    f1->sublist->next->next = new_node2("product2");

    // Farmer2: prod2, prod4, prod3
    node1_t *f2 = new_node1("farmer2");
    f2->sublist = new_node2("product2");
    f2->sublist->next = new_node2("product4");
    f2->sublist->next->next = new_node2("product3");

    // Farmer3: prod5, prod2
    node1_t *f3 = new_node1("farmer3");
    f3->sublist = new_node2("product5");
    f3->sublist->next = new_node2("product2");

    // Colleghiamo la lista principale: f1 -> f2 -> f3
    f1->next = f2;
    f2->next = f3;
    node1_t *head1 = f1;

    printf("--- INPUT: LISTA AGRICOLTORI ---\n");
    print_list(head1, "Agricoltore", "Prodotto");

    /* --- 2. CHIAMATA ALLA FUNZIONE DI INVERSIONE --- */
    node1_t *head2 = list_of_list_invert(head1);

    /* --- 3. VERIFICA OUTPUT --- */
    printf("\n--- OUTPUT: LISTA PRODOTTI (INVERTITA) ---\n");
    print_list(head2, "Prodotto", "Agricoltore");

    /* --- 4. PULIZIA MEMORIA --- */
    // È vitale liberare entrambe le liste perché sono strutture indipendenti in memoria.
    free_all(head1);
    free_all(head2);

    return 0;
}