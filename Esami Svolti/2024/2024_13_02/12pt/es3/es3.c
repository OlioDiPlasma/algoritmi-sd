#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==========================================
   SEZIONE 1: FUNZIONI AUSILIARIE
   ========================================== */

/*
 * Scopo: Determina se un carattere è una vocale.
 * Ritorna: 1 se è vocale, 0 se è consonante (o altro).
 */
int isVowel(char c) {
    c = tolower(c); // Gestiamo sia maiuscole che minuscole
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

/* * Scopo: Controlla se due parole possono essere adiacenti secondo le regole.
 * Regola: Se prev finisce con Vocale, next deve iniziare con Consonante (e viceversa).
 * Ovvero: il tipo (Vocale/Consonante) deve essere DIVERSO.
 */
int canAttach(char *prev, char *next) {
    if (prev == NULL) return 1; // Se è la prima parola, va sempre bene
    
    int lenPrev = strlen(prev);
    char lastChar = prev[lenPrev - 1];
    char firstChar = next[0];

    // Se entrambi sono vocali o entrambi consonanti, non va bene.
    // Devono essere diversi (1 e 0 oppure 0 e 1).
    if (isVowel(lastChar) == isVowel(firstChar)) {
        return 0; // Vincolo violato -> PRUNING
    }
    return 1; // OK
}

/* ==========================================
   SEZIONE 2: STRUTTURA PER LA SOLUZIONE
   ========================================== */

// Usiamo una variabile globale o una struct passata per riferimento
// per tenere traccia della soluzione migliore trovata finora.
typedef struct {
    char *bestString; // La stringa più lunga trovata
    int maxLen;       // La lunghezza della stringa migliore
} Solution;

/* ==========================================
   SEZIONE 3: CORE DEL BACKTRACKING
   ========================================== */

/*
 * Scopo: Funzione ricorsiva che prova tutte le combinazioni valide.
 * * Parametri:
 * - words: array di stringhe input
 * - n: numero di stringhe
 * - used: array booleano per segnare le parole già usate nel ramo corrente
 * - currentString: buffer che costruisce la soluzione corrente
 * - currentLen: lunghezza attuale della stringa corrente
 * - sol: puntatore alla struttura che contiene il "record" migliore
 */
void solve(char **words, int n, int *used, char *currentString, int currentLen, Solution *sol) {
    
    // 1. Controllo se la soluzione attuale è la migliore trovata finora
    if (currentLen > sol->maxLen) {
        sol->maxLen = currentLen;
        // Aggiorno la stringa migliore. Uso realloc o free+strdup.
        // Qui per sicurezza faccio free + strdup.
        free(sol->bestString);
        sol->bestString = strdup(currentString);
    }

    // 2. Ciclo su tutte le possibili parole candidate come "prossima parola"
    for (int i = 0; i < n; i++) {
        
        // Se la parola 'i' non è ancora stata usata in questo percorso
        if (!used[i]) {
            
            // --- PRUNING ---
            // Recupero l'ultima parola inserita per controllare il vincolo.
            // Se currentString è vuota, lastWord è NULL.
            // Nota: per semplicità, qui controllo l'intera stringa, ma idealmente 
            // basterebbe passare l'ultima parola come parametro alla ricorsione.
            char *lastWordPtr = NULL;
            // (Logica semplificata: controlliamo l'attacco direttamente qui)
            // Per farlo bene, dovrei sapere dove finisce l'ultima parola in currentString.
            // Tuttavia, il vincolo dipende solo dall'ULTIMO carattere di currentString
            // e dal PRIMO di words[i].
            
            int valid = 1;
            if (currentLen > 0) {
                char lastCharOfCurrent = currentString[currentLen - 1];
                char firstCharOfNext = words[i][0];
                if (isVowel(lastCharOfCurrent) == isVowel(firstCharOfNext)) {  //vb si è dimenticato che aveva la funzione sopra da usare
                    valid = 0; // Pruning: non rispetto l'alternanza V/C
                }
            }

            if (valid) {
                // DO (Mossa)
                used[i] = 1;
                
                // Concateno words[i] a currentString. 
                // Assumo che currentString sia abbastanza grande (allocato nel wrapper).
                strcat(currentString, words[i]);
                int wordLen = strlen(words[i]);

                // RECURSE (Ricorsione)
                solve(words, n, used, currentString, currentLen + wordLen, sol);

                // UNDO (Backtracking)
                used[i] = 0;
                // Rimuovo l'ultima parola aggiunta (tronco la stringa)
                currentString[currentLen] = '\0'; 
            }
        }
    }
}

/* ==========================================
   SEZIONE 4: FUNZIONE WRAPPER (INTERFACCIA)
   ========================================== */

char *bestConcat(char **words, int n) {
    // Allocazione strutture di supporto
    int *used = (int *)calloc(n, sizeof(int)); // Inizializzato a 0
    
    // Stimo una dimensione massima per il buffer temporaneo
    // (Somma di tutte le lunghezze + 1 terminatore)
    int totalLen = 0;
    for(int i=0; i<n; i++) totalLen += strlen(words[i]);
    
    char *currentString = (char *)calloc(totalLen + 1, sizeof(char));
    
    Solution sol;
    sol.maxLen = -1;
    sol.bestString = NULL;

    // Avvio la ricorsione
    solve(words, n, used, currentString, 0, &sol);

    // Pulizia memoria ausiliaria
    free(used);
    free(currentString);

    // Se non ho trovato nulla (es. lista vuota), ritorno stringa vuota
    if (sol.bestString == NULL) {
        return strdup("");
    }

    return sol.bestString;
}

/* ==========================================
   SEZIONE 5: MAIN DI TEST
   ========================================== */

int main() {
    // Esempio di dati
    char *elenco[] = {
        "mela",   // finisce 'a' (V)
        "ananas", // inizia 'a' (V) -> No dopo mela
        "pera",   // inizia 'p' (C) -> Ok dopo mela. Finisce 'a' (V)
        "orso",   // inizia 'o' (V) -> Ok dopo pera. Finisce 'o' (V)
        "tavolo"  // inizia 't' (C) -> Ok dopo orso.
    };
    int n = 5;

    printf("--- Calcolo Best Concat ---\n");
    char *result = bestConcat(elenco, n);

    printf("Stringa migliore ottenuta: %s\n", result);
    printf("Lunghezza: %lu\n", strlen(result));

    /* Analisi logica attesa:
       1. mela (V) -> pera (C..V) -> orso (V..V) -> tavolo (C..V)
          Lunghezza: 4 + 4 + 4 + 6 = 18. Stringa: "melaperaorsotavolo"
       
       Nota: "ananas" non può seguire "mela" (V-V clash). 
             "ananas" (..s) -> "tavolo" (t..) (C-C clash, NO).
             "ananas" (..s) -> "mela" (m..) (C-C clash, NO).
    */

    free(result);
    return 0;
}