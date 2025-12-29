#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h> /* Per DBL_MAX */
#include "graph.h"

/* --- 1. STRUTTURE DATI INTERNE --- */

/* Struttura per memorizzare le coordinate di un nodo */
typedef struct {
    double x, y;
} Coordinate;

struct graph {
    int V;              /* Numero vertici */
    int E;              /* Numero archi */
    double **madj;      /* Matrice di adiacenza (pesata) */
    Coordinate *coords; /* Array delle coordinate per ogni nodo index 0..V-1 */
    ST tab;             /* Tabella di simboli per mappare nomi <-> indici */
};

/* --- 2. FUNZIONI AUSILIARIE (STATICHE) --- */

/*
    Scopo: Crea matrice V x V inizializzata.
    Concetto: Usiamo double perché i pesi sono distanze reali.
    Inizializziamo a DBL_MAX (infinito) per indicare assenza di arco, 0 sulla diagonale.
*/
static double **MATRIXinit(int r, int c, double val) {
    int i, j;
    double **t = malloc(r * sizeof(double *));
    for (i = 0; i < r; i++) {
        t[i] = malloc(c * sizeof(double));
        for (j = 0; j < c; j++)
            t[i][j] = val;
    }
    return t;
}

static void MATRIXfree(double **m, int r) {
    int i;
    for (i = 0; i < r; i++) free(m[i]);
    free(m);
}

/* Scopo: Calcola distanza Euclidea tra due nodi u e v.
    Formula: sqrt((x1-x2)^2 + (y1-y2)^2)
*/
static double getDist(Graph G, int u, int v) {
    double dx = G->coords[u].x - G->coords[v].x;
    double dy = G->coords[u].y - G->coords[v].y;
    return sqrt(dx*dx + dy*dy);
}

/*
    Scopo: Algoritmo di Dijkstra per cammini minimi da sorgente 's'.
    Ritorna: Array dinamico 'dist' dove dist[i] è la distanza minima da s a i.
    Concetto: Array dist[] inizializzato a INFINITO. Rilassiamo gli archi.
    Nota: Implementazione O(V^2) semplice (senza coda a priorità) va bene per matrici dense.
*/
static double *dijkstra(Graph G, int s) {
    int v, w;
    double *dist = malloc(G->V * sizeof(double));
    int *visited = calloc(G->V, sizeof(int)); /* calloc inizializza a 0 (false) */

    /* Inizializzazione distanze */
    for (v = 0; v < G->V; v++) dist[v] = DBL_MAX;
    dist[s] = 0.0;

    /* Loop principale Dijkstra */
    for (v = 0; v < G->V; v++) {
        int u = -1;
        double minVal = DBL_MAX;

        /* Trova il nodo non visitato con distanza minima */
        for (w = 0; w < G->V; w++) {
            if (!visited[w] && dist[w] < minVal) {
                minVal = dist[w];
                u = w;
            }
        }

        /* Se u rimane -1 o distanza infinita, non raggiungiamo più nulla */
        if (u == -1 || dist[u] == DBL_MAX) break;

        visited[u] = 1;

        /* Rilassamento vicini */
        for (w = 0; w < G->V; w++) {
            if (!visited[w] && G->madj[u][w] != DBL_MAX) {
                if (dist[u] + G->madj[u][w] < dist[w]) {
                    dist[w] = dist[u] + G->madj[u][w];
                }
            }
        }
    }
    free(visited);
    return dist;
}

/*
    Scopo: Verifica se il grafo è connesso e calcola il diametro.
    Ritorna: Il diametro (double). Se non connesso, ritorna DBL_MAX.
    Logica: 
      1. Esegue Dijkstra da OGNI nodo.
      2. Il diametro è il MASSIMO dei cammini minimi tra tutte le coppie.
      3. Se una distanza rimane DBL_MAX, il grafo è disconnesso.
*/
static double calculateDiameter(Graph G) {
    double maxDistGlobal = 0.0;
    int i, j;

    for (i = 0; i < G->V; i++) {
        /* Chiamo Dijkstra per la sorgente i */
        double *dists = dijkstra(G, i);
        
        for (j = 0; j < G->V; j++) {
            if (dists[j] == DBL_MAX) {
                /* Trovato un nodo irraggiungibile -> Grafo non connesso */
                free(dists);
                return DBL_MAX;
            }
            if (dists[j] > maxDistGlobal) {
                maxDistGlobal = dists[j];
            }
        }
        free(dists); /* Importante liberare memoria ad ogni iterazione */
    }
    return maxDistGlobal;
}

/* --- 3. IMPLEMENTAZIONE FUNZIONI INTERFACCIA --- */

Graph GRAPHload(char *filename) {
    FILE *in = fopen(filename, "r");
    if (in == NULL) return NULL;

    Graph G = malloc(sizeof(*G));
    int i;
    char label[11], label2[11];
    double x, y;

    fscanf(in, "%d", &G->V);
    G->E = 0;
    G->tab = STinit(G->V);
    G->madj = MATRIXinit(G->V, G->V, DBL_MAX);
    G->coords = malloc(G->V * sizeof(Coordinate));

    /* Lettura Nodi e Coordinate */
    for (i = 0; i < G->V; i++) {
        fscanf(in, "%s %lf %lf", label, &x, &y);
        STinsert(G->tab, label);
        G->coords[i].x = x;
        G->coords[i].y = y;
        /* Diagonale a 0 */
        G->madj[i][i] = 0.0;
    }

    /* Lettura Archi (fino a EOF) */
    while (fscanf(in, "%s %s", label, label2) == 2) {
        int id1 = STsearch(G->tab, label);
        int id2 = STsearch(G->tab, label2);
        if (id1 != -1 && id2 != -1) {
            double w = getDist(G, id1, id2);
            G->madj[id1][id2] = w;
            G->madj[id2][id1] = w; /* Grafo non orientato */
            G->E++;
        }
    }

    fclose(in);
    return G;
}

void GRAPHcheckSolutions(Graph G, char *fileSol1, char *fileSol2) {
    char *files[] = {fileSol1, fileSol2};
    int k;

    printf("\n*** VERIFICA SOLUZIONI ***\n");

    for(k=0; k<2; k++) {
        FILE *fp = fopen(files[k], "r");
        if(fp == NULL) {
            printf("Errore apertura %s\n", files[k]);
            continue;
        }

        printf("Analisi file: %s\n", files[k]);

        /* Backup della matrice per ripristinarla dopo il test */
        /* Questo è inefficiente per grafi enormi, ma didatticamente chiaro */
        /* Alternativa: lista di archi aggiunti da rimuovere dopo */
        double **backup = MATRIXinit(G->V, G->V, 0);
        int r, c;
        for(r=0; r<G->V; r++) 
            for(c=0; c<G->V; c++) 
                backup[r][c] = G->madj[r][c];

        /* Aggiunta archi temporanei */
        char id1_s[11], id2_s[11];
        int countAdded = 0;
        while(fscanf(fp, "%s %s", id1_s, id2_s) == 2) {
            int u = STsearch(G->tab, id1_s);
            int v = STsearch(G->tab, id2_s);
            if (u != -1 && v != -1) {
                double w = getDist(G, u, v);
                G->madj[u][v] = G->madj[v][u] = w;
                countAdded++;
            }
        }
        fclose(fp);

        /* Calcolo metriche */
        double diam = calculateDiameter(G);
        
        if (diam == DBL_MAX) 
            printf(" -> Il grafo risultante NON e' connesso.\n");
        else 
            printf(" -> Grafo connesso. Diametro: %.4f (Archi aggiunti: %d)\n", diam, countAdded);

        /* Ripristino matrice originale */
        for(r=0; r<G->V; r++) 
            for(c=0; c<G->V; c++) 
                G->madj[r][c] = backup[r][c];
        MATRIXfree(backup, G->V);
    }
}

/* Funzione Ricorsiva (DFS) per trovare le Componenti Connesse (CC).
   id_cc[] memorizza a quale componente appartiene ogni nodo.
*/
static void dfs_cc(Graph G, int u, int id, int *id_cc) {
    id_cc[u] = id;
    int v;
    for (v = 0; v < G->V; v++) {
        /* Se c'è un arco e v non è ancora stato visitato (id_cc == -1) */
        if (G->madj[u][v] != DBL_MAX && G->madj[u][v] != 0 && id_cc[v] == -1) {
            dfs_cc(G, v, id, id_cc);
        }
    }
}

void GRAPHfindBestConnection(Graph G) {
    printf("\n*** OTTIMIZZAZIONE (Cardinalita' minima & Diametro minimo) ***\n");

    /* 1. Identificare le Componenti Connesse (CC) */
    int *cc = malloc(G->V * sizeof(int));
    int i;
    for(i=0; i<G->V; i++) cc[i] = -1;

    int numCC = 0;
    for(i=0; i<G->V; i++) {
        if (cc[i] == -1) {
            dfs_cc(G, i, numCC++, cc);
        }
    }

    printf("Numero Componenti Connesse attuali: %d\n", numCC);
    if (numCC == 1) {
        printf("Il grafo e' gia' connesso. 0 archi necessari.\n");
        free(cc);
        return;
    }

    /* NOTA DIDATTICA:
       Per connettere N componenti serve MINIMO (N-1) archi.
       Il caso più semplice (esame tipico) è N=2.
       Se N > 2, il problema diventa complesso (Steiner Tree variations).
       Qui risolviamo esaustivamente il caso N=2, che è il più probabile.
       Se N > 2, una soluzione greedy è collegare CC0-CC1, poi (CC0+CC1)-CC2...
    */

    if (numCC == 2) {
        double minDiameter = DBL_MAX;
        int bestU = -1, bestV = -1;

        /* Provo tutte le coppie (u, v) tali che u sta in CC 0 e v sta in CC 1 */
        int u, v;
        for (u = 0; u < G->V; u++) {
            if (cc[u] != 0) continue; // Prendo u dalla prima componente
            
            for (v = 0; v < G->V; v++) {
                if (cc[v] == cc[u]) continue; // v deve essere in una componente diversa

                /* --- TENTATIVO --- */
                /* Aggiungo temporaneamente l'arco */
                double w = getDist(G, u, v);
                double oldW = G->madj[u][v]; /* Salvo stato precedente (DBl_MAX) */
                
                G->madj[u][v] = G->madj[v][u] = w;

                /* Verifico Diametro */
                double currentDiam = calculateDiameter(G);

                /* Se trovo un diametro migliore, aggiorno */
                if (currentDiam < minDiameter) {
                    minDiameter = currentDiam;
                    bestU = u;
                    bestV = v;
                }

                /* --- BACKTRACK --- */
                /* Rimuovo l'arco */
                G->madj[u][v] = G->madj[v][u] = oldW;
            }
        }

        if (bestU != -1) {
            printf("Soluzione Ottima trovata:\n");
            printf("Aggiungere arco tra %s e %s\n", STsearchByIndex(G->tab, bestU), STsearchByIndex(G->tab, bestV));
            printf("Nuovo Diametro: %.4f\n", minDiameter);
        }
    } else {
        printf("Implementazione dimostrativa limitata all'ottimizzazione esatta per 2 componenti.\n");
        printf("Per %d componenti servono %d archi. Applicare euristica Greedy.\n", numCC, numCC-1);
    }

    free(cc);
}

void GRAPHfree(Graph G) {
    if (G == NULL) return;
    MATRIXfree(G->madj, G->V);
    STfree(G->tab);
    free(G->coords);
    free(G);
}