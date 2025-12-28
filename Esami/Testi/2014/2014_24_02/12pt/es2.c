#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT)
 * ============================================================================
 * Definiamo le strutture come richiesto dal testo.
 * 1. node_t: Il "vagone" della lista. Rappresenta una singola cella non nulla.
 * 2. matr_t: Il "contenitore" principale (ADT).
 */

/* Struttura per il singolo elemento non nullo (Nodo della lista) */
typedef struct node {
    int col;            // Indice della colonna
    float val;          // Valore (es. 3.14)
    struct node *next;  // Puntatore al prossimo elemento non nullo nella stessa riga
} node_t;

/* Struttura per la Matrice Sparsa (ADT) */
typedef struct {
    int NR;             // Numero totale di righe
    int NC;             // Numero totale di colonne
    node_t **rows;      // ARRAY DI LISTE: Puntatore a un vettore di puntatori
} matr_t;

/* ============================================================================
 * FUNZIONE HELPER: matrInit (Non richiesta esplicitamente ma necessaria)
 * ============================================================================
 * SCOPO:
 * Inizializza la struttura vuota. È fondamentale per allocare il vettore "rows".
 *
 * GESTIONE MEMORIA:
 * Usiamo 'calloc' invece di 'malloc' per allocare il vettore delle righe.
 * PERCHÉ? calloc inizializza tutto a 0 (NULL). Questo è vitale: significa che
 * tutte le liste partono come "liste vuote" (NULL) automaticamente.
 * ============================================================================
 */
matr_t *matrInit(int nr, int nc) {
    matr_t *M = (matr_t *)malloc(sizeof(matr_t));
    if (M == NULL) return NULL;

    M->NR = nr;
    M->NC = nc;

    /* Allocazione del vettore di puntatori alle righe */
    /* sizeof(node_t *) è la dimensione di un puntatore (solitamente 8 byte su 64bit) */
    M->rows = (node_t **)calloc(nr, sizeof(node_t *));
    
    if (M->rows == NULL) {
        free(M);
        return NULL;
    }
    
    return M;
}

/* ============================================================================
 * FUNZIONE RICHIESTA: MATRwrite
 * ============================================================================
 * SCOPO:
 * Scrive un valore 'val' nella cella (r, c).
 * Gestisce tre casi logicamente distinti per una matrice sparsa:
 * 1. INSERIMENTO: La cella era 0 (non esisteva nodo) e scriviamo un valore != 0.
 * 2. AGGIORNAMENTO: La cella esisteva già (c'era un nodo) e cambiamo il valore.
 * 3. CANCELLAZIONE: Scriviamo 0 in una cella che aveva un valore (il nodo va rimosso).
 *
 * PARAMETRI:
 * - matr_t *M: Puntatore alla struttura matrice.
 * - int r, c:  Coordinate riga e colonna.
 * - float val: Valore da inserire.
 *
 * CONCETTI CHIAVE:
 * Scorrimento con "prev" e "curr":
 * Poiché le liste sono ordinate per colonna, per inserire o rimuovere un nodo
 * dobbiamo sapere chi è il nodo PRECEDENTE per ricollegare i puntatori 'next'.
 * ============================================================================
 */
void MATRwrite(matr_t *M, int r, int c, float val) {
    
    /* --- 1. CONTROLLO VALIDITÀ INPUT --- */
    /* Verifichiamo che le coordinate siano dentro la matrice */
    if (M == NULL || r < 0 || r >= M->NR || c < 0 || c >= M->NC) {
        printf("Errore: Coordinate fuori limite o matrice NULL.\n");
        return;
    }

    /* --- 2. ACCESSO ALLA RIGA --- */
    /* M->rows[r] è il puntatore alla testa della lista che rappresenta la riga 'r' */
    node_t *curr = M->rows[r]; // Cursore corrente
    node_t *prev = NULL;       // Cursore al nodo precedente

    /* --- 3. RICERCA DELLA POSIZIONE --- */
    /* Scorriamo la lista finché non finisce OPPURE finché non superiamo la colonna 'c'.
       Manteniamo la lista ordinata per colonna (crescente). */
    while (curr != NULL && curr->col < c) {
        prev = curr;
        curr = curr->next;
    }

    /* A questo punto, 'curr' è NULL oppure curr->col >= c.
       Abbiamo trovato il punto dove agire. */

    /* --- CASO A: Il nodo esiste già (curr->col == c) --- */
    if (curr != NULL && curr->col == c) {
        if (val != 0.0) {
            /* Sottocaso A1: Aggiornamento valore (es. da 5.0 a 10.0) */
            curr->val = val;
        } else {
            /* Sottocaso A2: Cancellazione (scrittura di 0.0 su nodo esistente) */
            /* Dobbiamo rimuovere 'curr' dalla catena */
            
            if (prev == NULL) {
                /* Stiamo rimuovendo la testa della lista */
                M->rows[r] = curr->next;
            } else {
                /* Stiamo rimuovendo un nodo interno o finale */
                prev->next = curr->next;
            }
            
            /* Fondamentale: liberare la memoria per evitare Memory Leak */
            free(curr);
        }
    }
    /* --- CASO B: Il nodo non esiste (curr è NULL o curr->col > c) --- */
    else {
        /* Se il valore da scrivere è 0 e il nodo non c'è, non facciamo nulla 
           (la matrice sparsa "contiene" già implicitamente zeri ovunque non ci siano nodi). */
        if (val == 0.0) return;

        /* Altrimenti, dobbiamo creare un nuovo nodo */
        
        /* --- 4. ALLOCAZIONE NUOVO NODO --- */
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        if (new_node == NULL) return; // Errore allocazione

        new_node->col = c;
        new_node->val = val;

        /* --- 5. INSERIMENTO NELLA CATENA (LINKING) --- */
        /* Il nuovo nodo deve puntare al 'curr' (che è il successivo o NULL) */
        new_node->next = curr;

        if (prev == NULL) {
            /* Inserimento in TESTA alla riga */
            M->rows[r] = new_node;
        } else {
            /* Inserimento nel MEZZO o in CODA */
            prev->next = new_node;
        }
    }
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Verificare creazione, scrittura, stampa e pulizia.
 * ============================================================================
 */
int main() {
    /* Creiamo una matrice 3x3 */
    int NR = 3, NC = 3;
    matr_t *miaMatrice = matrInit(NR, NC);
    if (miaMatrice == NULL) return 1;

    printf("--- Inserimento Valori ---\n");
    /* Inseriamo alcuni valori sparsi */
    /* Riga 0: [ 10.5, 0, 0 ] -> Nodo a col 0 */
    MATRwrite(miaMatrice, 0, 0, 10.5); 
    
    /* Riga 1: [ 0, 0, 2.2 ] -> Nodo a col 2 */
    MATRwrite(miaMatrice, 1, 2, 2.2);

    /* Riga 2: [ 0, 3.3, 4.4 ] -> Nodi a col 1 e 2 */
    MATRwrite(miaMatrice, 2, 2, 4.4); // Inserisco prima l'ultimo per testare l'ordinamento
    MATRwrite(miaMatrice, 2, 1, 3.3); // Inserisco 'in mezzo'

    /* Test Cancellazione: Metto a 0 il valore in (2, 2) */
    printf("Test cancellazione nodo in (2,2)...\n");
    MATRwrite(miaMatrice, 2, 2, 0.0);

    /* --- STAMPA DI VERIFICA --- */
    printf("\n--- Contenuto Matrice Sparsa ---\n");
    for (int i = 0; i < NR; i++) {
        printf("Riga %d: ", i);
        node_t *temp = miaMatrice->rows[i]; // Testa della lista riga i
        
        /* Se la lista è vuota, stampa solo newline */
        while (temp != NULL) {
            printf("[Col:%d Val:%.1f] -> ", temp->col, temp->val);
            temp = temp->next;
        }
        printf("NULL\n");
    }

    /* --- PULIZIA MEMORIA (FREE COMPLETA) --- */
    /* Dobbiamo liberare: 
       1. Ogni nodo di ogni lista.
       2. Il vettore 'rows'.
       3. La struttura 'matr_t' stessa.
    */
    for (int i = 0; i < NR; i++) {
        node_t *head = miaMatrice->rows[i];
        while (head != NULL) {
            node_t *tmp = head;
            head = head->next;
            free(tmp); // Libero il singolo nodo
        }
    }
    free(miaMatrice->rows); // Libero l'array di puntatori
    free(miaMatrice);       // Libero l'ADT principale
    
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}