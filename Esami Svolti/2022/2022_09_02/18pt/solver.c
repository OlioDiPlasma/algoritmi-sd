#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

/* Array di spiazzamento: Nord, Est, Sud, Ovest */
int dr[] = {-1, 0, 1, 0};
int dc[] = { 0, 1, 0, -1};

/* Variabili Globali per il Backtracking */
int min_cambi = 9999;       // Record attuale di cambi direzione (inizializzato alto)
int **soluzione_migliore;   // Matrice per salvare il disegno del percorso migliore
int **matrice_passi;        // Matrice di lavoro per la ricorsione (tiene i numeri 1,2,3...)

/* --- FUNZIONI DI UTILITÀ --- */

// Copia la matrice di lavoro (matrice_passi) in quella di salvataggio (soluzione_migliore)
void salva_soluzione(int nr, int nc) {
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            soluzione_migliore[i][j] = matrice_passi[i][j];
        }
    }
}

// Stampa una matrice a video formattata
void stampa_matrice_soluzione(int nr, int nc) {
    printf("\n--- VISUALIZZAZIONE PERCORSO OTTIMO ---\n");
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if (soluzione_migliore[i][j] == 0) 
                printf("  # "); // Ostacolo o non visitato
            else 
                printf("%3d ", soluzione_migliore[i][j]); // Numero del passo
        }
        printf("\n");
    }
}

/* --- FUNZIONI RICHIESTE --- */

void leggi_griglia(char *nome_file, Griglia *g) {
    FILE *fp = fopen(nome_file, "r");
    if (!fp) { perror("Errore file"); exit(1); }

    fscanf(fp, "%d %d", &g->nr, &g->nc);
    
    g->celle = (int **)malloc(g->nr * sizeof(int *));
    g->celle_bianche_totali = 0;

    for (int i = 0; i < g->nr; i++) {
        g->celle[i] = (int *)malloc(g->nc * sizeof(int));
        for (int j = 0; j < g->nc; j++) {
            fscanf(fp, "%d", &g->celle[i][j]);
            if (g->celle[i][j] == BIANCO) g->celle_bianche_totali++;
        }
    }
    fclose(fp);
}

void verifica_proposta(Griglia g) {
    printf("\n--- VERIFICA PROPOSTA ---\n");
    FILE *fp = fopen("proposta.txt", "r");
    if (!fp) { printf("File proposta.txt non trovato.\n"); return; }

    char mosse[1000];
    if (fscanf(fp, "%s", mosse) != 1) { printf("File vuoto.\n"); fclose(fp); return; }
    fclose(fp);

    // Allocazione locale per verifica
    int **vis = (int **)malloc(g.nr * sizeof(int*));
    for(int i=0; i<g.nr; i++) vis[i] = (int*)calloc(g.nc, sizeof(int));

    int r = 0, c = 0;
    vis[0][0] = 1;
    int passaggi = 1;
    int cambi = 0;
    Direzione dir_prev = NESSUNA;
    int valida = 1;

    for (int i = 0; i < strlen(mosse); i++) {
        int dir = -1;
        if (mosse[i] == 'N') dir = NORD;
        else if (mosse[i] == 'E') dir = EST;
        else if (mosse[i] == 'S') dir = SUD;
        else if (mosse[i] == 'O') dir = OVEST;

        int nr = r + dr[dir];
        int nc = c + dc[dir];

        if (nr < 0 || nr >= g.nr || nc < 0 || nc >= g.nc || 
            g.celle[nr][nc] == NERO || vis[nr][nc]) {
            printf("Errore: mossa %c porta in (%d,%d) non valida.\n", mosse[i], nr, nc);
            valida = 0; break;
        }

        if (dir_prev != NESSUNA && dir_prev != dir) cambi++;
        
        vis[nr][nc] = 1;
        passaggi++;
        r = nr; c = nc;
        dir_prev = (Direzione)dir;
    }

    if (valida && passaggi == g.celle_bianche_totali)
        printf("Proposta VALIDA. Cambi direzione: %d\n", cambi);
    else if (valida)
        printf("Proposta incompleta. Celle visitate: %d/%d\n", passaggi, g.celle_bianche_totali);

    for(int i=0; i<g.nr; i++) free(vis[i]);
    free(vis);
}

/* Funzione Ricorsiva (Core dell'algoritmo) */
void cerca_cammino(Griglia g, int r, int c, Direzione dir_in, int cambi_attuali, int count_visite) {
    
    // POTATURA: Se ho già fatto più cambi del record migliore, inutile continuare
    if (cambi_attuali >= min_cambi) return;

    // CASO BASE: Ho visitato tutte le celle bianche
    if (count_visite == g.celle_bianche_totali) {
        min_cambi = cambi_attuali;
        salva_soluzione(g.nr, g.nc); // Salvo la "fotografia" della matrice attuale
        return;
    }

    // PASSO RICORSIVO: Provo le 4 direzioni
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        // Controllo: Dentro griglia? Cella bianca? Non visitata (matrice_passi == 0)?
        if (nr >= 0 && nr < g.nr && nc >= 0 && nc < g.nc && 
            g.celle[nr][nc] == BIANCO && matrice_passi[nr][nc] == 0) {

            // Calcolo nuovo costo
            int nuovi_cambi = cambi_attuali;
            if (dir_in != NESSUNA && dir_in != i) nuovi_cambi++;

            // DO (Faccio la mossa)
            matrice_passi[nr][nc] = count_visite + 1; // Segno il numero del passo (es. 2, 3...)

            // RECURSE
            cerca_cammino(g, nr, nc, (Direzione)i, nuovi_cambi, count_visite + 1);

            // UNDO (Backtracking - cancello la mossa per provare altre strade)
            matrice_passi[nr][nc] = 0;
        }
    }
}

void trova_soluzione_ottima(Griglia g) {
    printf("\n--- RICERCA OTTIMIZZAZIONE ---\n");

    // Allocazione matrici di supporto
    matrice_passi = (int **)malloc(g.nr * sizeof(int *));
    soluzione_migliore = (int **)malloc(g.nr * sizeof(int *));
    for(int i=0; i<g.nr; i++) {
        matrice_passi[i] = (int *)calloc(g.nc, sizeof(int));
        soluzione_migliore[i] = (int *)calloc(g.nc, sizeof(int));
    }

    // Inizializzazione start
    matrice_passi[0][0] = 1; // Primo passo
    min_cambi = 9999;        // Reset del minimo

    // Avvio ricorsione
    cerca_cammino(g, 0, 0, NESSUNA, 0, 1);

    if (min_cambi == 9999) {
        printf("Impossibile visitare tutte le celle.\n");
    } else {
        printf("Soluzione ottima trovata: %d cambi di direzione.\n", min_cambi);
        stampa_matrice_soluzione(g.nr, g.nc);
    }

    // Pulizia
    for(int i=0; i<g.nr; i++) { free(matrice_passi[i]); free(soluzione_migliore[i]); }
    free(matrice_passi); free(soluzione_migliore);
}

void libera_griglia(Griglia *g) {
    for (int i = 0; i < g->nr; i++) free(g->celle[i]);
    free(g->celle);
}