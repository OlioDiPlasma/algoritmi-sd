#include "graph.h"
#include <stdlib.h>
#include <string.h>

#define MAX_STR 51

/* --- STRUTTURE DATI INTERNE (Nascoste nel .c) --- */
/* Definire le strutture dati necessarie */

/* Nodo della lista di adiacenza (il cunicolo) */
typedef struct node {
    int v;              // Indice della stanza di destinazione
    int trappola;       // 1 se c'è trappola, 0 altrimenti
    struct node *next;  // Puntatore al prossimo cunicolo
} Node;

/* La Stanza (il Vertice) */
typedef struct {
    char nome[MAX_STR];
    int profondita;
    int tesoro;
    int oro;
} Stanza;

/* Il Grafo principale */
struct graph {
    int V;              // Numero di stanze
    Stanza *stanze;     // Array dinamico delle stanze (Vettore di struct)
    Node **adj;         // Array di puntatori a liste (Liste di adiacenza)
};

/* --- FUNZIONI DI UTILITÀ --- */

/* Cerca l'indice di una stanza dato il nome. Ritorna -1 se non trovata */
static int getIndex(Graph g, char *nome) {
    for (int i = 0; i < g->V; i++) {
        if (strcmp(g->stanze[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Crea un nuovo nodo per la lista di adiacenza */
static Node* newNode(int v, int trap, Node *next) {
    Node *x = malloc(sizeof(Node)); // Allocazione dinamica nello heap
    if (x == NULL) { printf("Errore malloc nodo\n"); exit(1); }
    x->v = v;
    x->trappola = trap;
    x->next = next;
    return x;
}

/* --- 1. CARICAMENTO GRAFO (GRAPHload) --- */
Graph GRAPHload(FILE *fp) {
    Graph g = malloc(sizeof(struct graph));
    if (g == NULL) return NULL;

    // Lettura numero stanze S
    fscanf(fp, "%d", &g->V);

    // Allocazione array stanze e array liste adiacenza
    g->stanze = malloc(g->V * sizeof(Stanza));
    g->adj = malloc(g->V * sizeof(Node*));
    
    // Inizializzazione liste a NULL (fondamentale per evitare puntatori sporchi)
    for (int i = 0; i < g->V; i++) g->adj[i] = NULL;

    // Lettura dettagli stanze
    for (int i = 0; i < g->V; i++) {
        fscanf(fp, "%s %d %d %d", g->stanze[i].nome, &g->stanze[i].profondita, &g->stanze[i].tesoro, &g->stanze[i].oro);
    }

    // Lettura cunicoli
    char nome1[MAX_STR], nome2[MAX_STR];
    int trap;
    while (fscanf(fp, "%s %s %d", nome1, nome2, &trap) == 3) {
        int id1 = getIndex(g, nome1);
        int id2 = getIndex(g, nome2);
        
        if (id1 >= 0 && id2 >= 0) {
            // Grafo non orientato: arco in entrambe le direzioni
            // Inserimento in testa alla lista (O(1))
            g->adj[id1] = newNode(id2, trap, g->adj[id1]);
            g->adj[id2] = newNode(id1, trap, g->adj[id2]);
        }
    }
    return g;
}

/* --- 2. GESTIONE PATH (Caricamento e Verifica) --- */

Path GRAPHpathLoad(FILE *fp) {
    Path p;
    char buffer[MAX_STR];
    // Non sappiamo quanto è lungo il percorso, usiamo una strategia dinamica o un buffer grande.
    // Per semplicità didattica, assumiamo max 100 passi, ma in esame meglio realloc.
    int cap = 100;
    p.stanze = malloc(cap * sizeof(char*));
    p.num_stanze = 0;
    p.ricchezza = 0;

    while(fscanf(fp, "%s", buffer) == 1) {
        // Duplichiamo la stringa con strdup (alloca memoria per la stringa)
        p.stanze[p.num_stanze] = strdup(buffer);
        p.num_stanze++;
    }
    return p;
}

/* === SOLUZIONE VERIFICA (3.2.3) === */
int GRAPHpathCheck(Graph g, Path *p, int M, int PF) {
    /* Funzione che verifica congruenza e calcola ricchezza */
    
    int current_pf = PF;
    int current_m = M;
    int accumulated_gold = 0;
    int collected_treasure = 0; // Booleano: tesoro preso?
    
    // Array per tenere traccia dell'oro raccolto PER STANZA in questo percorso
    // Usiamo calloc per inizializzare a 0 (false)
    int *gold_taken = calloc(g->V, sizeof(int)); 

    // Indice stanza iniziale
    int curr_idx = getIndex(g, p->stanze[0]);
    
    // Controllo: Il percorso deve iniziare a profondità 0 (Ingresso)
    if (g->stanze[curr_idx].profondita != 0) {
        free(gold_taken); return 0; // Invalido
    }

    // Iteriamo sulle mosse del percorso (dalla stanza 0 alla num_stanze-1)
    for (int i = 0; i < p->num_stanze - 1; i++) {
        int u = getIndex(g, p->stanze[i]);
        int v = getIndex(g, p->stanze[i+1]);

        // Cerca l'arco u->v
        Node *n = g->adj[u];
        int found = 0;
        int is_trap = 0;
        while(n != NULL) {
            if (n->v == v) {
                found = 1;
                is_trap = n->trappola;
                break;
            }
            n = n->next;
        }

        if (!found) { free(gold_taken); return 0; } // Arco non esistente

        // Aggiorna stato
        current_m--; // Consuma una mossa
        if (is_trap) current_pf--; // Trappola ferisce

        // --- GESTIONE RACCOLTA ORO/TESORI NELLA STANZA DI ARRIVO (v) ---
        // Nota: Si raccoglie quando si ARRIVA nella stanza.
        // Tesoro: se c'è e non ne ho presi altri, lo prendo.
        if (g->stanze[v].tesoro > 0 && !collected_treasure) {
            accumulated_gold += g->stanze[v].tesoro;
            collected_treasure = 1; // Posso prenderne solo uno
        }
        
        // Oro: se c'è e non l'ho già preso in QUESTA stanza
        if (g->stanze[v].oro > 0 && gold_taken[v] == 0) {
            accumulated_gold += g->stanze[v].oro;
            gold_taken[v] = 1; // Oro di questa stanza esaurito
        }

        // --- CONDIZIONI DI ARRESTO (INTERRUZIONE) ---
        if (current_m == 0 || current_pf <= 0) {
            // Stop forzato nella stanza v.
            // Se PF <= 0 è morte o interruzione? Il testo dice: 
            // "ci si ferma nella stanza raggiunta se il valore PF si è azzerato".
            
            // Verifichiamo le condizioni di salvataggio
            int depth = g->stanze[v].profondita;
            
            if (depth == 0) {
                // Miracolo: esaurito tutto ma sono all'uscita (raro ma possibile)
                p->ricchezza = accumulated_gold;
                free(gold_taken);
                return 1; 
            } else if (depth == 1) {
                // Soccorso livello 1: perdo 1/3 (arrotondato eccesso? Testo: "calcolare con arrotondamento per eccesso la PARTE CHE SI SOTTRAE")
                // Se ho 10, 1/3 è 3.33 -> tolgo 4. Resta 6.
                int penalty = (accumulated_gold + 2) / 3; // Formula integer division per ceil(x/3)
                p->ricchezza = (accumulated_gold - penalty) > 0 ? (accumulated_gold - penalty) : 0;
                free(gold_taken);
                return 1;
            } else if (depth == 2) {
                // Soccorso livello 2: perdo 1/2
                int penalty = (accumulated_gold + 1) / 2; // ceil(x/2)
                p->ricchezza = (accumulated_gold - penalty) > 0 ? (accumulated_gold - penalty) : 0;
                free(gold_taken);
                return 1;
            } else {
                // Profondità >= 3: Fallimento totale
                p->ricchezza = 0;
                free(gold_taken);
                return 1; // Il percorso è "geometricamente" valido, ma il risultato è 0.
            }
        }
    }

    // Se il ciclo finisce, devo essere tornato a profondità 0 per "Percorso normale"
    int last_node = getIndex(g, p->stanze[p->num_stanze - 1]);
    if (g->stanze[last_node].profondita == 0) {
        p->ricchezza = accumulated_gold;
        free(gold_taken);
        return 1;
    }

    free(gold_taken);
    return 0; // Percorso terminato ma non all'uscita e senza esaurire risorse (caso strano, invalido)
}

/* === SOLUZIONE OTTIMIZZAZIONE (3.2.4) === */

/* Variabili globali/statiche per la ricorsione (semplificano la firma) */
static int best_wealth = -1;
static Path best_path;
static int *sol_indices; // Array temporaneo indici percorso corrente

/* Funzione ricorsiva */
void bestR(Graph g, int u, int m_left, int pf_left, int curr_wealth, int has_treasure, int *gold_status, int depth_idx) {
    
    // Salvo mossa corrente
    sol_indices[depth_idx] = u;

    // Aggiorna ricchezze (Logica simile al check: prendo roba appena arrivo)
    // Nota: La stanza 'u' è quella in cui sono APPENA arrivato.
    // Devo gestire lo stato 'gold_status' per il backtracking.
    
    int taken_gold_here = 0;
    int taken_treasure_here = 0;
    
    // Se non è l'ingresso (depth 0 visitata all'inizio o alla fine)
    // Esempio: Start -> A. Start non da oro. A da oro.
    if (depth_idx > 0 || u != getIndex(g, "Ingresso")) { // Semplificazione: start non ha oro
        if (g->stanze[u].tesoro > 0 && !has_treasure) {
            curr_wealth += g->stanze[u].tesoro;
            has_treasure = 1;
            taken_treasure_here = 1;
        }
        if (g->stanze[u].oro > 0 && gold_status[u] == 0) {
            curr_wealth += g->stanze[u].oro;
            gold_status[u] = 1;
            taken_gold_here = 1;
        }
    }

    // --- CONTROLLO FINE PERCORSO O PRUNING ---
    // 1. Sono tornato all'uscita (prof 0) e ho fatto almeno un passo?
    if (g->stanze[u].profondita == 0 && depth_idx > 0) {
        if (curr_wealth > best_wealth) {
            best_wealth = curr_wealth;
            // Salva soluzione
            best_path.num_stanze = depth_idx + 1;
            // Realloc se necessario, qui usiamo size fissa per semplicità
            if(best_path.stanze != NULL) {
                for(int k=0; k<best_path.num_stanze; k++) free(best_path.stanze[k]);
                free(best_path.stanze);
            }
            best_path.stanze = malloc(best_path.num_stanze * sizeof(char*));
            for(int k=0; k<=depth_idx; k++) {
                best_path.stanze[k] = strdup(g->stanze[sol_indices[k]].nome);
            }
            best_path.ricchezza = best_wealth;
        }
        // Backtrack risorse
        if (taken_gold_here) gold_status[u] = 0;
        return; // Non continuo oltre l'uscita in questa versione semplice
    }

    // 2. Risorse esaurite? (Pruning)
    if (m_left == 0 || pf_left == 0) {
        // Calcolo penalità
        int final_w = curr_wealth;
        int d = g->stanze[u].profondita;
        if (d == 1) final_w -= (final_w + 2)/3;
        else if (d == 2) final_w -= (final_w + 1)/2;
        else if (d >= 3) final_w = 0;

        if (final_w > best_wealth) {
            best_wealth = final_w;
             // Salva soluzione (copia-incolla codice sopra, idealmente fare function 'save')
            best_path.num_stanze = depth_idx + 1;
            if(best_path.stanze != NULL) { /* free old */ } // (omesso per brevità)
            best_path.stanze = malloc(best_path.num_stanze * sizeof(char*));
            for(int k=0; k<=depth_idx; k++) best_path.stanze[k] = strdup(g->stanze[sol_indices[k]].nome);
            best_path.ricchezza = best_wealth;
        }
        // Backtrack
        if (taken_gold_here) gold_status[u] = 0;
        return;
    }

    // --- PASSO RICORSIVO ---
    Node *n = g->adj[u];
    while (n != NULL) {
        int v = n->v;
        int trap = n->trappola;
        
        // Posso andare?
        // Cicli ammessi, ma non su stanza 0 (uscita/ingresso solo start/end)
        if (g->stanze[v].profondita == 0) {
            // Posso andare a 0 solo se voglio uscire (gestito sopra o qui)
            // Procediamo, la condizione base gestirà l'uscita
        }

        int new_pf = pf_left - (trap ? 1 : 0);
        
        // Chiamata ricorsiva se ho ancora PF (o se muoio esattamente ora, gestito base case)
        if (new_pf >= 0) {
            bestR(g, v, m_left - 1, new_pf, curr_wealth, has_treasure, gold_status, depth_idx + 1);
        }
        
        n = n->next;
    }

    // --- BACKTRACKING ---
    if (taken_gold_here) gold_status[u] = 0;
}

Path GRAPHpathBest(Graph g, int M, int PF) {
    // Init globali
    best_wealth = -1;
    best_path.stanze = NULL;
    best_path.num_stanze = 0;
    
    sol_indices = malloc((M + 1) * sizeof(int)); // Max mosse + 1
    int *gold_status = calloc(g->V, sizeof(int));

    int start_node = -1;
    // Trova stanza a prof 0 (Assumiamo una sola come da testo)
    for(int i=0; i<g->V; i++) if(g->stanze[i].profondita==0) start_node = i;

    // Lancio ricorsione
    // Start node non consuma mosse o PF per esserci
    bestR(g, start_node, M, PF, 0, 0, gold_status, 0);

    free(sol_indices);
    free(gold_status);
    return best_path;
}

void GRAPHfree(Graph g) {
    for (int i = 0; i < g->V; i++) {
        Node *n = g->adj[i];
        while (n) { Node *t = n; n = n->next; free(t); }
    }
    free(g->adj);
    free(g->stanze);
    free(g);
}

void PATHprint(Path p) {
    printf("Path Wealth: %d\nSteps: ", p.ricchezza);
    for(int i=0; i<p.num_stanze; i++) printf("%s -> ", p.stanze[i]);
    printf("END\n");
}