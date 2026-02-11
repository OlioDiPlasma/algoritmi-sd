#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// =========================================================
// PARTE 1: DEFINIZIONE DI UNION-FIND (Necessaria per Kruskal)
// =========================================================

// 1. Definiamo la struttura QUI, così è visibile a tutto il file
struct Subset {
    int parent;
    int rank;
};

// 2. Inizializzazione Union-Find
void UFinit(struct Subset subsets[], int V) {
    for (int i = 0; i < V; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
}

// 3. Find con Path Compression
int UFfind(struct Subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = UFfind(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// 4. Union by Rank
void UFunion(struct Subset subsets[], int x, int y) {
    int xroot = UFfind(subsets, x);
    int yroot = UFfind(subsets, y);

    if (xroot == yroot) return;

    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// --- STRUTTURE PER KRUSKAL ---
struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V, E;
    struct Edge* edge; // Array di archi
};

struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (struct Edge*)malloc(E * sizeof(struct Edge));
    return graph;
}

// Comparatore per qsort (Kruskal richiede ordinamento archi)
int myComp(const void* a, const void* b) {
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight - b1->weight;
}

// ==========================================
// ALGORITMO DI KRUSKAL (Greedy + UnionFind)
// Complessità: O(E log E) o O(E log V)
// ==========================================
void KruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge result[V]; // Toveremo al massimo V-1 archi
    int e = 0; // Indice per result[]
    int i = 0; // Indice per gli archi ordinati

    // 1. Ordina tutti gli archi per peso crescente
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);

    // 2. Alloca memoria per Union-Find
    struct Subset* subsets = (struct Subset*)malloc(V * sizeof(struct Subset));
    UFinit(subsets, V);

    // 3. Itera sugli archi ordinati
    while (e < V - 1 && i < graph->E) {
        // Prendi il prossimo arco più piccolo
        struct Edge next_edge = graph->edge[i++];

        int x = UFfind(subsets, next_edge.src);
        int y = UFfind(subsets, next_edge.dest);

        // Se includere questo arco non crea un ciclo (cioè x != y)
        if (x != y) {
            result[e++] = next_edge; // Aggiungilo all'MST
            UFunion(subsets, x, y);  // Unisci i due insiemi
        }
        // Altrimenti scartalo (forma un ciclo)
    }

    // Stampa risultato
    printf("\n--- MST di Kruskal ---\n");
    int minimumCost = 0;
    for (i = 0; i < e; ++i) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        minimumCost += result[i].weight;
    }
    printf("Costo Totale MST: %d\n", minimumCost);
    free(subsets);
}

// ==========================================
// ALGORITMO DI PRIM (Matrice Adiacenza)
// Complessità: O(V^2) - Ottimo per grafi densi
// ==========================================

// Funzione utile per trovare il vertice con chiave minima non ancora incluso
int minKey(int key[], int mstSet[], int V) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (mstSet[v] == 0 && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    return min_index;
}

void PrimMST(int graph[5][5], int V) { // Esempio con V fisso a 5 per semplicità firma
    int parent[V]; // Array per memorizzare l'MST
    int key[V];    // Valori minimi per pescare il taglio
    int mstSet[V]; // Tiene traccia dei vertici già inclusi nell'MST

    // Inizializza
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    // Includi sempre il primo vertice nell'MST
    key[0] = 0;     // Chiave 0 così viene scelto per primo
    parent[0] = -1; // La radice non ha padre

    // L'MST avrà V vertici
    for (int count = 0; count < V - 1; count++) {
        // 1. Scegli il vertice u con chiave minima non ancora nell'MST
        int u = minKey(key, mstSet, V);

        // 2. Aggiungi u all'MST
        mstSet[u] = 1;

        // 3. Aggiorna le chiavi dei vertici adiacenti a u
        // Consideriamo solo vertici non ancora nell'MST
        for (int v = 0; v < V; v++) {
            // graph[u][v] è non zero solo se c'è un arco
            if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printf("\n--- MST di Prim ---\n");
    int totalWeight = 0;
    for (int i = 1; i < V; i++) {
        printf("%d -- %d == %d\n", parent[i], i, graph[i][parent[i]]);
        totalWeight += graph[i][parent[i]];
    }
    printf("Costo Totale MST: %d\n", totalWeight);
}

// --- MAIN DI ESEMPIO ---
int main() {
    /* Grafo:
        2    3
    (0)--(1)--(2)
     |   / \   |
    6| 8/   \5 |7
     | /     \ |
    (3)-------(4)
          9       */
    
    int V = 5; 
    int E = 7;
    struct Graph* g = createGraph(V, E);

    // Archi per Kruskal
    g->edge[0] = (struct Edge){0, 1, 2};
    g->edge[1] = (struct Edge){0, 3, 6};
    g->edge[2] = (struct Edge){1, 3, 8};
    g->edge[3] = (struct Edge){1, 2, 3};
    g->edge[4] = (struct Edge){1, 4, 5};
    g->edge[5] = (struct Edge){2, 4, 7};
    g->edge[6] = (struct Edge){3, 4, 9};

    KruskalMST(g);

    // Matrice per Prim (Stesso grafo)
    int graphMatrix[5][5] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };

    PrimMST(graphMatrix, 5);

    return 0;
}