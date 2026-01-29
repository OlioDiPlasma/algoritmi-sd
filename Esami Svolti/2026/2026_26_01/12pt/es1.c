#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Definizione della struttura (come da immagine)
typedef struct node {
    char *val;
    struct node *next;
} *link;

// Funzione helper per creare un nuovo nodo (utile per il main)
// NOTA: Usiamo strdup per duplicare la stringa, poiché removeDupl farà free(val)
link newNode(char *v) {
    link n = malloc(sizeof(struct node));
    if (n == NULL) return NULL;
    n->val = strdup(v); // Allocazione dinamica della stringa
    n->next = NULL;
    return n;
}

// 2. Implementazione della funzione richiesta
link removeDupl(link h) {
    link p, x0, x;

    // x0 è il nodo "pivot": controlliamo se esistono duplicati di x0 nel resto della lista
    for (x0 = h; x0 != NULL; x0 = x0->next) {
        
        // Loop interno: scansione del resto della lista (da x0->next in poi)
        // p è il predecessore di x, necessario per "ricucire" la lista quando eliminiamo x
        for (p = x0, x = x0->next; x != NULL; /* l'incremento è gestito dentro */) {
            
            // Confronto le stringhe
            if (strcmp(x->val, x0->val) == 0) {
                // Trovato duplicato!
                
                // 1. Bypassare il nodo x (il predecessore p punta al successore di x)
                p->next = x->next;
                
                // 2. Liberare la memoria della stringa e del nodo
                free(x->val);
                free(x);
                
                // 3. Aggiornare x per il prossimo giro
                // NOTA: p non avanza perché il nuovo p->next non è ancora stato controllato
                x = p->next;   //EXTRA
            } else {
                // Non è un duplicato
                // Avanziamo sia p che x
                p = x;
                x = x->next;  //EXTRA
            }
        }
    }
    
    // Ritorniamo la testa della lista (che non cambia mai in questo algoritmo, 
    // perché teniamo sempre la prima occorrenza)
    return h;
}

// Funzione di utilità per stampare la lista
void printList(link h) {
    while (h != NULL) {
        printf("->\"%s\" ", h->val);
        h = h->next;
    }
    printf("->NULL\n");
}

// 3. Main di test
int main() {
    // Creazione della lista esempio:
    // ->"Roma"->"Milano"->"Palermo"->"Roma"->"Torino"->"Genova"->"Roma"->"Palermo"
    
    link head = newNode("Roma");
    head->next = newNode("Milano");
    head->next->next = newNode("Palermo");
    head->next->next->next = newNode("Roma");       // Duplicato 1
    head->next->next->next->next = newNode("Torino");
    head->next->next->next->next->next = newNode("Genova");
    head->next->next->next->next->next->next = newNode("Roma"); // Duplicato 2
    head->next->next->next->next->next->next->next = newNode("Palermo"); // Duplicato 3

    printf("--- Lista Originale ---\n");
    printList(head);

    // Chiamata alla funzione
    head = removeDupl(head);

    printf("\n--- Lista Senza Duplicati ---\n");
    printList(head);
    // Output atteso: ->Roma ->Milano ->Palermo ->Torino ->Genova

    // Pulizia finale della memoria (buona norma)
    while (head != NULL) {
        link temp = head;
        head = head->next;
        free(temp->val);
        free(temp);
    }

    return 0;
}