#include <stdio.h>
#include <stdlib.h>
#include "GRID.h"
#include "WORDS.h"
#include "SOL.h"

/* --- LOGICA DI SUPPORTO --- */

/* Verifica se la parola w esiste in g. Restituisce coords e direzione */
/* Dir: 0=Orizz, 1=Vert, 2=Diag */
int trovaParola(Grid g, Word w, int *r, int *c, int *dir) {
    int i, j, k, match;

    for (i = 0; i < g.nr; i++) {
        for (j = 0; j < g.nc; j++) {
            
            /* Orizzontale */
            if (j + w.lun <= g.nc) {
                match = 1;
                for (k=0; k<w.lun; k++) if(g.mat[i][j+k] != w.parola[k]) match=0;
                if (match) { *r=i; *c=j; *dir=0; return 1; }
            }
            
            /* Verticale */
            if (i + w.lun <= g.nr) {
                match = 1;
                for (k=0; k<w.lun; k++) if(g.mat[i+k][j] != w.parola[k]) match=0;
                if (match) { *r=i; *c=j; *dir=1; return 1; }
            }
            
            /* Diagonale */
            if (i + w.lun <= g.nr && j + w.lun <= g.nc) {
                match = 1;
                for (k=0; k<w.lun; k++) if(g.mat[i+k][j+k] != w.parola[k]) match=0;
                if (match) { *r=i; *c=j; *dir=2; return 1; }
            }
        }
    }
    return 0;
}

/* * DOMANDA 6: Verifica Validità Insieme (Pag. 9)
 * Controlla sovrapposizioni (max 1 char condiviso).
 */
boolean checkValidita(Grid g, Sol s) {
    int **map; /* Mappa ID parole: -1 vuoto, 0..N indice parola */
    int i, j, k, r, c, d;
    boolean valid = VERO;

    /* Allocazione Mappa temporanea */
    map = malloc(g.nr * sizeof(int*));
    for(i=0; i<g.nr; i++) {
        map[i] = malloc(g.nc * sizeof(int));
        for(j=0; j<g.nc; j++) map[i][j] = -1;
    }

    for (i = 0; i < s.parole.numW && valid; i++) {
        Word w = s.parole.vettW[i];
        if (!trovaParola(g, w, &r, &c, &d)) {
            valid = FALSO; /* Parola non trovata (impossibile con pruning, ma safe check) */
        } else {
            /* Verifica intersezioni */
            for (k = 0; k < w.lun; k++) {
                int cr = r + (d==1 || d==2 ? k : 0);
                int cc = c + (d==0 || d==2 ? k : 0);

                if (map[cr][cc] != -1) {
                    /* Incrocio rilevato. Controlliamo se è valido */
                    int id_other = map[cr][cc];
                    /* Regola: Max 1 char. Errore se anche la cella PRECEDENTE
                       nel percorso di QUESTA parola era di id_other */
                    if (k > 0) {
                        int pr = r + (d==1 || d==2 ? k-1 : 0);
                        int pc = c + (d==0 || d==2 ? k-1 : 0);
                        if (map[pr][pc] == id_other) valid = FALSO;
                    }
                }
                map[cr][cc] = i; /* Marca cella */
            }
        }
    }

    /* Free mappa */
    for(i=0; i<g.nr; i++) free(map[i]);
    free(map);
    return valid;
}

/* * DOMANDA 7: Ottimizzazione (Pag. 11)
 * Modello: Powerset (Prendo/Lascio)
 */
void solve_recursive(Grid g, Words pool, int idx, Sol curr, Sol *best) {
    /* Caso Base: processate tutte le parole */
    if (idx == pool.numW) {
        if (checkValidita(g, curr)) {
            /* Aggiornamento Best: Valore maggiore o stesso valore ma più parole */
            if (curr.valore > best->valore || 
               (curr.valore == best->valore && curr.parole.numW > best->parole.numW)) {
                SOLfree(*best);
                *best = SOLcopy(curr);
            }
        }
        return;
    }

    /* 1. Ramo LASCIO (salto la parola idx) */
    solve_recursive(g, pool, idx+1, curr, best);

    /* 2. Ramo PRENDO (solo se la parola esiste nella griglia - PRUNING) */
    /* Nota: checkValidita completo è pesante, lo facciamo solo alla fine. 
       Qui controlliamo solo l'esistenza base. */
    int r,c,d;
    if (trovaParola(g, pool.vettW[idx], &r, &c, &d)) {
        SOLadd(&curr, pool.vettW[idx]);
        solve_recursive(g, pool, idx+1, curr, best);
        SOLremove(&curr); /* Backtrack */
    }
}

int main() {
    FILE *fpg, *fpw;
    Grid g;
    Words w;
    Sol curr, best;
    int i;

    /* --- CARICAMENTO FILE --- */
    fpg = fopen("griglia.txt", "r");
    if(!fpg) { perror("Errore griglia.txt"); return 1; }
    g = GRIDfile(fpg);
    fclose(fpg);

    fpw = fopen("parole.txt", "r");
    if(!fpw) { perror("Errore parole.txt"); return 1; }
    w = WORDSfile(fpw);
    fclose(fpw);

    printf("Griglia %dx%d caricata. Parole totali: %d\n", g.nr, g.nc, w.numW);
    GRIDprint(g);

    /* --- RISOLUZIONE --- */
    curr = SOLinit(w.numW);
    best = SOLinit(w.numW);

    solve_recursive(g, w, 0, curr, &best);

    printf("\n--- SOLUZIONE OTTIMA ---\n");
    printf("Valore: %d\n", best.valore);
    for(i=0; i<best.parole.numW; i++) {
        printf("- %s (%d)\n", best.parole.vettW[i].parola, best.parole.vettW[i].valore);
    }

    /* --- CLEANUP --- */
    GRIDfree(g);
    WORDSfree(w);
    SOLfree(curr);
    SOLfree(best);

    return 0;
}