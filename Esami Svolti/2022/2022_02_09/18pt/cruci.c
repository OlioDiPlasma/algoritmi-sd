#include "cruci.h"

/* ============================================================================
 * SEZIONE 1: GESTIONE MEMORIA E FILE (I/O)
 * ============================================================================ */

/**
 * Funzione: carica_griglia
 * -----------------------
 * Legge la griglia di caratteri dal file specificato.
 * Il file deve avere il formato:
 * <R> <C>
 * <stringa_riga_0>
 * <stringa_riga_1>
 * ...
 * * Ritorna: Una struct GRID inizializzata. In caso di errore termina il programma.
 */
GRID carica_griglia(char *filename) {
    FILE *fp = fopen(filename, "r");
    GRID g;
    g.mat = NULL; // Inizializzazione difensiva
    
    // Controllo apertura file
    if (fp == NULL) {
        printf("Errore apertura file griglia: %s\n", filename);
        exit(1); // Uscita forzata in caso di errore critico
    }

    // Lettura dimensioni
    fscanf(fp, "%d %d", &g.R, &g.C);
    
    // Allocazione Dinamica Matrice (Array di Array)
    g.mat = (char **)malloc(g.R * sizeof(char *));
    
    for (int i = 0; i < g.R; i++) {
        g.mat[i] = (char *)malloc(g.C * sizeof(char));
        
        // Uso un buffer temporaneo per leggere la stringa dal file
        char buffer[1024];
        fscanf(fp, "%s", buffer); 
        
        // Copio carattere per carattere nella matrice
        for(int j=0; j<g.C; j++) {
            g.mat[i][j] = buffer[j];
        }
    }
    
    fclose(fp);
    return g;
}

/**
 * Funzione: carica_parole
 * ----------------------
 * Legge l'elenco delle parole e i relativi valori dal file.
 * Formato file: <parola> <valore>
 *
 * Strategia:
 * 1. Scansione preliminare per contare le parole (per allocare la memoria esatta).
 * 2. Rewind del file.
 * 3. Seconda scansione per caricare i dati nella struct.
 */
WORDS carica_parole(char *filename) {
    FILE *fp = fopen(filename, "r");
    WORDS w;
    w.n_parole = 0;
    w.elenco = NULL;

    if (fp == NULL) {
        printf("Errore apertura file parole: %s\n", filename);
        return w; // Ritorna struttura vuota
    }

    // Variabili temporanee per la lettura di conteggio
    char tempStr[MAX_STR];
    int tempVal;
    
    // 1. Conta le righe
    while(fscanf(fp, "%s %d", tempStr, &tempVal) == 2) {
        w.n_parole++;
    }
    
    // 2. Torna all'inizio del file
    rewind(fp);

    // 3. Alloca il vettore di struct Word
    w.elenco = (Word *)malloc(w.n_parole * sizeof(Word));

    // 4. Carica i dati effettivi
    int i = 0;
    while(fscanf(fp, "%s %d", w.elenco[i].str, &w.elenco[i].val) == 2) {
        i++;
    }
    
    fclose(fp);
    return w;
}

/**
 * Funzione: carica_proposta
 * -------------------------
 * Legge una proposta di soluzione dal file.
 * Formato riga: <riga> <colonna> <direzione> <parola>
 * * Nota: La struct 'SOL' restituita conterrà gli indici delle parole (word_idx)
 * riferiti all'elenco 'parole_list' passato come parametro.
 */
SOL carica_proposta(char *filename, WORDS parole_list) {
    FILE *fp = fopen(filename, "r");
    SOL s;
    s.n_found = 0;
    s.placements = NULL;
    s.total_val = 0;

    if (fp == NULL) return s;

    // Variabili per parsing
    int r, c, dir;
    char str[MAX_STR];
    
    // 1. Conta le proposte
    while(fscanf(fp, "%d %d %d %s", &r, &c, &dir, str) == 4) s.n_found++;
    rewind(fp);

    // 2. Alloca vettore piazzamenti
    s.placements = (Placement *)malloc(s.n_found * sizeof(Placement));
    
    int i = 0;
    while(fscanf(fp, "%d %d %d %s", &r, &c, &dir, str) == 4) {
        s.placements[i].r = r;
        s.placements[i].c = c;
        s.placements[i].dir = dir;
        
        // Cerca l'indice della parola nell'elenco generale (mapping stringa -> ID)
        s.placements[i].word_idx = -1; // Default invalido
        for(int k=0; k<parole_list.n_parole; k++) {
            if(strcmp(parole_list.elenco[k].str, str) == 0) {
                s.placements[i].word_idx = k;
                break; // Trovata
            }
        }
        i++;
    }
    fclose(fp);
    return s;
}

/* --- Funzioni di pulizia memoria (Indispensabili in C) --- */

void libera_griglia(GRID g) {
    if(g.mat == NULL) return;
    for (int i = 0; i < g.R; i++) {
        free(g.mat[i]); // Libera le righe
    }
    free(g.mat); // Libera il vettore di puntatori
}

void libera_parole(WORDS w) {
    if(w.elenco != NULL) free(w.elenco);
}

void libera_soluzione(SOL s) {
    if (s.placements != NULL) free(s.placements);
}

/* ============================================================================
 * SEZIONE 2: VERIFICA E CONTROLLI LOGICI
 * ============================================================================ */

/**
 * Funzione: check_conflict
 * ------------------------
 * Verifica se due piazzamenti (p1, p2) sono in conflitto.
 * Regola: Due parole possono incrociarsi, ma NON possono sovrapporsi
 * per più di una casella (altrimenti non è un incrocio valido da cruciverba).
 * * Ritorna: 1 se c'è conflitto (sovrapposizione > 1 char), 0 altrimenti.
 */
int check_conflict(Placement p1, Placement p2, WORDS w) {
    int len1 = strlen(w.elenco[p1.word_idx].str);
    int len2 = strlen(w.elenco[p2.word_idx].str);
    int intersections = 0;

    // Doppio ciclo nidificato per confrontare ogni cella occupata da p1 con ogni cella di p2
    for (int i = 0; i < len1; i++) {
        // Calcola coordinate i-esima lettera p1
        // dir 0=orizz (c++), 1=vert (r++), 2=diag (r++, c++)
        int r1 = p1.r + (p1.dir == 1 || p1.dir == 2 ? i : 0);
        int c1 = p1.c + (p1.dir == 0 || p1.dir == 2 ? i : 0);

        for (int j = 0; j < len2; j++) {
            // Calcola coordinate j-esima lettera p2
            int r2 = p2.r + (p2.dir == 1 || p2.dir == 2 ? j : 0);
            int c2 = p2.c + (p2.dir == 0 || p2.dir == 2 ? j : 0);

            // Se le coordinate coincidono, è un'intersezione
            if (r1 == r2 && c1 == c2) {
                intersections++;
            }
        }
    }
    // Conflitto se si toccano in più di un punto
    return (intersections > 1);
}

/**
 * Funzione: check_in_grid
 * -----------------------
 * Verifica due cose:
 * 1. BOUNDS CHECK: La parola esce dalla griglia?
 * 2. MATCH CHECK: La parola corrisponde ai caratteri presenti nella griglia?
 */
int check_in_grid(GRID g, Placement p, char *wordStr) {
    int len = strlen(wordStr);
    
    // Calcola coordinate finali
    int endR = p.r + (p.dir == 1 || p.dir == 2 ? len - 1 : 0);
    int endC = p.c + (p.dir == 0 || p.dir == 2 ? len - 1 : 0);

    // Controllo limiti (Bounds)
    if (p.r < 0 || p.c < 0 || endR >= g.R || endC >= g.C) return 0;

    // Controllo corrispondenza caratteri
    for (int i = 0; i < len; i++) {
        int currR = p.r + (p.dir == 1 || p.dir == 2 ? i : 0);
        int currC = p.c + (p.dir == 0 || p.dir == 2 ? i : 0);
        
        if (g.mat[currR][currC] != wordStr[i]) return 0; // Mismatch!
    }
    return 1; // Tutto ok
}

/**
 * Funzione: verifica_soluzione
 * ----------------------------
 * Funzione "Master" di verifica richiesta dal punto 2 del compito.
 * Controlla validità semantica, geometrica e conflitti di una intera soluzione.
 */
int verifica_soluzione(GRID g, WORDS w, SOL s) {
    printf("\n--- VERIFICA PROPOSTA ---\n");
    int valid = 1;
    int calculated_val = 0;

    // Itera su tutti i piazzamenti proposti
    for (int i = 0; i < s.n_found; i++) {
        Placement p = s.placements[i];
        
        // Check esistenza parola
        if (p.word_idx == -1) {
            printf("Errore: Parola %d non esiste in elenco.\n", i);
            valid = 0; continue;
        }
        
        char *str = w.elenco[p.word_idx].str;

        // Check posizionamento nella griglia
        if (!check_in_grid(g, p, str)) {
            printf("Errore: Parola %s non trovata in pos (%d,%d) dir %d.\n", str, p.r, p.c, p.dir);
            valid = 0;
        }

        // Check conflitti con TUTTE le altre parole della soluzione
        for (int j = i + 1; j < s.n_found; j++) {
            if (check_conflict(p, s.placements[j], w)) {
                printf("Errore: Sovrapposizione illegale tra %s e %s.\n", str, w.elenco[s.placements[j].word_idx].str);
                valid = 0;
            }
        }
        
        // Se tutto ok fin qui, accumula il valore
        if (valid) calculated_val += w.elenco[p.word_idx].val;
    }

    if (valid) {
        printf("Proposta VALIDA. Valore totale: %d\n", calculated_val);
        return calculated_val;
    } else {
        printf("Proposta NON VALIDA.\n");
        return -1;
    }
}

/* ============================================================================
 * SEZIONE 3: ALGORITMO RICORSIVO (BACKTRACKING)
 * ============================================================================ */

SOL bestSol; // Variabile globale per mantenere traccia del record

/**
 * Funzione: solve_rec
 * -------------------
 * Cuore dell'algoritmo. Esplora lo spazio delle soluzioni.
 * * Parametri:
 * - idx: Indice del candidato che stiamo valutando (livello dell'albero di ricorsione).
 * - currentSol: Puntatore alla soluzione parziale che stiamo costruendo.
 * - candidates: Array di TUTTI i piazzamenti possibili pre-calcolati.
 * - n_cand: Dimensione array candidates.
 * * Modello: Insieme delle Parti (Subset Sum like).
 * Per ogni candidato abbiamo un bivio:
 * 1. Lo prendiamo (se compatibile).
 * 2. Non lo prendiamo.
 */
void solve_rec(int idx, SOL *currentSol, Placement *candidates, int n_cand, WORDS w) {
    
    /* --- 1. BASE CASE (Foglia dell'albero) --- */
    if (idx == n_cand) {
        // Abbiamo valutato tutti i candidati. Controlliamo se è un nuovo record.
        // Criterio: Maggior valore totale OPPURE Stesso valore ma più parole usate.
        if (currentSol->total_val > bestSol.total_val || 
           (currentSol->total_val == bestSol.total_val && currentSol->n_found > bestSol.n_found)) {
            
            // Aggiornamento Best Solution
            if (bestSol.placements != NULL) free(bestSol.placements); // Pulisci vecchia
            
            bestSol.n_found = currentSol->n_found;
            bestSol.total_val = currentSol->total_val;
            
            // Deep Copy del vettore piazzamenti
            bestSol.placements = malloc(bestSol.n_found * sizeof(Placement));
            memcpy(bestSol.placements, currentSol->placements, bestSol.n_found * sizeof(Placement));
        }
        return;
    }

    Placement cand = candidates[idx];

    /* --- 2. PASSO RICORSIVO --- */

    // RAMO 1: PROVO A PRENDERE IL CANDIDATO
    // Verifica compatibilità con ciò che ho già preso
    int compatibile = 1;
    for (int i = 0; i < currentSol->n_found; i++) {
        if (check_conflict(cand, currentSol->placements[i], w)) {
            compatibile = 0;
            break; // Conflitto trovato, inutile continuare controllo
        }
    }

    if (compatibile) {
        // DO: Aggiungo alla soluzione corrente
        currentSol->placements[currentSol->n_found] = cand;
        currentSol->n_found++;
        currentSol->total_val += w.elenco[cand.word_idx].val;

        // RECURSE: Scendo nel prossimo livello
        solve_rec(idx + 1, currentSol, candidates, n_cand, w);

        // BACKTRACK (UNDO): Ripristino stato per esplorare altre vie
        currentSol->n_found--;
        currentSol->total_val -= w.elenco[cand.word_idx].val;
    }

    // RAMO 2: NON PRENDO IL CANDIDATO (SKIP)
    // Esploro sempre l'opzione di scartare la parola corrente
    solve_rec(idx + 1, currentSol, candidates, n_cand, w);
}

/**
 * Funzione: trova_max_soluzione
 * -----------------------------
 * Wrapper per lanciare la ricorsione.
 * Strategia:
 * 1. PRE-CALCOLO (Filtering): Invece di provare parole a caso in posizioni a caso,
 * generiamo prima una lista di "Candidati Validi". Un candidato è una parola
 * che "entra" fisicamente nella griglia in una certa posizione.
 * 2. LANCIO BACKTRACKING: Chiamiamo solve_rec su questa lista raffinata.
 */
SOL trova_max_soluzione(GRID g, WORDS w) {
    // Stima allocazione massima (brutale ma sicura)
    Placement *candidates = malloc(g.R * g.C * 3 * w.n_parole * sizeof(Placement));
    int n_cand = 0;

    // Generazione Candidati: Provo ogni parola in ogni cella in ogni direzione
    for (int k = 0; k < w.n_parole; k++) {
        char *s = w.elenco[k].str;
        for(int r=0; r<g.R; r++) {
            for(int c=0; c<g.C; c++) {
                for(int dir=0; dir<3; dir++) {
                    Placement p = {r, c, dir, k};
                    // Se la parola "fitta" nella griglia, è un candidato valido
                    if (check_in_grid(g, p, s)) {
                        candidates[n_cand++] = p;
                    }
                }
            }
        }
    }

    printf("Trovati %d piazzamenti candidati totali. Inizio ricerca...\n", n_cand);

    // Inizializzazione Best Solution globale
    bestSol.n_found = 0;
    bestSol.total_val = 0;
    bestSol.placements = NULL;

    // Inizializzazione Soluzione Corrente (vuota)
    SOL curr;
    curr.placements = malloc(n_cand * sizeof(Placement)); // Max dimensione possibile
    curr.n_found = 0;
    curr.total_val = 0;

    // Start Ricorsione
    solve_rec(0, &curr, candidates, n_cand, w);

    // Pulizia memoria ausiliaria
    free(candidates);
    free(curr.placements);

    return bestSol;
}