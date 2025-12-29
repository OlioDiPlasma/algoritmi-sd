/* * File: graph.c
 * Scopo: Implementazione delle funzioni di gestione grafo e algoritmi.
 */

#include "graph.h"

/* =========================================================================
 * SEZIONE 1: FUNZIONI DI SUPPORTO (Symbol Table e Allocazione)
 * ========================================================================= */

/*
 * Funzione: getIndex
 * Scopo: Converte una stringa (nome) in un intero (indice). Se il nome non esiste, lo aggiunge.
 * Parametri:
 * - st: puntatore alla Symbol Table.
 * - name: stringa da cercare.
 * Concetti Chiave: 
 * - Ricerca Lineare: Scorre l'array per trovare il nome.
 * - Realloc: Se l'array è pieno, raddoppiamo la dimensione (gestione dinamica).
 */
int getIndex(SymbolTable *st, char *name) {
    // 1. Cerco se il nome esiste già
    for (int i = 0; i < st->size; i++) {
        if (strcmp(st->names[i], name) == 0) return i;
    }

    // 2. Se non esiste, devo aggiungerlo. Controllo se ho spazio.
    if (st->size >= st->max_size) {
        // Se è la prima volta (max_size 0), alloco 1, altrimenti raddoppio
        st->max_size = (st->max_size == 0) ? 1 : st->max_size * 2;
        
        // REALLOC: Modifica la dimensione del blocco di memoria puntato.
        // È fondamentale assegnare il risultato a st->names perché l'indirizzo potrebbe cambiare!
        st->names = realloc(st->names, st->max_size * sizeof(char *));
    }

    // 3. Allocazione della stringa specifica
    // strdup è comoda: fa malloc + strcpy in un colpo solo.
    st->names[st->size] = strdup(name); 
    return st->size++; // Ritorno l'indice corrente e POI incremento size
}

/*
 * Funzione: loadGraph
 * Scopo: Legge il file, costruisce la Symbol Table e la Matrice di Adiacenza.
 * Concetti Chiave:
 * - Doppia lettura non necessaria se usiamo una ST dinamica, ma qui dobbiamo 
 * sapere quanti vertici totali ci sono PRIMA di fare la matrice.
 * Approccio scelto: 
 * 1. Leggiamo tutto il file solo per riempire la ST e contare i vertici V.
 * 2. Allochiamo la matrice V x V.
 * 3. Rileggiamo il file per riempire i pesi.
 */
Graph *loadGraph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Errore apertura file grafo"); exit(1); }

    Graph *G = malloc(sizeof(Graph));
    // Inizializzazione Symbol Table vuota
    G->st.names = NULL; 
    G->st.size = 0; 
    G->st.max_size = 0;
    G->E = 0;

    char u_str[MAX_LEN], v_str[MAX_LEN];
    int w;

    // --- PRIMO PASSAGGIO: Costruzione Symbol Table ---
    while (fscanf(fp, "%s %s %d", u_str, v_str, &w) == 3) {
        getIndex(&G->st, u_str);
        getIndex(&G->st, v_str);
    }

    G->V = G->st.size; // Ora sappiamo quanti vertici unici ci sono

    // --- ALLOCAZIONE MATRICE ---
    // Usiamo calloc per inizializzare tutto a 0 automaticamente
    G->adj = malloc(G->V * sizeof(int *));
    for (int i = 0; i < G->V; i++) {
        G->adj[i] = calloc(G->V, sizeof(int));
    }

    // --- SECONDO PASSAGGIO: Riempimento Matrice ---
    rewind(fp); // Riporto il cursore all'inizio del file
    while (fscanf(fp, "%s %s %d", u_str, v_str, &w) == 3) {
        int id1 = getIndex(&G->st, u_str); // Recupero gli indici già creati
        int id2 = getIndex(&G->st, v_str);
        
        // Grafo non orientato: la matrice è simmetrica
        G->adj[id1][id2] = w;
        G->adj[id2][id1] = w;
        G->E++;
    }

    fclose(fp);
    return G;
}

/* =========================================================================
 * SEZIONE 2: ALGORITMI DI VERIFICA
 * ========================================================================= */

/*
 * Funzione ausiliaria: isClique
 * Verifica se un insieme di nodi forma un sottografo completo.
 */
int isClique(Graph *G, int *nodes, int n) {
    // Doppio ciclo annidato: ogni nodo deve essere collegato a tutti gli altri
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // nodes[i] è l'indice reale nella matrice del grafo
            if (G->adj[nodes[i]][nodes[j]] == 0) return 0; // Se peso 0, non c'è arco
        }
    }
    return 1;
}

/*
 * Funzione: checkSolution
 * Scopo: Legge un file con una proposta e verifica se è cricca massimale.
 * Ritorna: 1 (Vero) o 0 (Falso).
 */
int checkSolution(Graph *G, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int *nodes = malloc(G->V * sizeof(int)); // Vettore temporaneo
    int count = 0;
    char name[MAX_LEN];

    // Lettura vertici della proposta
    while (fscanf(fp, "%s", name) == 1 && count < G->V) {
        // Cerco indice senza aggiungere! (dobbiamo scorrere la ST manualmente o modificare getIndex)
        // Per semplicità qui faccio un ciclo manuale rapido
        int id = -1;
        for(int i=0; i<G->st.size; i++) {
            if(strcmp(G->st.names[i], name) == 0) {
                id = i; break;
            }
        }
        if (id != -1) nodes[count++] = id;
    }
    fclose(fp);

    printf("\n--- VERIFICA SOLUZIONE (%s) ---\n", filename);
    printf("Vertici letti: %d\n", count);

    // 1. Verifica Completezza
    if (!isClique(G, nodes, count)) {
        printf("NON e' una cricca (mancano archi interni).\n");
        free(nodes);
        return 0;
    }

    // 2. Verifica Massimalità
    // Una cricca NON è massimale se esiste un vertice esterno adiacente a TUTTI i suoi vertici.
    // Flag 'is_maximal' parte a vero. Se troviamo un nodo esterno che estende la cricca, diventa falso.
    int is_maximal = 1; 

    // Scorro tutti i vertici del grafo (i)
    for (int i = 0; i < G->V; i++) {
        // Controllo se il vertice 'i' è già nella cricca proposta
        int present = 0;
        for (int k = 0; k < count; k++) {
            if (nodes[k] == i) { present = 1; break; }
        }

        // Se 'i' è esterno alla cricca...
        if (!present) {
            // Controlliamo se è collegato a TUTTI i nodi della cricca
            int connected_to_all = 1;
            for (int k = 0; k < count; k++) {
                if (G->adj[i][nodes[k]] == 0) { // Se manca anche solo un arco
                    connected_to_all = 0;
                    break;
                }
            }
            // Se 'i' è collegato a tutti, allora la cricca poteva essere estesa!
            if (connected_to_all) {
                is_maximal = 0;
                printf("NON massimale: puo' essere estesa con %s\n", G->st.names[i]);
                break; 
            }
        }
    }

    if (is_maximal) printf("La soluzione e' una cricca MASSIMALE.\n");
    
    free(nodes);
    return is_maximal;
}
/* =========================================================================
 * SEZIONE 3: RICERCA ED OTTIMIZZAZIONE (Cricche Massimali e TSP)
 * ========================================================================= */

// Variabili globali statiche per la gestione delle cricche
static Clique *allCliques;
static int nCliques;
static int maxCliquesCapacity; 

// --- FUNZIONI DI SUPPORTO PER IL TSP (Permutazioni) ---

/* * Funzione: swap
 * Scopo: Scambia due interi in memoria. Utile per generare permutazioni.
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Funzione: solveTSP (Traveling Salesman Problem)
 * Scopo: Trova ricorsivamente la permutazione dei nodi che massimizza il peso del ciclo.
 * Parametri:
 * - G: Il grafo
 * - nodes: L'array di indici dei nodi della cricca (verrà permutato)
 * - start: L'indice da cui iniziare a permutare (fissiamo lo 0 per evitare rotazioni duplicati)
 * - n: Numero di nodi nella cricca
 * - maxWeight: Puntatore al peso massimo trovato finora
 * - bestPath: Array dove salviamo la sequenza migliore trovata
 */
void solveTSP(Graph *G, int *nodes, int start, int n, int *maxWeight, int *bestPath) {
    // Caso Base: Abbiamo fissato una permutazione completa
    if (start == n) {
        // Calcoliamo il peso di questo percorso
        int currentWeight = 0;
        
        // Somma pesi archi lineari: 0->1, 1->2, ...
        for (int i = 0; i < n - 1; i++) {
            currentWeight += G->adj[nodes[i]][nodes[i+1]];
        }
        // Aggiungi chiusura del cerchio: ultimo -> primo
        currentWeight += G->adj[nodes[n-1]][nodes[0]];

        // Se è migliore del massimo attuale, aggiorniamo
        if (currentWeight > *maxWeight) {
            *maxWeight = currentWeight;
            // Salviamo questa sequenza specifica in bestPath
            // memcpy è veloce: copia n * sizeof(int) byte da nodes a bestPath
            memcpy(bestPath, nodes, n * sizeof(int));
        }
        return;
    }

    // Ricorsione con Backtracking (Permutazioni)
    // Generiamo tutte le permutazioni scambiando l'elemento corrente con i successivi
    for (int i = start; i < n; i++) {
        swap(&nodes[start], &nodes[i]);          // Scambio
        solveTSP(G, nodes, start + 1, n, maxWeight, bestPath); // Ricorsione
        swap(&nodes[start], &nodes[i]);          // Backtrack (rimetto a posto per il prossimo giro)
    }
}


/* --- GESTIONE CRICCHE --- */

void storeClique(Graph *G, int *current_clique, int count) {
    if (nCliques >= maxCliquesCapacity) return;
    allCliques[nCliques].count = count;
    allCliques[nCliques].nodes = malloc(count * sizeof(int));
    memcpy(allCliques[nCliques].nodes, current_clique, count * sizeof(int));
    nCliques++;
}

void findMaximalCliquesRecursive(Graph *G, int *current, int curr_size, int *candidates, int cand_size) {
    if (cand_size == 0) {
        if (curr_size > 0) storeClique(G, current, curr_size);
        return;
    }

    int *new_candidates = malloc(cand_size * sizeof(int));

    for (int i = 0; i < cand_size; i++) {
        int v = candidates[i];
        current[curr_size] = v;

        int new_cand_count = 0;
        for (int j = i + 1; j < cand_size; j++) {
            int u = candidates[j];
            if (G->adj[v][u] != 0) {
                new_candidates[new_cand_count++] = u;
            }
        }
        findMaximalCliquesRecursive(G, current, curr_size + 1, new_candidates, new_cand_count);
    }
    free(new_candidates);
}

void solveCliques(Graph *G) {
    maxCliquesCapacity = G->V * 10; 
    allCliques = malloc(maxCliquesCapacity * sizeof(Clique));
    nCliques = 0;

    int *current = malloc(G->V * sizeof(int));
    int *candidates = malloc(G->V * sizeof(int));
    for(int i=0; i<G->V; i++) candidates[i] = i;

    findMaximalCliquesRecursive(G, current, 0, candidates, G->V);

    printf("\n--- RISULTATI --- \n");
    
    int maxVertIndex = -1;
    int maxVertCount = 0;

    for (int i = 0; i < nCliques; i++) {
        // Filtro per massimalità (come prima)
        int is_max = 1;
        for (int v = 0; v < G->V; v++) {
            int in_clique = 0;
            for(int k=0; k<allCliques[i].count; k++) if(allCliques[i].nodes[k] == v) in_clique=1;
            
            if(!in_clique) {
                int connected_all = 1;
                for(int k=0; k<allCliques[i].count; k++) 
                    if(G->adj[v][allCliques[i].nodes[k]] == 0) connected_all = 0;
                if(connected_all) { is_max = 0; break; }
            }
        }

        if (is_max) {
            printf("Cricca Massimale %d: { ", i);
            for(int k=0; k<allCliques[i].count; k++) 
                printf("%s ", G->st.names[allCliques[i].nodes[k]]);
            printf("}\n");

            // --- CALCOLO CICLO HAMILTONIANO MASSIMO (NUOVO) ---
            int cliqueSize = allCliques[i].count;
            
            if (cliqueSize < 2) {
                printf("  -> Troppo piccola per un ciclo.\n");
            } else {
                // Buffer per trovare il percorso migliore
                int *bestPath = malloc(cliqueSize * sizeof(int));
                // Copia temporanea dei nodi per "giocarci" (permutare)
                int *permNodes = malloc(cliqueSize * sizeof(int));
                memcpy(permNodes, allCliques[i].nodes, cliqueSize * sizeof(int));

                int maxWeight = -1; // Inizializzo a valore basso

                // Lanciamo il TSP
                // Partiamo dall'indice 1. Fissiamo l'indice 0 (permNodes[0]) come perno
                // per evitare di calcolare lo stesso ciclo ruotato (A-B-C è uguale a B-C-A).
                // permutiamo solo gli altri.
                solveTSP(G, permNodes, 1, cliqueSize, &maxWeight, bestPath);

                printf("  -> Ciclo Hamiltoniano MAX peso: %d\n", maxWeight);
                printf("  -> Percorso: ");
                for(int k=0; k<cliqueSize; k++) {
                     printf("%s -> ", G->st.names[bestPath[k]]);
                }
                printf("%s\n", G->st.names[bestPath[0]]); // Chiudo il cerchio visivamente

                free(bestPath);
                free(permNodes);
            }

            if (allCliques[i].count > maxVertCount) {
                maxVertCount = allCliques[i].count;
                maxVertIndex = i;
            }
        }
    }

    if (maxVertIndex != -1) {
        printf("\nLa cricca con numero massimo di vertici ha %d nodi.\n", maxVertCount);
    }

    free(current);
    free(candidates);
}

void freeGraph(Graph *G) {
    for(int i=0; i<G->V; i++) free(G->adj[i]);
    free(G->adj);
    for(int i=0; i<G->st.size; i++) free(G->st.names[i]);
    free(G->st.names);
    free(G);
    for(int i=0; i<nCliques; i++) free(allCliques[i].nodes);
    free(allCliques);
}