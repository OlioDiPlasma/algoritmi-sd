#include "Graph.h"
#include <stdlib.h>
/*
 * ======================================================================================
 * NOME FUNZIONE: GRAPHload
 * ======================================================================================
 * SCOPO:
 * Carica un grafo da file, allocando dinamicamente la struttura principale, la matrice
 * di adiacenza e il vettore degli archi (con ridimensionamento automatico).
 *
 * PARAMETRI:
 * - const char *filename: Il percorso del file di testo da leggere.
 *
 * CONCETTI CHIAVE:
 * 1. Doppia Malloc per Matrici: In C, 'int **' non alloca automaticamente una griglia.
 * Dobbiamo allocare il vettore delle righe e poi, in un ciclo, ogni singola riga.
 * 2. Realloc Ammortizzata: Non sapendo quanti archi ci sono, partiamo con una
 * capacità piccola e la raddoppiamo quando piena.
 * ======================================================================================
 */
Graph GRAPHload(const char *filename) {
    /* --- 1. APERTURA FILE --- */
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        return NULL;
    }

    /* --- 2. ALLOCAZIONE STRUTTURA GRAFO --- */
    /* Malloc: Allocazione dello spazio per la "testa" del grafo (V, E_count, puntatori).
     * sizeof(struct graph): Fondamentale per allocare i byte corretti della struct nascosta.
     */
    Graph G = (Graph)malloc(sizeof(struct graph));
    
    /* Lettura numero vertici (V) */
    /* Accesso G->V: Dereferenziamo il puntatore G per scrivere nel campo V. */
    fscanf(fp, "%d", &G->V);

    /* --- 3. ALLOCAZIONE MATRICE DI ADIACENZA (int **) --- */
    
    /* Passo A: Allocazione del vettore delle righe.
     * Stiamo allocando 'V' puntatori. Ogni cella conterrà un indirizzo di memoria (int*).
     * sizeof(int*): Usiamo la dimensione di un puntatore, NON di un intero.
     */
    G->adj = (int**)malloc(G->V * sizeof(int*));
    
    /* Passo B: Allocazione delle colonne per ogni riga */
    for (int i = 0; i < G->V; i++) {
        /* G->adj[i]: Accediamo alla riga i-esima.
         * Allochiamo 'V' interi per quella riga. Qui usiamo sizeof(int).
         */
        G->adj[i] = (int*)malloc(G->V * sizeof(int));
        
        /* Inizializzazione: -1 indica assenza di arco */
        for (int j = 0; j < G->V; j++) 
            G->adj[i][j] = -1; 
    }

    /* --- 4. PREPARAZIONE VETTORE ARCHI (Array Dinamico) --- */
    int capacity = 10; /* Capacità iniziale arbitraria */
    
    /* Allocazione iniziale del vettore di struct Edge */
    G->edges = (Edge*)malloc(capacity * sizeof(Edge));
    G->E_count = 0;

    /* --- 5. LETTURA ARCHI --- */
    int u, v, w;
    /* while: Continua finché fscanf legge correttamente 3 valori (sorgente, dest, peso) */
    while (fscanf(fp, "%d %d %d", &u, &v, &w) == 3) {
        
        /* Controllo Saturazione Vettore */
        if (G->E_count == capacity) {
            capacity *= 2; /* Strategia raddoppio */
            
            /* Realloc: Tenta di allargare il blocco di memoria. 
             * Se non ci riesce in loco, ne cerca uno nuovo, copia i dati e libera il vecchio.
             * Importante: Aggiorniamo il puntatore G->edges col nuovo indirizzo.
             */
            G->edges = (Edge*)realloc(G->edges, capacity * sizeof(Edge));
            
            /* Controllo fallimento allocazione (Best Practice) */
            if (G->edges == NULL) {
                /* Gestione errore memoria critica */
                exit(1); 
            }
        }
        
        /* Inserimento dati nel vettore */
        /* G->edges[index].campo: Accediamo all'elemento e poi al suo campo */
        G->edges[G->E_count].u = u;
        G->edges[G->E_count].v = v;
        G->edges[G->E_count].w = w;
        G->E_count++;

        /* Popolamento Matrice di Adiacenza */
        /* Poiché il grafo non è orientato, segniamo l'arco in entrambe le direzioni */
        G->adj[u][v] = w;
        G->adj[v][u] = w;
    }

    fclose(fp);
    return G;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: GRAPHgetV
 * ======================================================================================
 * SCOPO:
 * Funzione "Getter" per esporre il numero di vertici all'esterno senza dare
 * accesso diretto alla struct (principio di Incapsulamento).
 */
int GRAPHgetV(Graph G) { 
    return G->V; 
}

/*
 * ======================================================================================
 * NOME FUNZIONE: GRAPHcheckEdge
 * ======================================================================================
 * SCOPO:
 * Verifica l'esistenza di un arco e ne restituisce il peso in tempo costante O(1).
 *
 * PARAMETRI:
 * - Graph G: Il grafo.
 * - int u, v: I vertici da controllare.
 *
 * DETTAGLIO SINTATTICO:
 * G->adj[u][v]: Dereferenziazione doppia. 'adj' punta all'array di righe, '[u]' seleziona
 * il puntatore alla riga corretta, '[v]' accede all'offset nella riga.
 */
int GRAPHcheckEdge(Graph G, int u, int v) {
    /* Boundary Check: Protezione contro Segmentation Fault */
    if (u < 0 || v < 0 || u >= G->V || v >= G->V) return -1;
    
    return G->adj[u][v];
}

/*
 * ======================================================================================
 * NOME FUNZIONE: GRAPHfree
 * ======================================================================================
 * SCOPO:
 * Liberare la memoria allocata dinamicamente.
 *
 * GESTIONE MEMORIA (Critico):
 * L'ordine di 'free' deve essere inverso rispetto alla 'malloc'.
 * 1. Prima liberiamo il contenuto profondo (le singole righe della matrice).
 * 2. Poi il contenitore delle righe (il vettore adj).
 * 3. Poi il vettore degli archi (edges).
 * 4. Infine la struttura principale (G).
 */
void GRAPHfree(Graph G) {
    if (!G) return;
    
    /* 1. Loop per liberare ogni array di interi (righe) */
    for (int i = 0; i < G->V; i++) free(G->adj[i]);
    
    /* 2. Libero l'array di puntatori */
    free(G->adj);
    
    /* 3. Libero il vettore dinamico edges */
    free(G->edges);
    
    /* 4. Libero la struct graph */
    free(G);
}