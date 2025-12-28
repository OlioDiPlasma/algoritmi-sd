#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Per INT_MAX

/* =========================================================================
 * DEFINIZIONE STRUTTURE DATI
 * =========================================================================
 * Copiamo la struttura data dalla traccia.
 */
typedef struct {
    char *s;    // La stringa da sostituire (es. "er")
    int pos;    // La posizione in cui applicarla
    int costo;  // Il costo dell'operazione
} sub;

/* =========================================================================
 * PROTOTIPO FUNZIONI
 * =========================================================================
 */
void solve(char *start, char *end, sub *S, int nSubs);

/* Funzione helper ricorsiva (nascosta all'utente finale nel caso ADT, ma qui visibile) */
void solve_rec(char *curr_str, char *target, sub *S, int nSubs, 
               int *used, int curr_cost, int curr_count, int *curr_sol,
               int *min_cost, int *max_count, int *best_sol);

/* =========================================================================
 * IMPLEMENTAZIONE
 * =========================================================================
 */

/* * FUNZIONE RICORSIVA (CORE)
 * --------------------------
 * Scopo: Esplora l'albero delle decisioni provando ad applicare le sostituzioni disponibili.
 * * Parametri chiave:
 * - curr_str: La stringa com'è trasformata finora (copia locale o modificabile).
 * - used: Array booleano per non riutilizzare la stessa sostituzione nello stesso ramo 
 * (assumiamo che ogni elemento di S sia un "pezzo" unico consumabile).
 * - curr_sol: Array che memorizza gli indici delle sostituzioni fatte in questo ramo.
 * - best_sol: Array dove salviamo la soluzione vincente.
 */
void solve_rec(char *curr_str, char *target, sub *S, int nSubs, 
               int *used, int curr_cost, int curr_count, int *curr_sol,
               int *min_cost, int *max_count, int *best_sol) {

    /* --- 1. PRUNING (POTATURA) --- */
    /* Se il costo attuale ha già superato il minimo globale trovato finora, 
       non ha senso continuare su questo ramo. È un vicolo cieco. */
    if (curr_cost > *min_cost) {
        return;
    }

    /* --- 2. BASE CASE (SUCCESSO) --- */
    /* Se la stringa corrente è diventata uguale al target */
    if (strcmp(curr_str, target) == 0) {
        /*
         * Valutazione Ottimo:
         * 1. Costo strettamente minore? -> Nuovo ottimo.
         * 2. Costo uguale MA usiamo più pezzi (sequenza più lunga)? -> Nuovo ottimo.
         */
        if (curr_cost < *min_cost || (curr_cost == *min_cost && curr_count > *max_count)) {
            *min_cost = curr_cost;
            *max_count = curr_count;
            
            /* Salviamo la sequenza vincente.
             * memcpy copia velocemente il blocco di memoria degli indici. */
            memcpy(best_sol, curr_sol, curr_count * sizeof(int));
        }
        return;
    }

    /* --- 3. PASSO RICORSIVO --- */
    for (int i = 0; i < nSubs; i++) {
        /* Se questa sostituzione non è ancora stata usata in questo percorso */
        if (!used[i]) {
            
            /* --- PREPARAZIONE DATI --- */
            /* Dobbiamo capire se applicare la sostituzione S[i] è valido.
             * In questo problema "generico", si assume che le sostituzioni siano applicabili.
             * Tuttavia, verifichiamo che non si vada fuori dai bordi della stringa. */
            int len_sub = strlen(S[i].s);
            int len_str = strlen(curr_str);
            
            if (S[i].pos + len_sub <= len_str) {
                
                /* --- SALVATAGGIO STATO (UNDO PREPARATION) --- */
                /* Poiché dobbiamo modificare la stringa, abbiamo due strategie:
                 * A) Modificare, scendere ricorsivamente, e poi rimettere i caratteri vecchi.
                 * B) Creare una copia della stringa, modificare la copia, passarla giù.
                 * * La strategia A è più efficiente in memoria ma difficile da implementare (undo complesso).
                 * La strategia B è più didattica e sicura. Useremo B (o una variante leggera).
                 * * Qui, per semplicità ed efficienza, modifichiamo un buffer temporaneo.
                 */
                
                char *next_str = strdup(curr_str); // Duplichiamo la stringa corrente (Malloc implicita!)
                if (next_str == NULL) exit(1);

                /* --- APPLICAZIONE SOSTITUZIONE --- */
                /* Copiamo i caratteri della sostituzione sopra la stringa, carattere per carattere o memcpy */
                /* Sintassi: next_str + S[i].pos sposta il puntatore alla posizione giusta */
                memcpy(next_str + S[i].pos, S[i].s, len_sub);

                /* Aggiorniamo le strutture di controllo */
                used[i] = 1;            // Marchiamo come usato
                curr_sol[curr_count] = i; // Aggiungiamo alla lista passi

                /* --- RICORSIONE --- */
                solve_rec(next_str, target, S, nSubs, 
                          used, curr_cost + S[i].costo, curr_count + 1, curr_sol,
                          min_cost, max_count, best_sol);

                /* --- BACKTRACKING (RIPRISTINO) --- */
                used[i] = 0; // Smarchiamo per permettere al ciclo for di usarlo in altri rami futuri
                free(next_str); // Importante: liberiamo la copia creata per questo step
            }
        }
    }
}

/* * FUNZIONE WRAPPER
 * ----------------
 * Scopo: Prepara la memoria e lancia la ricorsione.
 */
void solve(char *start, char *end, sub *S, int nSubs) {
    /* Controlli di sicurezza base */
    if (strlen(start) != strlen(end)) {
        printf("Errore: lunghezze diverse.\n");
        return;
    }

    /* Variabili per tracciare la soluzione ottima */
    int min_cost = INT_MAX; // Infinito
    int max_count = 0;

    /* Allocazione array di supporto */
    /* curr_sol: conterrà gli indici delle sostituzioni tentate */
    int *curr_sol = (int *)malloc(nSubs * sizeof(int));
    /* best_sol: conterrà gli indici della soluzione migliore finale */
    int *best_sol = (int *)malloc(nSubs * sizeof(int));
    /* used: array di flag (0/1) per sapere quali sub abbiamo usato */
    int *used = (int *)calloc(nSubs, sizeof(int)); // calloc inizializza a 0

    if (!curr_sol || !best_sol || !used) {
        printf("Errore malloc.\n");
        exit(1);
    }

    printf("Inizio ricerca trasformazione: \"%s\" -> \"%s\"\n", start, end);

    /* Lancio ricorsione */
    solve_rec(start, end, S, nSubs, used, 0, 0, curr_sol, &min_cost, &max_count, best_sol);

    /* --- STAMPA RISULTATI --- */
    if (min_cost != INT_MAX) {
        printf("\nSoluzione trovata!\n");
        printf("Costo complessivo: %d\n", min_cost);
        printf("Passaggi (%d sostituzioni):\n", max_count);
        
        /* Ricostruiamo visivamente cosa succede per l'utente */
        char *temp = strdup(start);
        printf("Start: %s\n", temp);
        
        for (int i = 0; i < max_count; i++) {
            int sub_idx = best_sol[i]; // Recupero l'indice della sub usata
            sub op = S[sub_idx];
            
            /* Applico per mostrare */
            memcpy(temp + op.pos, op.s, strlen(op.s));
            
            printf("+ \"%s\" in pos %d (costo %d) -> %s\n", 
                   op.s, op.pos, op.costo, temp);
        }
        free(temp);
    } else {
        printf("Impossibile trasformare la stringa con le sostituzioni date.\n");
    }

    /* Pulizia memoria wrapper */
    free(curr_sol);
    free(best_sol);
    free(used);
}

/* =========================================================================
 * MAIN DI TEST
 * =========================================================================
 * Scopo: Riprodurre l'esempio "passato" -> "persona"
 */
int main() {
    /* Dati hardcoded dall'esempio */
    char start[] = "passato";
    char end[]   = "persona";

    /* Array di sostituzioni disponibili */
    sub S[] = {
        {"er", 1, 4},   // 0
        {"ers", 1, 5},  // 1
        {"sa", 3, 1},   // 2
        {"so", 3, 2},   // 3
        {"ato", 0, 1},  // 4
        {"on", 4, 2},   // 5
        {"ona", 4, 3},  // 6
        {"a", 6, 1}     // 7
    };
    int nSubs = sizeof(S) / sizeof(S[0]);

    solve(start, end, S, nSubs);

    return 0;
}