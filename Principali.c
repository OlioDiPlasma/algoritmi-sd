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