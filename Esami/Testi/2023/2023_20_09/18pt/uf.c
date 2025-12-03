#include "uf.h"
#include <stdlib.h>

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURA (Concrete Type)
 * ======================================================================================
 * Questa definizione è visibile SOLO all'interno di questo file .c.
 * Qui sveliamo i dettagli implementativi nascosti dall'ADT nel file .h.
 *
 * CAMPI:
 * - int *parent: Array dove parent[i] è il genitore del nodo i. Se parent[i] == i, è radice.
 * - int *rank:   Array che traccia l'altezza approssimativa dell'albero (per ottimizzare).
 * - int n:       Numero totale di elementi gestiti.
 */
struct uf_struct {
    int *parent;
    int *rank;
    int n;
};

/*
 * ======================================================================================
 * NOME FUNZIONE: UF_init
 * ======================================================================================
 * SCOPO:
 * Costruttore dell'ADT. Alloca memoria per la struttura principale e per gli array interni,
 * inizializzando ogni elemento come un insieme separato (ognuno è padre di se stesso).
 *
 * PARAMETRI:
 * - int n: Numero di elementi totali nel sistema.
 *
 * CONCETTI CHIAVE:
 * 1. Allocazione Multipla: Dobbiamo allocare prima il "contenitore" (struct) e poi
 * i "contenuti" (array parent e rank).
 * 2. Calloc vs Malloc: Usiamo calloc per il 'rank' per inizializzare tutto a 0 automaticamente.
 * ======================================================================================
 */
UF UF_init(int n) {
    /* --- 1. ALLOCAZIONE STRUTTURA --- */
    /* Allocazione dello spazio per la struct wrapper */
    UF uf = malloc(sizeof(struct uf_struct));
    
    /* Inizializzazione campo scalare */
    uf->n = n;

    /* --- 2. ALLOCAZIONE ARRAY INTERNI --- */
    /* Malloc per l'array dei padri (n interi) */
    uf->parent = malloc(n * sizeof(int));
    
    /* Calloc per l'array dei ranghi. 
     * calloc(n, size) alloca memoria E la setta a zero bit-a-bit.
     * È fondamentale perché all'inizio ogni albero ha altezza (rango) 0.
     */
    uf->rank = calloc(n, sizeof(int));

    /* --- 3. INIZIALIZZAZIONE LOGICA (Make-Set) --- */
    /* Inizialmente, ogni nodo è padre di se stesso (disgiunti) */
    for(int i=0; i<n; i++) 
        uf->parent[i] = i;
    
    return uf; /* Restituiamo il puntatore all'oggetto creato */
}

/*
 * ======================================================================================
 * NOME FUNZIONE: UF_find
 * ======================================================================================
 * SCOPO:
 * Trovare la radice dell'albero a cui appartiene il nodo 'i'.
 *
 * CONCETTI CHIAVE:
 * Path Compression (Compressione del Cammino):
 * Questa è una tecnica avanzata di ottimizzazione. Mentre risaliamo l'albero per
 * trovare la radice, aggiorniamo il puntatore 'parent' di ogni nodo attraversato
 * facendolo puntare DIRETTAMENTE alla radice.
 * Questo appiattisce l'albero, rendendo le future ricerche quasi istantanee O(1).
 * ======================================================================================
 */
int UF_find(UF uf, int i) {
    /* Caso base: Se il padre di i non è i, allora i non è la radice */
    if (uf->parent[i] != i)
        /* Passo Ricorsivo con Compressione:
         * 1. Chiamiamo UF_find sul genitore.
         * 2. Il risultato (la radice assoluta) viene assegnato direttamente a parent[i].
         */
        uf->parent[i] = UF_find(uf, uf->parent[i]);
    
    /* Ritorniamo il genitore (che ora è sicuramente la radice) */
    return uf->parent[i];
}

/*
 * ======================================================================================
 * NOME FUNZIONE: UF_union
 * ======================================================================================
 * SCOPO:
 * Unire due insiemi disgiunti collegando le loro radici.
 *
 * PARAMETRI:
 * - UF uf: La struttura dati.
 * - int i, j: I due nodi che vogliamo connettere.
 *
 * RITORNA:
 * - 1: Se l'unione è avvenuta con successo (erano staccati).
 * - 0: Se erano già nello stesso insieme (rileva cicli nei grafi).
 *
 * CONCETTI CHIAVE:
 * Union by Rank (Unione per Rango):
 * Per mantenere l'albero bilanciato ed evitare che diventi una lunga lista linkata,
 * attacchiamo sempre l'albero più basso (rango minore) sotto la radice dell'albero
 * più alto.
 * ======================================================================================
 */
int UF_union(UF uf, int i, int j) {
    /* --- 1. RICERCA DELLE RADICI --- */
    int root_i = UF_find(uf, i);
    int root_j = UF_find(uf, j);

    /* --- 2. CONTROLLO CONNETTIVITÀ --- */
    /* Se le radici sono diverse, i nodi sono in insiemi separati -> Uniamo */
    if (root_i != root_j) {
        
        /* --- 3. LOGICA DI LINKING (By Rank) --- */
        
        /* Caso A: L'albero i è più basso di j. Attacco i sotto j. */
        if (uf->rank[root_i] < uf->rank[root_j])
            uf->parent[root_i] = root_j;
        
        /* Caso B: L'albero i è più alto di j. Attacco j sotto i. */
        else if (uf->rank[root_i] > uf->rank[root_j])
            uf->parent[root_j] = root_i;
        
        /* Caso C: Hanno la stessa altezza.
         * Attacco arbitrariamente (es. i sotto j) e incremento il rango della nuova radice.
         */
        else {
            uf->parent[root_i] = root_j;
            uf->rank[root_j]++; /* L'albero j è diventato più alto di 1 */
        }
        
        return 1; /* Unione effettuata */
    }
    
    return 0; /* Erano già connessi */
}

/*
 * ======================================================================================
 * NOME FUNZIONE: UF_free
 * ======================================================================================
 * SCOPO:
 * Liberare correttamente la memoria per evitare Memory Leaks.
 *
 * GESTIONE MEMORIA:
 * L'ordine è inverso rispetto alla creazione (UF_init).
 * Prima liberiamo i "figli" (gli array puntati dalla struct), poi il "padre" (la struct).
 * Se liberassimo prima 'uf', perderemmo i puntatori agli array!
 * ======================================================================================
 */
void UF_free(UF uf) {
    /* Libero l'array dei genitori */
    free(uf->parent);
    
    /* Libero l'array dei ranghi */
    free(uf->rank);
    
    /* Infine, libero il contenitore principale */
    free(uf);
}