#include "Graph.h"

// Definizione interna (nascosta) della struttura Grafo
struct graph {
    int V;         // Numero di Vertici
    int E;         // Numero di Archi
    int **madj;    // Puntatore a un array di puntatori (la Matrice di Adiacenza)
    ST tab;        // Puntatore alla Symbol Table (per mappare nomi a indici)
};


// Funzione 'MATRIXinit' (helper)
// Alloca e inizializza una matrice r x c con un valore 'val'
static int **MATRIXinit(int r, int c, int val) {
    int i, j;
    // Alloca 'r' puntatori (le righe)
    int **t = malloc(r * sizeof(int *));
    
    // Per ogni riga, alloca 'c' interi (le colonne)
    for (i = 0; i < r; i++) 
        t[i] = malloc(c * sizeof(int));
    // Inizializza ogni cella a 'val' (es. 0)
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            t[i][j] = val;
    
    return t; // Ritorna il puntatore alla matrice
}

// Funzione 'EDGEcreate' (helper)
// Crea una struttura Edge (Arco)
static Edge EDGEcreate(int v, int w, int wt) {
    Edge e; // Dichiara la struct
    
    e.v = v;   // Vertice 1
    e.w = w;   // Vertice 2
    e.wt = wt; // Peso
    
    return e; // Ritorna la struct (copiata)
}


// Funzione 'GRAPHinit': inizializza un grafo vuoto con V vertici
Graph GRAPHinit(int V) {
    // Alloca la struttura wrapper
    Graph G = malloc(sizeof *G);

    G->V = V; // Imposta il numero di vertici
    G->E = 0; // Inizia con 0 archi
    // Alloca la matrice di adiacenza V x V, inizializzata a 0
    G->madj = MATRIXinit(V, V, 0); 
    // Inizializza la Symbol Table (assumendo STinit da ST.h)
    G->tab = STinit(V);

    return G; // Ritorna il puntatore al grafo
}

// Funzione 'GRAPHfree': libera tutta la memoria
void GRAPHfree(Graph G) {
    int i;

    // 1. Libera ogni colonna (array) della matrice
    for (i = 0; i < G->V; i++)
        free(G->madj[i]);
    // 2. Libera l'array delle righe (puntatori)
    free(G->madj);
    // 3. Libera la Symbol Table (assumendo STfree da ST.h)
    STfree(G->tab);
    // 4. Libera il wrapper del grafo
    free(G);
}


// Funzione 'GRAPHload': legge un grafo da file
Graph GRAPHload(FILE *fin) {
    int V, i, id1, id2, wt;
    char label1[MAXC], label2[MAXC]; // MAXC non definito qui
    Graph G;

    // 1. Legge il numero di vertici 'V'
    fscanf(fin, "%d", &V);
    // 2. Inizializza il grafo
    G = GRAPHinit(V);
    // 3. Legge V etichette (nomi) e le inserisce nella Symbol Table
    for (i = 0; i < V; i++) {
        fscanf(fin, "%s", label1);
        STinsert(G->tab, label1, i); // Assumendo STinsert(tab, key, index)
    }
    // 4. Legge gli archi (label1 label2 peso) finché il file non finisce
    while(fscanf(fin,"%s %s %d", label1, label2, &wt) == 3) {
        // Cerca gli indici interi corrispondenti alle etichette
        id1 = STsearch(G->tab, label1); // Assumendo STsearch(tab, key) -> index
        id2 = STsearch(G->tab, label2);
        // Se entrambe le etichette sono valide...
        if (id1 >= 0 && id2 >= 0)
            // ...inserisci l'arco nel grafo
            GRAPHinsertE(G, id1, id2, wt);       
    }

    return G; // Ritorna il grafo caricato
}

// Funzione 'GRAPHstore': salva un grafo su file
void GRAPHstore(Graph G, FILE *fout) {
    int i;
    Edge *a; // Puntatore a un array di archi

    // Alloca un array per contenere tutti gli E archi
    a = malloc(G->E * sizeof(Edge));
    // Riempie l'array 'a' con tutti gli archi del grafo
    GRAPHedges(G, a);
    
    // 1. Scrive il numero di vertici
    fprintf(fout, "%d\n", G->V);
    // 2. Scrive le etichette (nomi) dei vertici
    for (i = 0; i < G->V; i++)
        // Cerca l'etichetta dall'indice (assumendo STsearchByIndex)
        fprintf(fout, "%s\n", STsearchByIndex(G->tab, i));
    // 3. Scrive tutti gli archi (usando le etichette)
    for (i = 0; i < G->E; i++)
        fprintf(fout, "%s %s %d\n", 
            STsearchByIndex(G->tab, a[i].v), // Etichetta di v
            STsearchByIndex(G->tab, a[i].w), // Etichetta di w
            a[i].wt);                        // Peso
}

// Funzione 'insertE' (helper) - Inserisce un arco NON ORIENTATO
static void insertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;

    // Se l'arco non esisteva (peso 0), incrementa il contatore E
    if (G->madj[v][w] == 0)
        G->E++;
    
    // Imposta il peso (o 1 se non pesato)
    G->madj[v][w] = 1; // Questa riga è ridondante
    G->madj[v][w] = wt; // Questa sovrascrive la precedente
    
    // Rendi il grafo NON ORIENTATO (simmetrico)
    G->madj[w][v] = 1; // Ridondante
    G->madj[w][v] = wt; // Imposta il peso anche sull'arco inverso
}

// Funzione 'removeE' (helper) - Rimuove un arco NON ORIENTATO
static void removeE(Graph G, Edge e) {
    int v = e.v, w = e.w;
    
    // Se l'arco esisteva, decrementa il contatore E
    if (G->madj[v][w] != 0)
        G->E--;
        
    // Rimuovi l'arco (imposta a 0)
    G->madj[v][w] = 0;
    // Rimuovi l'arco inverso (simmetria)
    G->madj[w][v] = 0;
}

// Funzione 'GRAPHedges': Estrae tutti gli archi in un array 'a'
void GRAPHedges(Graph G, Edge *a) {
    int v, w, E = 0; // E = contatore per l'array 'a'
    
    // Scorre la METÀ SUPERIORE della matrice (v da 0 a V, w da v+1 a V)
    // per evitare di contare due volte gli archi (v,w) e (w,v)
    for (v = 0; v < G->V; v++)
        for (w = v + 1; w < G->V; w++) // w parte da v+1
            // Se l'arco esiste...
            if (G->madj[v][w] != 0)
                // ...crealo e inseriscilo nell'array 'a'
                a[E++] = EDGEcreate(v, w, G->madj[v][w]);
    return;
}

// Funzione 'GRAPHgetIndex': Ottiene l'indice di un'etichetta (o la crea)
int GRAPHgetIndex(Graph G, char *label) {
    int id;

    // 1. Cerca l'etichetta nella Symbol Table
    id = STsearch(G->tab, label);
    
    // 2. Se non c'è (id == -1)...
    if (id == -1) {
        // ...crea un nuovo indice...
        id = STcount(G->tab); // Assumendo STcount() = num elementi
        // ...e inserisci la nuova etichetta associata al nuovo indice
        STinsert(G->tab, label, id);
    }

    return id; // Ritorna l'indice (vecchio o nuovo)
}

// Funzione 'GRAPHinsertE' (wrapper pubblico)
void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    // Chiama l'helper 'insertE' creando prima un Edge
    insertE(G, EDGEcreate(id1, id2, wt));
}

// Funzione 'GRAPHremoveE' (wrapper pubblico)
void GRAPHremoveE(Graph G, int id1, int id2) {
    // Chiama l'helper 'removeE' (il peso 0 è irrilevante)
    removeE(G, EDGEcreate(id1, id2, 0));
}


// Funzione 'pathR' (helper): Ricerca in Profondità (DFS) per un cammino
static int pathR(Graph G, int v, int w, int *visited) {
    int t; // Iteratore per i vicini

    // Caso base 1: Trovato! (il vertice 'v' è la destinazione 'w')
    if (v == w)
        return 1; // 1 = vero (cammino trovato)
        
    // Marca 'v' come visitato per evitare cicli
    visited[v] = 1;
    
    // Scorre tutti i possibili vicini 't' di 'v'
    for (t = 0 ; t < G->V ; t++)
        // Se 't' è un vicino (arco esiste)...
        if (G->madj[v][t] == 1) // NOTA: Controlla solo se esiste (peso 1?),
                                // non se '!= 0' (pesato)
            // ...e se 't' non è ancora stato visitato...
            if (visited[t] == 0)
                // ...continua la ricerca ricorsiva da 't'
                if (pathR(G, t, w, visited)) {
                    // Se la ricorsione ha trovato 'w', stampa l'arco
                    // (Stampa al "ritorno" della ricorsione,
                    // quindi il cammino viene stampato al contrario)
                    printf("(%s, %s) in path\n", STsearchByIndex(G->tab, v), 
                                                STsearchByIndex(G->tab, t));
                    return 1; // Propaga il successo
                }
    
    // Se il loop 'for' finisce, non ci sono cammini da 'v'
    return 0; // 0 = falso (non trovato)
}

// Funzione 'pathRH' (helper): Ricerca Cammino Hamiltoniano (DFS limitata)
// 'd' è la lunghezza *rimanente* del cammino da trovare
static int pathRH(Graph G, int v, int w, int d, int *visited) {
    int t;
    
    // Caso base 1: Trovato!
    if (v == w) {
        // Se siamo arrivati a 'w' E abbiamo percorso
        // la distanza richiesta (d == 0), è un successo
        if (d == 0)
            return 1;
        else 
            return 0; // Arrivati, ma troppo presto
    }
    
    visited[v] = 1; // Marca 'v' come visitato
    
    // Scorre tutti i possibili vicini 't'
    for (t = 0 ; t < G->V ; t++)
        if (G->madj[v][t] == 1) // Controlla se l'arco esiste
            if (visited[t] == 0) // Se non visitato
                // Ricerca ricorsiva: cerca 'w' da 't',
                // con una distanza rimanente di 'd-1'
                if (pathRH(G, t, w, d-1, visited)) {
                    // Trovato: stampa l'arco e propaga il successo
                    printf("(%s, %s) in path \n", STsearchByIndex(G->tab, v), STsearchByIndex(G->tab, t));
                    return 1;
                }
    
    // Backtracking: "de-marca" 'v'
    // Se la ricerca da 'v' fallisce, 'v' deve essere
    // disponibile per altri cammini.
    visited[v] = 0; 

    return 0; // Non trovato da questo ramo
}

// Funzione 'GRAPHpath' (wrapper): Cerca se esiste un cammino
void GRAPHpath(Graph G, int id1, int id2) {
    int t, found, *visited;

    // Alloca e inizializza l'array dei visitati
    visited = malloc(G->V*sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
        
    if (id1 < 0 || id2 < 0) // Controllo indici
        return;
        
    // Chiama l'helper ricorsivo
    found = pathR(G, id1, id2, visited);
    
    if (found == 0) 
        printf("\n Path not found!\n");
}

// Funzione 'GRAPHpathH' (wrapper): Cerca un cammino Hamiltoniano
void GRAPHpathH(Graph G, int id1, int id2) {
    int t, found, *visited;

    // Alloca e inizializza l'array dei visitati
    visited = malloc(G->V*sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
        
    if (id1 < 0 || id2 < 0) 
        return;
        
    // Chiama l'helper ricorsivo, cercando un cammino di lunghezza V-1
    // (un cammino Hamiltoniano tocca tutti i V vertici,
    // quindi ha V-1 archi)
    found = pathRH(G, id1, id2, G->V-1, visited);
    
    if (found == 0) 
        printf("\n Path not found!\n");
}

// Funzione 'dfsRcc' (helper): DFS per Componenti Connesse
// (Questa funzione è scritta per LISTE DI ADIACENZA, non per MATRICI!)
static void dfsRcc(Graph G, int v, int id, int *cc) {
    link t; // 'link' non è definito in questa implementazione!

    cc[v] = id; // Assegna l'ID della componente connessa 'id' a 'v'
    
    // Questo 'for' loop è per le liste di adiacenza (G->ladj)
    for (t = G->ladj[v]; t != G->z; t = t->next)
        if (cc[t->v] == -1) // Se il vicino non è stato visitato
            dfsRcc(G, t->v, id, cc); // Visitalo
}

// Funzione 'GRAPHcc': Trova le Componenti Connesse
int GRAPHcc(Graph G) {
    int v, id = 0, *cc; // id = contatore delle componenti
    
    // Alloca l'array 'cc' (Connected Component)
    cc = malloc(G->V * sizeof(int));
    // Inizializza tutti i vertici come non visitati (-1)
    for (v = 0; v < G->V; v++) 
        cc[v] = -1;
        
    // Scorre tutti i vertici 'v' del grafo
    for (v = 0; v < G->V; v++)
        // Se 'v' non è ancora stato assegnato a una componente...
        if (cc[v] == -1) 
            // ...inizia una nuova DFS da 'v',
            // marcando tutti i nodi raggiungibili con il nuovo 'id'
            dfsRcc(G, v, id++, cc); // (id++ incrementa 'id' dopo la chiamata)
    
    // Stampa i risultati
    printf("Connected component(s) \n");
    for (v = 0; v < G->V; v++)
        printf("node %s in cc %d\n", STsearchByIndex(G->tab,v), cc[v]);

    return id; // Ritorna il numero totale di componenti
}
