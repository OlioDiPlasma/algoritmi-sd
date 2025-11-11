// Include il file header che (presumibilmente) definisce i prototipi
// di queste funzioni e le strutture/tipi (Graph, Edge, link, ST, Q, ecc.)
#include "Algoritmi_Visita.h"

/*
 * ======================================================================
 * DFS SEMPLICE
 * Scopo: Eseguire una visita DFS base, calcolando solo i tempi di scoperta.
 * NOTA: Questa implementazione usa LISTE di adiacenza (G->ladj).
 * ======================================================================
 */

/**
 * @brief Funzione ricorsiva (helper) per la DFS Semplice.
 * @param G Il grafo
 * @param e L'arco che porta al vertice corrente 'w'
 * @param cnt Puntatore al contatore del tempo di scoperta
 * @param pre Array dei tempi di scoperta (o -1 se non visitato)
 */
static void simpleDfsR(Graph G, Edge e, int *cnt, int *pre) {
    link t; // 'link' è (presumibilmente) un puntatore a un nodo della lista di adiacenza
    int w = e.w; // 'w' è il vertice *corrente* da visitare (destinazione dell'arco 'e')
    
    // Assegna il tempo di scoperta (pre-ordine).
    // Marca 'w' con il valore corrente di 'cnt', POI incrementa 'cnt'.
    pre[w] = (*cnt)++;
    
    // Cicla su tutti i vicini 't' del vertice 'w'
    // (Questa sintassi presume un'implementazione a LISTE di adiacenza)
    for (t = G->ladj[w]; t != G->z; t = t->next)
        // Se il vicino 't->v' non è ancora stato visitato (è -1)...
        if (pre[t->v] == -1)
            // ...chiamata ricorsiva: visita quel vicino.
            // Il nuovo arco è (w, t->v)
            simpleDfsR(G, EDGEcreate(w, t->v), cnt, pre);
}

/**
 * @brief Funzione pubblica (wrapper) per avviare la DFS Semplice da 'id'.
 */
void GRAPHsimpleDfs(Graph G, int id) {
    int v, cnt = 0, *pre; // 'cnt' è il contatore del tempo
    
    // Alloca l'array per i tempi di scoperta (dimensione V)
    pre = malloc(G->V * sizeof(int));
    if (pre == NULL) // Controllo fallimento malloc
        return;
        
    // Inizializza tutti i vertici come "non visitati" (-1)
    for (v = 0; v < G->V; v++) 
        pre[v] = -1;

    // Avvia la prima visita DFS partendo dal vertice 'id'.
    // EDGEcreate(id, id) è un trucco per passare 'id' come vertice
    // di partenza (e.w) alla funzione ricorsiva.
    simpleDfsR(G, EDGEcreate(id, id), &cnt, pre);
    
    // Gestione dei GRAFI NON CONNESSI:
    // Cicla su tutti i vertici 'v'
    for (v = 0; v < G->V; v++)
        // Se, dopo la prima DFS, 'v' è ancora -1 (non visitato)...
        if (pre[v] == -1)
            // ...significa che 'v' appartiene a una componente connessa diversa,
            // quindi avvia una nuova DFS da 'v'.
            simpleDfsR(G, EDGEcreate(v,v), &cnt, pre);
    
    // Stampa i risultati
    printf("discovery time labels \n");
    for (v = 0; v < G->V; v++)
        // STsearchByIndex (dalla Symbol Table) converte l'indice 'v' nel suo nome
        printf("vertex %s : %d \n", STsearchByIndex(G->tab, v), pre[v]);
}

/*
 * ======================================================================
 * DFS ESTESA
 * Scopo: Calcolare tempi di scoperta (pre), fine (post) e albero DFS (st).
 * NOTA: Questa implementazione usa LISTE di adiacenza (G->ladj).
 * ======================================================================
 */

/**
 * @brief Funzione ricorsiva (helper) per la DFS Estesa.
 * @param time Puntatore al contatore globale del tempo (usato sia per pre che per post)
 * @param pre Array dei tempi di scoperta
 * @param post Array dei tempi di fine elaborazione
 * @param st Array dei genitori (Spanning Tree)
 */
static void ExtendedDfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st) {
    link t; // Iteratore della lista
    int w = e.w; // Vertice corrente

    // Salva il genitore ('e.v') del nodo corrente ('e.w')
    st[e.w] = e.v;
    // Assegna il tempo di scoperta (pre-ordine)
    pre[w] = (*time)++;
    
    // Cicla su tutti i vicini 't' del vertice 'w'
    for (t = G->ladj[w]; t != G->z; t = t->next)
        // Se il vicino 't->v' non è stato visitato...
        if (pre[t->v] == -1)
            // ...chiamata ricorsiva
            ExtendedDfsR(G, EDGEcreate(w, t->v), time, pre, post, st);
            
    // DOPO che il ciclo 'for' è finito (cioè, dopo che tutti
    // i discendenti di 'w' sono stati visitati completamente)...
    // ...assegna il tempo di fine elaborazione (post-ordine).
    post[w] = (*time)++;
}

/**
 * @brief Funzione pubblica (wrapper) per avviare la DFS Estesa da 'id'.
 */
void GRAPHextendedDfs(Graph G, int id) {
    int v, time = 0, *pre, *post, *st;

    // Alloca memoria per i tre array
    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));
    
    // Inizializza tutti gli array a -1 (non visitato / nessun genitore)
    for (v = 0; v < G->V; v++) {
        pre[v] = -1; 
        post[v] = -1; 
        st[v] = -1;
    }

    // Avvia la prima visita DFS
    extendedDfsR(G, EDGEcreate(id, id), &time, pre, post, st);
    
    // Gestione dei GRAFI NON CONNESSI (come prima)
    for (v = 0; v < G->V; v++)
        if (pre[v] == -1)
            extendedDfsR(G, EDGEcreate(v, v), &time, pre, post, st);
    
    // Stampa i risultati
    printf("discovery/endprocessing time labels \n");
    for (v = 0; v < G->V; v++)
        printf("%s:%d/%d\n", STsearchByIndex(G->tab,v), pre[v], post[v]);
    
    printf("resulting DFS tree \n");
    for (v = 0; v < G->V; v++)
        printf("%s’s parent: %s \n", STsearchByIndex (G->tab, v),
                                      STsearchByIndex (G->tab, st[v]));
}

/*
 * ======================================================================
 * DFS COMPLETA
 * Scopo: Come la Estesa, ma classifica anche gli archi (T, B, F, C).
 * NOTA: Questa implementazione usa LISTE di adiacenza (G->ladj).
 * ======================================================================
 */

/**
 * @brief Funzione ricorsiva (helper) per la DFS Completa.
 */
static void dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st) {
    link t; // Iteratore
    int v, w = e.w; // w = vertice corrente, v = vicino temporaneo
    Edge x; // Arco temporaneo per archi non-T
    
    // Se l'arco non è "fittizio" (v != w)...
    if (e.v != e.w)
        // ...è un Arco dell'Albero (Tree edge)
        printf("(%s, %s):  T \n", STsearchByIndex(G->tab, e.v), 
                                  STsearchByIndex(G->tab, e.w));
                                  
    st[e.w] = e.v; // Salva il genitore
    pre[w] = (*time)++; // Assegna tempo 'pre'
    
    // Cicla sui vicini di 'w'
    for (t = G->ladj[w]; t != G->z; t = t->next)
        // Se il vicino 't->v' non è visitato...
        if (pre[t->v] == -1)
            // ...chiamata ricorsiva (sarà un arco T)
            dfsR(G, EDGEcreate(w, t->v), time, pre, post, st);
        else {
            // Se il vicino 't->v' è GIA' stato visitato,
            // l'arco (w, t->v) è un arco non-T.
            v = t->v; // Salva l'indice del vicino
            x = EDGEcreate(w, v); // Salva l'arco (w, v)
        }
    
    /* * Classificazione Archi (NOTA: questo blocco è posizionato male,
     * dovrebbe essere DENTRO il loop 'else' per classificare 'x'
     * nel momento in cui viene trovato)
     */
    
    /* Tentativo di classificazione per GRAFO NON ORIENTATO */
    // Se il tempo di scoperta del genitore 'w' è minore di quello del figlio 'v'
    // (e v non è il genitore diretto, controllo che manca)
    if (pre[w] < pre[v])
        printf("(%s, %s): B\n", STsearchByIndex(G->tab, x.v), // Stampa Arco Indietro
                                STsearchByIndex(G->tab, x.w));
    
    /* Tentativo di classificazione per GRAFO ORIENTATO */
    if (pre[w] < pre[v]) // Errore logico, dovrebbe essere pre[w] > pre[v] per B/F/C
        // Se il vicino 'v' è ancora in visita (non ha tempo 'post')...
        if (post[v] == -1)
            printf("(%s, %s): B\n", STsearchByIndex(G->tab, x.v), // Arco Indietro (B)
                                    STsearchByIndex(G->tab, x.w));
        else
            // Se 'v' è stato scoperto *dopo* 'w' (è un discendente)...
            if (pre[v] > pre[w])
                printf("(%s, %s): F\n", STsearchByIndex(G->tab, x.v), // Arco Avanti (F)
                                        STsearchByIndex(G->tab, x.w));
            else // Altrimenti (è in un altro ramo)...
                printf("(%s, %s): C\n", STsearchByIndex(G->tab, x.v), // Arco di Attraversamento (C)
                                        STsearchByIndex(G->tab, x.w));
    
    // Assegna il tempo di fine elaborazione (post-ordine)
    post[w] = (*time)++;
}

/**
 * @brief Funzione pubblica (wrapper) per avviare la DFS Completa.
 */
void GRAPHdfs(Graph G, int id) {
    int v, time = 0, *pre, *post, *st;

    // Alloca memoria per i tre array
    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));
    
    // Inizializza tutti gli array a -1
    for (v = 0; v < G->V; v++) {
        pre[v] = -1; 
        post[v] = -1; 
        st[v] = -1;
    }

    // Avvia la prima visita DFS
    dfsR(G, EDGEcreate(id, id), &time, pre, post, st);
    
    // Gestione dei GRAFI NON CONNESSI
    for (v = 0; v < G->V; v++)
        if (pre[v] == -1)
            dfsR(G, EDGEcreate(v, v), &time, pre, post, st);
    
    // Stampa i risultati (tempi e albero)
    printf("discovery/endprocessing time labels \n");
    for (v = 0; v < G->V; v++)
        printf("%s:%d/%d\n", STsearchByIndex(G->tab,v), pre[v], post[v]);
    printf("resulting DFS tree \n");
    for (v = 0; v < G->V; v++)
        printf("%s’s parent: %s \n", STsearchByIndex (G->tab, v),
                                      STsearchByIndex (G->tab, st[v]));
}

/*
 * ======================================================================
 * BFS (Breadth-First Search)
 * Scopo: Visitare "a livelli", calcolando l'albero BFS e le distanze minime.
 * NOTA: Questa implementazione usa MATRICE di adiacenza (G->madj).
 * ======================================================================
 */

/**
 * @brief Funzione helper (iterativa) per la BFS.
 * @param e Arco di partenza (es. id, id)
 * @param time Contatore (usato come 'pre')
 * @param pre Array ordine di visita
 * @param st Array dei genitori (Spanning Tree)
 * @param dist Array delle distanze (livelli)
 */
static void bfs(Graph G, Edge e, int *time, int *pre, int *st, int *dist) {
    int x; // Indice per scorrere i vicini
    Q q = Qinit(); // Inizializza una Coda (struttura e funzioni non mostrate)

    Qput(q, e); // Metti in coda l'arco di partenza (id, id)
    
    // Trucco: imposta la distanza del "genitore" (id) a -1,
    // così il primo nodo (id) avrà distanza 0 (dist[e.v]+1)
    dist[e.v] = -1; 
    
    // Cicla finché la coda non è vuota
    while (!Qempty(q))
        // 1. Estrai un arco (v, w) dalla coda
        // 2. Se il nodo di arrivo 'w' (e.w) non è stato visitato...
        // (Nota: l'assegnazione 'e = Qget(q)' avviene dentro l'if)
        if (pre[(e = Qget(q)).w] == -1) {
            
            // ...VISITA il nodo 'w' (e.w)
            pre[e.w] = (*time)++; // Assegna l'ordine di visita
            st[e.w] = e.v; // Salva il suo genitore (e.v)
            dist[e.w] = dist[e.v]+1; // Calcola la sua distanza (livello)
            
            // Scorre tutti i possibili vicini 'x' di 'w'
            // (Questa sintassi presume un'implementazione a MATRICE)
            for (x = 0; x < G->V; x++)
                // Se esiste un arco da 'w' (e.w) a 'x'...
                if (G->madj[e.w][x] == 1)
                    // ...e se 'x' non è stato ancora visitato...
                    if (pre[x] == -1)
                        // ...metti in coda l'arco (w, x) per visitarlo in futuro.
                        Qput(q, EDGEcreate(e.w, x));
        }
}

/**
 * @brief Funzione pubblica (wrapper) per avviare la BFS da 'id'.
 */
void GRAPHbfs(Graph G, int id) {
    int v, time=0, *pre, *st, *dist;
    
    // Alloca memoria per gli array
    pre = (int*) malloc(G->V*sizeof(int));
    st = (int*) malloc(G->V*sizeof(int));
    dist = (int*) malloc(G->V*sizeof(int));

    // Inizializza gli array
    for (v = 0; v < G->V; v++) {
        pre[v] = -1; // -1 = non visitato
        st[v] = -1; // -1 = nessun genitore
        dist[v] = INT_MAX; // Distanza infinita (INT_MAX da <limits.h>)
    }

    // Avvia la visita BFS
    // (Questa visita esplorerà solo la componente connessa di 'id')
    bfs(G, EDGEcreate(id,id), &time, pre, st, dist);
    
    // Stampa i risultati
    printf("\n Resulting BFS tree \n");
    for (v = 0; v < G->V; v++)
        if (st[v] != -1) // Se il nodo è stato raggiunto
            printf("%s’s parent is: %s\n", STsearchByIndex(G->tab, v), 
                                           STsearchByIndex(G->tab, st[v]));
            
    printf("\n Levelizing \n");
    for (v = 0; v < G->V; v++)
        if (st[v] != -1) // Se il nodo è stato raggiunto
            printf("%s: %d \n", STsearchByIndex(G->tab,v), dist[v]);
}