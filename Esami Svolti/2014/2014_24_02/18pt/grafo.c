#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "st.h"

/*
 * STRUTTURE DATI INTERNE
 * Usiamo LISTE DI ADIACENZA perche' il grafo e' sparso e dobbiamo iterare sugli archi.
 */

/* Nodo della lista di adiacenza (rappresenta un arco uscente) */
typedef struct node {
    int v;              /* Indice del vertice destinazione */
    int wt;             /* Peso dell'arco */
    struct node *next;  /* Puntatore al prossimo arco */
} Node;

/* Struttura principale del Grafo */
struct grafo {
    int V;              /* Numero di vertici */
    int E;              /* Numero di archi */
    Node **ladj;        /* Array di puntatori alle liste di adiacenza (uno per vertice) */
    ST st;              /* Tabella di simboli per mappare Nomi <-> Indici */
};

/* Funzione helper per creare un nuovo nodo della lista */
static Node *NEWnode(int v, int wt, Node *next) {
    Node *x = malloc(sizeof(Node));
    if (x == NULL) {
        printf("Errore allocazione nodo\n");
        exit(1);
    }
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

/* --- 1. CARICAMENTO DEL GRAFO --- */
/*
 * Concetto chiave: Non sappiamo quanti vertici ci sono.
 * Strategia suggerita dal testo:
 * 1. Leggiamo tutto il file una volta solo per riempire la Symbol Table e contare i vertici unici.
 * 2. Alochiamo la struttura del grafo.
 * 3. Rileggiamo il file (rewind) per aggiungere gli archi usando gli indici ormai noti.
 */
Graph GRAPHload(FILE *fp) {
    Graph G;
    char label1[21], label2[21]; /* Buffer per stringhe max 20 char + terminatore */
    int peso, id1, id2;

    /* Creiamo una ST temporanea abbastanza grande (es. 100).
     * Se non basta andrebbe ridimensionata, ma per un esame va bene una stima. */
    ST st = STinit(100);

    /* PRIMA PASSATA: Popolazione ST */
    while (fscanf(fp, "%s %d %s", label1, &peso, label2) == 3) {
        STinsert(st, label1);
        STinsert(st, label2);
    }

    /* Allocazione Grafo */
    G = malloc(sizeof(*G));
    G->st = st;
    G->V = STcount(st);
    G->E = 0;
    /* Calloc inizializza a NULL i puntatori delle liste (importante!) */
    G->ladj = calloc(G->V, sizeof(Node *));

    /* SECONDA PASSATA: Creazione Archi */
    rewind(fp); /* Riavvolgiamo il file all'inizio */
    while (fscanf(fp, "%s %d %s", label1, &peso, label2) == 3) {
        id1 = STsearch(G->st, label1);
        id2 = STsearch(G->st, label2);

        /* Inserimento in testa alla lista di adiacenza di id1 */
        G->ladj[id1] = NEWnode(id2, peso, G->ladj[id1]);
        G->E++;
    }

    return G;
}

/* --- 2. DEALLOCAZIONE --- */
void GRAPHfree(Graph G) {
    int i;
    Node *p, *temp;
    /* Liberiamo ogni lista di adiacenza */
    for (i = 0; i < G->V; i++) {
        p = G->ladj[i];
        while (p != NULL) {
            temp = p;
            p = p->next;
            free(temp);
        }
    }
    free(G->ladj);
    STfree(G->st);
    free(G);
}

/* --- 3. VERIFICA REGOLARITA' --- */
/*
 * Definizione: Un grafo orientato è regolare se tutti i vertici hanno
 * lo stesso in_degree e out_degree e questi sono uguali tra loro.
 * Cioe': esiste k tale che per ogni nodo v, in_degree(v) == k E out_degree(v) == k.
 */
void GRAPHcheckRegular(Graph G) {
    int *in_degree = calloc(G->V, sizeof(int));
    int *out_degree = calloc(G->V, sizeof(int));
    int i;
    Node *x;
    int regular = 1; /* Flag booleano */
    int k = -1;      /* Valore di riferimento per il grado */

    /* Calcolo gradi */
    for (i = 0; i < G->V; i++) {
        for (x = G->ladj[i]; x != NULL; x = x->next) {
            out_degree[i]++;      /* Arco esce da i */
            in_degree[x->v]++;    /* Arco entra in x->v */
        }
    }

    /* Verifica condizione */
    for (i = 0; i < G->V; i++) {
        /* Se e' il primo nodo, impostiamo il valore k di riferimento */
        if (i == 0) {
            if (in_degree[i] != out_degree[i]) {
                regular = 0;
                break;
            }
            k = in_degree[i];
        } else {
            /* Per gli altri nodi, devono essere uguali a k */
            if (in_degree[i] != k || out_degree[i] != k) {
                regular = 0;
                break;
            }
        }
    }

    if (regular)
        printf("Il grafo e' REGOLARE (grado %d)\n", k);
    else
        printf("Il grafo NON e' regolare\n");

    /* Pulizia */
    free(in_degree);
    free(out_degree);
}

/* --- 4. RICERCA CAMMINO MASSIMO (DFS + BACKTRACKING) --- */

/* Variabili globali "di supporto" alla ricorsione per non passare troppi parametri */
static int maxWeightFound;
static int *bestPath; /* Vettore di indici dei nodi nel percorso migliore */
static int bestLen;   /* Lunghezza (numero nodi) del percorso migliore */

/*
 * Funzione ricorsiva DFS
 * v: vertice corrente
 * currentW: peso accumulato finora
 * visited: array dei visitati
 * path: array temporaneo del percorso corrente
 * depth: profondita' corrente (numero nodi nel path)
 */
void dfsMax(Graph G, int v, int currentW, int *visited, int *path, int depth) {
    Node *x;
    int isLeaf = 1; /* Flag per capire se non possiamo piu' avanzare */

    visited[v] = 1;
    path[depth] = v;

    /* Iteriamo sui vicini */
    for (x = G->ladj[v]; x != NULL; x = x->next) {
        if (!visited[x->v]) {
            isLeaf = 0;
            /* Passo ricorsivo: aggiungo peso dell'arco */
            dfsMax(G, x->v, currentW + x->wt, visited, path, depth + 1);
        }
    }

    /*
     * Se siamo bloccati (leaf) o abbiamo finito l'esplorazione dei vicini,
     * controlliamo se questo percorso e' il migliore trovato finora.
     * Nota: Il testo chiede "cammino semplice... somma pesi massima".
     * Il controllo va fatto ogni volta che "chiudiamo" o aggiorniamo un percorso valido.
     * In realta' conviene controllare SEMPRE prima di tornare indietro se currentW > max.
     */
    if (currentW > maxWeightFound) {
        maxWeightFound = currentW;
        bestLen = depth + 1;
        /* Copiamo il percorso corrente in quello "best" */
        for (int i = 0; i <= depth; i++) {
            bestPath[i] = path[i];
        }
    }

    /* BACKTRACKING: smarchiamo il nodo per permettere altri percorsi */
    visited[v] = 0;
}

void GRAPHpathMaxWeight(Graph G, char *startName) {
    int idStart = STsearch(G->st, startName);
    if (idStart == -1) {
        printf("Vertice '%s' non presente nel grafo.\n", startName);
        return;
    }

    /* Allocazioni per la ricorsione */
    int *visited = calloc(G->V, sizeof(int));
    int *path = malloc(G->V * sizeof(int));
    bestPath = malloc(G->V * sizeof(int)); /* Max V nodi in un cammino semplice */

    /* Reset variabili statiche */
    maxWeightFound = -1; /* O 0 */
    bestLen = 0;

    printf("Ricerca cammino massimo partendo da %s...\n", startName);
    
    /* Avvio DFS partendo da idStart, peso iniziale 0, depth 0 */
    dfsMax(G, idStart, 0, visited, path, 0);

    /* Stampa Risultati */
    if (maxWeightFound != -1 && bestLen > 1) {
        printf("Cammino a peso massimo trovato (Peso tot: %d):\n", maxWeightFound);
        /* * Dobbiamo stampare gli archi e i pesi.
         * Abbiamo la sequenza di nodi: N1 -> N2 -> N3...
         * Dobbiamo ritrovare il peso dell'arco N1->N2 per stamparlo.
         */
        for (int i = 0; i < bestLen - 1; i++) {
            int u = bestPath[i];
            int v = bestPath[i+1];
            int w = 0;
            /* Cerchiamo il peso w tra u e v nella lista di adiacenza */
            Node *curr = G->ladj[u];
            while(curr != NULL) {
                if (curr->v == v) {
                    w = curr->wt;
                    break;
                }
                curr = curr->next;
            }
            printf("(%s -> %s, peso: %d)\n", 
                   STsearchByIndex(G->st, u), 
                   STsearchByIndex(G->st, v), 
                   w);
        }
    } else {
        printf("Nessun arco uscente o cammino trovato da %s.\n", startName);
    }

    free(visited);
    free(path);
    free(bestPath);
}