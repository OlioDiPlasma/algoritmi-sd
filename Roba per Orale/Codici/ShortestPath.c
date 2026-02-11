#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 5 // Numero di vertici fisso per l'esempio Dijkstra

// ==========================================
// 1. ALGORITMO DI DIJKSTRA (Greedy)
// ==========================================

// Funzione utilitaria per trovare il vertice con distanza minima
// non ancora incluso nell'insieme dei processati.
int minDistance(int dist[], int visited[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}

void Dijkstra(int graph[V][V], int src) {
    int dist[V];    // Distanze minime finali
    int visited[V]; // visited[i] è true se il nodo i è stato processato

    // Inizializzazione
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0; // Distanza sorgente -> sorgente è 0

    // Loop principale
    for (int count = 0; count < V - 1; count++) {
        // 1. Prendi il nodo u con distanza minima tra quelli non visitati
        // (Simula l'estrazione da una Priority Queue)
        int u = minDistance(dist, visited);

        // 2. Segnalo come visitato (chiuso)
        visited[u] = 1;

        // 3. RILASSAMENTO dei vicini
        for (int v = 0; v < V; v++) {
            // Se c'è un arco u-v, se v non è visitato, e se il nuovo percorso è migliore
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX 
                && dist[u] + graph[u][v] < dist[v]) {
                
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Stampa
    printf("\n--- Dijkstra (Sorgente: %d) ---\n", src);
    printf("Vertice \t Distanza dalla Sorgente\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// ==========================================
// 2. ALGORITMO DI BELLMAN-FORD (DP)
// ==========================================

struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V_bf, E_bf;
    struct Edge* edge;
};

struct Graph* createGraphBF(int v, int e) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V_bf = v;
    graph->E_bf = e;
    graph->edge = (struct Edge*)malloc(e * sizeof(struct Edge));
    return graph;
}

void BellmanFord(struct Graph* graph, int src) {
    int V_count = graph->V_bf;
    int E_count = graph->E_bf;
    int dist[V_count];

    // 1. Inizializzazione
    for (int i = 0; i < V_count; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    // 2. Rilassamento ripetuto V-1 volte
    // (Paradigma DP: propaga le distanze)
    for (int i = 1; i <= V_count - 1; i++) {
        for (int j = 0; j < E_count; j++) {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;

            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // 3. Controllo Cicli Negativi (V-esima iterazione)
    for (int j = 0; j < E_count; j++) {
        int u = graph->edge[j].src;
        int v = graph->edge[j].dest;
        int weight = graph->edge[j].weight;

        // Se riesco ancora a rilassare (migliorare) un arco, c'è un ciclo negativo
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("\n--- Bellman-Ford ---\n");
            printf("ERRORE: Il grafo contiene un ciclo di peso negativo!\n");
            return; // Esci
        }
    }

    // Stampa
    printf("\n--- Bellman-Ford (Sorgente: %d) ---\n", src);
    printf("Vertice \t Distanza dalla Sorgente\n");
    for (int i = 0; i < V_count; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// --- MAIN ---
int main() {
    // === TEST DIJKSTRA ===
    /* Grafo (Pesi positivi):
         0 --(4)--> 1 --(1)--> 2
         |          |
        (8)        (2)
         |          |
         v          v
         4 <--(3)-- 3
    */
    // Matrice Adiacenza (0 indica nessun arco, eccetto diagonale)
    // Nota: Dijkstra non distingue tra 0 (nessun arco) e 0 (costo zero).
    // In questo esempio assumiamo archi > 0.
    int graphDijkstra[V][V] = {
        {0, 4, 0, 0, 8}, // Da 0
        {0, 0, 1, 2, 0}, // Da 1
        {0, 0, 0, 0, 0}, // Da 2 (Pozzo)
        {0, 0, 0, 0, 3}, // Da 3
        {0, 0, 0, 0, 0}  // Da 4 (Pozzo)
    };
    
    // Attenzione: Dijkstra standard su matrice richiede gestione dell'assenza di arco.
    // Nella funzione sopra: if (graph[u][v]) controlla se c'è l'arco.
    
    Dijkstra(graphDijkstra, 0);


    // === TEST BELLMAN-FORD ===
    // Grafo con pesi anche negativi (ma senza cicli negativi)
    // 0 -> 1 (Peso -1)
    // 0 -> 2 (Peso 4)
    // 1 -> 2 (Peso 3)
    // 1 -> 3 (Peso 2)
    // 1 -> 4 (Peso 2)
    // 3 -> 2 (Peso 5)
    // 3 -> 1 (Peso 1)
    // 4 -> 3 (Peso -3)
    int V_bf = 5;
    int E_bf = 8;
    struct Graph* graphBF = createGraphBF(V_bf, E_bf);

    // Definizione archi
    graphBF->edge[0] = (struct Edge){0, 1, -1};
    graphBF->edge[1] = (struct Edge){0, 2, 4};
    graphBF->edge[2] = (struct Edge){1, 2, 3};
    graphBF->edge[3] = (struct Edge){1, 3, 2};
    graphBF->edge[4] = (struct Edge){1, 4, 2};
    graphBF->edge[5] = (struct Edge){3, 2, 5};
    graphBF->edge[6] = (struct Edge){3, 1, 1};
    graphBF->edge[7] = (struct Edge){4, 3, -3};

    BellmanFord(graphBF, 0);

    return 0;
}