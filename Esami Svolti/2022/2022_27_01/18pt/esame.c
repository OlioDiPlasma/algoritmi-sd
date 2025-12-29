#include "esame.h"

/* ============================================================================
 * SEZIONE 1: GESTIONE MEMORIA E INPUT
 * ============================================================================ */

/*
 * Funzione: caricaGriglia
 * Scopo: Legge il file griglia.txt e alloca la matrice in memoria.
 * Parametri:
 * - filename: nome del file da aprire.
 * Concetti Chiave:
 * - Malloc matrice 2D: Prima allochiamo il vettore delle righe (int*), 
 * poi per ogni riga allochiamo le colonne (int).
 */
Griglia caricaGriglia(const char *filename) {
    FILE *fp = fopen(filename, "r");
    Griglia g;
    g.nr = 0; g.nc = 0; g.celle = NULL;

    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        return g;
    }

    // Lettura dimensioni
    fscanf(fp, "%d %d", &g.nr, &g.nc);

    // --- 1. ALLOCAZIONE MATRICE DINAMICA ---
    // Allochiamo un array di puntatori (le righe)
    g.celle = (int **)malloc(g.nr * sizeof(int *));
    
    for (int i = 0; i < g.nr; i++) {
        // Per ogni riga, allochiamo l'array di interi (le colonne)
        g.celle[i] = (int *)malloc(g.nc * sizeof(int));
        for (int j = 0; j < g.nc; j++) {
            fscanf(fp, "%d", &g.celle[i][j]);
        }
    }

    fclose(fp);
    return g;
}

/*
 * Funzione: liberaGriglia
 * Scopo: Rilascia la memoria per evitare Memory Leaks.
 * Concetti Chiave:
 * - Ordine inverso di allocazione: Prima liberiamo le singole righe,
 * poi il vettore dei puntatori alle righe.
 */
void liberaGriglia(Griglia g) {
    if (g.celle != NULL) {
        for (int i = 0; i < g.nr; i++) {
            free(g.celle[i]); // Libero la riga i
        }
        free(g.celle); // Libero il vettore di puntatori
    }
}

void stampaGriglia(Griglia g) {
    printf("--- Griglia %dx%d ---\n", g.nr, g.nc);
    for (int i = 0; i < g.nr; i++) {
        for (int j = 0; j < g.nc; j++) {
            printf("%d ", g.celle[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* ============================================================================
 * SEZIONE 2: VERIFICA PROPOSTA (Parte 1 dell'esercizio)
 * ============================================================================ */

/*
 * Funzione: verificaCopertura
 * Scopo: Legge una proposta e controlla:
 * 1. Quadrati dentro i bordi.
 * 2. Quadrati solo su celle bianche.
 * 3. Nessuna sovrapposizione tra quadrati.
 * 4. Copertura totale delle celle bianche.
 * Ritorna: Il numero di quadrati se valida, -1 se invalida.
 */
int verificaCopertura(Griglia g, const char *filename_proposta) {
    FILE *fp = fopen(filename_proposta, "r");
    if (!fp) return -1;

    int n_prop;
    fscanf(fp, "%d", &n_prop);

    // Creiamo una matrice di supporto "coperta" inizializzata a 0.
    // Ci servirà per segnare le celle coperte dai quadrati della proposta.
    // 0 = non coperto, 1 = coperto.
    int **coperta = (int **)malloc(g.nr * sizeof(int *));
    for (int i = 0; i < g.nr; i++) {
        coperta[i] = (int *)calloc(g.nc, sizeof(int)); // calloc inizializza a 0
    }

    int valido = TRUE; // Flag di validità

    // --- CICLO DI LETTURA QUADRATI ---
    for (int k = 0; k < n_prop; k++) {
        int r, c, lato;
        fscanf(fp, "%d %d %d", &r, &c, &lato);

        // Controllo bordi
        if (r < 0 || c < 0 || r + lato > g.nr || c + lato > g.nc) {
            printf("[Errore] Quadrato %d esce dai bordi.\n", k);
            valido = FALSE;
        }

        if (valido) {
            // Controllo contenuto quadrato
            for (int i = r; i < r + lato; i++) {
                for (int j = c; j < c + lato; j++) {
                    // Cella nera? Errore.
                    if (g.celle[i][j] == NERO) {
                        printf("[Errore] Quadrato su cella NERA in (%d,%d)\n", i, j);
                        valido = FALSE; 
                    }
                    // Già coperta? Errore (sovrapposizione).
                    if (coperta[i][j] == 1) {
                        printf("[Errore] Sovrapposizione in (%d,%d)\n", i, j);
                        valido = FALSE;
                    }
                    // Segna come coperta
                    coperta[i][j] = 1;
                }
            }
        }
    }
    fclose(fp);

    // --- CONTROLLO COPERTURA TOTALE ---
    // Se fin qui è valido, dobbiamo controllare se TUTTE le celle bianche sono coperte.
    if (valido) {
        for (int i = 0; i < g.nr; i++) {
            for (int j = 0; j < g.nc; j++) {
                if (g.celle[i][j] == BIANCO && coperta[i][j] == 0) {
                    printf("[Errore] Cella bianca non coperta in (%d,%d)\n", i, j);
                    valido = FALSE;
                }
            }
        }
    }

    // Pulizia memoria matrice di supporto
    for (int i = 0; i < g.nr; i++) free(coperta[i]);
    free(coperta);

    return (valido ? n_prop : -1);
}

/* ============================================================================
 * SEZIONE 3: OTTIMIZZAZIONE (Backtracking)
 * ============================================================================ */

// Variabili globali per il backtracking (semplificano la firma delle funzioni ricorsive)
int min_quadrati = 999999;
Soluzione best_sol;

/*
 * Funzione ausiliaria: puoMettereQuadrato
 * Controlla se posso piazzare un quadrato di lato 'lato' in (r,c)
 * senza uscire dai bordi e toccando solo celle BIANCHE non ancora coperte.
 */
bool puoMettereQuadrato(Griglia g, int **coperto, int r, int c, int lato) {
    if (r + lato > g.nr || c + lato > g.nc) return FALSE;
    
    for (int i = r; i < r + lato; i++) {
        for (int j = c; j < c + lato; j++) {
            // Se è nera o già coperta, non posso mettere il quadrato qui
            if (g.celle[i][j] == NERO || coperto[i][j] == 1) return FALSE;
        }
    }
    return TRUE;
}

/*
 * Funzione ausiliaria: marcaQuadrato
 * Imposta le celle della matrice 'coperto' a 1 (se metto) o 0 (se tolgo - backtrack).
 */
void marcaQuadrato(int **coperto, int r, int c, int lato, int valore) {
    for (int i = r; i < r + lato; i++) {
        for (int j = c; j < c + lato; j++) {
            coperto[i][j] = valore;
        }
    }
}

/*
 * Funzione Ricorsiva: solve
 * Parametri:
 * - g: la griglia base (sola lettura)
 * - coperto: matrice che tiene traccia dello stato attuale
 * - count: quanti quadrati ho usato finora
 * - current_sol: la lista dei quadrati piazzati finora
 */
void solve(Griglia g, int **coperto, int count, Soluzione *current_sol) {
    
    // --- 1. PRUNING (Potatura) ---
    // Se stiamo già usando più quadrati della migliore soluzione trovata finora, ci fermiamo.
    if (count >= min_quadrati) return;

    // --- 2. TROVARE LA PRIMA CELLA LIBERA ---
    int r = -1, c = -1;
    bool trovato = FALSE;
    
    // Scansioniamo la griglia per trovare la prima cella bianca NON coperta
    for (int i = 0; i < g.nr && !trovato; i++) {
        for (int j = 0; j < g.nc && !trovato; j++) {
            if (g.celle[i][j] == BIANCO && coperto[i][j] == 0) {
                r = i;
                c = j;
                trovato = TRUE;
            }
        }
    }

    // --- 3. CASO BASE (Tutto coperto) ---
    if (!trovato) {
        // Abbiamo coperto tutte le celle bianche!
        // Siccome c'è il pruning all'inizio, se siamo qui 'count' è sicuramente < min_quadrati
        min_quadrati = count;
        
        // Salviamo questa come la soluzione migliore
        best_sol.n_quadrati = count;
        // Riallochiamo il vettore della soluzione migliore
        best_sol.quadrati = (Quadrato*)realloc(best_sol.quadrati, count * sizeof(Quadrato));
        // Copiamo i dati dalla soluzione corrente
        for(int k=0; k<count; k++) {
            best_sol.quadrati[k] = current_sol->quadrati[k];
        }
        return;
    }

    // --- 4. PASSO RICORSIVO ---
    // Siamo sulla cella libera (r,c). Dobbiamo coprirla.
    // Proviamo a mettere quadrati di dimensione decrescente partendo dal massimo possibile.
    // Euristica: provare prima i quadrati grandi spesso porta prima alla soluzione ottima.
    
    int max_lato = (g.nr > g.nc) ? g.nr : g.nc; // Lato massimo teorico
    
    for (int lato = max_lato; lato >= 1; lato--) {
        if (puoMettereQuadrato(g, coperto, r, c, lato)) {
            
            // a) Faccio la mossa
            marcaQuadrato(coperto, r, c, lato, 1);
            
            // Aggiungo alla soluzione temporanea (assicurandomi ci sia spazio)
            current_sol->quadrati = realloc(current_sol->quadrati, (count + 1) * sizeof(Quadrato));
            current_sol->quadrati[count].r = r;
            current_sol->quadrati[count].c = c;
            current_sol->quadrati[count].lato = lato;

            // b) Ricorsione
            solve(g, coperto, count + 1, current_sol);

            // c) Backtracking (annullo la mossa)
            marcaQuadrato(coperto, r, c, lato, 0);
            // Non serve "cancellare" dall'array current_sol, verrà sovrascritto
        }
    }
}

void trovaCoperturaOttima(Griglia g) {
    // Inizializzazione matrice di supporto per il backtracking
    int **coperto = (int **)malloc(g.nr * sizeof(int *));
    for (int i = 0; i < g.nr; i++) {
        coperto[i] = (int *)calloc(g.nc, sizeof(int));
    }

    // Inizializzazione variabili globali/strutture
    min_quadrati = g.nr * g.nc + 1; // Valore alto iniziale
    best_sol.quadrati = NULL;
    best_sol.n_quadrati = 0;

    Soluzione current_sol;
    current_sol.quadrati = NULL;
    current_sol.n_quadrati = 0;

    printf("Avvio ricerca ottima (Backtracking)...\n");
    solve(g, coperto, 0, &current_sol);

    printf("\n=== SOLUZIONE OTTIMA TROVATA ===\n");
    printf("Numero minimo regioni: %d\n", min_quadrati);
    for (int i = 0; i < min_quadrati; i++) {
        Quadrato q = best_sol.quadrati[i];
        printf("Regione %c: Riga %d, Col %d, Lato %d\n", 'A'+i, q.r, q.c, q.lato);
    }

    // Pulizia
    for (int i = 0; i < g.nr; i++) free(coperto[i]);
    free(coperto);
    free(current_sol.quadrati);
    // Nota: best_sol.quadrati andrebbe liberato nel main o qui alla fine se non serve più.
    free(best_sol.quadrati); 
}