#include "grafo.h"
#include <string.h>

/* =========================================================================
 * SEZIONE 1: GESTIONE MEMORIA E I/O
 * ========================================================================= */

/*
 * Funzione: leggiGrafo
 * Scopo: Implementazione della lettura da file.
 * Dettaglio: Alloca la struttura e poi la matrice.
 */
Grafo* leggiGrafo(const char *nomeFile) {
    FILE *f = fopen(nomeFile, "r");
    if (f == NULL) {
        perror("Errore apertura file grafo");
        exit(1);
    }

    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    if (g == NULL) { perror("Malloc struct fallita"); exit(1); }

    // Leggiamo V (numero vertici)
    fscanf(f, "%d", &g->V);
    g->E = 0;

    // Allocazione Matrice di Adiacenza (Array di puntatori)
    g->adj = (int**) malloc(g->V * sizeof(int*));
    for (int i = 0; i < g->V; i++) {
        // Calloc inizializza a 0 (false)
        g->adj[i] = (int*) calloc(g->V, sizeof(int));
    }

    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        // Grafo non orientato: aggiorno simmetricamente
        if(u < g->V && v < g->V) {
            g->adj[u][v] = 1;
            g->adj[v][u] = 1;
            g->E++;
        }
    }

    fclose(f);
    return g;
}

/*
 * Funzione: liberaGrafo
 * Scopo: Pulizia della memoria HEAP.
 */
void liberaGrafo(Grafo *g) {
    if (g == NULL) return;
    
    for (int i = 0; i < g->V; i++) {
        free(g->adj[i]); // Libero le colonne
    }
    free(g->adj); // Libero le righe (array di puntatori)
    free(g);      // Libero la struttura
}

/* =========================================================================
 * SEZIONE 2: LOGICA DI VERIFICA (DOMINATING SET)
 * ========================================================================= */

/*
 * Funzione Helper (Privata/Static)
 * Scopo: Controlla se l'insieme 'id_insieme' domina tutto il grafo.
 * Concetto Chiave: 'static' significa che questa funzione non può essere chiamata
 * dal main, ma solo dalle funzioni dentro questo file (grafo.c).
 */
static bool isDominatingSet(Grafo *g, int *partizione, int id_insieme) {
    for (int v = 0; v < g->V; v++) {
        // Se v è nell'insieme, è dominato.
        if (partizione[v] == id_insieme) continue;

        // Se v NON è nell'insieme, cerchiamo un vicino che lo sia.
        bool found = false;
        for (int neighbor = 0; neighbor < g->V; neighbor++) {
            if (g->adj[v][neighbor] == 1 && partizione[neighbor] == id_insieme) {
                found = true;
                break;
            }
        }
        if (!found) return false; // Vertice non dominato -> Insieme non valido
    }
    return true;
}

/*
 * Funzione: verificaPropostaFile
 * Scopo: Legge proposta.txt e verifica se è una Domatic Partition.
 */
bool verificaPropostaFile(Grafo *g, const char *nomeFile) {
    FILE *f = fopen(nomeFile, "r");
    if (!f) return false;

    int *partizione = (int*) malloc(g->V * sizeof(int));
    for(int i=0; i<g->V; i++) partizione[i] = -1; // Init

    int max_id = -1;
    int u, id;
    int count = 0;

    while(fscanf(f, "%d %d", &u, &id) == 2) {
        if (u >= 0 && u < g->V) {
            partizione[u] = id;
            if (id > max_id) max_id = id;
            count++;
        }
    }
    fclose(f);

    if (count < g->V) {
        free(partizione);
        return false; // Partizione incompleta
    }

    // Verifica per ogni insieme k trovato
    for (int k = 0; k <= max_id; k++) {
        if (!isDominatingSet(g, partizione, k)) {
            free(partizione);
            return false;
        }
    }

    free(partizione);
    return true;
}

/* =========================================================================
 * SEZIONE 3: OTTIMIZZAZIONE (RICORSIONE)
 * ========================================================================= */

/*
 * Funzione Helper Ricorsiva (Backtracking)
 * Scopo: Assegna colori ai vertici per cercare di formare K insiemi validi.
 */
static bool solveRecursiva(Grafo *g, int v_idx, int K, int *partizione) {
    // Caso Base: Tutti i vertici assegnati
    if (v_idx == g->V) {
        // Controllo validità finale
        for (int i = 0; i < K; i++) {
            if (!isDominatingSet(g, partizione, i)) return false;
        }
        return true;
    }

    // Provo ad assegnare al vertice corrente uno dei K insiemi
    for (int c = 0; c < K; c++) {
        partizione[v_idx] = c;
        // Passo ricorsivo
        if (solveRecursiva(g, v_idx + 1, K, partizione)) return true;
        // Backtrack (implicito, il valore verrà sovrascritto al prossimo loop)
    }

    return false;
}

/*
 * Funzione: trovaDomaticPartitionMassima
 * Scopo: Trova il massimo K possibile.
 */
void trovaDomaticPartitionMassima(Grafo *g) {
    int *partizione = (int*) malloc(g->V * sizeof(int));
    
    // Proviamo K decrescente da V fino a 1
    for (int k = g->V; k >= 1; k--) {
        // Reset partizione per sicurezza
        for(int i=0; i<g->V; i++) partizione[i] = -1;

        if (solveRecursiva(g, 0, k, partizione)) {
            printf("Ottimizzazione completata: Max Domatic Partition size = %d\n", k);
            printf("Soluzione trovata:\n");
            for(int i=0; i<k; i++) {
                printf("  Insieme %d: { ", i);
                for(int v=0; v<g->V; v++) {
                    if(partizione[v] == i) printf("%d ", v);
                }
                printf("}\n");
            }
            free(partizione);
            return;
        }
    }
    free(partizione);
}