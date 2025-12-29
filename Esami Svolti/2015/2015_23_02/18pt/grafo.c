#include <stdlib.h>
#include <string.h>
#include "grafo.h"

/* --- 1. STRUTTURE INTERNE --- */
/* Nodo della lista di adiacenza */
typedef struct node {
    int v;              /* Indice vertice destinazione */
    int wt;             /* Peso dell'arco */
    struct node *next;  /* Puntatore al prossimo arco */
} Node;

struct graph {
    int V;          /* Numero vertici */
    int E;          /* Numero archi */
    Node **ladj;    /* Array di puntatori a liste (Lista di Adiacenza) */
    ST st;          /* Riferimento alla Symbol Table */
};

/* --- 2. FUNZIONI DI SUPPORTO LISTE --- */
/* Inserimento in testa alla lista di adiacenza */
static Node* NEWnode(int v, int wt, Node *next) {
    Node *x = malloc(sizeof(Node));
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

/* --- 3. INIZIALIZZAZIONE E CARICAMENTO --- */

Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    /* Allocazione array di puntatori a Node, inizializzati a NULL con calloc */
    G->ladj = calloc(V, sizeof(Node *)); 
    return G;
}

void GRAPHfree(Graph G) {
    int i;
    Node *tmp, *t;
    for (i = 0; i < G->V; i++) {
        for (tmp = G->ladj[i]; tmp != NULL; tmp = t) {
            t = tmp->next;
            free(tmp);
        }
    }
    free(G->ladj);
    /* La ST viene liberata dal main solitamente, o qui se di proprietà del grafo */
    free(G);
}

Graph GRAPHload(FILE *f, ST st) {
    char id1[21], id2[21];
    int wt, u, v;
    Graph G;
    
    /* Nota: Il testo dice che non conosciamo V. 
       Strategia: Leggere file una volta per contare vertici (popolare ST),
       creare Grafo, rileggere file per archi. */

    /* PASSO 1: Popolamento ST */
    while (fscanf(f, "%s %d %s", id1, &wt, id2) == 3) {
        STinsert(st, id1);
        STinsert(st, id2);
    }

    /* Creazione Grafo con dimensione esatta */
    G = GRAPHinit(STcount(st));
    G->st = st;

    /* PASSO 2: Caricamento Archi */
    rewind(f); /* Torna all'inizio del file */
    while (fscanf(f, "%s %d %s", id1, &wt, id2) == 3) {
        u = STsearch(st, id1);
        v = STsearch(st, id2);
        /* Inserimento arco orientato u -> v */
        G->ladj[u] = NEWnode(v, wt, G->ladj[u]);
        G->E++;
    }

    return G;
}

/* Wrapper per comodità */
int GRAPHgetIndex(Graph G, char *name) {
    return STsearch(G->st, name);
}

char* GRAPHgetName(Graph G, int index) {
    return STsearchByIndex(G->st, index);
}

int GRAPHgetV(Graph G) {
    return G->V;
}

/* --- 4. FUNZIONI PER ALGORITMI --- */

/* Verifica se esiste arco u->v e ritorna il peso */
int GRAPHcheckEdge(Graph G, int u, int v) {
    Node *t;
    for (t = G->ladj[u]; t != NULL; t = t->next) {
        if (t->v == v) return t->wt;
    }
    return 0; /* 0 o -1 se pesi positivi */
}

/* --- 5. LOGICA OTTIMIZZAZIONE (Recursive) --- */
/* Variabili globali "statiche" per mantenere stato tra le ricorsioni in questo modulo */
static int maxWt;
static int *bestPath; /* Array di indici */
static int bestLen;

/* * Funzione ricorsiva (DFS).
 * u: nodo corrente
 * dest: destinazione
 * currentWt: peso accumulato
 * path: array del percorso corrente
 * len: lunghezza percorso corrente
 * visits: array conteggio visite per ogni nodo (per gestire vincolo k)
 * k: max nodi riattraversabili
 * p: max riattraversamenti totali
 * k_cnt: contatore nodi riattraversati finora (>1 visita)
 * p_cnt: contatore riattraversamenti totali finora
 */
void dfsBest(Graph G, int u, int dest, int currentWt, int *path, int len, int *visits, int k, int p, int k_cnt, int p_cnt) {
    
    /* Aggiungo nodo al path corrente */
    path[len] = u;
    
    /* BASE CASE: Destinazione Raggiunta */
    if (u == dest) {
        /* Se il peso è migliore, aggiorno la soluzione ottima */
        if (currentWt > maxWt) {
            maxWt = currentWt;
            bestLen = len + 1;
            int i;
            for(i=0; i<=len; i++) bestPath[i] = path[i];
        }
        /* Il testo dice: "una volta raggiunto il nodo di destinazione, il cammino è terminato".
           Quindi NON continuiamo la ricerca da qui (niente backtracking profondo oltre dest).
           Return immediato. */
        return;
    }

    /* RECURSIVE STEP: Esploro adiacenti */
    Node *t;
    for (t = G->ladj[u]; t != NULL; t = t->next) {
        int v = t->v;
        int w = t->wt;
        
        /* Controlliamo se possiamo andare in v */
        int new_k_cnt = k_cnt;
        int new_p_cnt = p_cnt;
        int allowed = 1;

        if (visits[v] > 0) {
            /* Sto riattraversando un nodo */
            new_p_cnt++; /* Incremento contatore riattraversamenti totali */
            
            if (visits[v] == 1) {
                /* È la prima volta che lo riattraverso -> diventa un nodo "multiplo" */
                new_k_cnt++;
            }
            
            /* Verifica vincoli */
            if (new_p_cnt > p || new_k_cnt > k) {
                allowed = 0;
            }
        }

        if (allowed) {
            visits[v]++; /* Marco visita */
            dfsBest(G, v, dest, currentWt + w, path, len + 1, visits, k, p, new_k_cnt, new_p_cnt);
            visits[v]--; /* Backtrack: Smarco visita */
        }
    }
}

void GRAPHpathBest(Graph G, int idS, int idD, int k, int p) {
    /* Allocazione strutture ausiliarie */
    int *visits = calloc(G->V, sizeof(int));
    int *path = malloc(G->V * (p + 2) * sizeof(int)); /* Stima dimensione max path */
    bestPath = malloc(G->V * (p + 2) * sizeof(int));
    
    maxWt = -1;
    bestLen = 0;

    /* Setup iniziale */
    visits[idS] = 1; /* Sorgente visitata 1 volta */
    
    /* Avvio Ricorsione */
    dfsBest(G, idS, idD, 0, path, 0, visits, k, p, 0, 0);

    /* Stampa Risultato */
    if (maxWt != -1) {
        printf("Cammino ottimo trovato (Peso %d): ", maxWt);
        int i;
        for (i = 0; i < bestLen; i++) {
            printf("%s ", GRAPHgetName(G, bestPath[i]));
        }
        printf("\n");
    } else {
        printf("Nessun cammino trovato con i vincoli dati.\n");
    }

    free(visits);
    free(path);
    free(bestPath);
}