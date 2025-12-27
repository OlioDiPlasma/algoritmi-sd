#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * FUNZIONE RICORSIVA: solveR
 * ============================================================================
 * SCOPO:
 * Cerca ricorsivamente di coprire la parte rimanente della stringa 'str'
 * usando le parole del dizionario.
 *
 * PARAMETRI:
 * - char *target: La stringa obiettivo (puntatore alla posizione corrente).
 * - char **dict: Il dizionario.
 * - int n: Dimensione dizionario.
 * - int s: Numero minimo di parole distinte richieste.
 * - int *usage_count: Array che conta quante volte è stata usata ogni parola del dizionario.
 * - int current_distinct: Quante parole distinte sono attualmente in uso.
 * - int *path: Array degli indici delle parole usate (per la stampa).
 * - int depth: Profondità della ricorsione (quante parole abbiamo concatenato finora).
 *
 * RITORNO:
 * - int: 1 se la soluzione è trovata, 0 altrimenti.
 * ============================================================================
 */
int solveR(char *target, char **dict, int n, int s, 
           int *usage_count, int current_distinct, int *path, int depth) {

    /* --- 1. CASO BASE (Successo) --- */
    // Se *target è '\0', significa che abbiamo consumato tutta la stringa str.
    if (*target == '\0') {
        // Controllo il vincolo: ho usato almeno s parole diverse?
        if (current_distinct >= s) {
            // Stampa della soluzione
            printf("Soluzione trovata (s=%d, distinte=%d): ", s, current_distinct);
            for (int i = 0; i < depth; i++) {
                printf("%s ", dict[path[i]]);
            }
            printf("\n");
            return 1; // Successo
        }
        return 0; // Stringa finita, ma vincolo non rispettato -> Backtrack
    }

    /* --- 2. CICLO DI SCELTA (Iterazione sul dizionario) --- */
    for (int i = 0; i < n; i++) {
        
        // Calcoliamo la lunghezza della parola candidata
        int len_word = strlen(dict[i]);
        
        /* --- 3. PRUNING (Potatura) --- */
        // Verifichiamo se dict[i] corrisponde ai primi caratteri di target.
        // strncmp restituisce 0 se le stringhe sono uguali per 'len_word' caratteri.
        if (strncmp(target, dict[i], len_word) == 0) {
            
            /* --- 4. AGGIORNAMENTO STATO (Do) --- */
            // 1. Aggiungo l'indice al percorso
            path[depth] = i;
            
            // 2. Gestione conteggio parole distinte
            int new_distinct = current_distinct;
            if (usage_count[i] == 0) {
                // Se il contatore era 0, è la prima volta che uso questa parola specifica
                new_distinct++;
            }
            usage_count[i]++; // Incremento sempre l'uso

            /* --- 5. PASSO RICORSIVO --- */
            // Avanzo il puntatore 'target' di 'len_word' posizioni.
            // Esempio: target="abracadabra", parola="abra" -> passo "cadabra"
            if (solveR(target + len_word, dict, n, s, usage_count, new_distinct, path, depth + 1)) {
                return 1; // Trovato! Risalgo velocemente
            }

            /* --- 6. BACKTRACKING (Undo) --- */
            // Se sono qui, la strada non era buona. Annulliamo le modifiche.
            usage_count[i]--; // Decremento l'uso
            // Nota: new_distinct è una variabile locale, non serve decrementarla,
            // ma usage_count è un array condiviso (puntatore), quindi VA ripristinato!
        }
    }

    return 0; // Nessuna parola del dizionario combacia o porta a soluzione
}

/* ============================================================================
 * FUNZIONE WRAPPER: solve
 * ============================================================================
 * SCOPO:
 * Prepara le strutture dati ausiliarie e lancia la ricorsione.
 *
 * PARAMETRI:
 * - char *str: Stringa target completa.
 * - char **dict: Dizionario.
 * - int n: Numero parole nel dizionario.
 * - int s: Soglia parole distinte.
 *
 * GESTIONE MEMORIA:
 * Alloca array temporanei per il backtracking che devono essere liberati.
 * ============================================================================
 */
void solve(char *str, char **dict, int n, int s) {
    
    /* --- 1. ALLOCAZIONE --- */
    // Array per contare quante volte uso ogni parola del dizionario (per il vincolo s)
    // calloc inizializza a 0.
    int *usage_count = (int *)calloc(n, sizeof(int));
    
    // Array per memorizzare la sequenza di parole (per la stampa).
    // Nel caso peggiore, la soluzione è composta da strlen(str) parole (lettere singole).
    int max_len = strlen(str);
    int *path = (int *)malloc(max_len * sizeof(int));

    if (usage_count == NULL || path == NULL) {
        printf("Errore malloc\n");
        return;
    }

    printf("Cerco di ricostruire \"%s\" con almeno %d parole distinte...\n", str, s);

    /* --- 2. CHIAMATA LOGICA --- */
    if (!solveR(str, dict, n, s, usage_count, 0, path, 0)) {
        printf("Nessuna soluzione trovata.\n");
    }

    /* --- 3. FREE --- */
    free(usage_count);
    free(path);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Replica l'esempio dell'immagine "abracadabra".
 * ============================================================================
 */
int main() {
    // Stringa Target
    char *target = "abracadabra";
    
    // Dizionario Hardcoded
    // "a", "ab", "cada", "abra", "ra", "da", "ca", "bra"
    char *dict[] = {
        "a",    // 0
        "ab",   // 1
        "cada", // 2
        "abra", // 3
        "ra",   // 4
        "da",   // 5
        "ca",   // 6
        "bra"   // 7
    };
    int n = 8; // Dimensione dizionario

    // Test 1: s = 3 (Come nell'immagine)
    // Soluzione attesa: a bra cada bra (distinte: a, bra, cada = 3) -> OK
    printf("--- TEST 1 ---\n");
    solve(target, dict, n, 3);

    // Test 2: s = 4 (Come nell'immagine)
    // Soluzione attesa: a bra ca da bra (distinte: a, bra, ca, da = 4) -> OK
    printf("\n--- TEST 2 ---\n");
    solve(target, dict, n, 4);
    
    // Test 3: s impossibile (es. 10 parole distinte)
    printf("\n--- TEST 3 (Impossibile) ---\n");
    solve(target, dict, n, 10);

    return 0;
}