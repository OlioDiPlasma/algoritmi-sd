#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "puzzle.h"

/* * ======================================================================================
 * FUNZIONE STATICA: swap_tiles
 * ======================================================================================
 * SCOPO:
 * Scambiare il contenuto di due variabili di tipo Tile direttamente in memoria.
 *
 * PARAMETRI:
 * - Tile *a: Indirizzo di memoria della prima tessera.
 * - Tile *b: Indirizzo di memoria della seconda tessera.
 *
 * DETTAGLIO SINTATTICO:
 * - `*a`: (Dereferenziazione) Accediamo alla cella di memoria puntata da `a`.
 * - Perché i puntatori? In C i parametri passano per valore (copia). Se passassimo `Tile a`,
 * scambieremmo solo delle copie temporanee. Passando l'indirizzo (`Tile *`), modifichiamo
 * la griglia originale.
 * ======================================================================================
 */
static void swap_tiles(Tile *a, Tile *b) {
    /* Creo una variabile temporanea nello stack e ci copio il valore puntato da 'a' */
    Tile temp = *a; 
    
    /* Sovrascrivo la memoria all'indirizzo 'a' con il valore che trovo all'indirizzo 'b' */
    *a = *b; 
    
    /* Scrivo il valore salvato nella temp all'indirizzo 'b' */
    *b = temp;
}

/* * ======================================================================================
 * FUNZIONE STATICA: are_connected
 * ======================================================================================
 * SCOPO:
 * Verificare la compatibilità geometrica tra due tubi adiacenti.
 * ======================================================================================
 */
static bool are_connected(Tile from, Tile to, char dir_from_to) {
    /* I buchi interrompono sempre il flusso */
    if (from.is_hole || to.is_hole) return false;

    /* Controllo incastri: Se vado a NORD, devo uscire da N ed entrare da S */
    if (dir_from_to == 'N') return (from.n && to.s);
    if (dir_from_to == 'S') return (from.s && to.n);
    if (dir_from_to == 'W') return (from.w && to.e);
    if (dir_from_to == 'E') return (from.e && to.w);
    
    return false;
}

/* * ======================================================================================
 * FUNZIONE STATICA: dfs_path (Depth First Search)
 * ======================================================================================
 * SCOPO:
 * Algoritmo ricorsivo per esplorare il grafo dei tubi.
 *
 * PARAMETRI:
 * - Game *g: La griglia.
 * - int r, c: Posizione attuale.
 * - int start_r, start_c: Posizione di PARTENZA (per evitare falsi positivi).
 * - bool **visited: Matrice per evitare cicli infiniti.
 * - int *end_found: Puntatore a flag per comunicare il successo al chiamante.
 *
 * CONCETTI CHIAVE:
 * 1. Caso Base: Se trovo un terminale DIVERSO da quello di partenza -> Vittoria.
 * 2. Ricorsione: Provo tutte e 4 le direzioni.
 * ======================================================================================
 */
static bool dfs_path(Game *g, int r, int c, int start_r, int start_c, bool **visited, int *end_found) {
    /* Segno la cella corrente come visitata nella matrice di supporto */
    visited[r][c] = true;
    Tile t = g->grid[r][c];

    int connections = t.n + t.s + t.w + t.e;
    
    /* --- LOGICA DI VITTORIA --- */
    /* Un terminale ha 1 sola connessione.
     * MA ATTENZIONE: Anche la casella di partenza ha 1 connessione!
     * Quindi vinco solo se sono su un terminale E (&&) le coordinate sono diverse dallo start.
     */
    if (connections == 1) {
        if (r != start_r || c != start_c) { 
            *end_found = 1; // Scrivo 1 all'indirizzo del flag
            return true; 
        }
    }

    /* Vettori direzione per esplorare i vicini (Nord, Sud, Ovest, Est) */
    int dr[] = {-1, 1, 0, 0}; //convenzione VETTORI DI DIREZIONE
    int dc[] = {0, 0, -1, 1};
    char dirs[] = {'N', 'S', 'W', 'E'};

    /* Ciclo sulle 4 direzioni possibili */
    for(int i=0; i<4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        /* CHECK BOUNDS (Fondamentale in C):
         * Prima di accedere a g->grid[nr][nc], devo essere CERTO che nr e nc siano validi.
         * Se nr fosse -1, andrei a leggere memoria non mia -> Segmentation Fault.
         */
        if (nr >= 0 && nr < g->N && nc >= 0 && nc < g->N && !visited[nr][nc]) {
            
            /* Se geometricamente il tubo prosegue... */
            if (are_connected(t, g->grid[nr][nc], dirs[i])) {
                /* ...faccio la chiamata ricorsiva (scendo nel tubo) */
                if (dfs_path(g, nr, nc, start_r, start_c, visited, end_found)) return true;
            }
        }
    }
    return false;
}

/* * ======================================================================================
 * FUNZIONE: load_game [RIF. DOMANDA: Strutture dati e acquisizione]
 * ======================================================================================
 * SCOPO:
 * Allocazione dinamica della matrice di gioco.
 *
 * GESTIONE MEMORIA (DOPPIO MALLOC):
 * Per creare una matrice dinamica 2D in C, dobbiamo fare due passaggi.
 * Immagina la matrice non come un blocco unico, ma come un elenco di righe.
 * 
 * 1. Allocare un array verticale di puntatori (`Tile**`).
 * 2. Per ogni elemento di questo array, allocare una riga orizzontale (`Tile*`).
 *
 * DETTAGLIO SINTATTICO:
 * - `sizeof(Tile*)`: Dimensione di un puntatore (indirizzo), usata per l'array verticale.
 * - `sizeof(Tile)`: Dimensione della struttura dati vera, usata per le righe.
 * ======================================================================================
 */
Game* load_game(char *filename) {
    /* 1. Apertura File */
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Errore apertura file"); return NULL; }

    /* 2. Allocazione Struttura Game */
    Game *g = (Game*)malloc(sizeof(Game));
    int T_dummy;
    fscanf(fp, "%d %d", &g->N, &T_dummy);

    /* 3. Allocazione Matrice (Array di Puntatori) */
    /* Qui allochiamo 'N' caselle, ognuna grande quanto un puntatore */
    g->grid = (Tile**)malloc(g->N * sizeof(Tile*));
    
    /* 4. Allocazione Righe e Lettura */
    for(int i=0; i < g->N; i++) {
        /* Qui allochiamo, per ogni riga, 'N' caselle grandi quanto una Tile intera */
        g->grid[i] = (Tile*)malloc(g->N * sizeof(Tile));
        
        for(int j=0; j < g->N; j++) {
            Tile *t = &g->grid[i][j];
            fscanf(fp, "%d %d %d %d", &t->n, &t->s, &t->w, &t->e);
            /* Impostazione rapida del flag buco */
            t->is_hole = (t->n==0 && t->s==0 && t->w==0 && t->e==0);
        }
    }
    fclose(fp);
    return g;
}

/* * ======================================================================================
 * FUNZIONE: free_game
 * ======================================================================================
 * SCOPO:
 * Liberare la memoria heap per evitare Memory Leaks.
 *
 * CONCETTO CHIAVE:
 * L'ordine è INVERSO all'allocazione.
 * 1. Prima distruggo il contenuto (le righe).
 * 2. Poi distruggo il contenitore (l'array di puntatori).
 * 3. Infine distruggo la struttura principale.
 * Se liberassi prima 'grid', perderei l'accesso alle righe!
 * ======================================================================================
 */
void free_game(Game *g) {
    if(!g) return;

    /* 1. Libero le righe */
    for(int i=0; i<g->N; i++) free(g->grid[i]); 
    
    /* 2. Libero l'array di puntatori */
    free(g->grid);
    
    /* 3. Libero la struct principale */
    free(g);
}


//Print grid di test
void print_grid(Game *g) {
    for(int i=0; i<g->N; i++) {
        for(int j=0; j<g->N; j++) {
            if(g->grid[i][j].is_hole) printf(" . ");
            else printf(" T ");
        }
        printf("\n");
    }
    printf("\n");
}

/* * ======================================================================================
 * FUNZIONE: apply_move [RIF. DOMANDA: Descrizione del problema - Regola di movimento]
 * ======================================================================================
 * SCOPO:
 * Spostare le tessere riempiendo i buchi.
 *
 * CONCETTI CHIAVE - ORDINE DI ITERAZIONE:
 * Per rispettare il movimento "Simultaneo" (e ottenere R U U come da esempio),
 * dobbiamo evitare che un buco appena creato venga riempito nello stesso turno.
 * * QUINDI:
 * - Se vado SU (il buco è sopra), parto dal BASSO. Chi sta sotto trova il muro (la tessera sopra non si è ancora mossa "nel passato").
 * - Se vado GIÙ (il buco è sotto), parto dall'ALTO.
 */
bool apply_move(Game *g, char direction) {
    int moved = 0;
    int N = g->N;

    if (direction == 'U') { 
        /* PRIMA: for (int i = 1; i < N; i++)  <-- Sbagliato (Effetto valanga) */
        /* ORA: Parto dal basso, così muovo solo chi aveva DAVVERO spazio libero all'inizio */
        for (int i = N - 1; i > 0; i--) 
            for (int j = 0; j < N; j++) 
                if (!g->grid[i][j].is_hole && g->grid[i-1][j].is_hole) {
                    swap_tiles(&g->grid[i][j], &g->grid[i-1][j]); 
                    moved++;
                }
    } else if (direction == 'D') {
        /* PRIMA: for (int i = N - 2; i >= 0; i--) */
        /* ORA: Parto dall'alto */
        for (int i = 0; i < N - 1; i++) 
            for (int j = 0; j < N; j++) 
                if (!g->grid[i][j].is_hole && g->grid[i+1][j].is_hole) {
                    swap_tiles(&g->grid[i][j], &g->grid[i+1][j]); 
                    moved++;
                }
    } else if (direction == 'L') {
        /* ORA: Parto da destra */
        for (int j = N - 1; j > 0; j--) 
            for (int i = 0; i < N; i++) 
                if (!g->grid[i][j].is_hole && g->grid[i][j-1].is_hole) {
                    swap_tiles(&g->grid[i][j], &g->grid[i][j-1]); 
                    moved++;
                }
    } else if (direction == 'R') {
        /* ORA: Parto da sinistra */
        for (int j = 0; j < N - 1; j++) 
            for (int i = 0; i < N; i++) 
                if (!g->grid[i][j].is_hole && g->grid[i][j+1].is_hole) {
                    swap_tiles(&g->grid[i][j], &g->grid[i][j+1]); 
                    moved++;
                }
    }
    return (moved > 0);
}

/* * ======================================================================================
 * FUNZIONE: check_win [RIF. DOMANDA: Descrizione del problema - Obiettivo]
 * ======================================================================================
 * SCOPO:
 * Verifica la connessione.
 *
 * GESTIONE MEMORIA:
 * Utilizziamo `calloc` per creare la matrice `visited` inizializzata a zero (false).
 * È fondamentale chiamare `free(visited)` alla fine per evitare leaks ricorrenti.
 * ======================================================================================
 */
bool check_win(Game *g) {
    /* 1. Trova START (uno dei due terminali) */
    int start_r = -1, start_c = -1, terminals = 0;
    for(int i=0; i<g->N; i++) {
        for(int j=0; j<g->N; j++) {
            Tile t = g->grid[i][j];
            if (!t.is_hole && (t.n+t.s+t.w+t.e) == 1) {
                if(start_r == -1) { start_r = i; start_c = j; }
                terminals++;
            }
        }
    }
    if (terminals != 2) return false; 

    /* 2. Setup DFS */
    /* Allocazione dinamica della matrice visited per la DFS */
    bool **visited = (bool**)malloc(g->N * sizeof(bool*));
    for(int i=0; i<g->N; i++) visited[i] = (bool*)calloc(g->N, sizeof(bool));

    /* 3. Esecuzione (Passiamo start_r/c come origine per escluderli dalla vittoria) */
    int found = 0;
    dfs_path(g, start_r, start_c, start_r, start_c, visited, &found);

    /* 4. Cleanup (Cruciale!) */
    for(int i=0; i<g->N; i++) free(visited[i]);
    free(visited);

    return (found == 1);
}

/* * ======================================================================================
 * FUNZIONE: problema_verifica [RIF. DOMANDA: Problema di verifica]
 * ======================================================================================
 * SCOPO: Esegue le mosse da file e controlla la vittoria.
 * ======================================================================================
 */
void problema_verifica(Game *g, char *filename_mosse) {
    FILE *fp = fopen(filename_mosse, "r");
    if (!fp) { printf("File mosse non trovato.\n"); return; }
    
    char m;
    int step = 0;
    bool possible = true;

    printf("\n--- VERIFICA MOSSE ---\n");
    while(fscanf(fp, " %c", &m) == 1) {
        step++;
        if(!apply_move(g, m)) {
            printf("Mossa %d (%c): NON VALIDA (Nessuna tessera mossa)\n", step, m);
            possible = false;
        } else {
            printf("Mossa %d (%c): OK\n", step, m);
            if(check_win(g)) {
                printf(" -> VITTORIA raggiunta alla mossa %d!\n", step);
                fclose(fp);
                return;
            }
        }
    }
    printf("Fine sequenza. Puzzle %s.\n", possible ? "non risolto ma mosse valide" : "con errori");
    fclose(fp);
}

/* * ======================================================================================
 * FUNZIONE: solve_recursive [RIF. DOMANDA: Problema di ricerca e ottimizzazione]
 * ======================================================================================
 * SCOPO: Funzione "Helper" per il Backtracking.
 *
 * CONCETTI CHIAVE - BACKTRACKING:
 * 
 * Questa tecnica si chiama "Snapshotting":
 * 1. Snapshot: Prima di toccare la griglia, la copiamo INTERAMENTE (con memcpy) in 'backup'.
 * 2. Do: Facciamo una mossa 'sperimentale'.
 * 3. Recurse: Vediamo se questa strada porta alla vittoria.
 * 4. Undo: Se la strada fallisce, ripristiniamo la griglia usando il 'backup'.
 * ======================================================================================
 */
bool solve_recursive(Game *g, char *path, int depth, int max_depth) {
    /* Casi Base */
    if (check_win(g)) { path[depth] = '\0'; return true; } // ho già vinto, metto carattere per fermarmi
    if (depth == max_depth) return false; //non ho vinto 

    char moves[] = {'U', 'D', 'L', 'R'};
    
    /* --- 1. SNAPSHOT (Salvataggio preventivo dello stato) --- */
    Tile **backup = (Tile**)malloc(g->N * sizeof(Tile*));
    for(int i=0; i<g->N; i++) {
        backup[i] = (Tile*)malloc(g->N * sizeof(Tile));
        /* memcpy: Copia brutale e veloce di N tessere dalla griglia al backup */
        memcpy(backup[i], g->grid[i], g->N * sizeof(Tile));
    }

    /* --- 2. TENTATIVI (Branching) --- */
    for (int i=0; i<4; i++) {
        if (apply_move(g, moves[i])) { 
            path[depth] = moves[i]; // Registro la mossa nel percorso
            
            /* Ricorsione: Scendo di un livello (depth + 1) */
            if (solve_recursive(g, path, depth + 1, max_depth)) {
                /* SUCCESSO: Ho trovato la soluzione nel ramo profondo.
                 * Devo liberare il backup prima di risalire, altrimenti Memory Leak! */
                for(int k=0; k<g->N; k++) free(backup[k]);
                free(backup);
                return true; 
            }

            /* --- 3. UNDO (Backtracking) --- */
            /* Se sono qui, la mossa 'moves[i]' non ha portato alla vittoria.
             * Devo ANNULLARLA per provare la prossima direzione del ciclo for.
             * Come? Ricopio i dati DAL backup ALLA griglia reale. */
            for(int k=0; k<g->N; k++) memcpy(g->grid[k], backup[k], g->N * sizeof(Tile));
        }
    }
    
    /* Fallimento del ramo: nessuna delle 4 direzioni ha funzionato qui */
    for(int k=0; k<g->N; k++) free(backup[k]);
    free(backup);
    
    return false;
}


/* * ======================================================================================
 * FUNZIONE: problema_ottimizzazione (Versione ITERATIVE DEEPENING)
 * ======================================================================================
 * SCOPO:
 * Trovare la soluzione OTTIMA (la più breve).
 * * CONCETTI CHIAVE - DFS vs ITERATIVE DEEPENING:
 * 
 * - La DFS classica scende in un ramo a caso fino a max_depth. Potrebbe trovare 
 * una soluzione valida ma lunghissima (es. 10 mosse invece di 2).
 * - L'Iterative Deepening lancia la DFS con limiti crescenti (1, 2, 3...).
 * Se trovo una soluzione a limite 2, sono CERTO che non ne esistono a limite 1.
 * Quindi è la soluzione ottima (più breve).
 * ======================================================================================
 */
void problema_ottimizzazione(Game *g, int max_mosse) {
    char *path = malloc((max_mosse + 1) * sizeof(char));
    bool found = false;

    printf("\n--- OTTIMIZZAZIONE (Iterative Deepening fino a %d) ---\n", max_mosse);
    
    /* CICLO ITERATIVE DEEPENING */
    /* Provo a cercare una soluzione lunga 1, poi lunga 2, poi 3... */
    for (int limit = 1; limit <= max_mosse; limit++) {
        
        /* Resetto la stringa path per pulizia */
        memset(path, 0, (max_mosse + 1) * sizeof(char)); //riempie un blocco di memoria con un valore a scelta
        
        /* Lancio il solver con il limite attuale.
         * Se ritorna true, ho trovato la soluzione più breve possibile per definizione. */
        if (solve_recursive(g, path, 0, limit)) {
            printf("Soluzione OTTIMA trovata con %d mosse: %s\n", limit, path);
            found = true;
            break; // Esco subito!
        }
    }

    if (!found) {
        printf("Nessuna soluzione trovata entro %d mosse.\n", max_mosse);
    }
    
    free(path);
}