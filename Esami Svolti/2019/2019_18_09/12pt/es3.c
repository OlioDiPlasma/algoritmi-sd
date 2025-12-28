#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* Per INT_MAX */

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI
 * ======================================================================================
 */
typedef struct {
    int v;      // Primo vertice dell'arco
    int w;      // Secondo vertice dell'arco
    int wt;     // Peso dell'arco (weight)
} Edge;

/*
 * ======================================================================================
 * NOME FUNZIONE: dfs
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria per attraversare il grafo (Depth First Search).
 * Serve a verificare la connettività: partendo da un nodo, quanti altri nodi riesco a
 * raggiungere usando SOLO gli archi selezionati?
 *
 * PARAMETRI:
 * - int u: Il nodo corrente che stiamo visitando.
 * - int nV: Numero totale di vertici (per i limiti).
 * - Edge *subset: L'array degli archi che stiamo testando (la combinazione corrente).
 * - int subsetSize: Quanti archi ci sono nel subset (sarà sempre nV-1).
 * - int *visited: Array di flag (0 o 1) per tenere traccia dei nodi già visitati.
 *
 * CONCETTI CHIAVE:
 * - Grafo Non Orientato: Se l'arco è (v, w), posso attraversarlo sia da v a w che da w a v.
 * ======================================================================================
 */
void dfs(int u, int nV, Edge *subset, int subsetSize, int *visited) {
    visited[u] = 1; // Marco il nodo corrente come visitato

    /* Scorro tutti gli archi del sottoinsieme per vedere se c'è un collegamento */
    for (int i = 0; i < subsetSize; i++) {
        int neighbor = -1;

        /* Controllo se l'arco i-esimo è collegato al nodo u */
        if (subset[i].v == u) {
            neighbor = subset[i].w;
        } else if (subset[i].w == u) {
            neighbor = subset[i].v;
        }

        /* Se ho trovato un vicino e non l'ho ancora visitato, vado in profondità */
        if (neighbor != -1 && !visited[neighbor]) {
            dfs(neighbor, nV, subset, subsetSize, visited);
        }
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: checkSpanningTree (Richiesta dall'esercizio - 2 punti)
 * ======================================================================================
 * SCOPO:
 * Verifica se un dato insieme di archi forma un albero ricoprente valido.
 *
 * PARAMETRI:
 * - Edge *subset: L'array di archi da verificare.
 * - int subsetSize: Numero di archi (deve essere nV-1).
 * - int nV: Numero totale di vertici del grafo.
 *
 * RITORNA:
 * - 1 (Vero) se è un albero ricoprente, 0 (Falso) altrimenti.
 *
 * CONCETTI CHIAVE:
 * - Definizione di Albero Ricoprente: Un grafo con V nodi e V-1 archi è un albero
 * SE E SOLO SE è connesso (cioè da un nodo posso arrivare a tutti gli altri).
 * Non serve controllare i cicli esplicitamente: se ho V nodi, V-1 archi e sono connesso,
 * matematicamente non possono esserci cicli.
 *
 * GESTIONE MEMORIA:
 * - Allocazione dinamica di 'visited' con calloc (che inizializza a 0).
 * - Importante: free(visited) prima di ritornare per evitare memory leaks.
 * ======================================================================================
 */
int checkSpanningTree(Edge *subset, int subsetSize, int nV) {
    /* Un albero ricoprente su nV nodi deve avere esattamente nV-1 archi */
    if (subsetSize != nV - 1) {
        return 0;
    }

    /* --- 1. ALLOCAZIONE ARRAY VISITED --- */
    /* calloc alloca memoria e setta tutti i bit a 0 (molto comodo per i flag) */
    int *visited = (int*)calloc(nV, sizeof(int));
    if (visited == NULL) exit(1);

    /* --- 2. VERIFICA CONNETTIVITÀ --- */
    /* Lancio la DFS partendo dal nodo 0 */
    dfs(0, nV, subset, subsetSize, visited);

    /* Conto quanti nodi ho raggiunto */
    int count = 0;
    for (int i = 0; i < nV; i++) {
        if (visited[i]) count++;
    }

    /* --- 3. PULIZIA E RITORNO --- */
    free(visited); // Fondamentale!

    /* Se ho visitato tutti i nodi, è connesso -> è uno Spanning Tree */
    if (count == nV) return 1;
    return 0;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: combinazioni (Motore Ricorsivo)
 * ======================================================================================
 * SCOPO:
 * Genera tutte le combinazioni semplici di nV-1 archi presi dagli archi totali.
 *
 * PARAMETRI:
 * - int pos: Indice corrente nell'array soluzione (da 0 a k-1).
 * - int start: Indice da cui partire nell'array degli archi originali (per evitare duplicati).
 * - Edge *edges: Array di tutti gli archi disponibili.
 * - int nE: Numero totale di archi.
 * - Edge *sol: Array temporaneo dove costruiamo la combinazione corrente.
 * - int k: Dimensione della combinazione (nV - 1).
 * - int nV: Numero vertici.
 * - int *minWeight: Puntatore al peso minimo trovato finora (per aggiornarlo).
 * ======================================================================================
 */
void combinazioni(int pos, int start, Edge *edges, int nE, Edge *sol, int k, int nV, int *minWeight) {
    
    /* --- 1. CASO BASE (TERMINAZIONE) --- */
    /* Abbiamo selezionato k archi. Verifichiamo se è una soluzione valida. */
    if (pos == k) {
        if (checkSpanningTree(sol, k, nV)) {
            /* Calcolo il peso totale di questa combinazione */
            int currentWeight = 0;
            for (int i = 0; i < k; i++) {
                currentWeight += sol[i].wt;
            }

            /* Se è il peso minore trovato finora, aggiorno */
            if (currentWeight < *minWeight) {
                *minWeight = currentWeight;
            }
        }
        return;
    }

    /* --- 2. PASSO RICORSIVO --- */
    /*
     * Ciclo standard delle Combinazioni Semplici:
     * i va da 'start' fino alla fine, ma ci fermiamo prima se non ci sono 
     * abbastanza elementi rimasti per riempire il resto della soluzione (ottimizzazione).
     */
    for (int i = start; i < nE; i++) {
        /* Inserisco l'arco i-esimo nella soluzione temporanea */
        sol[pos] = edges[i]; // Assegnamento di struttura (copia v, w, wt)

        /* Ricorsione: avanzo di posizione (pos+1) e avanzo l'indice di partenza (i+1) */
        combinazioni(pos + 1, i + 1, edges, nE, sol, k, nV, minWeight);
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: calcolaMST (Richiesta dall'esercizio - 4 punti)
 * ======================================================================================
 * SCOPO:
 * Wrapper che prepara i dati e lancia la ricorsione combinatoria.
 *
 * PARAMETRI:
 * - Edge *edges: Vettore degli archi.
 * - int nV: Numero vertici.
 * - int nE: Numero archi.
 *
 * RITORNA:
 * - int: Il peso dell'albero ricoprente minimo.
 * ======================================================================================
 */
int calcolaMST(Edge *edges, int nV, int nE) {
    /* * Un MST ha sempre nV - 1 archi.
     * Dobbiamo scegliere k = nV - 1 archi tra gli nE disponibili.
     */
    int k = nV - 1;

    /* Allocazione vettore soluzione temporanea */
    Edge *sol = (Edge*)malloc(k * sizeof(Edge));
    if (sol == NULL) exit(1);

    /* Inizializzo il minimo a un valore altissimo (infinito) */
    int minWeight = INT_MAX;

    /* Lancio il generatore di combinazioni */
    combinazioni(0, 0, edges, nE, sol, k, nV, &minWeight);

    /* Libero la memoria di supporto */
    free(sol);

    return minWeight;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DATI DI PROVA --- */
    /* * Creiamo un grafo semplice a triangolo con una "coda":
     * 0 --(1)-- 1
     * |       /
     *(4)    (2)
     * |     /
     * 2 --(3)-- 3
     * * Vertici: 4 (0, 1, 2, 3). Archi necessari per MST: 3.
     */
    int nV = 4;
    int nE = 5;
    
    // Alloco array di archi
    Edge *edges = (Edge*)malloc(nE * sizeof(Edge));
    
    // Inizializzo gli archi (v, w, peso)
    edges[0] = (Edge){0, 1, 1}; // Arco leggero
    edges[1] = (Edge){1, 2, 2}; // Arco medio
    edges[2] = (Edge){2, 3, 3}; // Arco
    edges[3] = (Edge){0, 2, 4}; // Arco pesante (crea ciclo 0-1-2)
    edges[4] = (Edge){1, 3, 10}; // Arco molto pesante (inutile)

    printf("--- Inizio Calcolo MST (Forza Bruta) ---\n");
    printf("Numero Vertici: %d, Numero Archi Totali: %d\n", nV, nE);

    /* --- 2. CALCOLO --- */
    int pesoMST = calcolaMST(edges, nV, nE);

    /* --- 3. RISULTATO --- */
    if (pesoMST == INT_MAX) {
        printf("Il grafo non è connesso, impossibile trovare MST.\n");
    } else {
        printf("Peso Minimo trovato: %d\n", pesoMST);
        /* * Analisi manuale attesa:
         * Archi migliori: (0,1) peso 1, (1,2) peso 2, (2,3) peso 3.
         * Totale = 1 + 2 + 3 = 6.
         * L'arco (0,2) peso 4 verrebbe scartato perché 1+2 < 4.
         */
    }

    /* --- 4. PULIZIA --- */
    free(edges);
    
    return 0;
}