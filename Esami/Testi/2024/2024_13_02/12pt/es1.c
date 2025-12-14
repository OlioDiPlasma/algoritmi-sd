#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================
   SEZIONE 1: DEFINIZIONE DEI TIPI (Richiesti)
   ========================================== */

// Definizione dell'Item come da traccia
typedef struct {
    char name[16];
    int val;
} Item;

// Definizione del Nodo della lista
typedef struct node {
    Item item;
    struct node *next;
} Node;

// Definizione del tipo ADT SLIST (puntatore al primo nodo)
typedef Node *SLIST;

/* ==========================================
   SEZIONE 2: LA FUNZIONE RICHIESTA
   ========================================== */

/*
 * Scopo: Fonde due liste ordinate sommando i valori per chiavi uguali.
 * Genera una NUOVA lista senza modificare le originali.
 *
 * Parametri:
 * - SLIST a: Prima lista ordinata.
 * - SLIST b: Seconda lista ordinata.
 *
 * Concetti Chiave:
 * 1. Scansione Parallela: Usiamo un ciclo while per avanzare su 'a' o 'b'
 * scegliendo sempre il nodo "minore" alfabeticamente.
 * 2. Aggregazione sulla Coda: Controlliamo sempre l'ultimo nodo inserito
 * nella lista risultato ('tail'). Se il nome coincide con quello che stiamo
 * processando, sommiamo invece di allocare.
 */
SLIST SLISTmerge(SLIST a, SLIST b) {
    SLIST res = NULL;  // Testa della lista risultato
    SLIST tail = NULL; // Coda della lista risultato (per inserimenti veloci O(1))
    
    // Puntatori temporanei per scorrere le liste input senza perderne la testa
    SLIST currA = a;
    SLIST currB = b;

    // Ciclo finché c'è almeno un elemento in una delle due liste
    while (currA != NULL || currB != NULL) {
        
        // --- FASE 1: SELEZIONE DEL "VINCITORE" (IL MINORE) ---
        // Dobbiamo decidere quale nodo processare: quello di A o quello di B?
        SLIST selected = NULL;

        if (currA == NULL) {
            // A è finita, prendiamo per forza da B
            selected = currB;
            currB = currB->next;
        } 
        else if (currB == NULL) {
            // B è finita, prendiamo per forza da A
            selected = currA;
            currA = currA->next;
        } 
        else {
            // Entrambe hanno nodi: confrontiamo i nomi con strcmp
            int cmp = strcmp(currA->item.name, currB->item.name);
            
            if (cmp < 0) {
                // A < B: processiamo A
                selected = currA;
                currA = currA->next;
            } else {
                // B <= A: processiamo B.
                // Nota: se sono uguali, ne prendiamo uno a caso (es. B), 
                // il prossimo giro del while prenderà A e verrà aggregato.
                selected = currB;
                currB = currB->next;
            }
        }

        // --- FASE 2: AGGREGAZIONE O INSERIMENTO ---
        
        // Controllo se devo AGGREGARE (sommare) all'ultimo nodo inserito
        // Condizione: la lista result non è vuota E il nome coincide con la coda
        if (tail != NULL && strcmp(tail->item.name, selected->item.name) == 0) {
            // Caso: Duplicato trovato.
            // Aggiorno il valore sommando.
            tail->item.val += selected->item.val;
        } 
        else {
            // Caso: Nuovo nome.
            // Devo allocare un nuovo nodo.
            Node *newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) return NULL; // Gestione errore memoria

            // Copio i dati
            strcpy(newNode->item.name, selected->item.name);
            newNode->item.val = selected->item.val;
            newNode->next = NULL;

            // Inserisco in coda alla lista result
            if (res == NULL) {
                res = newNode;  // Primo elemento assoluto
                tail = newNode;
            } else {
                tail->next = newNode; // Attacco al precedente
                tail = newNode;       // Aggiorno il puntatore alla coda
            }
        }
    }

    return res;
}

/* ==========================================
   SEZIONE 3: MAIN DI TEST E HELPER
   ========================================== */

// Funzione helper per creare nodi velocemente (solo per il test)
SLIST newNodeForTest(const char *name, int val) {
    Node *n = (Node *)malloc(sizeof(Node));
    strcpy(n->item.name, name);
    n->item.val = val;
    n->next = NULL;
    return n;
}

// Funzione per stampare la lista
void printList(SLIST l) {
    printf("(");
    while (l != NULL) {
        printf("\"%s\", %d", l->item.name, l->item.val);
        if (l->next) printf("), (");
        l = l->next;
    }
    printf(")\n");
}

int main() {
    // Creiamo le liste dell'esempio
    // a) ("roma", 7), ("torino", 4), ("zagabria", 5)
    SLIST a = newNodeForTest("roma", 7);
    a->next = newNodeForTest("torino", 4);
    a->next->next = newNodeForTest("zagabria", 5);

    // b) ("roma", 3), ("torino", 3), ("torino", 2), ("venezia", 10)
    SLIST b = newNodeForTest("roma", 3);
    b->next = newNodeForTest("torino", 3);
    b->next->next = newNodeForTest("torino", 2);
    b->next->next->next = newNodeForTest("venezia", 10);

    printf("Lista A: "); printList(a);
    printf("Lista B: "); printList(b);

    // Eseguiamo il merge
    SLIST res = SLISTmerge(a, b);

    printf("\nRisultato Atteso: (\"roma\", 10), (\"torino\", 9), (\"venezia\", 10), (\"zagabria\", 5)\n");
    printf("Risultato Ottenuto: "); 
    printList(res);

    // NOTA: Qui bisognerebbe fare il free di tutte le liste per pulizia memoria.
    return 0;
}