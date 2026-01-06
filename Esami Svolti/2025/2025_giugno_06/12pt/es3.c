#include <stdio.h>
#include <stdlib.h>

/* ========================================================================= */
/* DEFINIZIONE ADT GRAFO (Liste di Adiacenza)                                */
/* ========================================================================= */

/*
 * Struttura del Nodo della lista di adiacenza.
 * Ogni nodo rappresenta un arco uscente verso il vertice 'v'.
 */
typedef struct node *link;
struct node {
    int v;      /* Indice del vertice destinazione */
    link next;  /* Puntatore al prossimo nodo nella lista */
};

/*
 * Struttura del Grafo.
 * Utilizziamo un vettore di liste (ladj).
 * V: numero di vertici.
 * E: numero di archi.
 */
struct graph {
    int V;
    int E;
    link *ladj; /* Vettore di puntatori: ladj[i] punta alla testa della lista del vertice i */
};
typedef struct graph *Graph;

/* ========================================================================= */
/* FUNZIONI DI UTILITÀ PER IL TEST (Creazione e Distruzione)                 */
/* ========================================================================= */

/* Crea un nuovo nodo per la lista */
link NEW(int v, link next) {
    link x = malloc(sizeof *x);
    if (x == NULL) return NULL; /* Controllo malloc sempre buona norma */
    x->v = v;
    x->next = next;
    return x;
}

/* Inizializza un grafo con V vertici e 0 archi */
Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof *G);
    G->V = V;
    G->E = 0;
    /* Allocazione del vettore di puntatori (le teste delle liste) */
    G->ladj = malloc(V * sizeof(link));
    int i;
    for (i = 0; i < V; i++)
        G->ladj[i] = NULL; /* Inizialmente tutte le liste sono vuote */
    return G;
}

/* Inserisce un arco orientato da id_sorgente a id_dest */
void GRAPHinsertE(Graph G, int id_sorgente, int id_dest) {
    /* Inserimento in testa alla lista di adiacenza di id_sorgente */
    G->ladj[id_sorgente] = NEW(id_dest, G->ladj[id_sorgente]);
    G->E++;
}

/* Libera tutta la memoria del grafo */
void GRAPHfree(Graph G) {
    int v;
    link t, next;
    /* Per ogni vertice, scorriamo la sua lista e liberiamo i nodi */
    for (v = 0; v < G->V; v++) {
        for (t = G->ladj[v]; t != NULL; t = next) {
            next = t->next; /* Salviamo il prossimo prima di fare free */
            free(t);        /* Rilascio memoria nodo */
        }
    }
    free(G->ladj); /* Rilascio vettore delle teste */
    free(G);       /* Rilascio struttura grafo */
}

/* ========================================================================= */
/* SOLUZIONE DELL'ESERCIZIO                                                  */
/* ========================================================================= */

/* * Funzione Helper (Ausiliaria)
 * Scopo: Controllare se esiste un arco specifico da 'src' a 'dest'.
 * Necessaria perché con le liste di adiacenza non abbiamo accesso diretto O(1) come le matrici.
 */
static int checkEdge(Graph g, int src, int dest) {
    link t;
    /* Scorriamo la lista di adiacenza del vertice 'src' */
    for (t = g->ladj[src]; t != NULL; t = t->next) {
        if (t->v == dest) 
            return 1; /* Trovato! */
    }
    return 0; /* Non trovato dopo aver scansionato tutta la lista */
}

/*
 * Funzione: GRAPHcheckSimmetry
 * -------------------------------------------------------------------------
 * Scopo:       Verifica due condizioni:
 * 1. Assenza di cappi (self-loops): nessun arco v->v.
 * 2. Simmetria: se esiste v->w, deve esistere w->v.
 * Parametri:   
 * - g: il grafo da analizzare.
 * Ritorna:     1 (Vero) se il grafo è simmetrico e senza cappi, 0 (Falso) altrimenti.
 * -------------------------------------------------------------------------
 */
int GRAPHcheckSimmetry(Graph g) {
    int v, w;
    link t;

    /* --- 1. ITERAZIONE SUI VERTICI --- */
    /* Dobbiamo esaminare ogni vertice del grafo per controllare i suoi archi uscenti */
    for (v = 0; v < g->V; v++) {

        /* --- 2. ITERAZIONE SULLE LISTE DI ADIACENZA --- */
        /* * Accediamo a g->ladj[v], che è il puntatore alla testa della lista del vertice v.
         * Usiamo il puntatore ausiliario 't' per scorrere la lista finché non è NULL.
         * t = t->next sposta il puntatore al nodo successivo.
         */
        for (t = g->ladj[v]; t != NULL; t = t->next) {
            
            w = t->v; /* w è il vertice destinazione dell'arco v->w */

            /* --- 3. CONTROLLO CAPPI (SELF-LOOPS) --- */
            /* Se la destinazione è uguale alla sorgente, è un cappio. */
            if (v == w) {
                /* Trovato un cappio: viola la condizione richiesta */
                return 0; 
            }

            /* --- 4. CONTROLLO SIMMETRIA --- */
            /* * Abbiamo trovato un arco v->w.
             * Ora dobbiamo verificare se esiste l'arco inverso w->v.
             * Chiamiamo la funzione helper checkEdge.
             */
            if (checkEdge(g, w, v) == 0) {
                /* L'arco inverso NON esiste. Il grafo non è simmetrico. */
                return 0;
            }
        }
    }

    /* --- 5. CONCLUSIONE --- */
    /* * Se siamo arrivati qui, significa che abbiamo controllato TUTTI gli archi
     * e nessuno ha violato le regole. Il grafo è valido.
     */
    return 1;
}

/* ========================================================================= */
/* MAIN DI TEST                                                              */
/* ========================================================================= */

int main() {
    printf("--- LEZIONE: Verifica Simmetria Grafo ---\n");

    /* --- TEST 1: Grafo Simmetrico Corretto --- */
    /* Creiamo archi: 0->1 e 1->0. È simmetrico. */
    Graph g1 = GRAPHinit(3);
    GRAPHinsertE(g1, 0, 1);
    GRAPHinsertE(g1, 1, 0); 
    
    printf("\nTest 1 (0->1, 1->0): Atteso 1 (Vero). Risultato: %d\n", GRAPHcheckSimmetry(g1));
    GRAPHfree(g1);

    /* --- TEST 2: Grafo Asimmetrico --- */
    /* Creiamo arco: 0->1 ma NON 1->0. */
    Graph g2 = GRAPHinit(3);
    GRAPHinsertE(g2, 0, 1);
    
    printf("Test 2 (Solo 0->1):  Atteso 0 (Falso). Risultato: %d\n", GRAPHcheckSimmetry(g2));
    GRAPHfree(g2);

    /* --- TEST 3: Grafo con Cappio --- */
    /* Creiamo arco: 0->0. Viola la regola dei cappi. */
    Graph g3 = GRAPHinit(3);
    GRAPHinsertE(g3, 0, 0); 
    /* Nota: aggiungiamo anche un arco simmetrico valido altrove per confondere le acque */
    GRAPHinsertE(g3, 1, 2);
    GRAPHinsertE(g3, 2, 1);

    printf("Test 3 (Cappio 0->0): Atteso 0 (Falso). Risultato: %d\n", GRAPHcheckSimmetry(g3));
    GRAPHfree(g3);

    return 0;
}