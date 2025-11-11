// Include il file header che definisce i tipi (Graph, Item, ST, Edge)
#include "Graph.h"

// --- DEFINIZIONE STRUTTURA (Matrice di Adiacenza) ---
struct graph {
    int V;         // Numero di Vertici (nodi)
    int E;         // Numero di Archi
    int **madj;    // Puntatore alla MATRICE di adiacenza (int ** = array di array)
    ST tab;        // Symbol Table (per mappare nomi stringa a indici interi)
};

// --- FUNZIONI HELPER (STATICHE) ---

/**
 * @brief Alloca e inizializza una matrice r x c con un valore 'val'
 */
static int **MATRIXinit(int r, int c, int val) {
    int i, j;
    // Alloca 'r' puntatori (uno per ogni riga)
    int **t = malloc(r * sizeof(int *));
    
    // Per ogni riga, alloca 'c' interi (le colonne)
    for (i = 0; i < r; i++) 
        t[i] = malloc(c * sizeof(int));
    // Inizializza ogni cella a 'val' (di solito 0)
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            t[i][j] = val;
    
    return t; // Ritorna il puntatore alla matrice
}

/**
 * @brief Crea una struttura Edge (Arco)
 */
static Edge EDGEcreate(int v, int w, int wt) {
    Edge e; // Dichiara la struct
    e.v = v;   // Vertice di partenza
    e.w = w;   // Vertice di arrivo
    e.wt = wt; // Peso
    return e; // Ritorna la struct
}

// --- FUNZIONI DI INIZIALIZZAZIONE E I/O ---

/**
 * @brief Inizializza un grafo vuoto con V vertici
 */
Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof *G); // Alloca il wrapper
    G->V = V; // Imposta il numero di vertici
    G->E = 0; // Inizia con 0 archi
    // Alloca la matrice V x V, inizializzata a 0 (nessun arco)
    G->madj = MATRIXinit(V, V, 0); // ERRORE DI BATTITURA: MATRIXint -> MATRIXinit
    G->tab = STinit(V); // Inizializza la Symbol Table
    return G;
}

/**
 * @brief Libera tutta la memoria allocata per il grafo
 */
void GRAPHfree(Graph G) {
    int i;

    // 1. Libera ogni array "colonna"
    for (i = 0; i < G->V; i++)
        free(G->madj[i]);
    // 2. Libera l'array "riga" (di puntatori)
    free(G->madj);
    // 3. Libera la Symbol Table (assumendo esista STfree)
    STfree(G->tab);
    // 4. Libera il wrapper del grafo
    free(G);
}

/**
 * @brief Legge un grafo (con etichette) da file
 */
Graph GRAPHload(FILE *fin) {
    int V, i, id1, id2, wt;
    char label1[MAXC], label2[MAXC]; // MAXC non definito qui
    Graph G;

    fscanf(fin, "%d", &V); // 1. Legge il numero di vertici
    G = GRAPHinit(V); // 2. Inizializza il grafo
    
    // 3. Legge V etichette (nomi) e le inserisce nella ST
    for (i = 0; i < V; i++) {
        fscanf(fin, "%s", label1);
        STinsert(G->tab, label1, i); // Assumendo STinsert(tab, key, id)
    }
    
    // 4. Legge gli archi (etichetta1 etichetta2 peso)
    while(fscanf(fin,"%s %s %d", label1, label2, &wt) == 3) {
        // Cerca gli indici interi corrispondenti alle etichette
        id1 = STsearch(G->tab, label1); // Assumendo STsearch(tab, key) -> id
        id2 = STsearch(G->tab, label2);
        // Se entrambi i nomi sono validi...
        if (id1 >= 0 && id2 >= 0)
            GRAPHinsertE(G, id1, id2, wt); // ...inserisci l'arco
    }
    return G;
}

/**
 * @brief Salva un grafo (con etichette) su file
 */
void GRAPHstore(Graph G, FILE *fout) {
    int i;
    Edge *a; // Array temporaneo per gli archi

    // Alloca spazio per tutti gli E archi
    a = malloc(G->E * sizeof(Edge));
    // Riempie l'array 'a' con gli archi
    GRAPHedges(G, a);
    
    fprintf(fout, "%d\n", G->V); // 1. Scrive V
    
    // 2. Scrive le V etichette (nomi)
    for (i = 0; i < G->V; i++)
        fprintf(fout, "%s\n", STsearchByIndex(G->tab, i)); // Assumendo STsearchByIndex
        
    // 3. Scrive gli E archi usando le etichette
    for (i = 0; i < G->E; i++)
        fprintf(fout, "%s %s %d\n", 
            STsearchByIndex(G->tab, a[i].v), // Etichetta di v
            STsearchByIndex(G->tab, a[i].w), // Etichetta di w
            a[i].wt);                        // Peso
}

// --- FUNZIONI DI MODIFICA ARCHI (Implementazione ORIENTATA) ---

/**
 * @brief Funzione 'insertE' (helper) - Inserisce un arco ORIENTATO (v -> w)
 */
static void insertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;

    // Incrementa il contatore E solo se l'arco è nuovo
    if (G->madj[v][w] == 0)
        G->E++;
        
    G->madj[v][w] = 1; // QUESTA RIGA è inutile e sovrascritta
    G->madj[v][w] = wt; // Imposta il peso nella cella [v][w]
    
    // Manca G->madj[w][v] = wt. Questa è un'implementazione
    // per un grafo ORIENTATO (o DIRETTO).
}

/**
 * @brief Funzione 'removeE' (helper) - Rimuove un arco ORIENTATO (v -> w)
 */
static void removeE(Graph G, Edge e) {
    int v = e.v, w = e.w;
    
    // Decrementa E se l'arco esisteva
    if (G->madj[v][w] != 0)
        G->E--;
    G->madj[v][w] = 0; // Rimuove l'arco [v][w]
    // Non tocca [w][v] -> Grafo ORIENTATO
}

/**
 * @brief Estrae tutti gli archi in un array 'a' (per grafo ORIENTATO)
 */
void GRAPHedges(Graph G, Edge *a) {
    int v, w, E = 0;
    
    // Scorre l'INTERA matrice V x V
    for (v = 0; v < G->V; v++)
        for (w = 0; w < G->V; w++) // Cerca [v][w] (non solo w = v+1)
            if (G->madj[v][w] != 0) // Se l'arco (v -> w) esiste
                a[E++] = EDGEcreate(v, w, G->madj[v][w]); // Aggiungilo
    return;
}

/**
 * @brief Ottiene l'indice di un'etichetta (o la crea se non esiste)
 */
int GRAPHgetIndex(Graph G, char *label) {
    int id;
    id = STsearch(G->tab, label); // Cerca l'etichetta
    
    if (id == -1) { // Se non c'è
        id = STcount(G->tab); // Ottieni un nuovo ID (l'attuale dimensione)
        STinsert(G->tab, label, id); // Inserisci la nuova etichetta
    }
    return id; // Ritorna l'indice
}

/**
 * @brief Inserisce un arco (wrapper pubblico)
 */
void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    insertE(G, EDGEcreate(id1, id2, wt));
}

/**
 * @brief Rimuove un arco (wrapper pubblico)
 */
void GRAPHremoveE(Graph G, int id1, int id2) {
    removeE(G, EDGEcreate(id1, id2, 0)); // Il peso 0 è irrilevante per rimuovere
}

// --- FUNZIONI DI VISITA (Coerenti con la Matrice) ---

/**
 * @brief Funzione 'pathR' (helper): Ricerca in Profondità (DFS) per un cammino
 */
static int pathR(Graph G, int v, int w, int *visited) {
    int t; // Iteratore per i vicini

    if (v == w) // Caso base 1: Trovato!
        return 1;
        
    visited[v] = 1; // Marca 'v' come visitato
    
    for (t = 0 ; t < G->V ; t++) // Scorre tutti i possibili vertici
        // NOTA: Controlla se G->madj[v][t] == 1.
        // Se il grafo è pesato (wt > 1), questo fallirà.
        // Dovrebbe essere G->madj[v][t] != 0.
        if (G->madj[v][t] == 1) 
            if (visited[t] == 0) // Se 't' è un vicino non visitato
                if (pathR(G, t, w, visited)) {
                    // Stampa l'arco al "ritorno" (cammino al contrario)
                    printf("(%s, %s) in path\n", STsearchByIndex(G->tab, v), STsearchByIndex(G->tab, t));
                    return 1; // Trovato
                }
    
    return 0; // Non trovato
}

/**
 * @brief Funzione 'pathRH' (helper): Ricerca Cammino Hamiltoniano
 * ('d' è la lunghezza *rimanente* del cammino)
 */
static int pathRH(Graph G, int v, int w, int d, int *visited) {
    int t;
    
    if (v == w) { // Caso base: Arrivato a destinazione
        if (d == 0) // Se la distanza è 0, è un successo
            return 1;
        else 
            return 0; // Arrivato, ma troppo presto (non Hamiltoniano)
    }
    
    visited[v] = 1; // Marca
    
    for (t = 0 ; t < G->V ; t++)
        // NOTA: Stesso problema di pathR, controlla '== 1' (non pesato?)
        if (G->madj[v][t] == 1) 
            if (visited[t] == 0) // Se non visitato
                // Chiama ricorsivamente con distanza diminuita (d-1)
                if (pathRH(G, t, w, d-1, visited)) {
                    printf("(%s, %s) in path \n", STsearchByIndex(G->tab, v), STsearchByIndex(G->tab, t));
                    return 1;
                }
                
    visited[v] = 0; // BACKTRACKING: "smarca" 'v' per altri percorsi
    return 0;
}

/**
 * @brief Funzione 'GRAPHpath' (wrapper): Cerca se esiste un cammino
 */
void GRAPHpath(Graph G, int id1, int id2) {
    int t, found, *visited;
    visited = malloc(G->V*sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
    if (id1 < 0 || id2 < 0) 
        return;
        
    // !!! BUG CRITICO !!!
    // Chiama pathR, ma passa G->V-1 come 'visited' array.
    // La chiamata corretta (dal codice precedente) è:
    // found = pathR(G, id1, id2, visited);
    found = pathR(G, id1, id2, G->V-1, visited); // ERRORE
    
    if (found == 0) 
        printf("\n Path not found!\n");
}

/**
 * @brief Funzione 'GRAPHpathH' (wrapper): Cerca cammino Hamiltoniano
 */
void GRAPHpathH(Graph G, int id1, int id2) {
    int t, found, *visited;
    visited = malloc(G->V*sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
    if (id1 < 0 || id2 < 0) 
        return;
    // Cerca un cammino di lunghezza V-1 (che tocca V vertici)
    found = pathRH(G, id1, id2, G->V-1, visited);
    if (found == 0) 
        printf("\n Path not found!\n");
}

// --- FUNZIONI ALGORITMICHE (Implementazione per LISTE - NON FUNZIONANTI) ---

/**
 * @brief Inverte un grafo
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
Graph GRAPHreverse(Graph G) {
    int v;
    link t; // ERRORE: 'link' non è definito in questa struct
    Graph R = GRAPHinit(G->V);

    for (v = 0; v < G->V; v++)
        // ERRORE: G->ladj e G->z non esistono.
        for (t = G->ladj[v]; t != G->z; t = t->next)
            // ERRORE: t->v non esiste
            GRAPHinsertE(R, t->v, v);
    return R;
}

/**
 * @brief Helper DFS per SCC
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
static void SCCdfsR(Graph G, int w, int *scc, int *time0, int time1, int *post) {
    link t; // ERRORE: 'link' non definito

    scc[w] = time1;
    // ERRORE: G->ladj, G->z, t->next, t->v non esistono
    for (t = G->ladj[w]; t != G->z; t = t->next)
        if (scc[t->v] == -1)
            SCCdfsR(G, t->v, scc, time0, time1, post);
    post[(*time0)++]= w;
}

/**
 * @brief Trova Componenti Fortemente Connesse (Algoritmo di Kosaraju)
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
int GRAPHscc(Graph G) {
    int v, time0 = 0, time1 = 0, *sccG, *sccR, *postG, *postR;
    // ERRORE: GRAPHreverse non funzionerà
    Graph R = GRAPHreverse(G);

    // Allocazioni (OK)
    sccG = malloc(G->V * sizeof(int));
    sccR = malloc(G->V * sizeof(int));
    postG = malloc(G->V * sizeof(int));
    postR = malloc(G->V * sizeof(int));

    // Inizializzazione (OK)
    for (v = 0; v < G->V; v++) {
        sccG[v] = -1; sccR[v] = -1; postG[v] = -1; postR[v] = -1;
    }
    
    // Prima DFS (su Grafo Inverso R)
    for (v=0; v < G->V; v++)
        if (sccR[v] == -1)
            SCCdfsR(R, v, sccR, &time0, time1, postR); // ERRORE (usa liste)
    
    time0 = 0; time1 = 0;
    
    // Seconda DFS (su Grafo G, in ordine post-visita inverso)
    for (v = G->V-1; v >= 0; v--)
        if (sccG[postR[v]] == -1){
            SCCdfsR(G, postR[v], sccG, &time0, time1, postG); // ERRORE (usa liste)
            time1++;
        }
    
    // Stampa (OK)
    printf("strongly connected components \n");
    for (v = 0; v < G->V; v++)
        printf("node %s in scc %d\n", STsearchByIndex(G->tab,v), sccG[v]);
    
    return time1;
}

/**
 * @brief Helper per Kruskal (MST)
 * !!! ERRORE: Dipende da funzioni esterne (sort, UF) !!!
 */
static int mstE(Graph G, Edge *mst, Edge *a) {
    int i, k;

    GRAPHedges(G, a); // OK (usa la matrice)
    // ERRORE: 'sort' non è definita (serve qsort)
    sort(a, 0, G->E-1);
    // ERRORE: 'UFinit' non è definita (serve Union-Find)
    UFinit(G->V); 
    
    for (i = 0, k = 0; i < G->E && k < G->V-1; i++)
        // ERRORE: 'UFfind' non è definita
        if (!UFfind(a[i].v, a[i].w)) { 
            // ERRORE: 'UFunion' non è definita
            UFunion(a[i].v, a[i].w); 
            mst[k++] = a[i];
        }
    return k;
}

/**
 * @brief Algoritmo di Kruskal per MST (wrapper)
 * !!! ERRORE: Dipende da mstE (che ha dipendenze esterne) !!!
 */
void GRAPHmstK(Graph G) {
    int i, k, weight = 0;
    Edge *mst = malloc((G->V-1) * sizeof(Edge)); 
    Edge *a = malloc(G->E * sizeof(Edge));  

    k = mstE(G, mst, a); // Chiama l'helper con errori

    // Stampa (OK)
    printf("\nEdges in the MST: \n");
    for (i=0; i < k; i++) {
        printf("(%s - %s) \n", STsearchByIndex(G->tab, mst[i].v), 
                               STsearchByIndex(G->tab, mst[i].w));
        weight += mst[i].wt;
    }
    printf("minimum weight: %d\n", weight);
}

/**
 * @brief Helper per Prim (MST) (scritto PER MATRICE)
 * !!! ERRORE: Contiene bug e dipendenze esterne !!!
 */
static void mstV(Graph G, int *st, int *wt) {
    int v, w, min, *fr = malloc(G->V*sizeof(int));
    
    for (v=0; v < G->V; v++) { 
        st[v] = -1; // -1 = non nell'MST
        fr[v] = v;  // 'fr' (from) = genitore nell'MST
        // ERRORE: 'maxWT' non è definita (serve INT_MAX da <limits.h>)
        wt[v] = maxWT; 
    }
    st[0] = 0; // Inizia dal vertice 0
    wt[0] = 0; 
    
    // ERRORE: 'wt' è allocato con G->V+1 elementi (in GRAPHmstP),
    // ma qui si accede a G->V, che è l'ultimo slot
    wt[G->V] = maxWT; // ERRORE (maxWT non definito)

    // Ciclo principale di Prim
    for (min = 0; min != G->V; ) {
        v = min; // 'v' è il vertice aggiunto all'MST
        st[min] = fr[min]; // Conferma il genitore
        
        // Cerca il prossimo vertice 'min'
        for (w = 0, min = G->V; w < G->V; w++)
            if (st[w] == -1) { // Se 'w' non è ancora nell'MST
                // Rilassamento dell'arco (v, w)
                if (G->madj[v][w] < wt[w]) { 
                    wt[w] = G->madj[v][w]; // Aggiorna costo
                    fr[w] = v; // Aggiorna genitore
                }
                // Trova il nuovo 'min' (vertice non nell'MST
                // con il costo minore)
                if (wt[w] < wt[min]) 
                    min = w;
            }
    }
}

/**
 * @brief Algoritmo di Prim per MST (wrapper)
 * !!! ERRORE: Dipende da mstV (che ha errori) !!!
 */
void GRAPHmstP(Graph G) {
    int v, *st, *wt, weight = 0;

    st = malloc(G->V*sizeof(int)); 
    wt = malloc((G->V+1)*sizeof(int)); // Alloca G->V+1 slot

    mstV(G, st, wt); // Chiama l'helper con errori

    // Stampa (OK)
    printf("\nEdges in the MST: \n");
    for (v = 0; v < G->V; v++) {
        if (st[v] != v) { // Non stampare la radice (genitore di se stessa)
            printf("(%s-%s)\n", STsearchByIndex(G->tab, st[v]),
                                 STsearchByIndex(G->tab, v));
            weight += wt[v];
        }
    }
    printf("\nminimum weight: %d\n", weight);
}

/**
 * @brief Algoritmo di Dijkstra (Cammini Minimi)
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 * !!! ERRORE: Dipende da PQ (Coda con Priorità) non definita !!!
 */
void GRAPHspD(Graph G, int id) {
    int v;
    link t; // ERRORE: 'link' non definito
    PQ pq = PQinit(G->V); // ERRORE: PQinit non definito
    int *st, *d;

    st = malloc(G->V*sizeof(int));
    d = malloc(G->V*sizeof(int));
    for (v = 0; v < G->V; v++) {
        st[v] = -1;
        d[v] = maxWT; // ERRORE: 'maxWT' non definito
        PQinsert(pq, d, v); // ERRORE: 'PQinsert' non definito
    }

    d[id] = 0;
    st[id] = id;
    PQchange(pq, d, id); // ERRORE: 'PQchange' non definito

    while (!PQempty(pq)) { // ERRORE: 'PQempty' non definito
        // ERRORE: 'PQextractMin' non definito
        if (d[v = PQextractMin(pq, d)] != maxWT) 
            // ERRORE: Ciclo su 'ladj' (liste)
            for (t = G->ladj[v]; t != G->z ; t = t->next)
                if (d[v] + t->wt < d[t->v]) { // ERRORE: 't->wt', 't->v'
                    d[t->v] = d[v] + t->wt;
                    PQchange(pq, d, t->v); // ERRORE
                    st[t->v] = v;
                }
    }
    // ... (stampa)
    PQfree(pq); // ERRORE: 'PQfree' non definito
}

/**
 * @brief Algoritmo di Bellman-Ford (Cammini Minimi)
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
void GRAPHspBF(Graph G, int id) {
    int v, i, negcycfound;
    link t; // ERRORE: 'link' non definito
    int *st, *d;

    st = malloc(G->V*sizeof(int));
    d = malloc(G->V*sizeof(int));

    for (v = 0; v < G->V; v++) {
        st[v]= -1;
        d[v] = maxWT; // ERRORE: 'maxWT' non definito
    }
    d[id] = 0;
    st[id] = id;

    // Ciclo di rilassamento (V-1 volte)
    for (i = 0; i < G->V-1; i++)
        for (v = 0; v < G->V; v++)
            if (d[v] < maxWT)
                // ERRORE: Ciclo su 'ladj' (liste)
                for (t = G->ladj[v]; t != G->z ; t = t->next)
                    if (d[t->v] > d[v] + t->wt) { // ERRORE
                        d[t->v] = d[v] + t->wt;
                        st[t->v] = v;
                    }
                    
    // Controllo cicli negativi
    negcycfound = 0;
    for (v = 0; v < G->V; v++)
        if (d[v] < maxWT)
            // ERRORE: Ciclo su 'ladj' (liste)
            for (t = G->ladj[v]; t != G->z ; t = t->next)
                if (d[t->v] > d[v] + t->wt) // ERRORE
                    negcycfound = 1;
    // ... (stampa)
}

/**
 * @brief Helper DFS per Ordinamento Topologico
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
void TSdfsR(Graph G, int v, int *ts, int *pre, int *time) {
    link t; // ERRORE: 'link' non definito
    pre[v] = 0; // Marca 'v' come "in visita"

    // ERRORE: Ciclo su 'ladj' (liste)
    for (t = G->ladj[v]; t != G->z; t = t->next)
        if (pre[t->v] == -1) // -1 = non visitato
            TSdfsR(G, t->v, ts, pre, time);

    // Inserisci 'v' nell'array 'ts' (in ordine di post-visita)
    ts[(*time)++] = v;
}

/**
 * @brief Ordinamento Topologico (Reverse Topological Sort)
 * !!! ERRORE: Implementazione per LISTE di adiacenza !!!
 */
void DAGrts(Graph G) {
    int v, time = 0, *pre, *ts; // pre = array visitati, ts = array risultato

    pre = (int*) malloc(G->V * sizeof(int));
    ts = (int*) malloc(G->V * sizeof(int));

    for (v = 0; v < G->V; v++) { 
        pre[v] = -1; // -1 = non visitato
        ts[v] = -1; 
    }
    
    // Esegui la DFS
    for (v = 0; v < G->V; v++)
        if (pre[v] == -1) 
            TSdfsR(G, v, ts, pre, &time); // ERRORE (usa liste)

    // Stampa (OK)
    printf("DAG nodes in reverse topological order \n");
    for (v = 0; v < G->V; v++) // Stampa 'ts'
        printf("%s ", STsearchByIndex(G->tab, ts[v]));
    printf("\n");
}