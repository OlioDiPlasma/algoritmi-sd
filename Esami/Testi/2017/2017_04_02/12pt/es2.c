#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI
 * ======================================================================================
 * Definiamo il nodo dell'albero e il tipo puntatore 'link' come richiesto dal prototipo.
 */
typedef struct node *link;

struct node {
    int item;   // Il dato contenuto nel nodo
    link left;  // Puntatore al sottoalbero sinistro
    link right; // Puntatore al sottoalbero destro
};

/*
 * ======================================================================================
 * FUNZIONE DI SUPPORTO: newNode
 * ======================================================================================
 * Scopo:
 * Creare un nuovo nodo in memoria, inizializzarlo e restituirne l'indirizzo.
 * Questa funzione ci aiuta a mantenere pulito il codice della funzione mirror.
 *
 * Parametri:
 * - val: (int) Il valore intero da inserire nel nodo.
 * - l:   (link) Puntatore al figlio sinistro (può essere NULL).
 * - r:   (link) Puntatore al figlio destro (può essere NULL).
 *
 * Ritorna:
 * - link: L'indirizzo del nuovo nodo allocato nello Heap.
 * ======================================================================================
 */
link newNode(int val, link l, link r) {
    /* --- 1. ALLOCAZIONE --- */
    // Allocazione dinamica nello Heap della dimensione esatta di una struct node.
    link x = (link)malloc(sizeof(struct node));
    
    // Controllo difensivo: se la memoria è piena, malloc ritorna NULL.
    if (x == NULL) {
        printf("Errore di allocazione memoria.\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    // Accesso ai campi tramite operatore freccia (->) che dereferenzia il puntatore.
    x->item = val;
    x->left = l;
    x->right = r;

    return x;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: mirror
 * ======================================================================================
 * SCOPO:
 * Costruire un NUOVO albero binario che sia la copia speculare (mirror) dell'albero
 * passato in input. Invece di modificare l'albero originale, ne creiamo una copia
 * dove per ogni nodo i figli sinistro e destro sono invertiti.
 *
 * PARAMETRI:
 * - root: (link) Puntatore alla radice dell'albero originale (o sottoalbero) da specchiare.
 *
 * RITORNA:
 * - link: Puntatore alla radice del NUOVO albero speculare appena creato.
 *
 * CONCETTI CHIAVE:
 * 1. Ricorsione Strutturale: Un albero è speculare se la sua radice ha come figlio
 * sinistro lo speculare del figlio destro originale, e viceversa.
 * 2. Deep Copy (Copia Profonda): Non copiamo solo i puntatori (che punterebbero 
 * ai vecchi nodi), ma allochiamo nuova memoria per ogni nodo.
 * ======================================================================================
 */
link mirror(link root) {
    
    /* --- 1. CASO BASE (TERMINAZIONE) --- */
    // Se il puntatore in ingresso è NULL, significa che l'albero è vuoto 
    // o siamo arrivati a una foglia. Lo specchio del nulla è il nulla.
    if (root == NULL) {
        return NULL;
    }

    /* --- 2. PASSO RICORSIVO (DIVIDE ET IMPERA) --- */
    // Qui avviene la magia della ricorsione.
    // Chiediamo alla funzione di calcolare lo specchio dei due sottoalberi.
    // NOTA BENE: Incrociamo le chiamate!
    // Lo specchio del sottoalbero DESTRO originale diventerà il SINISTRO del nuovo nodo.
    link newLeft = mirror(root->right); 
    
    // Lo specchio del sottoalbero SINISTRO originale diventerà il DESTRO del nuovo nodo.
    link newRight = mirror(root->left);

    /* --- 3. COSTRUZIONE DEL NODO CORRENTE (CONQUER) --- */
    // Creiamo il nuovo nodo radice per questo sottoalbero.
    // Copiamo il valore (item) dal nodo originale.
    // Colleghiamo i puntatori ai sottoalberi speculari appena ottenuti.
    // malloc viene chiamata dentro newNode.
    link newRoot = newNode(root->item, newLeft, newRight);

    /* --- 4. RITORNO DEL RISULTATO --- */
    // Restituiamo al chiamante (il nodo padre nella ricorsione) l'indirizzo del nuovo nodo.
    return newRoot;
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST
 * ======================================================================================
 */

// Stampa l'albero in pre-ordine (Radice -> Sinistra -> Destra) per verificare la struttura.
void preOrderPrint(link h) {
    if (h == NULL) return;
    printf("%d ", h->item);
    preOrderPrint(h->left);
    preOrderPrint(h->right);
}

// Funzione per liberare la memoria di un albero (Post-Order Traversal).
// Fondamentale per evitare Memory Leaks nel main.
void freeTree(link h) {
    if (h == NULL) return;
    freeTree(h->left);   // Libera prima i figli
    freeTree(h->right);
    free(h);             // Poi libera il padre
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare l'albero mostrato nell'immagine dell'esercizio, invocare mirror
 * e verificare che l'output sia corretto.
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE ALBERO ORIGINALE (Hardcoded) --- */
    // Costruiamo l'albero dal basso verso l'alto (dalle foglie alla radice)
    // per semplicità di codice, seguendo la struttura dell'immagine.
    
    // Foglie e livello più basso
    link n2 = newNode(2, NULL, NULL);
    link n4 = newNode(4, NULL, NULL);
    link n17 = newNode(17, NULL, NULL);
    link n20 = newNode(20, NULL, NULL);

    // Livello intermedio
    // Nodo 3 ha figli 2 e 4
    link n3 = newNode(3, n2, n4);
    // Nodo 7 ha figli 17 e 20 (dedotto dall'immagine: sotto il 18 c'è un 7? 
    // No, nell'immagine sotto il 6 ci sono 3 e 7. Sotto il 18 ci sono 17 e 20.
    // Correggo la struttura basandomi sull'immagine visiva sx:
    // 15 -> (6, 18). 6 -> (3, 7). 18 -> (17, 20). 3 -> (2, 4).
    
    // Ricostruzione corretta livello intermedio:
    link n7 = newNode(7, NULL, NULL); // Il 7 sembra una foglia nel disegno sx sotto il 6? 
                                      // Aspetta, il 7 nel disegno a sinistra è figlio destro del 6.
    
    // Nodo 18 (figlio destro della radice): ha figli 17 e 20
    link n18 = newNode(18, n17, n20);

    // Nodo 6 (figlio sinistro della radice): ha figli 3 e 7
    link n6 = newNode(6, n3, n7);

    // Radice 15
    link root = newNode(15, n6, n18);

    /* --- 2. STAMPA ALBERO ORIGINALE --- */
    printf("--- Albero Originale (Pre-Order) ---\n");
    printf("Atteso (approx): 15 6 3 2 4 7 18 17 20\n");
    printf("Reale:           ");
    preOrderPrint(root);
    printf("\n\n");

    /* --- 3. CREAZIONE MIRROR --- */
    // Chiamata alla funzione dell'esercizio
    link mirroredRoot = mirror(root);

    /* --- 4. VERIFICA RISULTATO --- */
    printf("--- Albero Mirror (Pre-Order) ---\n");
    // Pre-order del mirror dovrebbe essere: 15 -> (specchio 18) -> (specchio 6)
    // 15 -> 18 -> 20 -> 17 -> 6 -> 7 -> 3 -> 4 -> 2
    printf("Atteso (approx): 15 18 20 17 6 7 3 4 2\n");
    printf("Reale:           ");
    preOrderPrint(mirroredRoot);
    printf("\n");

    /* --- 5. PULIZIA MEMORIA --- */
    // Dobbiamo liberare ENTRAMBI gli alberi perché sono strutture distinte in memoria.
    freeTree(root);
    freeTree(mirroredRoot);

    return 0;
}