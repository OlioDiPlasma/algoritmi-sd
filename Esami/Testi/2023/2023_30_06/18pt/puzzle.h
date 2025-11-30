#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdbool.h>

/* ======================================================================================
 * [RIF. DOMANDA: Strutture dati e acquisizione (Immagine 2)]
 * DEFINIZIONE TIPI DI DATO
 * ====================================================================================== */

/* * STRUTTURA: Tile (Tessera)
 * SCOPO: Rappresenta la singola cella della griglia.
 * DETTAGLI:
 * - n, s, w, e: Interi usati come booleani (1=tubo, 0=muro).
 * - is_hole: Flag di comodità per identificare rapidamente i buchi vuoti (0 0 0 0).
 */
typedef struct {
    int n, s, w, e; 
    bool is_hole;   
} Tile;

/* * STRUTTURA: Game
 * SCOPO: Rappresenta l'intero stato del gioco.
 * CAMPI:
 * - N: Dimensione del lato della griglia quadrata.
 * - grid: Puntatore doppio (Tile**) che gestisce la matrice dinamica delle tessere.
 */
typedef struct {
    int N;          
    Tile **grid;    
} Game;


/* ======================================================================================
 * SEZIONE 1: GESTIONE MEMORIA E I/O
 * ====================================================================================== */

/*
 * FUNZIONE: load_game
 * SCOPO: Apre il file, legge N e alloca la memoria necessaria per la griglia.
 */
Game* load_game(char *filename);

/*
 * FUNZIONE: free_game
 * SCOPO: Libera la memoria in ordine inverso all'allocazione (Righe -> Array -> Struct).
 * Fondamentale per prevenire Memory Leaks.
 */
void free_game(Game *g);

/*
 * FUNZIONE: print_grid
 * SCOPO: Stampa una visualizzazione ASCII dello stato corrente (utile per debug).
 */
void print_grid(Game *g);


/* ======================================================================================
 * SEZIONE 2: MOTORE LOGICO (CORE)
 * Funzioni descritte in "Descrizione del problema"
 * ====================================================================================== */

/*
 * FUNZIONE: apply_move
 * SCOPO: Applica la fisica dello scivolamento (Sliding).
 * RITORNA: true se almeno una tessera si è spostata, false se la mossa è bloccata.
 */
bool apply_move(Game *g, char direction);

/*
 * FUNZIONE: check_win
 * SCOPO: Verifica se esiste un percorso valido (tubo continuo) tra i due terminali.
 * RITORNA: true se il puzzle è risolto.
 */
bool check_win(Game *g);


/* ======================================================================================
 * SEZIONE 3: SOLUZIONI AI QUESITI D'ESAME
 * ====================================================================================== */

/*
 * [RIF. DOMANDA: Problema di verifica (Immagine 1)]
 * FUNZIONE: problema_verifica
 * SCOPO: Legge un file di mosse e ne verifica la validità e l'esito passo dopo passo.
 */
void problema_verifica(Game *g, char *filename_mosse);

/*
 * [RIF. DOMANDA: Problema di ricerca e ottimizzazione (Immagine 1)]
 * FUNZIONE: problema_ottimizzazione
 * SCOPO: Trova la soluzione OTTIMA (la più breve possibile) usando un algoritmo
 * di Iterative Deepening (Approfondimento Iterativo).
 */
void problema_ottimizzazione(Game *g, int max_mosse);

#endif