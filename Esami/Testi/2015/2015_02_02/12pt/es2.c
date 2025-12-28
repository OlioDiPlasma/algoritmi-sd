#include <stdio.h>
#include <stdlib.h>

/* Definiamo N. Nel testo è generico, qui lo fissiamo a 3 per testare il codice. */
#define N 3

/* * Struttura del nodo come definita nell'immagine.
 * Rappresenta un nodo di un albero N-ario.
 */
struct node {
    int key;                    /* Valore intero del nodo */
    struct node *children[N];   /* Array di puntatori ai figli (dimensione fissa N) */
};

/*
 * ======================================================================================
 * NOME FUNZIONE: printNodesAtSpecificLevel
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria ricorsiva. Attraversa l'albero ma stampa la chiave del nodo
 * SOLO se la profondità corrente corrisponde esattamente al livello target richiesto.
 *
 * PARAMETRI:
 * * root        : Puntatore al nodo corrente dell'albero.
 * currentLevel  : Intero che traccia la profondità attuale nella ricorsione.
 * targetLevel   : Intero che rappresenta il livello che vogliamo stampare.
 *
 * CONCETTI CHIAVE:
 * 1. Ricorsione con controllo di profondità: Non stampiamo tutto. Scendiamo nell'albero
 * incrementando 'currentLevel'. Quando 'currentLevel' == 'targetLevel', stampiamo.
 * 2. Pruning (Potatura): Se 'currentLevel' supera 'targetLevel', potremmo anche fermarci,
 * ma in questa implementazione semplice scendiamo fino alle foglie o al match.
 * ======================================================================================
 */
void printNodesAtSpecificLevel(struct node *root, int currentLevel, int targetLevel) {
    /* --- 1. CONTROLLO BASE --- */
    /* Se il nodo è NULL, siamo arrivati oltre una foglia o l'albero è vuoto. */
    if (root == NULL) {
        return;
    }

    /* --- 2. LOGICA DI STAMPA (CASO BASE) --- */
    /* Se la profondità corrente è quella desiderata, stampiamo la chiave. */
    if (currentLevel == targetLevel) {
        /*
         * DETTAGLIO SINTATTICO:
         * Accediamo al membro 'key' tramite l'operatore freccia '->'
         * perché 'root' è un puntatore. È equivalente a (*root).key
         */
        printf("%d ", root->key);
        /* Non serve return qui, perché tecnicamente un nodo a livello X
           non può essere figlio di un altro nodo a livello X, ma per sicurezza
           o ottimizzazione potremmo fermarci se non ci interessano i sottoalberi.
           Tuttavia, continuiamo la logica standard. */
    }

    /* --- 3. PASSO RICORSIVO --- */
    /* * Se non siamo ancora arrivati al livello target (siamo sopra),
     * dobbiamo scendere nei figli.
     */
    if (currentLevel < targetLevel) {
        for (int i = 0; i < N; i++) {
            /*
             * DETTAGLIO SINTATTICO:
             * root->children[i] è il puntatore all'i-esimo figlio.
             * Chiamiamo ricorsivamente la funzione incrementando currentLevel (+1).
             */
            printNodesAtSpecificLevel(root->children[i], currentLevel + 1, targetLevel);
        }
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: visitLevelByLevel
 * ======================================================================================
 * SCOPO:
 * Funzione richiesta dall'esercizio. Stampa i nodi dell'albero livello per livello,
 * partendo dal livello l1 fino al livello l2 inclusi.
 *
 * PARAMETRI:
 * * root : Puntatore alla radice dell'albero.
 * l1     : Livello di partenza (profondità iniziale).
 * l2     : Livello di fine (profondità finale).
 *
 * CONCETTI CHIAVE:
 * 1. Approccio Iterativo su Ricorsivo: Invece di usare una Coda (complessa da fare in C),
 * usiamo un ciclo for che va da l1 a l2. Per ogni iterazione 'i', chiediamo alla
 * funzione ausiliaria di stampare SOLO quel livello.
 * 2. Efficienza: Questo metodo ha complessità temporale maggiore rispetto a una BFS con coda
 * (visitiamo i nodi superiori più volte), ma soddisfa perfettamente il suggerimento
 * del testo ("l'albero sia visitato più di una volta") ed è O(1) in spazio extra.
 * ======================================================================================
 */
void visitLevelByLevel(struct node *root, int l1, int l2) {
    /* --- 1. CONTROLLO INPUT --- */
    if (root == NULL) return;

    /* --- 2. CICLO DEI LIVELLI --- */
    /* Iteriamo dal livello l1 fino a l2 compreso */
    for (int livello = l1; livello <= l2; livello++) {
        
        // Opzionale: stampa estetica per separare i livelli
        // printf("\n[Livello %d]: ", livello); 

        /* Chiamiamo l'helper partendo dalla radice (che è a profondità 0 o 1 a seconda delle convenzioni.
           Qui assumiamo che la radice passata sia a profondità 0 rispetto a se stessa,
           ma l'esercizio chiede l1 e l2 assoluti. Assumeremo che 'root' sia il livello 0 assoluto. */
        printNodesAtSpecificLevel(root, 0, livello);
    }
    printf("\n"); // A capo finale
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST (NON RICHIESTE MA NECESSARIE PER COMPILARE)
 * ======================================================================================
 */

/* Crea un nuovo nodo allocando memoria */
struct node* createNode(int key) {
    /*
     * GESTIONE MEMORIA (MALLOC):
     * Allocazione dinamica nell'HEAP.
     * sizeof(struct node) calcola i byte necessari per l'intero e l'array di puntatori.
     */
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    
    /* Controllo fallimento malloc */
    if (newNode == NULL) {
        perror("Errore malloc");
        exit(1);
    }

    newNode->key = key;
    
    /* Inizializziamo tutti i figli a NULL per evitare "dangling pointers" o spazzatura */
    for (int i = 0; i < N; i++) {
        newNode->children[i] = NULL;
    }
    
    return newNode;
}

/* Libera la memoria dell'albero ricorsivamente (Post-order traversal) */
void freeTree(struct node* root) {
    if (root == NULL) return;
    
    /* Prima libero i figli */
    for (int i = 0; i < N; i++) {
        freeTree(root->children[i]);
    }
    
    /* * GESTIONE MEMORIA (FREE):
     * Solo dopo aver liberato i sottoalberi posso liberare il nodo corrente.
     * Se facessi free(root) prima, perderei i puntatori ai figli!
     */
    free(root);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE DATI DI PROVA --- */
    /*
       Costruiamo manualmente un albero 3-ario (N=3)
       Livello 0:        10
                      /   |   \
       Livello 1:   20    30    40
                   /|\    |    / | \
       Livello 2: 5 6 7   -   8  9  -
    */

    printf("--- Creazione Albero di Test (N=%d) ---\n", N);
    struct node *root = createNode(10); // Livello 0

    // Figli del livello 0 (diventano Livello 1)
    root->children[0] = createNode(20);
    root->children[1] = createNode(30);
    root->children[2] = createNode(40);

    // Figli di 20 (diventano Livello 2)
    root->children[0]->children[0] = createNode(5);
    root->children[0]->children[1] = createNode(6);
    root->children[0]->children[2] = createNode(7);

    // Figli di 40 (diventano Livello 2)
    root->children[2]->children[0] = createNode(8);
    root->children[2]->children[1] = createNode(9);

    /* --- 2. ESECUZIONE TEST --- */
    int l1 = 1;
    int l2 = 2;
    
    printf("Eseguo visitLevelByLevel da %d a %d...\n", l1, l2);
    printf("Output atteso: 20 30 40 5 6 7 8 9 (ordine per livelli)\n");
    printf("Output reale : ");
    
    visitLevelByLevel(root, l1, l2);

    /* --- 3. PULIZIA MEMORIA --- */
    printf("\n--- Pulizia Memoria ---\n");
    freeTree(root);
    printf("Memoria liberata correttamente.\n");

    return 0;
}