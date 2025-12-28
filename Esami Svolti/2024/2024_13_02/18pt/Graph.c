#include "Graph.h"

/* --- 1. STRUTTURE DATI INTERNE --- */
// Nodo della lista di adiacenza
struct edge_node {
    int v;          // Destinazione
    struct edge_node *next;
};

// Struttura principale del Grafo
struct graph {
    int V;          // Numero Vertici
    int E;          // Numero Archi
    NodeData *nodes; // Vettore di struct per i dati dei nodi (tabella simboli interna)
    Link *ladj;     // Vettore di liste di adiacenza
};

/* * Scopo: Creare un nuovo nodo della lista di adiacenza
 * Parametri:
 * - v: vertice destinazione
 * - next: puntatore al prossimo elemento
 */
static Link newNode(int v, Link next) {
    Link x = malloc(sizeof *x);
    if (x == NULL) {
        printf("Errore allocazione nodo lista\n");
        exit(EXIT_FAILURE);
    }
    x->v = v;
    x->next = next;
    return x;
}

/* * Scopo: Inserire un arco (non orientato -> doppia inserzione)
 */
static void insertEdge(Graph G, int u, int v) {
    // Inserisco v nella lista di u
    G->ladj[u] = newNode(v, G->ladj[u]);
    // Inserisco u nella lista di v (Grafo Non Orientato [cite: 89])
    G->ladj[v] = newNode(u, G->ladj[v]);
}

/* * Scopo: Caricare il grafo da file
 * Concetti Chiave:
 * - Lettura formattata per stringhe e interi.
 * - Allocazione dinamica della stringa nome.
 */
Graph GRAPHload(FILE *f) {
    Graph G = malloc(sizeof *G);
    int i, id, val, u, v;
    char buffer[21]; // Buffer temporaneo statico [cite: 90]

    // Lettura NV e NE
    if(fscanf(f, "%d %d", &G->V, &G->E) != 2) return NULL;

    // Allocazione vettori
    G->ladj = calloc(G->V, sizeof(Link)); // Calloc inizializza a NULL
    G->nodes = malloc(G->V * sizeof(NodeData));

    /* --- 2. LETTURA NODI --- */
    // Formato: <id> <nome> <valore> [cite: 117]
    for (i = 0; i < G->V; i++) {
        fscanf(f, "%d %s %d", &id, buffer, &val);
        // id si assume ordinato 0..V-1, usiamo id come indice diretto
        G->nodes[id].val = val;
        G->nodes[id].name = strdup(buffer); // Copia dinamica della stringa
    }

    /* --- 3. LETTURA ARCHI --- */
    for (i = 0; i < G->E; i++) {
        fscanf(f, "%d %d", &u, &v);
        insertEdge(G, u, v);
    }
    
    return G;
}

/* * Scopo: Liberare tutta la memoria
 * Gestione Memoria: 
 * 1. Free dei nodi delle liste.
 * 2. Free delle stringhe nei nodi.
 * 3. Free dei vettori.
 * 4. Free della struttura.
 */
void GRAPHfree(Graph G) {
    int v;
    Link t, next;
    
    for (v = 0; v < G->V; v++) {
        // Libera lista adiacenza
        for (t = G->ladj[v]; t != NULL; t = next) {
            next = t->next;
            free(t);
        }
        // Libera stringa nome
        free(G->nodes[v].name);
    }
    free(G->nodes);
    free(G->ladj);
    free(G);
}

// Funzioni Accessorie (Wrapper)
int GRAPHgetNV(Graph G) { return G->V; }
char* GRAPHgetName(Graph G, int id) { return G->nodes[id].name; }
int GRAPHgetVal(Graph G, int id) { return G->nodes[id].val; }
Link GRAPHgetHead(Graph G, int v) { return G->ladj[v]; }
Link GRAPHgetNext(Link t) { return t->next; }
int GRAPHgetDest(Link t) { return t->v; }