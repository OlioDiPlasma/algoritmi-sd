/* ////////////////////////////////////////////////////
 1. Backtracking  
 Risolve problemi di: "trova la combinazione ottima", "riempi lo zaino", "trova il percorso
*/

//Strutture
typedef struct {
    // Dati del problema (es. array di oggetti, matrice)
    int *valori; 
    int n; 
} InputData;

typedef struct {
    // La tua soluzione (ottima o corrente)
    int *scelte; 
    int costo_totale;
    int n_scelte;
} Soluzione;

//Wrapper
Soluzione solve(InputData in) {
    Soluzione sol, bestSol;
    
    // 1. Inizializzazione
    sol.scelte = calloc(in.n, sizeof(int));
    bestSol.scelte = calloc(in.n, sizeof(int));
    sol.costo_totale = 0;
    bestSol.costo_totale = -1; // O un valore sentinella per "nessuna soluzione"

    // 2. Chiamata ricorsiva
    // Parametri: livello ricorsione, input, soluzione corrente, soluzione ottima, marker
    cerca(0, in, &sol, &bestSol); 

    // 3. Pulizia e ritorno
    free(sol.scelte);
    return bestSol;
}

//Funzion ricorsiva (Backtracking)
void cerca(int pos, InputData in, Soluzione *sol, Soluzione *bestSol) {
    int i;

    // A. PRUNING (Opzionale ma spesso richiesto) [1][2][3]
    // Se la soluzione parziale viola i vincoli o è peggiore della bestSol, fermati.
    if (/* condizione di taglio */) return;

    // B. TERMINAZIONE (Base Case)
    if (pos >= in.n /* o altra condizione di fine */) {
        // C. CHECK VALIDITÀ E AGGIORNAMENTO OTTIMO
        if (/* sol è valida */) {
            if (bestSol->costo_totale == -1 || sol->costo_totale > bestSol->costo_totale) {
                // Copia sol in bestSol (memcpy o ciclo manuale)
                aggiornaBest(bestSol, sol); 
            }
        }
        return;
    }

    // D. CICLO DELLE SCELTE (Il cuore dell'algoritmo) [4, 5]
    // Se COMBINAZIONI (l'ordine NON conta): il ciclo parte da 'start' passato come parametro
    // Se PERMUTAZIONI (l'ordine CONTA): ciclo 0..n con vettore di mark (visitati)
    for (i = 0; i < /* numero opzioni possibili */; i++) {
        
        // 1. Applica scelta
        sol->scelte[pos] = i; 
        sol->costo_totale += /* costo scelta i */;
        // mark[i] = 1; (se permutazioni)

        // 2. Ricorsione
        cerca(pos + 1, in, sol, bestSol);

        // 3. Backtrack (Annulla scelta)
        sol->costo_totale -= /* costo scelta i */;
        // mark[i] = 0; (se permutazioni)
    }
}


/* ////////////////////////////////////////////////////
 2. Manipolazione Liste: Filtro/Cancellazione
 es. "rimuovi nodi pari", "tieni solo vocali
*/

typedef struct node {
    int val;
    struct node *next;
} Node; // [8]

// Funzione che modifica la lista (passaggio puntatore a testa: Node **head)
void filterList(Node **head, int soglia) {
    Node *curr = *head;
    Node *prev = NULL;
    Node *temp;

    while (curr != NULL) {
        if (/* condizione di cancellazione, es: curr->val < soglia */) {
            temp = curr; // Salva nodo da cancellare
            
            if (prev == NULL) {
                // Caso 1: Sto cancellando la testa
                *head = curr->next;
                curr = *head;
            } else {
                // Caso 2: Sto cancellando un nodo interno
                prev->next = curr->next;
                curr = prev->next;
            }
            free(temp); // IMPORTANTE: deallocare [9]
        } else {
            // Avanza solo se non hai cancellato
            prev = curr;
            curr = curr->next;
        }
    }
}

/* ////////////////////////////////////////////////////
 3. Matrici: Allocazione e Visita Griglia
 es. "Crucipuzzle", "Labirinto" o "Robot"
*/

//Allocazione Dinamica Matrice 2D
int **malloc2d(int R, int C) {
    int i;
    int **mat = malloc(R * sizeof(int *));
    for (i = 0; i < R; i++) {
        mat[i] = malloc(C * sizeof(int)); // O calloc per inizializzare a 0
    }
    return mat;
}

// Pattern "Spostamento nelle 4 direzioni" (Griglia) vettori di spostamento.
// Nord, Est, Sud, Ovest
int dr[] = {-1, 0, 1, 0}; 
int dc[] = {0, 1, 0, -1};

void esploraGriglia(int r, int c, int R, int C, int **mat) {
    for (int k = 0; k < 4; k++) {
        int nr = r + dr[k]; // nuova riga
        int nc = c + dc[k]; // nuova colonna

        // Verifica sempre i bordi prima di accedere! [12]
        if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
            if (mat[nr][nc] == /* condizione valida */) {
                // Fai mossa
            }
        }
    }
}

/* ////////////////////////////////////////////////////
 4. Lettura File e ADT "Opaco"
 es. "Definire una struttura dati adeguata..." (ADT di prima classe).
*/

//header.h
typedef struct wrapper_s *Tabella; // Puntatore opaco
Tabella loadData(char *filename);

//struttura.c
struct wrapper_s {
    Item *vettore; // Array dinamico di elementi
    int n_elementi;
};

Tabella loadData(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return NULL;

    Tabella t = malloc(sizeof(struct wrapper_s));
    
    // Pattern classico: leggi N, poi cicla N volte
    fscanf(f, "%d", &t->n_elementi);
    
    t->vettore = malloc(t->n_elementi * sizeof(Item));
    
    for (int i = 0; i < t->n_elementi; i++) {
        // Lettura riga per riga
        fscanf(f, "%s %d", t->vettore[i].nome, &t->vettore[i].valore);
    }
    
    fclose(f);
    return t;
}


/* ////////////////////////////////////////////////////
MISTO ALTRO
*/


/* ////////////////////////////////////////////////////
CONFRONTO LISTE
*/
typedef struct node *link;
struct node { int val; link next; }; // O char* key...

link operazioneListe(link h1, link h2) {
    link head = NULL, tail = NULL; // Lista risultato
    // Helper per aggiungere in coda (evita di scorrere ogni volta)
    // In esame puoi anche usare una funzione 'insertTail' a parte
    
    while (h1 != NULL && h2 != NULL) {
        if (h1->val < h2->val) {
            // Caso: h1 è minore. Se serve unione: prendi h1. Se intersezione: ignora.
            // Esempio UNIONE:
            // insertTail(&head, &tail, h1->val); 
            h1 = h1->next;
        } 
        else if (h2->val < h1->val) {
            // Caso: h2 è minore.
            // Esempio UNIONE:
            // insertTail(&head, &tail, h2->val);
            h2 = h2->next;
        } 
        else {
            // Caso: UGUALI (h1->val == h2->val). Qui succede la magia.
            // Spesso chiedono: somma i valori, oppure "prendi solo se comune"
            int nuovoValore = h1->val + h2->val; // Esempio somma
            // insertTail(&head, &tail, nuovoValore);
            
            h1 = h1->next;
            h2 = h2->next; // Avanzo entrambi
        }
    }
    // Se è unione, attacca i rimanenti (h1 o h2 non ancora finiti)
    // while (h1 != NULL) { ... h1=h1->next; }
    // while (h2 != NULL) { ... h2=h2->next; }
    
    return head;
}

/* ////////////////////////////////////////////////////
VISITA DFS PER BST 
*/


// Esempio: Cerca il cammino con la proprietà X o conta nodi
void bst_recursive(link root, int currentDepth, int *bestResult) {
    // 1. Caso Base: Albero finito
    if (root == NULL) {
        return;
    }

    // 2. Pre-Order (Lavoro sul nodo PRIMA di scendere)
    // Esempio: Controlla se il nodo ha una proprietà
    if (checkProprieta(root->val)) {
        // Aggiorna un massimo o un contatore globale (passato per puntatore)
        if (currentDepth > *bestResult) 
            *bestResult = currentDepth;
    }

    // 3. Passo Ricorsivo
    bst_recursive(root->left, currentDepth + 1, bestResult);
    bst_recursive(root->right, currentDepth + 1, bestResult);
    
    // (Opzionale) Post-Order: Lavoro sul nodo DOPO essere risalito
}

// Wrapper per chiamarla
int wrapperBST(link root) {
    int result = 0;
    bst_recursive(root, 0, &result);
    return result;
}

/* ////////////////////////////////////////////////////
//WRAPPER 18 PUNTI RICORSIONE
*/

int solveWrapper(DatiInput *in) {
    // 1. Alloca vettore soluzione temporanea (sol)
    int *sol = malloc(in->N * sizeof(int));
    
    // 2. Alloca vettore soluzione ottima (bestSol)
    int *bestSol = malloc(in->N * sizeof(int));
    
    // 3. Alloca vettore "visitati" (se non posso riusare gli elementi)
    int *mark = calloc(in->N, sizeof(int)); // calloc mette a 0
    
    int maxProfit = -1; // O minCosto (inizializzato a infinito)

    // 4. Chiama la ricorsiva
    // pos: livello della ricorsione (indice nel vettore sol o oggetto corrente)
    solve_recursive(0, in, sol, bestSol, mark, &maxProfit);

    // 5. Stampa o usa bestSol ...
    
    // 6. Libera memoria
    free(sol); free(bestSol); free(mark);
    return maxProfit;
}

/* ////////////////////////////////////////////////////
Backtracking con pruning
*/

void solve_recursive(int pos, DatiInput *in, int *sol, int *bestSol, int *mark, int *bestMetric) {
    
    // A. PRUNING (Potatura) - FONDAMENTALE per i 18pt
    // Se la soluzione parziale viola già i vincoli, FERMATI SUBITO.
    // Esempio: ho superato il peso massimo, o il costo attuale è già peggiore del bestMetric trovato.
    if (!checkValid(sol, pos)) return; 

    // B. TERMINAZIONE
    // Ho raggiunto la fine del vettore o processato tutti gli elementi?
    if (pos >= in->N) { // Oppure condition tipo (peso_corrente > TOT)
        int currentMetric = calcolaValore(sol, pos);
        
        // Se è migliore della best trovata finora, salva tutto
        if (currentMetric > *bestMetric) {
            *bestMetric = currentMetric;
            memcpy(bestSol, sol, in->N * sizeof(int)); // Copia profonda
        }
        return;
    }

    // C. CICLO DELLE SCELTE (Branching)
    // Esempio: Per l'oggetto 'pos', provo tutte le possibili assegnazioni
    // Oppure: provo a inserire ogni candidato 'i' nella posizione 'pos'
    
    for (int i = 0; i < NUM_SCELTE; i++) { 
        // (Opzionale) Controllo se 'i' è già usato
        if (mark[i] == 0) {
            
            // 1. DO (Faccio la scelta)
            mark[i] = 1;
            sol[pos] = i; 
            
            // 2. RECUR (Scendo nel livello successivo)
            solve_recursive(pos + 1, in, sol, bestSol, mark, bestMetric);
            
            // 3. BACKTRACK (Annulla la scelta per provare la prossima)
            mark[i] = 0;
            // sol[pos] = 0; // Spesso non serve resettare il valore, viene sovrascritto
        }
    }
    
    // D. (Opzionale) Caso "Non prendo nulla" / Salto l'elemento
    // sol[pos] = -1;
    // solve_recursive(pos + 1, ...);
}


/* ////////////////////////////////////////////////////
ITERAZIONE SU GRAFI LISTE DI ADIACENZA
*/


// Struttura tipica di un nodo in lista di adiacenza
typedef struct node *link;
struct node { int v; int wt; link next; }; // v = destinazione, wt = peso

struct graph { int V; int E; link *ladj; }; // Vettore di liste

// SNIPPET: "Per ogni vicino del nodo u..."
void visitaGrafo(Graph G, int u, int *visited) {
    link t;
    // G->ladj[u] è la testa della lista dei vicini di u
    for (t = G->ladj[u]; t != NULL; t = t->next) {
        int v = t->v; // Nodo vicino
        int peso = t->wt; 
        
        if (visited[v] == 0) {
            // Fai qualcosa (es. ricorsione DFS)
            visited[v] = 1;
            visitaGrafo(G, v, visited);
            // Backtrack (se serve cercare tutti i cammini)
            visited[v] = 0; 
        }
    }
}