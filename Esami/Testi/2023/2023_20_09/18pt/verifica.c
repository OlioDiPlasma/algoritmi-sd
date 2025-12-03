#include "Graph.h"
#include "uf.h" 
#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: verifica_soluzione
 * ======================================================================================
 * SCOPO:
 * Verifica se una soluzione proposta su file è un "k-capacitated tree partitioning" valido.
 * Deve controllare:
 * 1. Ogni componente è un Albero (connesso, aciclico, Archi = Vertici - 1).
 * 2. Ogni componente ha almeno k vertici.
 * 3. I componenti sono disgiunti (ogni vertice appare una sola volta).
 * 4. Tutti i vertici del grafo sono coperti.
 *
 * PARAMETRI:
 * - Graph G: Il grafo originale caricato in memoria.
 * - int k: Il parametro di capacità minima.
 * - const char *file_soluzione: Percorso del file contenente la soluzione da verificare.
 *
 * CONCETTI CHIAVE:
 * Union-Find (UF): Usiamo questa struttura dati per verificare efficientemente due proprietà:
 * a) Ciclicità: Se aggiungiamo un arco tra due nodi già nello stesso set -> Ciclo.
 * b) Connessione: Alla fine, tutti i nodi di un componente devono avere la stessa radice.
 * ======================================================================================
 */
int verifica_soluzione(Graph G, int k, const char *file_soluzione) {
    /* --- 1. APERTURA FILE SOLUZIONE --- */
    FILE *fp = fopen(file_soluzione, "r");
    if (!fp) return 0;

    int num_componenti;
    fscanf(fp, "%d", &num_componenti);

    /* Array per tracciare globalmente quali vertici sono stati coperti.
     * calloc inizializza a 0.
     */
    int *vertex_covered = calloc(GRAPHgetV(G), sizeof(int)); 
    int total_vertices_covered = 0;

    /* --- 2. CICLO SUI COMPONENTI --- */
    for (int i = 0; i < num_componenti; i++) {
        int n_edges;
        fscanf(fp, "%d", &n_edges);
        
        /* Strutture temporanee per il singolo componente corrente */
        int *comp_vertices = calloc(GRAPHgetV(G), sizeof(int));
        int comp_v_count = 0;
        
        /* Inizializzazione Union-Find locale per questo componente.
         * UF_init restituisce un puntatore opaco (ADT).
         */
        UF uf_comp = UF_init(GRAPHgetV(G));
        
        int u, v, w;
        int cicli = 0;

        /* --- 3. LETTURA ARCHI DEL COMPONENTE --- */
        for (int j = 0; j < n_edges; j++) {
            fscanf(fp, "%d %d %d", &u, &v, &w);

            /* Check integrità: L'arco esiste nel grafo originale col peso corretto? */
            if (GRAPHcheckEdge(G, u, v) != w) {
                printf("Errore: Arco %d-%d non esistente o peso errato.\n", u, v);
                /* Pulizia memoria prima del return anticipato */
                UF_free(uf_comp); 
                free(comp_vertices);
                free(vertex_covered);
                fclose(fp);
                return 0;
            }

            /* Tracciamento vertici nel componente corrente */
            if (comp_vertices[u] == 0) { comp_vertices[u]=1; comp_v_count++; }
            if (comp_vertices[v] == 0) { comp_vertices[v]=1; comp_v_count++; }

            /* Check disgiunzione: Il vertice è già stato usato in un altro componente? */
            if (vertex_covered[u] || vertex_covered[v]) {
                printf("Errore: Vertici non disgiunti tra componenti.\n");
                UF_free(uf_comp);
                free(comp_vertices);
                free(vertex_covered);
                fclose(fp);
                return 0;
            }

            /* Check Cicli: Se UF_union ritorna 0, i nodi erano già connessi -> Ciclo */
            if (!UF_union(uf_comp, u, v)) {
                cicli = 1;
            }
        }

        /* --- 4. AGGIORNAMENTO STATO GLOBALE --- */
        for(int v_idx=0; v_idx<GRAPHgetV(G); v_idx++) {
            if (comp_vertices[v_idx]) {
                vertex_covered[v_idx] = 1; /* Marco come coperto globalmente */
                total_vertices_covered++;
            }
        }

        /* --- 5. VERIFICHE VALIDITÀ COMPONENTE --- */
        
        /* A. Cardinalità minima k */
        if (comp_v_count < k) {
            printf("Errore: Componente %d ha %d vertici (minimo %d).\n", i, comp_v_count, k);
            UF_free(uf_comp); free(comp_vertices); free(vertex_covered); fclose(fp); return 0;
        }
        
        /* B. Proprietà topologica Albero: Archi == Vertici - 1 */
        if (n_edges != comp_v_count - 1) {
             printf("Errore: Componente %d non è un albero (Archi != V-1).\n", i);
             UF_free(uf_comp); free(comp_vertices); free(vertex_covered); fclose(fp); return 0;
        }
        
        /* C. Assenza Cicli */
        if (cicli) {
            printf("Errore: Componente %d contiene cicli.\n", i);
            UF_free(uf_comp); free(comp_vertices); free(vertex_covered); fclose(fp); return 0;
        }
        
        /* D. Connessione: Tutti i vertici devono appartenere allo stesso set nella UF */
        int root = -1;
        int connesso = 1;
        for(int v_idx=0; v_idx<GRAPHgetV(G); v_idx++) {
            if (comp_vertices[v_idx]) {
                /* Primo vertice trovato diventa il riferimento (root) */
                if (root == -1) root = UF_find(uf_comp, v_idx);
                /* Gli altri devono avere la stessa root */
                else if (UF_find(uf_comp, v_idx) != root) {
                    connesso = 0; break;
                }
            }
        }
        if (!connesso) {
            printf("Errore: Componente %d non connesso.\n", i);
            UF_free(uf_comp); free(comp_vertices); free(vertex_covered); fclose(fp); return 0;
        }

        /* Pulizia memoria locale al componente */
        free(comp_vertices);
        UF_free(uf_comp);
    }

    /* --- 6. VERIFICA FINALE GLOBALE --- */
    /* Tutti i vertici del grafo originale devono essere stati assegnati a un componente */
    if (total_vertices_covered != GRAPHgetV(G)) {
        printf("Errore: Non tutti i vertici del grafo sono coperti.\n");
        free(vertex_covered); fclose(fp); return 0;
    }

    /* Chiusura e successo */
    fclose(fp);
    free(vertex_covered);
    printf("Soluzione Valida!\n");
    return 1;
}