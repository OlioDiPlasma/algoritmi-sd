#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// --- STRUTTURE DATI ---

// Nodo della lista di adiacenza
struct Node {
    int dest;
    int weight; // Aggiunto peso per la domanda sui ponti pesati
    struct Node* next;
};

// Struttura Grafo
struct Graph {
    int V;
    struct Node** adj;
};

// Creazione nuovo nodo
struct Node* newNode(int dest, int weight) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

// Creazione Grafo
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adj = (struct Node**)malloc(V * sizeof(struct Node*));
    for (int i = 0; i < V; ++i)
        graph->adj[i] = NULL;
    return graph;
}

// Aggiungi arco non orientato
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    // Arco src -> dest
    struct Node* node = newNode(dest, weight);
    node->next = graph->adj[src];
    graph->adj[src] = node;

    // Arco dest -> src
    node = newNode(src, weight);
    node->next = graph->adj[dest];
    graph->adj[dest] = node;
}

// --- UTILITÀ PER METODO NAÏVE ---

void DFS_Naive(struct Graph* g, int v, int visited[], int skipNode) {
    visited[v] = 1;
    struct Node* temp = g->adj[v];
    while (temp) {
        if (!visited[temp->dest] && temp->dest != skipNode) {
            DFS_Naive(g, temp->dest, visited, skipNode);
        }
        temp = temp->next;
    }
}

int countComponents(struct Graph* g, int skipNode) {
    int count = 0;
    int* visited = (int*)calloc(g->V, sizeof(int));
    
    for (int i = 0; i < g->V; i++) {
        // Se il nodo è quello da saltare, lo ignoriamo (come se fosse rimosso)
        if (i == skipNode) continue;

        if (!visited[i]) {
            DFS_Naive(g, i, visited, skipNode);
            count++;
        }
    }
    free(visited);
    return count;
}

// Funzione Naïve richiesta: Verifica se un nodo specifico è AP
int isSpecificVertexAP(struct Graph* g, int u) {
    // 1. Contiamo componenti con il grafo originale (nessun nodo saltato = -1)
    int initialComp = countComponents(g, -1);
    
    // 2. Se il grafo è già sconnesso, la definizione vale comunque:
    // se rimuovendo u le componenti aumentano ULTERIORMENTE, è un AP.
    
    // 3. Contiamo componenti saltando u
    int finalComp = countComponents(g, u);
    
    // Se le componenti sono aumentate, è un AP
    if (finalComp > initialComp) return 1;
    return 0;
}

// --- ALGORITMO OTTIMIZZATO (DFS / Tarjan) ---
// Variabili globali per semplicità nella ricorsione
int timer;
int minBridgeWeight = INT_MAX;

// Algoritmo ricorsivo
void AP_Bridge_Util(struct Graph* g, int u, int visited[], int disc[], 
                    int low[], int parent[], int isAP[]) {
    int children = 0;
    visited[u] = 1;
    
    // Inizializza discovery time e low value
    disc[u] = low[u] = ++timer;

    struct Node* temp = g->adj[u];
    while (temp) {
        int v = temp->dest; 

        // Se v è il genitore di u, ignoralo
        if (v == parent[u]) {
            temp = temp->next;
            continue;
        }

        if (visited[v]) {
            // (u, v) è un Back-Edge
            // Aggiorna low[u] considerando disc[v]
            if (disc[v] < low[u])
                low[u] = disc[v];
        } else {
            // (u, v) è un Tree-Edge
            children++;
            parent[v] = u;
            
            AP_Bridge_Util(g, v, visited, disc, low, parent, isAP);

            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            if (low[v] < low[u])
                low[u] = low[v];

            // --- CHECK PUNTO DI ARTICOLAZIONE ---
            // 1. u è radice e ha più di 1 figlio nel DFS tree
            if (parent[u] == -1 && children > 1)
                isAP[u] = 1;
            
            // 2. u non è radice e low[v] >= disc[u]
            if (parent[u] != -1 && low[v] >= disc[u])
                isAP[u] = 1;

            // --- CHECK PONTE (BRIDGE) ---
            if (low[v] > disc[u]) {
                printf(" -> Ponte trovato: %d - %d (Peso: %d)\n", u, v, temp->weight);
                if (temp->weight < minBridgeWeight) {
                    minBridgeWeight = temp->weight;
                }
            }
        }
        temp = temp->next;
    }
}

// Funzione wrapper principale
void findAPs_and_Bridges(struct Graph* g) {
    int* visited = (int*)calloc(g->V, sizeof(int));
    int* disc = (int*)calloc(g->V, sizeof(int));
    int* low = (int*)calloc(g->V, sizeof(int));
    int* parent = (int*)malloc(g->V * sizeof(int));
    int* isAP = (int*)calloc(g->V, sizeof(int)); // Boolean array

    for (int i = 0; i < g->V; i++) {
        parent[i] = -1;
        visited[i] = 0;
        isAP[i] = 0;
    }

    timer = 0;
    minBridgeWeight = INT_MAX;

    printf("\n--- Analisi Ponti e Articolazioni (Algoritmo Ottimizzato) ---\n");
    
    // Gestione grafi non connessi: loop su tutti i nodi
    for (int i = 0; i < g->V; i++) {
        if (visited[i] == 0)
            AP_Bridge_Util(g, i, visited, disc, low, parent, isAP);
    }

    printf("Punti di Articolazione trovati: ");
    int ap_found = 0;
    for (int i = 0; i < g->V; i++) {
        if (isAP[i] == 1) {
            printf("%d ", i);
            ap_found = 1;
        }
    }
    if (!ap_found) printf("Nessuno");
    printf("\n");

    if (minBridgeWeight == INT_MAX)
        printf("Peso minimo bridge: N/A (Nessun ponte)\n");
    else
        printf("Peso minimo bridge: %d\n", minBridgeWeight);

    free(visited); free(disc); free(low); free(parent); free(isAP);
}

int main() {
    // Esempio: Grafo "Farfallino" + una coda
    // 0---1
    // | \ |
    // 3---2---4---5
    //         | /
    //         6
    
    int V = 7;
    struct Graph* g = createGraph(V);
    
    // Triangolo sinistro (pesi casuali)
    addEdge(g, 0, 1, 10);
    addEdge(g, 1, 2, 10);
    addEdge(g, 2, 0, 10);
    addEdge(g, 0, 3, 10); // Diagonale extra
    addEdge(g, 3, 2, 10);

    // Ponte verso 4
    addEdge(g, 2, 4, 5); // Questo sarà un ponte (peso 5)

    // Triangolo destro
    addEdge(g, 4, 5, 20);
    addEdge(g, 5, 6, 20);
    addEdge(g, 6, 4, 20);

    // 1. Test metodo Naïve su nodo specifico
    int testNode = 2; // Nodo centrale, dovrebbe essere AP
    printf("Verifica se nodo %d e' AP (Naive): %s\n", 
           testNode, isSpecificVertexAP(g, testNode) ? "SI" : "NO");
    
    testNode = 0; // Nodo in un ciclo, non dovrebbe essere AP
    printf("Verifica se nodo %d e' AP (Naive): %s\n", 
           testNode, isSpecificVertexAP(g, testNode) ? "SI" : "NO");

    // 2. Algoritmo Ottimizzato
    findAPs_and_Bridges(g);

    return 0;
}