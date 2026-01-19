#include "grafo.h"
#include <string.h>

// --- Funzioni di supporto ---

Graph *initGraph(int V) {
    Graph *G = malloc(sizeof(Graph));
    G->V = V;
    G->E = 0;
    G->madj = malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        G->madj[i] = calloc(V, sizeof(int));
    }
    return G;
}

void freeGraph(Graph *G) {
    if (!G) return;
    for (int i = 0; i < G->V; i++) free(G->madj[i]);
    free(G->madj);
    free(G);
}

Packing *allocPacking(int max_size) {
    Packing *p = malloc(sizeof(Packing));
    p->n_triangles = 0;
    p->tris = malloc(max_size * sizeof(Triangle));
    return p;
}

void freePacking(Packing *p) {
    if (!p) return;
    free(p->tris);
    free(p);
}

// --- Acquisizione ---

Graph *loadGraph(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    int V, u, v;
    if (fscanf(f, "%d", &V) != 1) { fclose(f); return NULL; }

    Graph *G = initGraph(V);

    // Legge coppie fino alla fine del file
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            if (G->madj[u][v] == 0) { // Evita duplicati se ci sono
                G->madj[u][v] = G->madj[v][u] = 1;
                G->E++;
            }
        }
    }
    fclose(f);
    return G;
}

// --- Problema di Verifica ---

int verifySolution(Graph *G, const char *sol_filename) {
    FILE *f = fopen(sol_filename, "r");
    if (!f) {
        printf("Errore apertura file soluzione.\n");
        return 0; // Invalid
    }

    int n_sol;
    if (fscanf(f, "%d", &n_sol) != 1) { fclose(f); return 0; }

    int *visited = calloc(G->V, sizeof(int));
    int valid = 1;
    int u, v, w;

    printf("Verifica in corso su %d triangoli...\n", n_sol);

    for (int i = 0; i < n_sol; i++) {
        if (fscanf(f, "%d %d %d", &u, &v, &w) != 3) {
            valid = 0; break;
        }
        
        // 1. Check indici validi
        if (u < 0 || u >= G->V || v < 0 || v >= G->V || w < 0 || w >= G->V) {
            printf("Errore: vertici fuori range (%d, %d, %d)\n", u, v, w);
            valid = 0; break;
        }

        // 2. Check triangolo reale (tutti e 3 gli archi devono esistere)
        if (!G->madj[u][v] || !G->madj[u][w] || !G->madj[v][w]) {
            printf("Errore: non e' un triangolo valido (%d, %d, %d)\n", u, v, w);
            valid = 0; break;
        }

        // 3. Check disgiunti (vertici mai usati prima)
        if (visited[u] || visited[v] || visited[w]) {
            printf("Errore: vertici sovrapposti/ripetuti (%d, %d, %d)\n", u, v, w);
            valid = 0; break;
        }

        visited[u] = 1;
        visited[v] = 1;
        visited[w] = 1;
    }

    free(visited);
    fclose(f);
    return valid;
}

// --- Problema di Ricerca e Ottimizzazione ---

// Funzione ricorsiva di backtracking
// candidates: array di tutti i possibili triangoli nel grafo
// n_cand: numero totale di candidati
// idx: indice corrente in candidates
// curr_pack: soluzione corrente
// best_pack: miglior soluzione trovata finora
// used: array booleano per i vertici occupati
void backtrack(Triangle *candidates, int n_cand, int idx, 
               Packing *curr_pack, Packing *best_pack, int *used, int V) {

    // Pruning: Se i triangoli correnti + quelli rimanenti non possono superare il best, stop
    if (curr_pack->n_triangles + (n_cand - idx) <= best_pack->n_triangles) {
        return;
    }

    // Base case: nessun candidato rimasto da controllare
    // aggiornamento soluzione best pack
    if (idx == n_cand) {
        if (curr_pack->n_triangles > best_pack->n_triangles) {
            best_pack->n_triangles = curr_pack->n_triangles;
            for(int i=0; i<curr_pack->n_triangles; i++) {
                best_pack->tris[i] = curr_pack->tris[i];
            }
        }
        return;
    }

    Triangle t = candidates[idx];

    // Opzione 1: Prova a includere il triangolo t (se i suoi vertici sono liberi)
    if (!used[t.u] && !used[t.v] && !used[t.w]) {
        // Do
        used[t.u] = used[t.v] = used[t.w] = 1;
        curr_pack->tris[curr_pack->n_triangles] = t;
        curr_pack->n_triangles++;

        // Recur
        backtrack(candidates, n_cand, idx + 1, curr_pack, best_pack, used, V);

        // Undo (Backtrack)
        curr_pack->n_triangles--;
        used[t.u] = used[t.v] = used[t.w] = 0;
    }

    // Opzione 2: Non includere il triangolo t (passa al prossimo)
    backtrack(candidates, n_cand, idx + 1, curr_pack, best_pack, used, V);
}

Packing *findMaxTrianglePacking(Graph *G) {
    // 1. Generare tutti i possibili triangoli geometrici nel grafo
    // Un triangolo e' definito da u < v < w
    int max_possible = G->V * G->V; // Stima lasca
    Triangle *candidates = malloc(max_possible * sizeof(Triangle));
    int n_cand = 0;

    for (int i = 0; i < G->V; i++) {
        for (int j = i + 1; j < G->V; j++) {
            if (G->madj[i][j]) {
                for (int k = j + 1; k < G->V; k++) {         
                    if (G->madj[i][k] && G->madj[j][k]) {
                        // Trovato un triangolo
                        if (n_cand >= max_possible) {
                            max_possible *= 2;
                            candidates = realloc(candidates, max_possible * sizeof(Triangle));
                        }
                        candidates[n_cand].u = i;
                        candidates[n_cand].v = j;
                        candidates[n_cand].w = k;
                        n_cand++;
                    }
                }
            }
        }
    }

    printf("Trovati %d possibili triangoli candidati nel grafo.\nInizio ottimizzazione...\n", n_cand);

    // 2. Preparare strutture per backtracking
    Packing *best_pack = allocPacking(G->V / 3); // Max teorico
    Packing *curr_pack = allocPacking(G->V / 3);
    int *used = calloc(G->V, sizeof(int));

    // 3. Lanciare backtracking
    backtrack(candidates, n_cand, 0, curr_pack, best_pack, used, G->V);

    // Pulizia
    free(candidates);
    freePacking(curr_pack);
    free(used);

    return best_pack;
}

void saveSolution(Packing *p, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "%d\n", p->n_triangles);
    for (int i = 0; i < p->n_triangles; i++) {
        fprintf(f, "%d %d %d\n", p->tris[i].u, p->tris[i].v, p->tris[i].w);
    }
    fclose(f);
}