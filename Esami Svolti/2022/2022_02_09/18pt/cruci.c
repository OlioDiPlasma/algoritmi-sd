#include "cruci.h"

/* ============================================================================
 * SEZIONE 1: GESTIONE MEMORIA E FILE (I/O)
 * ============================================================================ */

GRID carica_griglia(char *filename) {
    FILE *fp = fopen(filename, "r");
    GRID g;
    g.mat = NULL;
    
    if (fp == NULL) {
        printf("Errore apertura file griglia: %s\n", filename);
        exit(1);
    }

    fscanf(fp, "%d %d", &g.R, &g.C);
    
    g.mat = (char **)malloc(g.R * sizeof(char *));
    for (int i = 0; i < g.R; i++) {
        g.mat[i] = (char *)malloc(g.C * sizeof(char));
        char buffer[1024];
        fscanf(fp, "%s", buffer); 
        for(int j=0; j<g.C; j++) {
            g.mat[i][j] = buffer[j];
        }
    }
    fclose(fp);
    return g;
}

WORDS carica_parole(char *filename) {
    FILE *fp = fopen(filename, "r");
    WORDS w;
    w.n_parole = 0;
    w.elenco = NULL;

    if (fp == NULL) {
        printf("Errore apertura file parole: %s\n", filename);
        return w;
    }

    char tempStr[MAX_STR];
    int tempVal;
    
    while(fscanf(fp, "%s %d", tempStr, &tempVal) == 2) {
        w.n_parole++;
    }
    rewind(fp);

    w.elenco = (Word *)malloc(w.n_parole * sizeof(Word));

    int i = 0;
    while(fscanf(fp, "%s %d", w.elenco[i].str, &w.elenco[i].val) == 2) {
        i++;
    }
    fclose(fp);
    return w;
}

SOL carica_proposta(char *filename, WORDS parole_list) {
    FILE *fp = fopen(filename, "r");
    SOL s;
    s.n_found = 0;
    s.placements = NULL;
    s.total_val = 0;

    if (fp == NULL) return s;

    int r, c, dir;
    char str[MAX_STR];
    while(fscanf(fp, "%d %d %d %s", &r, &c, &dir, str) == 4) s.n_found++;
    rewind(fp);

    s.placements = (Placement *)malloc(s.n_found * sizeof(Placement));
    
    int i = 0;
    while(fscanf(fp, "%d %d %d %s", &r, &c, &dir, str) == 4) {
        s.placements[i].r = r;
        s.placements[i].c = c;
        s.placements[i].dir = dir;
        
        s.placements[i].word_idx = -1;
        for(int k=0; k<parole_list.n_parole; k++) {
            if(strcmp(parole_list.elenco[k].str, str) == 0) {
                s.placements[i].word_idx = k;
                break;
            }
        }
        i++;
    }
    fclose(fp);
    return s;
}

void libera_griglia(GRID g) {
    if(g.mat == NULL) return;
    for (int i = 0; i < g.R; i++) {
        free(g.mat[i]);
    }
    free(g.mat);
}
void libera_parole(WORDS w) {
    if(w.elenco != NULL) free(w.elenco);
}
void libera_soluzione(SOL s) {
    if (s.placements != NULL) free(s.placements);
}

/* ============================================================================
 * SEZIONE 2: VERIFICA
 * ============================================================================ */

int check_conflict(Placement p1, Placement p2, WORDS w) {
    int len1 = strlen(w.elenco[p1.word_idx].str);
    int len2 = strlen(w.elenco[p2.word_idx].str);
    int intersections = 0;

    for (int i = 0; i < len1; i++) {
        int r1 = p1.r + (p1.dir == 1 || p1.dir == 2 ? i : 0);
        int c1 = p1.c + (p1.dir == 0 || p1.dir == 2 ? i : 0);

        for (int j = 0; j < len2; j++) {
            int r2 = p2.r + (p2.dir == 1 || p2.dir == 2 ? j : 0);
            int c2 = p2.c + (p2.dir == 0 || p2.dir == 2 ? j : 0);

            if (r1 == r2 && c1 == c2) {
                intersections++;
            }
        }
    }
    return (intersections > 1);
}

int check_in_grid(GRID g, Placement p, char *wordStr) {
    int len = strlen(wordStr);
    int endR = p.r + (p.dir == 1 || p.dir == 2 ? len - 1 : 0);
    int endC = p.c + (p.dir == 0 || p.dir == 2 ? len - 1 : 0);

    if (p.r < 0 || p.c < 0 || endR >= g.R || endC >= g.C) return 0;

    for (int i = 0; i < len; i++) {
        int currR = p.r + (p.dir == 1 || p.dir == 2 ? i : 0);
        int currC = p.c + (p.dir == 0 || p.dir == 2 ? i : 0);
        if (g.mat[currR][currC] != wordStr[i]) return 0;
    }
    return 1;
}

int verifica_soluzione(GRID g, WORDS w, SOL s) {
    printf("\n--- VERIFICA PROPOSTA ---\n");
    int valid = 1;
    int calculated_val = 0;

    for (int i = 0; i < s.n_found; i++) {
        Placement p = s.placements[i];
        
        if (p.word_idx == -1) {
            printf("Errore: Parola %d non esiste in elenco.\n", i);
            valid = 0; continue;
        }
        
        char *str = w.elenco[p.word_idx].str;

        if (!check_in_grid(g, p, str)) {
            printf("Errore: Parola %s non trovata in pos (%d,%d) dir %d.\n", str, p.r, p.c, p.dir);
            valid = 0;
        }

        for (int j = i + 1; j < s.n_found; j++) {
            if (check_conflict(p, s.placements[j], w)) {
                printf("Errore: Sovrapposizione illegale tra %s e %s.\n", str, w.elenco[s.placements[j].word_idx].str);
                valid = 0;
            }
        }
        
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
 * SEZIONE 3: ALGORITMO RICORSIVO
 * ============================================================================ */

SOL bestSol;

void solve_rec(int idx, SOL *currentSol, Placement *candidates, int n_cand, WORDS w) {
    if (idx == n_cand) {
        if (currentSol->total_val > bestSol.total_val || 
           (currentSol->total_val == bestSol.total_val && currentSol->n_found > bestSol.n_found)) {
            
            if (bestSol.placements != NULL) free(bestSol.placements);
            bestSol.n_found = currentSol->n_found;
            bestSol.total_val = currentSol->total_val;
            bestSol.placements = malloc(bestSol.n_found * sizeof(Placement));
            memcpy(bestSol.placements, currentSol->placements, bestSol.n_found * sizeof(Placement));
        }
        return;
    }

    Placement cand = candidates[idx];

    // --- RAMO 1: PRENDO ---
    int compatibile = 1;
    for (int i = 0; i < currentSol->n_found; i++) {
        if (check_conflict(cand, currentSol->placements[i], w)) {
            compatibile = 0;
            break;
        }
    }

    if (compatibile) {
        currentSol->placements[currentSol->n_found] = cand;
        currentSol->n_found++;
        currentSol->total_val += w.elenco[cand.word_idx].val;

        solve_rec(idx + 1, currentSol, candidates, n_cand, w);

        currentSol->n_found--;
        currentSol->total_val -= w.elenco[cand.word_idx].val;
    }

    // --- RAMO 2: LASCIO ---
    solve_rec(idx + 1, currentSol, candidates, n_cand, w);
}

SOL trova_max_soluzione(GRID g, WORDS w) {
    Placement *candidates = malloc(g.R * g.C * 3 * w.n_parole * sizeof(Placement));
    int n_cand = 0;

    for (int k = 0; k < w.n_parole; k++) {
        char *s = w.elenco[k].str;
        for(int r=0; r<g.R; r++) {
            for(int c=0; c<g.C; c++) {
                for(int dir=0; dir<3; dir++) {
                    Placement p = {r, c, dir, k};
                    if (check_in_grid(g, p, s)) {
                        candidates[n_cand++] = p;
                    }
                }
            }
        }
    }

    printf("Trovati %d piazzamenti candidati totali.\n", n_cand);

    bestSol.n_found = 0;
    bestSol.total_val = 0;
    bestSol.placements = NULL;

    SOL curr;
    curr.placements = malloc(n_cand * sizeof(Placement));
    curr.n_found = 0;
    curr.total_val = 0;

    solve_rec(0, &curr, candidates, n_cand, w);

    free(candidates);
    free(curr.placements);

    return bestSol;
}