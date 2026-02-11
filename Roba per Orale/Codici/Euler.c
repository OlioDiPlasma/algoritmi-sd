#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUTTURE DATI (Matrice di Adiacenza) ---
// Usiamo la matrice qui perché rimuovere gli archi è O(1),
// rendendo l'algoritmo più facile da seguire didatticamente rispetto alle liste.

#define MAX 100

struct Graph {
    int V; // Numero vertici
    int adj[MAX][MAX]; // Matrice adiacenza: adj[u][v] = 1 se esiste arco, 0 altrimenti
    int degree[MAX];   // Array per memorizzare il grado di ogni vertice
};

// Inizializzazione Grafo
void initGraph(struct Graph* g, int V) {
    g->V = V;
    for (int i = 0; i < V; i++) {
        g->degree[i] = 0;
        for (int j = 0; j < V; j++) {
            g->adj[i][j] = 0;
        }
    }
}

// Aggiunge arco non orientato
void addEdge(struct Graph* g, int u, int v) {
    g->adj[u][v] = 1; // O incrementare se fosse multigrafo
    g->adj[v][u] = 1;
    g->degree[u]++;
    g->degree[v]++;
}

// --- 1. FUNZIONI DI CONTROLLO (Connectivity & Degrees) ---

// DFS per verificare la connettività
void DFS_Connectivity(struct Graph* g, int u, int visited[]) {
    visited[u] = 1;
    for (int v = 0; v < g->V; v++) {
        if (g->adj[u][v] && !visited[v]) {
            DFS_Connectivity(g, v, visited);
        }
    }
}

// Verifica se il grafo è connesso (ignorando vertici isolati di grado 0)
int isConnected(struct Graph* g) {
    int visited[MAX] = {0};
    int i;

    // Trova un vertice con grado > 0 per iniziare la DFS
    for (i = 0; i < g->V; i++)
        if (g->degree[i] > 0)
            break;

    // Se non ci sono archi, è tecnicamente euleriano (cammino vuoto)
    if (i == g->V) return 1;

    // Lancia DFS dal primo vertice non isolato
    DFS_Connectivity(g, i, visited);

    // Controlla se tutti i vertici con grado > 0 sono stati visitati
    for (i = 0; i < g->V; i++)
        if (g->degree[i] > 0 && visited[i] == 0)
            return 0; // Trovato un vertice con archi ma non raggiungibile

    return 1;
}

/*
 * Restituisce:
 * 0 -> Non Euleriano
 * 1 -> Cammino di Eulero (Semi-Euleriano)
 * 2 -> Ciclo di Eulero (Euleriano)
 */
int checkEuler(struct Graph* g) {
    // Passo 1: Verifica connettività
    if (!isConnected(g)) return 0;

    // Passo 2: Conta vertici di grado dispari
    int odd = 0;
    for (int i = 0; i < g->V; i++)
        if (g->degree[i] % 2 != 0)
            odd++;

    // Passo 3: Applica Teorema
    if (odd > 2) return 0;          // Impossibile
    if (odd == 2) return 1;         // Cammino (inizia in un dispari, finisce nell'altro)
    if (odd == 0) return 2;         // Ciclo
    
    return 0; // Caso odd=1 impossibile in grafi non orientati
}

// --- 2. STAMPA DEL CAMMINO (DFS con rimozione archi) ---

// Algoritmo ricorsivo per stampare il percorso
void printEulerUtil(struct Graph* g, int u) {
    // Itera su tutti i vertici adiacenti
    for (int v = 0; v < g->V; v++) {
        // Se c'è un arco e "conviene" attraversarlo
        if (g->adj[u][v]) {
            // Rimuovi l'arco (brucia il ponte) per non ripassarci
            g->adj[u][v] = 0;
            g->adj[v][u] = 0;
            g->degree[u]--;
            g->degree[v]--;
            
            // Stampa e vai avanti
            // Nota: in una stampa topologica corretta, si stamperebbe in post-order o usando uno stack,
            // ma per dimostrazione semplice stampiamo l'arco attraversato.
            printf("%d -> %d ", u, v);
            
            printEulerUtil(g, v);
        }
    }
}

void findEulerPath(struct Graph* g) {
    int res = checkEuler(g);
    if (res == 0) {
        printf("Il grafo NON e' Euleriano.\n");
        return;
    }

    printf("Il grafo ammette un %s.\nPercorso: ", (res == 2) ? "CICLO" : "CAMMINO");

    // Trova il punto di partenza
    int startNode = 0;
    
    // Se è un Cammino (res=1), DEVE iniziare da un nodo dispari
    if (res == 1) {
        for (int i = 0; i < g->V; i++) {
            if (g->degree[i] % 2 != 0) {
                startNode = i;
                break;
            }
        }
    } 
    // Se è un Ciclo (res=2), può iniziare da qualsiasi nodo con grado > 0
    else {
        for (int i = 0; i < g->V; i++) {
            if (g->degree[i] > 0) {
                startNode = i;
                break;
            }
        }
    }

    printEulerUtil(g, startNode);
    printf("FINE\n");
}

int main() {
    struct Graph g1;
    initGraph(&g1, 5);
    
    // Esempio 1: Grafo Euleriano (Ciclo)
    // Quadrato 0-1-2-3-0 con diagonale 0-2 (No, aspetta, facciamo un ciclo pulito)
    // 0 -- 1
    // |    |
    // 3 -- 2
    // + diagonali 0-2 e 1-3 renderebbe tutti i gradi 3 (dispari) -> Non Euleriano
    // Facciamo un "8": due triangoli che condividono vertice 0
    // 1--0--3
    // | /    \ |
    // 2       4
    
    printf("--- Test 1 (Farfallino/Clessidra) ---\n");
    addEdge(&g1, 0, 1);
    addEdge(&g1, 1, 2);
    addEdge(&g1, 2, 0); // Triangolo sx
    addEdge(&g1, 0, 3);
    addEdge(&g1, 3, 4);
    addEdge(&g1, 4, 0); // Triangolo dx
    
    // Gradi: 1(2), 2(2), 3(2), 4(2), 0(4). Tutti pari.
    findEulerPath(&g1);

    // Esempio 2: Cammino Euleriano
    printf("\n--- Test 2 (Aggiungo un 'ponte' 2-4) ---\n");
    struct Graph g2;
    initGraph(&g2, 5);
    // Ricostruisco g1
    addEdge(&g2, 0, 1); addEdge(&g2, 1, 2); addEdge(&g2, 2, 0);
    addEdge(&g2, 0, 3); addEdge(&g2, 3, 4); addEdge(&g2, 4, 0);
    // Aggiungo arco tra 2 e 4
    addEdge(&g2, 2, 4);
    
    // Ora nodo 2 ha grado 3 (dispari), nodo 4 ha grado 3 (dispari).
    // Nodo 0 ha grado 4 (pari). Altri pari.
    // Esattamente 2 dispari -> Cammino.
    findEulerPath(&g2);

    return 0;
}