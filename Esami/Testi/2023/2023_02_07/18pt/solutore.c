#include "solutore.h"
#include <stdbool.h> // Serve per usare true/false invece di 1/0

/* --- VARIABILI GLOBALI STATICHE ---
 * 'static' qui significa che queste variabili sono visibili SOLO in questo file .c.
 * Sono "private" per il modulo solutore.
 * Servono per mantenere traccia del risultato migliore trovato durante le 
 * migliaia di chiamate ricorsive.
 */
static int max_punteggio = -1;       // Il record attuale di punti
static Coord *miglior_percorso = NULL; // La sequenza di coordinate del record

/**
 * Funzione Helper: calcola_punteggio
 * Scopo: Una volta riempito il buffer, controlla quali bonus abbiamo soddisfatto.
 * Parametri:
 * - game: i dati del gioco (per sapere quali sono i bonus).
 * - buffer_tokens: l'array di stringhe (es. "55", "E9", "1C") raccolte nel percorso.
 */
static int calcola_punteggio(GameData *game, char **buffer_tokens) {
    int tot = 0;

    // Cicla attraverso ogni regola bonus disponibile
    for (int b = 0; b < game->num_bonus; b++) {
        Bonus bonus = game->lista_bonus[b];
        
        // Ottimizzazione: se la sequenza bonus è più lunga del nostro buffer, 
        // è impossibile che ci stia dentro. Saltiamo.
        if (bonus.len_seq > game->L) continue;

        bool trovato = false;
        
        /* --- RICERCA SOTTOSTRINGA (Sliding Window) ---
         * Dobbiamo vedere se la sequenza bonus (es. "55", "1C") compare 
         * dentro il nostro buffer (es. "BD", "55", "1C", "E9").
         * i scorre la posizione di inizio nel buffer.
         */
        for (int i = 0; i <= game->L - bonus.len_seq; i++) {
            bool match = true;
            
            // k scorre i caratteri della sequenza bonus
            for (int k = 0; k < bonus.len_seq; k++) {
                // strcmp restituisce 0 se le stringhe sono identiche
                if (strcmp(buffer_tokens[i+k], bonus.sequenza[k]) != 0) {
                    match = false; // Trovata discrepanza
                    break;         // Interrompiamo il ciclo interno
                }
            }
            // Se il ciclo interno è finito con match ancora true, l'abbiamo trovata!
            if (match) { trovato = true; break; }
        }
        
        // Se la sequenza è presente, aggiungiamo i punti
        if (trovato) tot += bonus.valore;
    }
    return tot;
}

/**
 * Funzione Core: Backtracking Ricorsivo
 * Scopo: Esplorare l'albero delle decisioni.
 * Parametri:
 * - step: A che punto del buffer siamo (0, 1, 2... fino a L).
 * - r, c: La cella attuale in cui ci troviamo.
 * - visited: Matrice booleana per non passare due volte sulla stessa casella.
 * - curr_buff: Il buffer che stiamo costruendo in QUESTO tentativo.
 * - curr_path: Le coordinate del percorso di QUESTO tentativo.
 */
static void backtracking(GameData *game, int step, int r, int c, 
                         bool visited[MAX_N][MAX_N], char **curr_buff, Coord *curr_path) {
    
    // --- 1. SALVA STATO CORRENTE ---
    // Segniamo la cella come visitata per non tornarci in questo ramo.
    visited[r][c] = true;
    
    // Salviamo il token (puntatore alla stringa nella griglia) nel buffer temporaneo
    curr_buff[step] = game->grid[r][c];
    
    // Salviamo le coordinate
    curr_path[step].r = r;
    curr_path[step].c = c;

    // --- 2. CASO BASE (Condizione di terminazione) ---
    // Abbiamo riempito tutto il buffer (siamo all'ultimo step).
    if (step == game->L - 1) {
        
        // Calcoliamo quanti punti vale questo percorso
        int score = calcola_punteggio(game, curr_buff);
        
        // Se è un nuovo record, aggiorniamo le variabili globali
        if (score > max_punteggio) {
            max_punteggio = score;
            // Copiamo il percorso attuale in quello "migliore" per ricordarlo
            for(int i=0; i<game->L; i++) miglior_percorso[i] = curr_path[i];
        }
    } 
    else {
        // --- 3. PASSO RICORSIVO (Esplorazione) ---
        int next_step = step + 1;
        
        /* REGOLA DI MOVIMENTO:
         * Step 0 (appena piazzato): Cerchiamo il prossimo muovendoci in VERTICALE (stessa colonna, cambia riga).
         * Step 1 (appena piazzato): Cerchiamo il prossimo muovendoci in ORIZZONTALE (stessa riga, cambia colonna).
         * E così via in alternanza.
         *
         * Se step è pari (0, 2, 4) -> il prossimo movimento è Verticale.
         * Se step è dispari (1, 3, 5) -> il prossimo movimento è Orizzontale.
         */
        bool vertical = (step % 2 == 0); 

        if (vertical) {
            // MOVIMENTO VERTICALE: Blocco la colonna 'c', ciclo su tutte le righe 'i'
            for (int i = 0; i < game->N; i++) { 
                // Se la cella [i][c] non è già stata usata in questo percorso...
                if (!visited[i][c]) {
                    // ...faccio una chiamata ricorsiva saltando lì.
                    backtracking(game, next_step, i, c, visited, curr_buff, curr_path);
                }
            }
        } else {
            // MOVIMENTO ORIZZONTALE: Blocco la riga 'r', ciclo su tutte le colonne 'j'
            for (int j = 0; j < game->N; j++) { 
                if (!visited[r][j]) {
                    backtracking(game, next_step, r, j, visited, curr_buff, curr_path);
                }
            }
        }
    }

    // --- 4. BACKTRACKING (Undo) ---
    // Fondamentale: prima di risalire al livello superiore, devo "liberare" la cella.
    // Così, quando il ciclo for precedente proverà un'altra strada, potrà ripassare di qui se necessario.
    visited[r][c] = false;
}

/**
 * Funzione Pubblica: Inizializza e avvia la ricerca
 */
void trova_ottimo(GameData *game) {
    // --- ALLOCAZIONI TEMPORANEE ---
    // Matrice visited inizializzata tutta a false (0)
    bool visited[MAX_N][MAX_N] = {false};
    
    // Array per contenere i token del tentativo corrente (array di puntatori char*)
    char **curr_buff = malloc(game->L * sizeof(char*));
    
    // Array per contenere le coordinate del tentativo corrente
    Coord *curr_path = malloc(game->L * sizeof(Coord));
    
    // --- ALLOCAZIONE OUTPUT ---
    // Spazio per salvare il risultato finale (miglior percorso)
    miglior_percorso = malloc(game->L * sizeof(Coord));
    max_punteggio = -1; // Reset del punteggio (caso base negativo)

    // --- AVVIO LOOP INIZIALE ---
    // Le regole del gioco dicono che si parte sempre dalla RIGA 0 (in alto).
    // Proviamo a iniziare da ogni colonna della prima riga.
    for (int j = 0; j < game->N; j++) {
        // Avvia la ricorsione partendo da [0][j] allo step 0
        backtracking(game, 0, 0, j, visited, curr_buff, curr_path);
    }

    // --- STAMPA RISULTATI ---
    printf("\n=== RISULTATO OTTIMO ===\n");
    if (max_punteggio == -1) {
        printf("Nessuna soluzione trovata (forse buffer troppo piccolo?).\n");
    } else {
        printf("Punti Totali: %d\nPercorso: ", max_punteggio);
        for(int i=0; i<game->L; i++) {
            // Stampa coordinate e valore. Es: [0,1]55
            printf("[%d,%d]%s ", miglior_percorso[i].r, miglior_percorso[i].c, 
                   game->grid[miglior_percorso[i].r][miglior_percorso[i].c]);
        }
        printf("\n");
    }

    // --- PULIZIA FINALE ---
    free(curr_buff); 
    free(curr_path); 
    free(miglior_percorso);
}