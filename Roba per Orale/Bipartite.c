#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// --- STRUTTURE DATI ---
struct Node {
    int dest;
    struct Node* next;
};

struct Graph {
    int V;
    struct Node** adj;
};

// Funzioni base per il grafo
struct Graph* createGraph(int V) {
    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    g->V = V;
    g->adj = (struct Node**)malloc(V * sizeof(struct Node*));
    for (int i = 0; i < V; ++i) g->adj[i] = NULL;
    return g;
}

void addEdge(struct Graph* g, int src, int dest) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->dest = dest; n->next = g->adj[src]; g->adj[src] = n;
    
    n = (struct Node*)malloc(sizeof(struct Node)); // Grafo non orientato
    n->dest = src; n->next = g->adj[dest]; g->adj[dest] = n;
}

// ============================================================
// PARTE 1: APPROCCIO STANDARD EFFICIENTE (BFS) - O(V+E)
// ============================================================

int isBipartiteBFS(struct Graph* g, int src, int color[]) {
    color[src] = 1; // Assegna primo colore (1)
    
    // Coda semplice per BFS (array statico per semplicità)
    int queue[100];
    int front = 0, rear = 0;
    queue[rear++] = src;

    while (front < rear) {
        int u = queue[front++];

        struct Node* temp = g->adj[u];
        while (temp) {
            int v = temp->dest;
            
            // Se v non è colorato, assegnagli il colore opposto a u
            if (color[v] == -1) {
                color[v] = 1 - color[u]; // Se u è 1 -> v diventa 0, e viceversa
                queue[rear++] = v;
            }
            // Se v è già colorato con lo stesso colore di u -> CONFLITTO
            else if (color[v] == color[u]) {
                return 0; // Non bipartito
            }
            temp = temp->next;
        }
    }
    return 1;
}

void checkBipartiteStandard(struct Graph* g) {
    int* color = (int*)malloc(g->V * sizeof(int));
    for(int i=0; i<g->V; i++) color[i] = -1;

    int possible = 1;
    // Gestione componenti sconnesse
    for (int i = 0; i < g->V; i++) {
        if (color[i] == -1) {
            if (!isBipartiteBFS(g, i, color)) {
                possible = 0;
                break;
            }
        }
    }

    printf("\n[Standard BFS] Il grafo %s bipartito.\n", possible ? "E'" : "NON e'");
    free(color);
}

// ============================================================
// PARTE 2: APPROCCIO COMBINATORIO (RICHIESTA SPECIFICA)
// Genera partizioni usando Disposizioni con Ripetizione
// ============================================================

// Variabili globali per tracciare la soluzione migliore (minima differenza)
int minDiff = 999999;
int bestPartitionFound = 0;
int* bestColors = NULL; // Per salvare la configurazione migliore

// Funzione di verifica: Controlla se una specifica assegnazione di colori è valida
// Complessità: O(V + E) per ogni verifica
int isValidPartition(struct Graph* g, int colors[]) {
    for (int u = 0; u < g->V; u++) {
        struct Node* temp = g->adj[u];
        while (temp) {
            int v = temp->dest;
            // Se due nodi collegati hanno lo stesso colore (insieme), partizione non valida
            if (colors[u] == colors[v]) return 0;
            temp = temp->next;
        }
    }
    return 1;
}

// Funzione Ricorsiva (Backtracking/Brute Force)
// Modello: Disposizioni con ripetizione di {0, 1} su V posizioni (2^V casi)
void generatePartitionsRecursive(struct Graph* g, int colors[], int k) {
    // Caso base: abbiamo assegnato un colore (insieme) a tutti i nodi
    if (k == g->V) {
        // 1. Verifica se la partizione è valida (proprietà bipartita)
        if (isValidPartition(g, colors)) {
            
            // Calcola cardinalità dei due insiemi
            int countSet0 = 0, countSet1 = 0;
            for(int i=0; i<g->V; i++) {
                if(colors[i] == 0) countSet0++;
                else countSet1++;
            }
            int diff = abs(countSet0 - countSet1);

            printf(" -> Partizione valida trovata: ");
            printf("Set A: %d nodi, Set B: %d nodi (Diff: %d)\n", countSet0, countSet1, diff);

            // 2. Cerca la partizione "migliore" (minima differenza)
            if (diff < minDiff) {
                minDiff = diff;
                bestPartitionFound = 1;
                for(int i=0; i<g->V; i++) bestColors[i] = colors[i];
            }
        }
        return;
    }

    // Ricorsione: Prova ad assegnare il nodo k all'insieme 0
    colors[k] = 0;
    generatePartitionsRecursive(g, colors, k + 1);

    // Ricorsione: Prova ad assegnare il nodo k all'insieme 1
    colors[k] = 1;
    generatePartitionsRecursive(g, colors, k + 1);
}

void solveCombinatorial(struct Graph* g) {
    int* colors = (int*)malloc(g->V * sizeof(int));
    bestColors = (int*)malloc(g->V * sizeof(int));
    minDiff = 999999;
    bestPartitionFound = 0;

    printf("\n[Combinatorio] Generazione di tutte le 2^%d partizioni...\n", g->V);
    
    // Avvia la ricorsione dal nodo 0
    generatePartitionsRecursive(g, colors, 0);

    if (bestPartitionFound) {
        printf("\nRISULTATO OTTIMO (Minima differenza): Diff = %d\n", minDiff);
        printf("Insieme 0: { ");
        for(int i=0; i<g->V; i++) if(bestColors[i]==0) printf("%d ", i);
        printf("}\nInsieme 1: { ");
        for(int i=0; i<g->V; i++) if(bestColors[i]==1) printf("%d ", i);
        printf("}\n");
    } else {
        printf("\nNessuna partizione valida trovata (Il grafo NON e' bipartito).\n");
    }

    free(colors);
    free(bestColors);
}

// --- MAIN ---
int main() {
    // Esempio: Un quadrato (0-1-2-3-0) -> Bipartito
    // Aggiungiamo un nodo 4 collegato al 2 -> Bipartito
    // 0 -- 1
    // |    |
    // 3 -- 2 -- 4
    
    int V = 5;
    struct Graph* g = createGraph(V);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 3, 0);
    addEdge(g, 2, 4);

    // Test 1: Metodo BFS Efficiente
    checkBipartiteStandard(g);

    // Test 2: Metodo Combinatorio (Tutte le partizioni)
    solveCombinatorial(g);

    // Esempio 2: Triangolo (Ciclo dispari) -> NON Bipartito
    printf("\n--- Test con Triangolo ---\n");
    struct Graph* g2 = createGraph(3);
    addEdge(g2, 0, 1);
    addEdge(g2, 1, 2);
    addEdge(g2, 2, 0);

    checkBipartiteStandard(g2);
    solveCombinatorial(g2); // Non stamperà nulla di valido

    return 0;
}