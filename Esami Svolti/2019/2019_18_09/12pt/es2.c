#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 50 /* Lunghezza massima stringa come da traccia */

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT)
 * ======================================================================================
 * Definiamo i nodi per l'Albero (Tree) e per la Lista (List) separatamente,
 * come richiesto dal testo.
 */

/* NODO ALBERO */
typedef struct nodeT {
    char key[MAXC];      // Il dato (stringa)
    struct nodeT *left;  // Figlio sinistro
    struct nodeT *right; // Figlio destro
} nodeT_t, *linkT;

/* NODO LISTA */
typedef struct nodeL {
    char key[MAXC];      // Il dato (stringa copiata dall'albero)
    struct nodeL *next;  // Puntatore al prossimo nodo della lista
} nodeL_t, *linkL;


/*
 * ======================================================================================
 * NOME FUNZIONE: nuovoNodoLista
 * ======================================================================================
 * SCOPO:
 * Funzione di utilità per allocare un nodo della lista.
 * Isola la logica della malloc e dell'inizializzazione.
 *
 * PARAMETRI:
 * - char *val: La stringa da copiare nel nodo.
 *
 * RITORNA:
 * - linkL: L'indirizzo del nuovo nodo nello Heap.
 *
 * GESTIONE MEMORIA:
 * - malloc: Allocazione dinamica. Se fallisce, terminiamo il programma.
 * - strcpy: Copia profonda della stringa (non copiamo solo il puntatore!).
 * ======================================================================================
 */
linkL nuovoNodoLista(char *val) {
    linkL x = (linkL)malloc(sizeof(nodeL_t));
    if (x == NULL) {
        printf("Errore malloc nodo lista\n");
        exit(1);
    }
    strcpy(x->key, val); // Copia i caratteri della stringa
    x->next = NULL;      // Il nuovo nodo nasce isolato
    return x;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: inserisciInCoda
 * ======================================================================================
 * SCOPO:
 * Inserisce un nuovo elemento alla fine della lista.
 * Questa è la funzione richiesta esplicitamente dal testo ("senza fare uso di funzioni di libreria").
 *
 * PARAMETRI:
 * - linkL *headRef: Puntatore alla testa (doppio puntatore per modificare la testa se NULL).
 * - linkL *tailRef: Puntatore alla coda (doppio puntatore per ottimizzazione O(1)).
 * - char *val: Il valore da inserire.
 *
 * CONCETTI CHIAVE:
 * - Ottimizzazione Tail: Manteniamo un puntatore all'ultimo elemento (*tailRef).
 * Senza di esso, dovremmo scorrere tutta la lista ogni volta (costo O(N^2)).
 * Con la tail, l'inserimento è immediato (costo O(1)).
 * - Doppi Puntatori (**): Modifichiamo i puntatori head e tail che risiedono nella funzione chiamante.
 * ======================================================================================
 */
void inserisciInCoda(linkL *headRef, linkL *tailRef, char *val) {
    /* --- 1. ALLOCAZIONE --- */
    linkL nuovo = nuovoNodoLista(val);

    /* --- 2. COLLEGAMENTO --- */
    if (*headRef == NULL) {
        /* Caso Lista Vuota: il nuovo nodo è sia testa che coda */
        *headRef = nuovo;
        *tailRef = nuovo;
    } else {
        /* Caso Lista Non Vuota: attacco in fondo */
        (*tailRef)->next = nuovo; // L'attuale ultimo punta al nuovo
        *tailRef = nuovo;         // Aggiorno il puntatore di coda
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: wrapperVisit (Funzione Ricorsiva Interna)
 * ======================================================================================
 * SCOPO:
 * Attraversa l'albero e popola la lista. È il "motore" della ricorsione.
 *
 * PARAMETRI:
 * - linkT root: Nodo corrente dell'albero.
 * - linkL *headRef, *tailRef: Riferimenti alla lista in costruzione.
 * - int visit: Strategia (1=Inorder, 2=Preorder, 3=Postorder).
 *
 * LOGICA:
 * Implementiamo i tre ordini di visita cambiando QUANDO chiamiamo inserisciInCoda.
 * - Preorder:  (Root) -> Left -> Right
 * - Inorder:   Left -> (Root) -> Right
 * - Postorder: Left -> Right -> (Root)
 * ======================================================================================
 */
void wrapperVisit(linkT root, linkL *headRef, linkL *tailRef, int visit) {
    /* Base della ricorsione: se il nodo non esiste, torno indietro */
    if (root == NULL) return;

    /* PRE-ORDER (2): Prima elaboro la radice */
    if (visit == 2) {
        inserisciInCoda(headRef, tailRef, root->key);
    }

    /* Visita Ricorsiva Sinistra */
    wrapperVisit(root->left, headRef, tailRef, visit);

    /* IN-ORDER (1): Elaboro la radice tra sinistra e destra */
    if (visit == 1) {
        inserisciInCoda(headRef, tailRef, root->key);
    }

    /* Visita Ricorsiva Destra */
    wrapperVisit(root->right, headRef, tailRef, visit);

    /* POST-ORDER (3): Elaboro la radice alla fine */
    if (visit == 3) {
        inserisciInCoda(headRef, tailRef, root->key);
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: tree2List (Richiesta dall'esercizio)
 * ======================================================================================
 * SCOPO:
 * Interfaccia principale. Inizializza la lista e lancia la ricorsione.
 *
 * PARAMETRI:
 * - linkT root: Radice dell'albero.
 * - int visit: Tipo di visita.
 *
 * RITORNA:
 * - linkL: La testa della nuova lista creata.
 * ======================================================================================
 */
linkL tree2List(linkT root, int visit) {
    /* --- 1. INIZIALIZZAZIONE LISTA --- */
    linkL head = NULL;
    linkL tail = NULL;

    /* --- 2. AVVIO RICORSIONE --- */
    /* Passiamo gli indirizzi di head e tail per permettere alla funzione
       ricorsiva di riempirle */
    wrapperVisit(root, &head, &tail, visit);

    /* --- 3. RITORNO --- */
    return head;
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO PER IL TEST
 * ======================================================================================
 */
linkT nuovoNodoAlbero(char *k) {
    linkT x = (linkT)malloc(sizeof(nodeT_t));
    strcpy(x->key, k);
    x->left = NULL;
    x->right = NULL;
    return x;
}

void stampaLista(linkL h) {
    printf("Lista: ");
    while (h != NULL) {
        printf("%s -> ", h->key);
        h = h->next;
    }
    printf("NULL\n");
}

void liberaLista(linkL h) {
    linkL temp;
    while (h != NULL) {
        temp = h->next;
        free(h);
        h = temp;
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE ALBERO DI PROVA --- */
    /*
            "C"
           /   \
         "A"   "E"
           \   /
           "B" "D"
    */
    linkT root = nuovoNodoAlbero("C");
    root->left = nuovoNodoAlbero("A");
    root->right = nuovoNodoAlbero("E");
    root->left->right = nuovoNodoAlbero("B");
    root->right->left = nuovoNodoAlbero("D");

    /* --- 2. TEST STRATEGIE --- */
    
    printf("--- Test 1: Inorder (visit=1) ---\n");
    /* Atteso: A -> B -> C -> D -> E (Ordinato alfabeticamente) */
    linkL L1 = tree2List(root, 1);
    stampaLista(L1);
    liberaLista(L1);

    printf("\n--- Test 2: Preorder (visit=2) ---\n");
    /* Atteso: C -> A -> B -> E -> D (Radice prima di tutto) */
    linkL L2 = tree2List(root, 2);
    stampaLista(L2);
    liberaLista(L2);

    printf("\n--- Test 3: Postorder (visit=3) ---\n");
    /* Atteso: B -> A -> D -> E -> C (Radice per ultima) */
    linkL L3 = tree2List(root, 3);
    stampaLista(L3);
    liberaLista(L3);

    /* Pulizia memoria albero (omessa per brevità, ma andrebbe fatta in un codice reale) */
    
    return 0;
}