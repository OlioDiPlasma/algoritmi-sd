#include "esame.h"

/* ============================================================================
 * GESTIONE STRUTTURE E FILE
 * ============================================================================ */

/* * Funzione: init_grafo
 * Scopo: Allocazione dinamica della matrice.
 * Dettaglio: Usiamo calloc per garantire che la matrice sia piena di zeri (nessun arco).
 */
Grafo* init_grafo(int V) {
    Grafo *G = (Grafo*)malloc(sizeof(Grafo));
    G->V = V;
    G->E = 0;
    G->madj = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        G->madj[i] = (int*)calloc(V, sizeof(int));
    }
    return G;
}

void free_grafo(Grafo *G) {
    if (G == NULL) return;
    for (int i = 0; i < G->V; i++) free(G->madj[i]);
    free(G->madj);
    free(G);
}

void free_cicli(Ciclo *c, int n) {
    if (c == NULL) return;
    for(int i=0; i<n; i++) free(c[i].nodi);
    free(c);
}

Grafo* load_grafo(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    int V, E, s, d;
    if (fscanf(f, "%d %d", &V, &E) != 2) return NULL;
    Grafo *G = init_grafo(V);
    G->E = E;
    for (int i = 0; i < E; i++) {
        fscanf(f, "%d %d", &s, &d);
        if (s >= 0 && s < V && d >= 0 && d < V) G->madj[s][d] = 1;
    }
    fclose(f);
    return G;
}

Ciclo* load_cicli(char *filename, int *num_cicli) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    Ciclo *elenco = NULL;
    int count = 0, len;
    while (fscanf(f, "%d", &len) == 1) {
        elenco = (Ciclo*)realloc(elenco, (count + 1) * sizeof(Ciclo));
        elenco[count].len = len;
        elenco[count].id = count;
        elenco[count].nodi = (int*)malloc(len * sizeof(int));
        for (int i = 0; i < len; i++) fscanf(f, "%d", &elenco[count].nodi[i]);
        count++;
    }
    *num_cicli = count;
    fclose(f);
    return elenco;
}

/* ============================================================================
 * 6.2.3 VERIFICA
 * ============================================================================ */
int verifica_ciclo(Grafo *G, int *vett, int len) {
    if (len < 2) return 0;
    for (int i = 0; i < len; i++) {
        int u = vett[i];
        int v = vett[(i + 1) % len]; // Modulo per tornare all'inizio
        if (u < 0 || u >= G->V || v < 0 || v >= G->V) return 0;
        if (G->madj[u][v] == 0) return 0; // Arco mancante
    }
    return 1;
}

/* ============================================================================
 * 6.2.4 OTTIMIZZAZIONE
 * ============================================================================ */

/* * Variabili STATIC:
 * Le dichiariamo qui fuori dalle funzioni ma con 'static'.
 * 'static' significa che sono visibili SOLO dentro questo file esame.c.
 * Il main non può vederle o modificarle per sbaglio. Ottimo per l'incapsulamento.
 */
static int *copertura_globale; 
static int V_globale;

/*
 * Funzione ausiliaria (privata) per controllare lo stato della copertura.
 */
static int check_status(int *disjoint_out) {
    int dis = 1;
    for (int i = 0; i < V_globale; i++) {
        if (copertura_globale[i] == 0) return 0; // Incompleta
        if (copertura_globale[i] > 1) dis = 0;   // Sovrapposta
    }
    *disjoint_out = dis;
    return 1; // Completa
}

/*
 * Funzione Ricorsiva (cuore dell'algoritmo)
 */
static void solve_ric(Ciclo *cicli, int n_cicli, int pos, int *att_sol, int cnt, Soluzione *best) {
    
    // Pruning: Se abbiamo già usato più cicli del best, ci fermiamo.
    if (cnt >= best->count) return;

    if (pos == n_cicli) {
        int dis = 0;
        if (check_status(&dis)) {
            // Trovata soluzione valida e migliore (grazie al pruning cnt < best o disgiunta migliore)
            // Criterio esatto: minor numero cicli, a parità preferisco disgiunta
            if (cnt < best->count || (cnt == best->count && dis > best->disjoint)) {
                best->count = cnt;
                best->disjoint = dis;
                for (int i = 0; i < cnt; i++) best->cicli_scelti[i] = att_sol[i];
            }
        }
        return;
    }

    // Strategia: Provo PRIMA a prendere il ciclo (greedy approach spesso aiuta a trovare prima una soluzione)
    // Ma l'ordine standard è: Escludo / Includo.
    
    // 1. NON PRENDO il ciclo 'pos'
    solve_ric(cicli, n_cicli, pos + 1, att_sol, cnt, best);

    // 2. PRENDO il ciclo 'pos'
    // Modifico stato
    att_sol[cnt] = pos;
    for(int i=0; i<cicli[pos].len; i++) copertura_globale[cicli[pos].nodi[i]]++;
    
    // Ricorsione
    solve_ric(cicli, n_cicli, pos + 1, att_sol, cnt + 1, best);
    
    // Backtracking (ripristino stato)
    for(int i=0; i<cicli[pos].len; i++) copertura_globale[cicli[pos].nodi[i]]--;
}

/*
 * Wrapper function pubblica.
 * Questa è l'unica funzione che il main chiama per risolvere il problema.
 * Si occupa di allocare la memoria temporanea necessaria per la ricorsione.
 */
Soluzione trova_copertura_ottima(Grafo *G, Ciclo *cicli, int n_cicli) {
    Soluzione best;
    
    // Inizializzazione Best Sol con valori "pessimi"
    best.count = n_cicli + 1; 
    best.disjoint = 0;
    best.cicli_scelti = (int*)malloc(n_cicli * sizeof(int)); // Allocazione risultato

    // Allocazione strutture ausiliarie per la ricorsione
    int *att_sol = (int*)malloc(n_cicli * sizeof(int));
    copertura_globale = (int*)calloc(G->V, sizeof(int));
    V_globale = G->V;

    // Lancio ricorsione
    solve_ric(cicli, n_cicli, 0, att_sol, 0, &best);

    // Pulizia memoria ausiliaria
    free(att_sol);
    free(copertura_globale);

    return best;
}