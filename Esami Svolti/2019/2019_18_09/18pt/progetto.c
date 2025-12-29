/*
 * FILE: progetto.c
 * SCOPO: Implementazione logica. Gestione memoria, BFS e Backtracking.
 */

#include "progetto.h"

// Costanti per le direzioni (Su, Giù, Sinistra, Destra) per l'adiacenza
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};

/* --- 1. GESTIONE MEMORIA E UTILITÀ --- */

/*
 * Funzione: inizializzaMatrice
 * Scopo: Allocare una matrice 2D contigua o a vettori di puntatori.
 * Concetti Chiave:
 * - malloc vs calloc: Qui uso malloc + loop per chiarezza didattica sui doppi puntatori.
 * - int **mat: mat è un puntatore a un array di puntatori (le righe).
 */
int** inizializzaMatrice(int nr, int nc, int val) {
    int **mat = (int**)malloc(nr * sizeof(int*));
    if (mat == NULL) { printf("Errore malloc righe\n"); exit(1); }

    for (int i = 0; i < nr; i++) {
        mat[i] = (int*)malloc(nc * sizeof(int));
        if (mat[i] == NULL) { printf("Errore malloc colonna %d\n", i); exit(1); }
        for (int j = 0; j < nc; j++) {
            mat[i][j] = val; // Inizializzazione valore
        }
    }
    return mat;
}

/*
 * Funzione: liberaMatrice
 * Scopo: Evitare memory leaks.
 * Concetti Chiave: Bisogna liberare PRIMA le righe, POI il vettore dei puntatori alle righe.
 */
void liberaMatrice(int **mat, int nr) {
    if (mat == NULL) return;
    for (int i = 0; i < nr; i++) {
        free(mat[i]);
    }
    free(mat);
}

void liberaMappa(Mappa *m) {
    if (m) {
        liberaMatrice(m->griglia, m->nr);
        free(m);
    }
}

void liberaSoluzione(Soluzione *s) {
    if (s) {
        if (s->risorse) free(s->risorse);
        if (s->copertura) liberaMatrice(s->copertura, 0); // La matrice copertura ha dim variabili, gestita nel main solitamente, ma qui passo 0 per sicurezza o la gestisco puntualmente
        // Nota: Nel codice reale, dovremmo passare nr al distruttore.
        // Per semplicità qui assumo che chi chiama sappia le dimensioni o liberi manualmente.
        free(s);
    }
}

/* --- 2. LETTURA FILE --- */

Mappa* leggiMappa(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    Mappa *m = (Mappa*)malloc(sizeof(Mappa));
    fscanf(f, "%d %d", &m->nr, &m->nc);

    // 0 = libera, -1 = ostacolo
    m->griglia = inizializzaMatrice(m->nr, m->nc, 0);

    int r, c;
    // Leggo coordinate ostacoli fino a fine file
    while (fscanf(f, "%d %d", &r, &c) == 2) {
        if(r >= 0 && r < m->nr && c >= 0 && c < m->nc)
            m->griglia[r][c] = -1; // Marco ostacolo
    }
    fclose(f);
    return m;
}

Soluzione* leggiProposta(char *filename, int nr, int nc) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    Soluzione *s = (Soluzione*)malloc(sizeof(Soluzione));
    fscanf(f, "%d %d", &s->k, &s->n_risorse);

    // Allocazione array risorse
    s->risorse = (Coordinate*)malloc(s->n_risorse * sizeof(Coordinate));
    for (int i = 0; i < s->n_risorse; i++) {
        fscanf(f, "%d %d", &s->risorse[i].r, &s->risorse[i].c);
    }

    // Lettura matrice proposta
    s->copertura = inizializzaMatrice(nr, nc, 0);
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            fscanf(f, "%d", &s->copertura[i][j]);
        }
    }
    fclose(f);
    return s;
}

/* --- 3. LOGICA DI COPERTURA (BFS) --- */

/*
 * Funzione: bfsCopertura
 * Scopo: Esegue una BFS partendo da una risorsa per marcare le celle raggiungibili entro K passi.
 * Parametri:
 * - matCopertura: La matrice dove scriviamo l'ID della risorsa (modificata per side-effect).
 * - idRisorsa: Numero della risorsa (1, 2, ...).
 * Concetti Chiave:
 * - Coda: Usiamo array statici per semplicità (o allocati grandi quanto la mappa) per gestire la frontiera della BFS.
 * - Distanza: Teniamo traccia della distanza per fermarci a K.
 */
int bfsCopertura(Mappa *m, int startR, int startC, int k, int idRisorsa, int **matCopertura) {
    // Controllo base: se la cella di partenza è un muro o fuori
    if (startR < 0 || startR >= m->nr || startC < 0 || startC >= m->nc) return 0;
    if (m->griglia[startR][startC] == -1) return 0; // Risorsa su muro non ammessa

    // Se la cella è già coperta da UN'ALTRA risorsa -> Errore (sovrapposizione)
    if (matCopertura[startR][startC] != 0 && matCopertura[startR][startC] != idRisorsa) return -1; // Collisione

    // Struttura interna per la Coda BFS
    typedef struct { int r, c, dist; } Nodo;
    int size = m->nr * m->nc;
    Nodo *coda = (Nodo*)malloc(size * sizeof(Nodo));
    int head = 0, tail = 0;
    
    // Matrice visitati LOCALE per questa singola BFS (per non ciclare infinito)
    int **visitato = inizializzaMatrice(m->nr, m->nc, 0);

    // Push start
    coda[tail++] = (Nodo){startR, startC, 0};
    visitato[startR][startC] = 1;
    matCopertura[startR][startC] = idRisorsa; // Copro la cella della risorsa

    int celleCoperte = 0;

    while (head < tail) {
        Nodo curr = coda[head++];
        celleCoperte++;

        if (curr.dist < k) {
            // Provo le 4 direzioni
            for (int i = 0; i < 4; i++) {
                int nr = curr.r + dr[i];
                int nc = curr.c + dc[i];

                // Controlli validità
                if (nr >= 0 && nr < m->nr && nc >= 0 && nc < m->nc) { // Bounds
                    if (m->griglia[nr][nc] != -1 && !visitato[nr][nc]) { // No Muro e No Già Visitato in questa BFS
                        
                        // Controllo CRUCIALE: Se la cella è già coperta da ALTRA risorsa globalmente
                        if (matCopertura[nr][nc] != 0 && matCopertura[nr][nc] != idRisorsa) {
                            free(coda); liberaMatrice(visitato, m->nr);
                            return -1; // ERRORE: Sovrapposizione
                        }

                        // Marco e aggiungo
                        matCopertura[nr][nc] = idRisorsa;
                        visitato[nr][nc] = 1;
                        coda[tail++] = (Nodo){nr, nc, curr.dist + 1};
                    }
                }
            }
        }
    }

    free(coda);
    liberaMatrice(visitato, m->nr);
    return celleCoperte;
}

/* --- 4. VERIFICA E CALCOLO --- */

/*
 * Calcola l'intera matrice di copertura basata SOLO sulle posizioni delle risorse.
 * Ritorna 1 se valido, 0 se invalido (sovrapposizioni o risorse su muri).
 */
int calcolaCoperturaTotale(Mappa *m, Coordinate *risorse, int n_ris, int k, int **matOutput) {
    // Pulisco la matrice output
    for(int i=0; i<m->nr; i++) 
        for(int j=0; j<m->nc; j++) matOutput[i][j] = 0;

    for (int i = 0; i < n_ris; i++) {
        // ID risorsa è i+1
        int res = bfsCopertura(m, risorse[i].r, risorse[i].c, k, i + 1, matOutput);
        if (res == -1 || res == 0) return 0; // Sovrapposizione o posizione non valida
    }
    return 1;
}

int verificaAmmissibilita(Mappa *m, Soluzione *s) {
    printf("\n--- VERIFICA PROPOSTA ---\n");
    int **matCalcolata = inizializzaMatrice(m->nr, m->nc, 0);
    
    // 1. Verifica validità logica (sovrapposizioni, muri)
    if (!calcolaCoperturaTotale(m, s->risorse, s->n_risorse, s->k, matCalcolata)) {
        printf("ERRORE: Configurazione risorse non valida (sovrapposizioni o su ostacoli).\n");
        liberaMatrice(matCalcolata, m->nr);
        return 0;
    }

    // 2. Verifica corrispondenza con file proposta
    int corrisponde = 1;
    for (int i = 0; i < m->nr; i++) {
        for (int j = 0; j < m->nc; j++) {
            if (matCalcolata[i][j] != s->copertura[i][j]) {
                corrisponde = 0;
                // Debug didattico
                // printf("Mismatch a (%d,%d): Calcolato %d vs File %d\n", i, j, matCalcolata[i][j], s->copertura[i][j]);
            }
        }
    }

    liberaMatrice(matCalcolata, m->nr);

    if (corrisponde) {
        printf("La proposta e' VALIDA e la mappa corrisponde.\n");
        return 1;
    } else {
        printf("La configurazione risorse e' lecita, ma la mappa nel file proposta NON e' corretta.\n");
        return 0; // È tecnicamente non ammissibile perché il file mente, anche se le risorse sono ok
    }
}

/* --- 5. OTTIMIZZAZIONE E CORREZIONE (Backtracking) --- */

int max_celle_coperte = -1;
Coordinate *best_risorse = NULL; // Per salvare la soluzione migliore

/*
 * Funzione Ricorsiva per trovare la migliore combinazione di Z risorse
 */
void backtrack(Mappa *m, int k, int z_da_piazzare, int startR, int startC, Coordinate *curr_risorse, int count, int **matriceGlobale) {
    
    // CASO BASE: Abbiamo piazzato tutte le risorse
    if (count == z_da_piazzare) {
        // Calcoliamo quante celle sono coperte
        int coperte = 0;
        for(int i=0; i<m->nr; i++)
            for(int j=0; j<m->nc; j++)
                if(matriceGlobale[i][j] > 0) coperte++;
        
        if (coperte > max_celle_coperte) {
            max_celle_coperte = coperte;
            // Salvo le coordinate
            for(int i=0; i<z_da_piazzare; i++) best_risorse[i] = curr_risorse[i];
        }
        return;
    }

    // Iterazione sulla griglia per posizionare la risorsa 'count'
    // Ottimizzazione: Non ripartire da 0,0 ma dall'ultima posizionata (per evitare permutazioni identiche)
    // Per semplicità didattica, scorriamo tutto linearmente convertendo indice in r,c
    int startIdx = startR * m->nc + startC;
    int totalCells = m->nr * m->nc;

    for (int i = startIdx; i < totalCells; i++) {
        int r = i / m->nc;
        int c = i % m->nc;

        if (m->griglia[r][c] == 0 && matriceGlobale[r][c] == 0) { // Cella libera e non coperta (euristica)
            // ATTENZIONE: matriceGlobale==0 qui è una condizione forte. 
            // Il testo dice: risorse su celle libere. Non dice che la risorsa non può stare su cella coperta da altra risorsa?
            // "non e' possibile che una cella sia coperta da piu di una risorsa".
            // Quindi se piazzo qui, devo verificare che la sua BFS non tocchi le aree delle altre.
            
            // Salvo stato
            int **backupMat = inizializzaMatrice(m->nr, m->nc, 0);
            // Copio stato attuale
            for(int rr=0; rr<m->nr; rr++) for(int cc=0; cc<m->nc; cc++) backupMat[rr][cc] = matriceGlobale[rr][cc];

            // Provo a piazzare (simulo BFS)
            int res = bfsCopertura(m, r, c, k, count + 1, matriceGlobale);
            
            if (res != -1 && res != 0) { // Se valido (no collisioni)
                curr_risorse[count].r = r;
                curr_risorse[count].c = c;
                
                backtrack(m, k, z_da_piazzare, r, c + 1, curr_risorse, count + 1, matriceGlobale);
            }

            // BACKTRACK: Ripristino stato
            for(int rr=0; rr<m->nr; rr++) for(int cc=0; cc<m->nc; cc++) matriceGlobale[rr][cc] = backupMat[rr][cc];
            liberaMatrice(backupMat, m->nr);
        }
    }
}

void generaCorretto(Mappa *m, Soluzione *s) {
    // Se la verifica ha fallito ma "esiste soluzione ammissibile con le stesse risorse",
    // significa usare lo stesso NUMERO di risorse Z per trovare una configurazione valida.
    // O più semplicemente: se le coordinate in s->risorse erano valide ma la mappa proposta errata,
    // basta ricalcolare. Se le coordinate erano invalide, dobbiamo cercarne di nuove.
    // Assumiamo il caso complesso: troviamo nuove coordinate per Z risorse.

    printf("\n--- GENERAZIONE CORRETTO.TXT ---\n");
    max_celle_coperte = -1;
    best_risorse = (Coordinate*)malloc(s->n_risorse * sizeof(Coordinate));
    Coordinate *temp_risorse = (Coordinate*)malloc(s->n_risorse * sizeof(Coordinate));
    int **matriceVuota = inizializzaMatrice(m->nr, m->nc, 0);

    // Lancio Backtracking
    backtrack(m, s->k, s->n_risorse, 0, 0, temp_risorse, 0, matriceVuota);

    if (max_celle_coperte > 0) {
        FILE *f = fopen("corretto.txt", "w");
        fprintf(f, "%d %d\n", s->k, s->n_risorse);
        for(int i=0; i<s->n_risorse; i++) {
            fprintf(f, "%d %d\n", best_risorse[i].r, best_risorse[i].c);
        }
        
        // Genero la mappa finale per il file
        int **matFinale = inizializzaMatrice(m->nr, m->nc, 0);
        calcolaCoperturaTotale(m, best_risorse, s->n_risorse, s->k, matFinale);
        for(int i=0; i<m->nr; i++) {
            for(int j=0; j<m->nc; j++) {
                fprintf(f, "%d ", matFinale[i][j]);
            }
            fprintf(f, "\n");
        }
        fclose(f);
        liberaMatrice(matFinale, m->nr);
        printf("File corretto.txt generato con %d celle coperte.\n", max_celle_coperte);
    } else {
        printf("Impossibile trovare soluzione valida con %d risorse.\n", s->n_risorse);
    }

    free(best_risorse); free(temp_risorse); liberaMatrice(matriceVuota, m->nr);
}