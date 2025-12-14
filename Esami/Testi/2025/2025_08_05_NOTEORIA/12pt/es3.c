#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE STRUTTURE DATI (ADT Graph)
 * ======================================================================================
 * Anche se il testo dice "non è necessario definire la struttura", per compilare
 * e testare il codice dobbiamo sapere come è fatto il grafo (Liste di Adiacenza).
 */

// Nodo della lista di adiacenza
typedef struct node {
    int v;              // Vertice di destinazione
    struct node *next;  // Prossimo nodo nella lista
} Node;

// Struttura principale del Grafo
struct graph {
    int V;          // Numero di vertici
    int E;          // Numero di archi
    Node **adj;     // Vettore di puntatori alle liste di adiacenza
                    // adj[u] punta alla testa della lista dei vicini di u
};
typedef struct graph *Graph;

/*
 * ======================================================================================
 * FUNZIONE: GRAPHcheckHam
 * ======================================================================================
 * Scopo:
 * Verificare se la sequenza di vertici in 'p' rappresenta un cammino Hamiltoniano.
 * * Parametri:
 * - Graph g: Il grafo (rappresentato con liste di adiacenza).
 * - int *p: Array di interi contenente la sequenza di vertici candidata.
 * - int n: Dimensione dell'array 'p'.
 * * Concetti Chiave:
 * - Verifica dimensione: Un cammino Hamiltoniano deve avere lunghezza pari a V vertici.
 * - Unicità: Ogni vertice deve apparire una sola volta (usiamo un array di flag).
 * - Adiacenza: Per ogni coppia (p[i], p[i+1]), deve esistere l'arco nel grafo.
 * * Ritorna:
 * - 1 (Vero) se è un cammino Hamiltoniano, 0 (Falso) altrimenti.
 */
int GRAPHcheckHam(Graph g, int *p, int n) {
    
    /* --- 1. CONTROLLO DIMENSIONE BASE --- */
    // Definizione: Un cammino Hamiltoniano visita TUTTI i vertici una sola volta.
    // Quindi la lunghezza del cammino deve essere esattamente uguale al numero di vertici del grafo.
    if (g == NULL || p == NULL) return 0;
    if (n != g->V) {
        return 0; // Se i vertici nel cammino sono meno o più di V, non è Hamiltoniano.
    }

    /* --- 2. CONTROLLO UNICITÀ (SET CHECK) --- */
    // Dobbiamo assicurarci che non ci siano duplicati nel cammino p.
    // Usiamo un array di supporto 'visited' come tabella hash diretta (Direct Addressing).
    // Usiamo calloc per inizializzare tutto a 0 (falso).
    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) {
        printf("Errore malloc\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        int vertice = p[i];
        
        // Controllo validità indice (paranoia check)
        if (vertice < 0 || vertice >= g->V) {
            free(visited);
            return 0;
        }

        // Se abbiamo già visto questo vertice, il cammino non è semplice (ha un ciclo),
        // oppure è una lista non valida di vertici.
        if (visited[vertice] == 1) {
            free(visited); // Importante: liberare prima di uscire!
            return 0;
        }
        
        visited[vertice] = 1; // Marchiamo come visitato
    }
    
    // Se siamo qui, 'p' è una permutazione valida dei vertici. Possiamo liberare l'array ausiliario.
    free(visited);

    /* --- 3. CONTROLLO CONNETTIVITÀ (EDGE CHECK) --- */
    // Scorriamo il cammino fino al PENULTIMO elemento.
    // Per ogni vertice p[i], verifichiamo se esiste un arco verso p[i+1].
    for (int i = 0; i < n - 1; i++) {
        int u = p[i];     // Sorgente
        int v = p[i+1];   // Destinazione attesa

        // Cerchiamo 'v' nella lista di adiacenza di 'u'
        int trovato = 0;
        Node *curr = g->adj[u]; // Accediamo alla testa della lista di u

        while (curr != NULL) {
            if (curr->v == v) {
                trovato = 1;
                break; // Trovato l'arco u -> v
            }
            curr = curr->next; // Scorrimento lista concatenata
        }

        // Se la lista finisce e non abbiamo trovato 'v', il cammino è interrotto.
        if (!trovato) {
            return 0;
        }
    }

    /* --- 4. SUCCESSO --- */
    // Se passiamo tutti i controlli, è un cammino Hamiltoniano.
    return 1;
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO (PER IL MAIN DI TEST)
 * ======================================================================================
 */

// Crea un grafo vuoto
Graph graphInit(int V) {
    Graph g = malloc(sizeof(*g));
    g->V = V;
    g->E = 0;
    g->adj = malloc(V * sizeof(Node*));
    for (int i = 0; i < V; i++) g->adj[i] = NULL;
    return g;
}

// Aggiunge un arco orientato u -> v (inserimento in testa alla lista)
void graphInsertE(Graph g, int u, int v) {
    Node *new_node = malloc(sizeof(Node));
    new_node->v = v;
    new_node->next = g->adj[u];
    g->adj[u] = new_node;
    g->E++;
}

// Pulisce la memoria del grafo
void graphFree(Graph g) {
    for (int i = 0; i < g->V; i++) {
        Node *curr = g->adj[i];
        while (curr != NULL) {
            Node *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Creiamo un grafo semplice a 4 nodi (0, 1, 2, 3)
    // 0 -> 1 -> 2 -> 3 -> 0 (un ciclo quadrato)
    // Aggiungiamo anche una diagonale 0 -> 2
    int V = 4;
    Graph g = graphInit(V);
    
    graphInsertE(g, 0, 1);
    graphInsertE(g, 1, 2);
    graphInsertE(g, 2, 3);
    graphInsertE(g, 3, 0);
    graphInsertE(g, 0, 2); // Diagonale

    printf("Grafo creato con %d vertici.\n", V);

    /* --- TEST 1: Cammino Hamiltoniano Valido --- */
    // Sequenza: 0 -> 1 -> 2 -> 3
    // Copre tutti i nodi una volta e gli archi esistono.
    int path1[] = {0, 1, 2, 3};
    printf("Test 1 (0-1-2-3): %s (Atteso: 1)\n", 
           GRAPHcheckHam(g, path1, 4) ? "SI" : "NO");

    /* --- TEST 2: Cammino Valido ma non Hamiltoniano (troppo corto) --- */
    // Sequenza: 0 -> 1 -> 2
    // Gli archi esistono, ma manca il vertice 3.
    int path2[] = {0, 1, 2};
    printf("Test 2 (0-1-2):   %s (Atteso: 0)\n", 
           GRAPHcheckHam(g, path2, 3) ? "SI" : "NO");

    /* --- TEST 3: Cammino con arco inesistente --- */
    // Sequenza: 0 -> 3 -> 2 -> 1
    // L'arco 0 -> 3 non esiste (esiste 3 -> 0). È orientato!
    int path3[] = {0, 3, 2, 1};
    printf("Test 3 (0-3-2-1): %s (Atteso: 0)\n", 
           GRAPHcheckHam(g, path3, 4) ? "SI" : "NO");

    /* --- TEST 4: Cammino con vertici ripetuti --- */
    // Sequenza: 0 -> 1 -> 2 -> 0
    // Lunghezza 4, ma il 3 manca e lo 0 c'è due volte.
    int path4[] = {0, 1, 2, 0};
    printf("Test 4 (0-1-2-0): %s (Atteso: 0)\n", 
           GRAPHcheckHam(g, path4, 4) ? "SI" : "NO");

    // Pulizia
    graphFree(g);
    return 0;
}