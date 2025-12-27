#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ============================================================================
 * FUNZIONE AUSILIARIA: differiscono_per_uno
 * ============================================================================
 * SCOPO:
 * Determina se due stringhe differiscono esattamente per un solo carattere.
 * Restituisce l'indice del carattere diverso, oppure -1 se non soddisfano la condizione.
 *
 * PARAMETRI:
 * - char *s1, char *s2: Le due stringhe da confrontare.
 *
 * RITORNO:
 * - int: L'indice (0..L-1) dove differiscono, o -1 se diff != 1.
 * ============================================================================
 */
int differiscono_per_uno(char *s1, char *s2) {
    int diff_count = 0;
    int diff_index = -1;
    // Assumiamo che abbiano la stessa lunghezza come da specifica
    for (int i = 0; s1[i] != '\0'; i++) {
        /* --- DETTAGLIO SINTATTICO ---
         * Accesso agli array di char: s1[i] legge il valore (char) all'offset i.
         * Confrontiamo carattere per carattere.
         */
        if (s1[i] != s2[i]) {
            diff_count++;
            diff_index = i; // Memorizzo dove ho trovato la differenza
        }
        if (diff_count > 1) return -1; // Ottimizzazione: se trovo 2 diff, esco subito
    }
    return (diff_count == 1) ? diff_index : -1;
}

/* ============================================================================
 * FUNZIONE RICORSIVA: solveR
 * ============================================================================
 * SCOPO:
 * Implementa il motore di backtracking (Depth First Search) per esplorare
 * le sequenze di parole valide.
 *
 * PARAMETRI:
 * - int curr_idx: Indice della parola corrente nel dizionario 'dict'.
 * - int end_idx: Indice della parola target 'end' nel dizionario.
 * - char **dict: Il dizionario (array di stringhe).
 * - int n: Numero totale parole.
 * - int k: Vincolo minimo di cambi per posizione.
 * - int *visited: Array booleano (0/1) per marcare le parole usate nel percorso attuale.
 * - int *changes: Array di contatori (dimensione L). changes[i] conta quante volte
 * il carattere in posizione i è cambiato lungo il percorso.
 * - int L: Lunghezza delle stringhe.
 * - int *path: Memorizza la sequenza di indici trovata (per la stampa).
 * - int depth: Profondità attuale della ricorsione (lunghezza percorso attuale).
 *
 * CONCETTI CHIAVE:
 * - Backtracking: Provo una strada, se non va bene "torno indietro" annullando le modifiche
 * (rimuovo il visited, decremento il changes).
 * ============================================================================
 */
int solveR(int curr_idx, int end_idx, char **dict, int n, int k, 
           int *visited, int *changes, int L, int *path, int depth) {

    // Aggiungo la parola corrente al percorso
    path[depth] = curr_idx;
    visited[curr_idx] = 1; // Marco come visitata

    /* --- 1. CASO BASE (Successo o Fallimento al target) --- */
    if (curr_idx == end_idx) {
        // Siamo arrivati a 'end'. Ora dobbiamo verificare il vincolo K.
        int condizione_k_soddisfatta = 1;
        
        for (int i = 0; i < L; i++) {
            // Se anche solo una posizione è cambiata meno di k volte, falliamo.
            if (changes[i] < k) {
                condizione_k_soddisfatta = 0;
                break;
            }
        }

        if (condizione_k_soddisfatta) {
            // Stampa della soluzione trovata
            printf("Soluzione trovata (lunghezza %d): ", depth + 1);
            for (int i = 0; i <= depth; i++) {
                printf("%s ", dict[path[i]]);
                if (i < depth) printf("-> ");
            }
            printf("\n");
            return 1; // Ritorna 1 (true) per dire "ho finito, ferma tutto"
        } else {
            // Arrivati a destinazione ma vincolo k non rispettato.
            // Dobbiamo tornare indietro (backtrack) per cercare un altro percorso.
            visited[curr_idx] = 0; 
            return 0; 
        }
    }

    /* --- 2. PASSO RICORSIVO --- */
    // Provo a saltare su ogni altra parola del dizionario
    for (int next = 0; next < n; next++) {
        
        // Criteri di Pruning:
        // 1. Non devo averla già visitata in questo percorso (!visited[next])
        // 2. Deve differire di esattamente 1 carattere
        if (!visited[next]) {
            int changed_pos = differiscono_per_uno(dict[curr_idx], dict[next]);
            
            if (changed_pos != -1) {
                // Mossa valida trovata.
                
                // --- APPLICAZIONE MODIFICHE (DO) ---
                changes[changed_pos]++; // Incremento il contatore per la posizione cambiata
                
                // Chiamata ricorsiva
                if (solveR(next, end_idx, dict, n, k, visited, changes, L, path, depth + 1)) {
                    return 1; // Se la chiamata figlia ha trovato la soluzione, risalgo col successo
                }

                // --- BACKTRACKING (UNDO) ---
                // Se sono qui, la strada 'next' non ha portato a soluzione valida.
                // Annulliamo le modifiche per provare il prossimo 'next'.
                changes[changed_pos]--; // Decremento il contatore
            }
        }
    }

    // Se nessun vicino porta a soluzione, s-marco me stesso e torno 0
    visited[curr_idx] = 0;
    return 0;
}

/* ============================================================================
 * FUNZIONE WRAPPER: solve
 * ============================================================================
 * SCOPO:
 * Prepara le strutture dati necessarie (array ausiliari) e lancia la ricorsione.
 *
 * PARAMETRI:
 * - char *start, *end: Stringhe di inizio e fine.
 * - char **dict: Array di puntatori a stringhe (il dizionario).
 * - int n: Numero di stringhe nel dizionario.
 * - int k: Il vincolo k.
 *
 * GESTIONE MEMORIA:
 * Fondamentale l'uso di calloc per inizializzare a zero i contatori.
 * Fondamentale la free() alla fine per evitare memory leaks.
 * ============================================================================
 */
void solve(char *start, char *end, char **dict, int n, int k) {
    
    /* --- 1. IDENTIFICAZIONE INDICI --- */
    // Dobbiamo trovare a quali indici del dizionario corrispondono start e end.
    int start_idx = -1;
    int end_idx = -1;

    // Assumiamo che start e end SIANO nel dizionario come da testo.
    // Usiamo strcmp (string compare) che ritorna 0 se le stringhe sono uguali.
    for (int i = 0; i < n; i++) {
        if (strcmp(dict[i], start) == 0) start_idx = i;
        if (strcmp(dict[i], end) == 0) end_idx = i;
    }

    if (start_idx == -1 || end_idx == -1) {
        printf("Errore: Start o End non trovati nel dizionario.\n");
        return;
    }

    /* --- 2. ALLOCAZIONE MEMORIA --- */
    int L = strlen(start); // Calcolo lunghezza parole

    // visited: array di n interi (o bool). Usiamo calloc per averli a 0 (false).
    int *visited = (int *)calloc(n, sizeof(int));
    
    // changes: array di L interi. Conta i cambiamenti per ogni colonna.
    int *changes = (int *)calloc(L, sizeof(int));

    // path: array per salvare la strada. Al massimo lunga n (se usiamo tutte le parole).
    int *path = (int *)malloc(n * sizeof(int));

    // Controllo allocazione
    if (visited == NULL || changes == NULL || path == NULL) {
        printf("Errore allocazione memoria.\n");
        free(visited); free(changes); free(path); // Cleanup parziale safe
        return;
    }

    /* --- 3. LANCIO ALGORITMO --- */
    printf("Cerco percorso da %s a %s con k=%d...\n", start, end, k);
    
    // Lanciamo la ricorsione partendo da start_idx, a profondità 0
    if (!solveR(start_idx, end_idx, dict, n, k, visited, changes, L, path, 0)) {
        printf("Nessuna soluzione trovata che soddisfi i requisiti.\n");
    }

    /* --- 4. LIBERAZIONE MEMORIA (FREE) --- */
    // Importante: ogni malloc/calloc deve avere una free corrispondente.
    free(visited);
    free(changes);
    free(path);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Crea un piccolo dizionario hardcoded e testa la funzione.
 * Esempio basato sull'immagine: PALI -> POLI -> VOLI -> VOLA -> COLA -> COSA
 * ============================================================================
 */
int main() {
    // Creiamo un dizionario di prova.
    // Nota: char *d[] è un array di puntatori a stringhe letterali (read-only), 
    // ma per l'esercizio va bene poiché non le modifichiamo, le leggiamo solo.
    char *myDict[] = {
        "PALI", // 0 (Start)
        "POLI", // 1
        "VOLI", // 2
        "VOLA", // 3
        "COLA", // 4
        "COSA", // 5 (End)
        "ROSA", // 6 (Distrattore)
        "CASA", // 7 (Distrattore)
        "VILA"  // 8 (Distrattore)
    };
    
    int n = 9; // Numero parole
    int k = 1; // Proviamo con k=1 (almeno 1 cambio per posizione)
               // Lunghezza stringhe = 4. 
               // PALI -> COSA:
               // Pos 0: P->P->V->V->C->C (Cambia: P->V, V->C) OK
               // Pos 1: A->O->O->O->O->O (Cambia: A->O) OK
               // Pos 2: L->L->L->L->L->S (Cambia: L->S) OK
               // Pos 3: I->I->I->A->A->A (Cambia: I->A) OK
               
    // Il prototipo chiede char **dict. In C, un array di stringhe 'char *d[]'
    // decade in 'char **' quando passato a funzione.
    
    printf("--- TEST 1 (k=1) ---\n");
    solve("PALI", "COSA", myDict, n, 1);

    printf("\n--- TEST 2 (k=2) ---\n");
    // Con k=2 potrebbe fallire perché la pos 1 (A->O) cambia solo una volta
    // nella sequenza PALI->POLI->...
    solve("PALI", "COSA", myDict, n, 2);

    return 0;
}