#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * STRUTTURE DATI
 * ======================================================================================
 * Definizione standard di un nodo per una lista collegata semplice.
 */
struct node {
    int val;            // Il valore intero (il dato)
    struct node *next;  // Il puntatore al nodo successivo
};

typedef struct node *link; // Alias per comodità: 'link' è un puntatore a 'struct node'

/*
 * ======================================================================================
 * FUNZIONE: newNode
 * ======================================================================================
 * SCOPO:
 * Funzione helper (di utilità) per creare un nuovo nodo.
 * Isola la logica di allocazione della memoria per rendere il codice principale più leggibile.
 *
 * PARAMETRI:
 * - int val: Il numero da inserire nel nodo.
 * - link next: Il nodo a cui il nuovo nodo deve puntare.
 *
 * GESTIONE MEMORIA:
 * Utilizza malloc. È CRUCIALE capire che malloc alloca memoria nello HEAP.
 * Questa memoria sopravvive alla fine della funzione e deve essere liberata manualmente con free().
 */
link newNode(int val, link next) {
    /* --- 1. ALLOCAZIONE --- */
    /* sizeof(struct node) calcola esattamente quanti byte servono per un nodo (int + puntatore).
       Il cast (link) è implicito in C, ma esplicitarlo aiuta a capire il tipo di ritorno. */
    link x = (link)malloc(sizeof(struct node));

    /* Controllo difensivo: se la RAM è piena, malloc restituisce NULL. */
    if (x == NULL) {
        printf("Errore: memoria esaurita!\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    x->val = val;   // Assegno il valore
    x->next = next; // Collego il nodo al successivo
    return x;       // Ritorno l'indirizzo del nuovo nodo creato
}

/*
 * ======================================================================================
 * FUNZIONE: aggiungi
 * ======================================================================================
 * SCOPO:
 * Scorre una lista ordinata e inserisce i numeri mancanti per rendere la sequenza continua.
 * Esempio: trasforma 4->7 in 4->5->6->7.
 *
 * PARAMETRI:
 * - link head: Puntatore al primo nodo della lista.
 *
 * RITORNA:
 * - int: Il numero totale di nodi inseriti.
 *
 * CONCETTI CHIAVE:
 * - Scorrimento Look-Ahead: Non guardiamo solo il nodo corrente, ma confrontiamo
 * il corrente ('curr') con il successivo ('curr->next').
 * - Inserimento in testa vs Inserimento in mezzo: Qui facciamo sempre inserimento "in mezzo"
 * (dopo curr e prima di curr->next).
 * ======================================================================================
 */
int aggiungi(link head) {
    /* Se la lista è vuota o ha un solo elemento, non ci sono "buchi" da riempire. */
    if (head == NULL || head->next == NULL) {
        return 0;
    }

    int count = 0;    // Contatore dei nodi aggiunti
    link curr = head; // Cursore per scorrere la lista. Partiamo dalla testa.

    /* --- 1. CICLO DI SCORRIMENTO --- */
    /* Continuiamo finché esiste un nodo successivo con cui confrontarci.
       Non usiamo 'curr != NULL' perché ci serve accedere a 'curr->next->val'.
       Se 'curr' fosse l'ultimo nodo, 'curr->next' sarebbe NULL e non potremmo leggere il valore. */
    while (curr->next != NULL) {
        
        /* Leggiamo i valori per chiarezza */
        int valore_corrente = curr->val;
        int valore_prossimo = curr->next->val;

        /* --- 2. VERIFICA "BUCO" --- */
        /* C'è un buco se la differenza è maggiore di 1.
           Es: Tra 4 e 5 (diff 1) NON c'è buco. Tra 4 e 7 (diff 3) c'è buco. */
        if (valore_prossimo - valore_corrente > 1) {
            
            /* --- 3. CREAZIONE E INNESTO (LOGICA CORE) --- */
            /* Dobbiamo inserire il numero subito successivo al corrente.
               Es: se ho 4 e 7, creo un nodo con 5. */
            int nuovo_valore = valore_corrente + 1;

            /*
             * DETTAGLIO SINTATTICO E MEMORIA:
             * Chiamiamo newNode.
             * 1. Il nuovo nodo avrà valore 'nuovo_valore' (es. 5).
             * 2. Il suo campo 'next' punterà a 'curr->next' (ovvero al nodo 7).
             * Questo collega il NUOVO nodo al RESTO della lista (5 -> 7).
             */
            link nuovo_nodo = newNode(nuovo_valore, curr->next);

            /*
             * 3. Ora dobbiamo collegare il nodo corrente (4) al nuovo nodo (5).
             * Dereferenziamo curr per modificare il suo campo next.
             */
            curr->next = nuovo_nodo;

            /* Incrementiamo il contatore dei nodi aggiunti */
            count++;

            /*
             * NOTA FONDAMENTALE SULL'AVANZAMENTO:
             * Non facciamo 'curr = curr->next' qui dentro nel senso classico di "saltare".
             * Poiché abbiamo appena inserito il 5, al prossimo giro del while
             * 'curr' sarà ancora 4? NO.
             * Dobbiamo decidere: o rimaniamo sul 4 (ma non ha senso, il buco dopo il 4 è tappato),
             * oppure ci spostiamo sul 5 appena creato per vedere se tra 5 e 7 c'è un altro buco.
             * * Strategia corretta: Spostiamo 'curr' sul nodo appena creato.
             * Così al prossimo giro confronteremo 5 con 7 e inseriremo il 6.
             */
            curr = nuovo_nodo; 

        } else {
            /* --- 4. NESSUN BUCO --- */
            /* I numeri sono consecutivi (es. 4 e 5). Avanziamo normalmente. */
            curr = curr->next;
        }
    }

    return count;
}

/*
 * ======================================================================================
 * FUNZIONE: printList
 * ======================================================================================
 * SCOPO: Visualizza la lista a video per verifica.
 */
void printList(link head) {
    link t = head;
    while (t != NULL) {
        printf("%d -> ", t->val);
        t = t->next; // Aritmetica dei puntatori: sposto t al nodo puntato dal next corrente
    }
    printf("NULL\n");
}

/*
 * ======================================================================================
 * FUNZIONE: freeList
 * ======================================================================================
 * SCOPO: Libera tutta la memoria allocata per evitare Memory Leaks.
 */
void freeList(link head) {
    link t = head;
    link next_node;
    while (t != NULL) {
        next_node = t->next; // Salvo il riferimento al prossimo PRIMA di distruggere il corrente
        free(t);             // Libero la memoria del nodo corrente
        t = next_node;       // Avanzo
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * SCOPO: Crea i dati dell'esempio (4, 7, 10), chiama la funzione e stampa i risultati.
 */
int main() {
    printf("--- INIZIO TEST COMPLETAMENTO LISTA ---\n");

    /* --- 1. COSTRUZIONE LISTA HARDCODED --- */
    /* Costruiamo la lista dell'esempio: 4 -> 7 -> 10 
       Costruisco dalla coda verso la testa per semplicità (o uso newNode innestati). */
    
    // Nodo 10 (coda) -> punta a NULL
    link n3 = newNode(10, NULL);
    // Nodo 7 -> punta a 10
    link n2 = newNode(7, n3);
    // Nodo 4 (testa) -> punta a 7
    link head = newNode(4, n2);

    printf("Lista Iniziale: ");
    printList(head);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    printf("\nEsecuzione funzione aggiungi(head)...\n");
    int inseriti = aggiungi(head);

    /* --- 3. VERIFICA RISULTATI --- */
    printf("Nodi inseriti: %d\n", inseriti);
    printf("Lista Finale:   ");
    printList(head);

    /* Verifica automatica basata sull'esempio */
    // La lista dovrebbe essere 4,5,6,7,8,9,10. Lunghezza finale 7. Nodi inseriti 4.
    if (inseriti == 4) {
        printf("[OK] Numero nodi inseriti corretto.\n");
    } else {
        printf("[FAIL] Numero nodi inseriti errato (Atteso: 4).\n");
    }

    /* --- 4. PULIZIA MEMORIA --- */
    /* Fondamentale liberare anche i nodi nuovi che abbiamo creato dentro la funzione! */
    freeList(head);
    
    printf("\n--- FINE TEST ---\n");
    return 0;
}