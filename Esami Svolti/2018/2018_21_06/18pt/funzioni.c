#include "defines.h"

/*
 * Funzione: calcolaPunteggio
 * Scopo: Calcola il punteggio totale della scacchiera piena.
 * Concetti Chiave: 
 * - Scorrimento matrice.
 * - Gestione rotazione: se rot=1, orizzontale diventa verticale e viceversa.
 */
int calcolaPunteggio(Cella **scacchiera, int R, int C, Tessera *tessere) {
    int i, j;
    int puntiTotali = 0;
    
    // 1. Controllo le RIGHE (Tubi orizzontali)
    for (i = 0; i < R; i++) {
        char primoColore = '\0'; // Carattere nullo
        int sommaRiga = 0;
        int rigaValida = 1; // Flag booleano

        for (j = 0; j < C; j++) {
            int id = scacchiera[i][j].id_tessera;
            int rot = scacchiera[i][j].rot;
            
            // Determino colore e valore corrente orizzontale
            // Se rot=0 prendo c1, se rot=1 prendo c2 (perché ruotata)
            char colH = (rot == 0) ? tessere[id].c1 : tessere[id].c2;
            int valH  = (rot == 0) ? tessere[id].v1 : tessere[id].v2;

            if (j == 0) {
                primoColore = colH; // Inizializzo col primo della riga
            } else {
                if (colH != primoColore) {
                    rigaValida = 0; // Colore diverso, riga annullata
                    break;          // Esco dal ciclo interno, inutile continuare
                }
            }
            sommaRiga += valH;
        }
        if (rigaValida) puntiTotali += sommaRiga;
    }

    // 2. Controllo le COLONNE (Tubi verticali)
    for (j = 0; j < C; j++) {
        char primoColore = '\0';
        int sommaCol = 0;
        int colValida = 1;

        for (i = 0; i < R; i++) {
            int id = scacchiera[i][j].id_tessera;
            int rot = scacchiera[i][j].rot;
            
            // Determino colore verticale
            // Se rot=0 prendo c2, se rot=1 prendo c1
            char colV = (rot == 0) ? tessere[id].c2 : tessere[id].c1;
            int valV  = (rot == 0) ? tessere[id].v2 : tessere[id].v1;

            if (i == 0) {
                primoColore = colV;
            } else {
                if (colV != primoColore) {
                    colValida = 0;
                    break;
                }
            }
            sommaCol += valV;
        }
        if (colValida) puntiTotali += sommaCol;
    }

    return puntiTotali;
}

/*
 * Funzione: risolviBacktracking
 * Scopo: Algoritmo ricorsivo per trovare la disposizione ottima.
 * Parametri:
 * - pos: Indice lineare della cella (da 0 a R*C - 1). Serve per evitare doppi cicli for nella ricorsione.
 * - maxPunti: Puntatore a intero, per mantenere il massimo globale trovato finora.
 */
void risolviBacktracking(int pos, Cella **scacchiera, int R, int C, Tessera *tessere, int T, int *maxPunti, Cella **bestScacchiera) {
    
    /* --- 1. CASO BASE (Terminazione) --- */
    // Se pos raggiunge R*C, significa che abbiamo visitato tutte le celle
    if (pos >= R * C) {
        // La scacchiera è piena, calcoliamo il punteggio
        int puntiAttuali = calcolaPunteggio(scacchiera, R, C, tessere);
        
        // Se è migliore del record attuale, aggiorniamo
        if (puntiAttuali > *maxPunti) {
            *maxPunti = puntiAttuali;
            
            // Salviamo la configurazione (copia profonda)
            for(int i=0; i<R; i++)
                for(int j=0; j<C; j++)
                    bestScacchiera[i][j] = scacchiera[i][j];
        }
        return; // Torna indietro (Backtrack)
    }

    /* --- 2. CONVERSIONE INDICE LINEARE -> MATRICIALE --- */
    int r = pos / C; // Riga corrente
    int c = pos % C; // Colonna corrente

    /* --- 3. GESTIONE CELLE FISSE --- */
    // Se la cella è fissa (caricata da file), non posso toccarla. Vado alla prossima.
    if (scacchiera[r][c].fissa == 1) {
        risolviBacktracking(pos + 1, scacchiera, R, C, tessere, T, maxPunti, bestScacchiera);
        return;
    }

    /* --- 4. CUORE DEL BACKTRACKING --- */
    // Itero su tutte le tessere disponibili
    for (int i = 0; i < T; i++) {
        
        if (tessere[i].usata == 0) { // Se la tessera è libera
            
            /* TENTATIVO A: Inserisco NON ruotata (rot = 0) */
            tessere[i].usata = 1;        // Marco come occupata
            scacchiera[r][c].id_tessera = i;
            scacchiera[r][c].rot = 0;
            
            // Passo ricorsivo
            risolviBacktracking(pos + 1, scacchiera, R, C, tessere, T, maxPunti, bestScacchiera);
            
            // Backtrack (Annullamento mossa per provare la prossima iterazione)
            // Nota: tessere[i].usata = 0 viene fatto dopo aver provato anche la rotazione, 
            // oppure qui se consideriamo rotazione come scelta distinta.
            // Per chiarezza didattica, faccio Backtrack subito se separo i blocchi.
            
            /* TENTATIVO B: Inserisco RUOTATA (rot = 1) */
            // Non serve de-marcare 'usata' tra tent A e B, è la stessa tessera
            scacchiera[r][c].rot = 1;
            
            // Passo ricorsivo
            risolviBacktracking(pos + 1, scacchiera, R, C, tessere, T, maxPunti, bestScacchiera);

            /* --- FINE TENTATIVI PER QUESTA TESSERA --- */
            // Libero la tessera per il prossimo ciclo del 'for' (prossima tessera j)
            // e resetto la cella (opzionale ma pulito)
            tessere[i].usata = 0; 
            scacchiera[r][c].id_tessera = -1; 
        }
    }
}