#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * ======================================================================================
 * FUNZIONE HELPER: is_vowel
 * ======================================================================================
 * Scopo: Determina se un carattere è una vocale.
 * Parametri:
 * - char c: il carattere da controllare.
 * Ritorna: 1 se è vocale, 0 altrimenti.
 */
int is_vowel(char c) {
    c = tolower(c); // Gestiamo maiuscole/minuscole uniformando
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

/*
 * ======================================================================================
 * FUNZIONE HELPER: check_constraint
 * ======================================================================================
 * Scopo: Verifica se due parole possono essere concatenate secondo le regole.
 * Parametri:
 * - char *prima: la parola che viene prima nella sequenza.
 * - char *seconda: la parola che vogliamo attaccare dopo.
 *
 * Concetti Chiave:
 * Accesso all'ultimo carattere: usiamo `strlen(prima) - 1`.
 */
int check_constraint(char *prima, char *seconda) {
    if (prima == NULL) return 1; // Caso iniziale: nessuna parola precedente, tutto lecito.

    char last_char = prima[strlen(prima) - 1];
    char first_char = seconda[0];

    // Se finisce con vocale, la seconda NON può iniziare con vocale (deve essere Consonante)
    if (is_vowel(last_char) && is_vowel(first_char)) return 0;

    // Se finisce con consonante, la seconda NON può iniziare con consonante (deve essere Vocale)
    if (!is_vowel(last_char) && !is_vowel(first_char)) return 0;

    return 1; // Vincolo rispettato
}

/*
 * ======================================================================================
 * FUNZIONE RICORSIVA: solve
 * ======================================================================================
 * Scopo: Esplora tutte le combinazioni valide (Backtracking) per trovare la lunghezza massima.
 *
 * Parametri:
 * - char **parole: dizionario delle parole disponibili.
 * - int nparole: dimensione del dizionario.
 * - int *used: array booleano per sapere quali parole abbiamo già usato in questo ramo.
 * - int last_idx: indice nel dizionario dell'ultima parola aggiunta (-1 all'inizio).
 * - int current_len: lunghezza della stringa costruita finora.
 * - int *max_len: puntatore alla lunghezza record trovata finora (passaggio per riferimento!).
 * - int *sol: array degli indici della soluzione attuale.
 * - int *best_sol: array degli indici della soluzione migliore trovata.
 * - int level: profondità della ricorsione (quante parole ho concatenato).
 * - int *best_level: per sapere quante parole compongono la soluzione migliore (per ricostruirla).
 *
 * Concetti Chiave:
 * 1. Modello Combinatorio: Disposizioni Semplici (l'ordine conta, no ripetizioni).
 * 2. Pruning: Tagliamo il ramo se check_constraint fallisce.
 */
void solve(char **parole, int nparole, int *used, int last_idx, int current_len, 
           int *max_len, int *sol, int *best_sol, int level, int *best_level) {

    // Tentiamo di aggiungere ogni parola disponibile
    for (int i = 0; i < nparole; i++) {
        
        // PRUNING 1: La parola non deve essere già stata usata in questo percorso
        if (used[i]) continue;

        // PRUNING 2: Verifica vincolo vocale/consonante
        char *prev_word = (last_idx == -1) ? NULL : parole[last_idx];
        
        if (check_constraint(prev_word, parole[i])) {
            
            // --- PASSO AVANTI (DO) ---
            used[i] = 1;                // Marco come visitato
            sol[level] = i;             // Aggiungo indice alla soluzione corrente
            int word_len = strlen(parole[i]);
            int new_len = current_len + word_len;

            // Controllo se ho trovato un nuovo ottimo
            if (new_len > *max_len) {
                *max_len = new_len;
                *best_level = level + 1; // Salvo quanti elementi compongono la soluzione
                // Copio la soluzione corrente in quella "best"
                for (int k = 0; k <= level; k++) {
                    best_sol[k] = sol[k];
                }
            }

            // --- RICORSIONE ---
            solve(parole, nparole, used, i, new_len, max_len, sol, best_sol, level + 1, best_level);

            // --- PASSO INDIETRO (BACKTRACK) ---
            used[i] = 0; // Smarco per rendere la parola disponibile ad altre combinazioni
        }
    }
}

/*
 * ======================================================================================
 * FUNZIONE WRAPPER: BESTbestConcat
 * ======================================================================================
 * Scopo: Prepara le strutture dati, lancia la ricorsione e costruisce la stringa finale.
 *
 * Gestione Memoria:
 * Dobbiamo allocare vettori di supporto (used, sol, best_sol) e poi liberarli.
 * La stringa finale deve essere allocata dinamicamente e restituita al chiamante.
 */
char *BESTbestConcat(char **parole, int nparole) {
    
    // Allocazione strutture di supporto
    // calloc inizializza a 0, utile per l'array 'used'
    int *used = (int *)calloc(nparole, sizeof(int));
    int *sol = (int *)malloc(nparole * sizeof(int));
    int *best_sol = (int *)malloc(nparole * sizeof(int));
    
    int max_len = 0;
    int best_level = 0;

    // Lancio la ricorsione
    // last_idx = -1 indica che non c'è nessuna parola precedente
    solve(parole, nparole, used, -1, 0, &max_len, sol, best_sol, 0, &best_level);

    // Costruzione stringa risultato
    /* * Malloc: Allocazione stringa finale. 
     * Dimensione: max_len (caratteri totali) + 1 (terminatore '\0').
     */
    char *result_string = (char *)malloc((max_len + 1) * sizeof(char));
    
    if (result_string == NULL) {
        // Gestione errore malloc, ma puliamo prima il resto!
        free(used); free(sol); free(best_sol);
        return NULL;
    }

    result_string[0] = '\0'; // Inizializzo stringa vuota per usare strcat

    if (max_len > 0) {
        for (int i = 0; i < best_level; i++) {
            // best_sol contiene gli indici delle parole nell'ordine corretto
            strcat(result_string, parole[best_sol[i]]);
        }
    }

    // Pulizia memoria temporanea (Garbage Collection manuale)
    free(used);
    free(sol);
    free(best_sol);

    return result_string;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Esempio hardcoded
    int n = 5;
    char *parole[] = {
        "mela",   // Finisce 'a' (V)
        "orso",   // Inizia 'o' (V), Finisce 'o' (V)
        "rospo",  // Inizia 'r' (C), Finisce 'o' (V)
        "altare", // Inizia 'a' (V), Finisce 'e' (V)
        "tana"    // Inizia 't' (C), Finisce 'a' (V)
    };

    printf("--- Inizio Test ---\n");
    printf("Parole disponibili:\n");
    for(int i=0; i<n; i++) printf("%d) %s\n", i, parole[i]);

    char *risultato = BESTbestConcat(parole, n);

    printf("\nStringa ottima generata: '%s'\n", risultato);
    printf("Lunghezza: %lu\n", (risultato ? strlen(risultato) : 0));

    // Non dimentichiamo di liberare la memoria restituita dalla funzione!
    free(risultato);

    return 0;
}