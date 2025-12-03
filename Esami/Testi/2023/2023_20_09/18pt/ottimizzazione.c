#include "Graph.h"
#include "uf.h" 
#include <limits.h>
#include <stdlib.h> 

/* ======================================================================================
 * VARIABILI GLOBALI (Stato della Ricorsione)
 * ======================================================================================
 * Usiamo variabili globali per evitare di passare troppi puntatori attraverso
 * lo stack della ricorsione. In un contesto d'esame è accettabile per semplificare.
 */
int best_cost = INT_MAX;    /* Il miglior costo trovato finora (minimo) */
int *best_partition = NULL; /* La migliore configurazione di partizione salvata */
int *curr_partition = NULL; /* La configurazione che stiamo costruendo attualmente */
int V_global;               /* Numero totale di vertici (per i cicli) */

/*
 * ======================================================================================
 * NOME FUNZIONE: compare_edges
 * ======================================================================================
 * SCOPO:
 * Funzione di comparazione per qsort(). Serve per ordinare gli archi in base al peso
 * crescente, necessario per l'algoritmo di Kruskal.
 *
 * PARAMETRI:
 * - const void *a, *b: Puntatori generici agli elementi da confrontare.
 *
 * DETTAGLIO SINTATTICO:
 * ((Edge*)a)->w: Casting esplicito da void* a Edge*, poi accesso al campo w.
 * ======================================================================================
 */
int compare_edges(const void *a, const void *b) {
    return ((Edge*)a)->w - ((Edge*)b)->w;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: get_MST_cost_induced
 * ======================================================================================
 * SCOPO:
 * Calcola il costo del Minimum Spanning Tree (MST) per un sottoinsieme specifico di vertici.
 * Verifica anche due vincoli fondamentali:
 * 1. Cardinalità: Il sottoinsieme deve avere almeno k vertici.
 * 2. Connessione: Il sottoinsieme deve formare un grafo connesso (altrimenti niente albero).
 *
 * PARAMETRI:
 * - Graph G: Il grafo completo.
 * - int *partition: L'array che dice a quale insieme appartiene ogni vertice.
 * - int set_id: L'ID dell'insieme che stiamo analizzando (es. insieme 0, insieme 1...).
 * - int k: Dimensione minima richiesta.
 *
 * RITORNA:
 * - Il peso totale del MST se valido.
 * - INT_MAX se i vincoli non sono rispettati o il grafo non è connesso.
 *
 * CONCETTI CHIAVE:
 * Algoritmo di Kruskal su Sottografo Indotto: Costruiamo un grafo temporaneo usando
 * solo i vertici del set_id e gli archi che collegano due vertici DELLO STESSO set.
 * ======================================================================================
 */
int get_MST_cost_induced(Graph G, int *partition, int set_id, int k) {
    /* --- 1. CONTEGGIO VERTICI NEL SET --- */
    int v_count = 0;
    int V = GRAPHgetV(G);
    
    for(int i=0; i<V; i++) 
        if(partition[i] == set_id) v_count++;

    /* Pruning rapido: se non abbiamo abbastanza vertici, è invalido */
    if (v_count < k) return INT_MAX; 

    /* --- 2. FILTRAGGIO DEGLI ARCHI (Sottografo Indotto) --- */
    /* Allocazione dinamica temporanea per gli archi interni al set */
    Edge *induced_edges = malloc(G->E_count * sizeof(Edge)); 
    int ie_count = 0;
    
    for (int i=0; i < G->E_count; i++) {
        /*
         * Controllo Appartenenza:
         * Un arco fa parte del sottografo indotto SOLO SE entrambi gli estremi (u e v)
         * appartengono all'insieme 'set_id'.
         */
        if (partition[G->edges[i].u] == set_id && 
            partition[G->edges[i].v] == set_id) {
            induced_edges[ie_count++] = G->edges[i];
        }
    }

    /* --- 3. ORDINAMENTO (Kruskal) --- */
    qsort(induced_edges, ie_count, sizeof(Edge), compare_edges);
    
    /* --- 4. ALGORITMO DI KRUSKAL --- */
    /* Inizializziamo la Union-Find con TUTTI i vertici del grafo originale
     * per semplicità di indicizzazione.
     */
    UF uf = UF_init(V);
    
    int mst_weight = 0;
    int edges_selected = 0;

    for (int i=0; i<ie_count; i++) {
        Edge e = induced_edges[i];
        /* * UF_union restituisce 1 (true) se i vertici erano disgiunti e sono stati uniti.
         * Restituisce 0 (false) se erano già collegati (ciclo).
         */
        if (UF_union(uf, e.u, e.v)) {
            mst_weight += e.w;
            edges_selected++;
        }
    }

    /* --- 5. GESTIONE MEMORIA E VERIFICA FINALE --- */
    UF_free(uf);          /* Libero la struttura Union-Find */
    free(induced_edges);  /* Libero il vettore temporaneo degli archi */

    /* * Proprietà degli Alberi: Un albero con N nodi ha sempre esattamente N-1 archi.
     * Se edges_selected < v_count - 1, significa che il grafo è disconnesso (foresta).
     */
    if (edges_selected == v_count - 1) 
        return mst_weight;
    
    return INT_MAX; /* Non connesso */
}

/*
 * ======================================================================================
 * NOME FUNZIONE: partition_backtrack
 * ======================================================================================
 * SCOPO:
 * Genera ricorsivamente tutte le possibili partizioni dei vertici in insiemi (Set Partitioning).
 *
 * PARAMETRI:
 * - Graph G: Il grafo.
 * - int k: Parametro dimensione minima.
 * - int index: Il vertice corrente che stiamo decidendo dove mettere (0..V-1).
 * - int num_sets: Il numero di insiemi attualmente non vuoti creati.
 *
 * CONCETTI CHIAVE:
 * Numeri di Bell: Stiamo esplorando lo spazio delle partizioni.
 * Per ogni elemento 'index', abbiamo due scelte:
 * 1. Metterlo in uno degli insiemi già esistenti (0 ... num_sets-1).
 * 2. Creare un nuovo insieme (num_sets), ma solo se ha senso.
 * ======================================================================================
 */
void partition_backtrack(Graph G, int k, int index, int num_sets) {
    /* --- 1. CASO BASE (Foglia della ricorsione) --- */
    if (index == V_global) {
        int total_cost = 0;
        int valid = 1;

        /* Iteriamo su ogni insieme creato per calcolarne il costo MST */
        for (int s = 0; s < num_sets; s++) {
            int cost = get_MST_cost_induced(G, curr_partition, s, k);
            
            /* Se anche solo un insieme è invalido (disconnesso o < k), scartiamo tutto */
            if (cost == INT_MAX) {
                valid = 0;
                break;
            }
            total_cost += cost;
        }

        /* Aggiornamento dell'ottimo globale */
        if (valid && total_cost < best_cost) {
            best_cost = total_cost;
            /* Copiamo l'array corrente in quello "best" per salvarlo */
            for(int i=0; i<V_global; i++) best_partition[i] = curr_partition[i];
        }
        return;
    }

    /* --- 2. PASSO RICORSIVO A: Aggiungi a insieme esistente --- */
    for (int s = 0; s < num_sets; s++) {
        curr_partition[index] = s; /* Assegno vertice 'index' al set 's' */
        partition_backtrack(G, k, index + 1, num_sets);
    }

    /* --- 3. PASSO RICORSIVO B: Crea nuovo insieme --- */
    /* Assegno vertice 'index' a un nuovo set (ID = num_sets).
     * Incremento il contatore dei set nella chiamata ricorsiva.
     */
    curr_partition[index] = num_sets;
    partition_backtrack(G, k, index + 1, num_sets + 1);
}

/*
 * ======================================================================================
 * NOME FUNZIONE: solve_optimization
 * ======================================================================================
 * SCOPO:
 * Funzione Wrapper che prepara le strutture dati, lancia il backtracking e stampa i risultati.
 *
 * PARAMETRI:
 * - Graph G: Il grafo.
 * - int k: Dimensione minima k.
 *
 * GESTIONE MEMORIA:
 * Allocazione di 'curr_partition' e 'best_partition' necessaria per tutto il corso
 * della ricorsione. Vengono liberati solo alla fine.
 * ======================================================================================
 */
void solve_optimization(Graph G, int k) {
    V_global = GRAPHgetV(G);
    
    /* Allocazione array per le soluzioni (dimensione V interi) */
    curr_partition = malloc(V_global * sizeof(int));
    best_partition = malloc(V_global * sizeof(int));
    
    /* * TRUCCO DI OTTIMIZZAZIONE (Rottura Simmetria):
     * Fissiamo sempre il vertice 0 nell'insieme 0.
     * Questo evita di generare partizioni duplicate che sono solo permutazioni degli ID
     * (es: {0} U {1,2} è uguale a {1,2} U {0}). Riduce drasticamente i calcoli.
     */
    curr_partition[0] = 0;
    
    printf("Avvio ricerca partizionamento ottimo (k=%d)...\n", k);
    
    /* Avviamo la ricorsione dal vertice 1, con 1 insieme già attivo (quello dello 0) */
    partition_backtrack(G, k, 1, 1); 

    if (best_cost == INT_MAX) {
        printf("Nessuna soluzione trovata.\n");
    } else {
        printf("Soluzione ottima trovata con costo totale: %d\n", best_cost);
        printf("Partizione vertici: ");
        for(int i=0; i<V_global; i++) printf("%d ", best_partition[i]);
        printf("\n");
    }

    /* Pulizia finale */
    free(curr_partition);
    free(best_partition);
}