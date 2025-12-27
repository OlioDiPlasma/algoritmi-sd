#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* =================================================================================
 * FUNZIONI DI UTILITÀ (HELPER)
 * =================================================================================
 * Per mantenere il codice pulito, separiamo la logica di controllo dei caratteri.
 * Usiamo un array di interi 'usati[26]' come se fosse una tabella booleana.
 * Indice 0 = 'A', Indice 1 = 'B', ..., Indice 25 = 'Z'.
 */

/*
 * Scopo: Controlla se la stringa 's' contiene caratteri già marcati come usati.
 * Ritorna: 1 se c'è conflitto (collisione), 0 se è pulita.
 */
int conflitto(char *s, int *usati) {
    for (int i = 0; s[i] != '\0'; i++) {
        // Calcolo indice: 'A' diventa 0, 'B' diventa 1...
        int idx = s[i] - 'A'; 
        // Se la cella è già a 1, significa che un'altra stringa nel set corrente usa questa lettera.
        if (usati[idx] == 1) {
            return 1; // Trovato conflitto
        }
    }
    return 0; // Nessun conflitto
}

/*
 * Scopo: Segna (o desegna) i caratteri della stringa 's' nell'array 'usati'.
 * Parametri:
 * - val: 1 per occupare le lettere, 0 per liberarle (Backtracking).
 */
void aggiorna_usati(char *s, int *usati, int val) {
    for (int i = 0; s[i] != '\0'; i++) {
        int idx = s[i] - 'A';
        usati[idx] = val;
    }
}

/* =================================================================================
 * FUNZIONE RICORSIVA (CORE)
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Esplora l'albero delle decisioni. Per ogni stringa all'indice 'i', abbiamo due strade:
 * 1. ESCLUDERLA: Passiamo alla stringa successiva (i+1) senza cambiare nulla.
 * 2. INCLUDERLA: Se non ha conflitti, la aggiungiamo al set, incrementiamo il punteggio
 * e passiamo alla successiva.
 *
 * PARAMETRI:
 * - char **stringhe: L'array di stringhe originale.
 * - int n: Dimensione totale dell'array.
 * - int i: Indice della stringa corrente che stiamo valutando.
 * - int *usati: Array che mantiene lo stato delle lettere occupate finora.
 *
 * RITORNA:
 * Il numero massimo di stringhe disgiunte trovate da questo punto in poi.
 * =================================================================================
 */
int solve_rec(char **stringhe, int n, int i, int *usati) {
    
    /* --- 1. CASO BASE --- */
    // Se abbiamo analizzato tutte le stringhe, non possiamo aggiungerne altre.
    if (i == n) {
        return 0;
    }

    /* --- 2. OPZIONE 1: ESCLUDERE LA STRINGA CORRENTE --- */
    // Proviamo a non prenderla e vediamo qual è il massimo che otteniamo dal resto.
    int max_senza = solve_rec(stringhe, n, i + 1, usati);

    /* --- 3. OPZIONE 2: INCLUDERE LA STRINGA CORRENTE (Se possibile) --- */
    int max_con = 0;
    
    // Controlliamo se è compatibile con le lettere già prese
    if (conflitto(stringhe[i], usati) == 0) {
        
        // AZIONE (Do Move): Occupiamo le lettere
        aggiorna_usati(stringhe[i], usati, 1);
        
        // RICORSIONE: 1 (questa stringa) + il meglio che possiamo fare con le rimanenti
        max_con = 1 + solve_rec(stringhe, n, i + 1, usati);
        
        // BACKTRACKING (Undo Move): Liberiamo le lettere
        // Questo è cruciale: dobbiamo lasciare l'array pulito per quando la ricorsione risale
        // e prova l'opzione "escludi" o altre strade.
        aggiorna_usati(stringhe[i], usati, 0);
    }

    /* --- 4. SCELTA DEL MIGLIORE --- */
    // Ritorniamo il massimo tra averla presa o non averla presa
    if (max_con > max_senza) return max_con;
    else return max_senza;
}

/* =================================================================================
 * FUNZIONE WRAPPER: disgiunte
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Prepara le strutture dati ausiliarie (l'array 'usati') e lancia la ricorsione.
 *
 * CONCETTI CHIAVE:
 * - calloc vs malloc: Usiamo calloc per l'array 'usati' perché ci serve inizializzato
 * tutto a 0 (nessuna lettera usata all'inizio).
 * =================================================================================
 */
int disgiunte(char **stringhe, int n) {
    /* --- 1. ALLOCAZIONE STRUTTURE AUSILIARIE --- */
    // Array di 26 interi (uno per ogni lettera maiuscola)
    int *usati = (int *)calloc(26, sizeof(int));
    
    if (usati == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* --- 2. CHIAMATA AL CORE RICORSIVO --- */
    int risultato = solve_rec(stringhe, n, 0, usati);

    /* --- 3. PULIZIA --- */
    free(usati);

    return risultato;
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Riprodurre l'esempio esatto della traccia (image_66e6fb.png).
 *
 * GESTIONE MEMORIA:
 * Creiamo le stringhe dinamicamente per simulare un caso reale e mostrare
 * la corretta liberazione della memoria (free).
 * =================================================================================
 */
int main() {
    int n = 7;
    
    /* --- 1. ALLOCAZIONE ARRAY DI PUNTATORI --- */
    // Stiamo creando un char** (array di stringhe)
    char **S = (char **)malloc(n * sizeof(char *));

    /* --- 2. CREAZIONE DATI (Hardcoded come da esempio) --- */
    // S0 = ABGCIEF, S1 = BA, S2 = CD, S3 = FE, S4 = GHBD, S5 = JKLGHI, S6 = FK
    
    // Nota: uso strdup (se disponibile) o malloc+strcpy. Qui uso malloc standard per didattica.
    S[0] = strdup("ABGCIEF");
    S[1] = strdup("BA");
    S[2] = strdup("CD");
    S[3] = strdup("FE");
    S[4] = strdup("GHBD");
    S[5] = strdup("JKLGHI");
    S[6] = strdup("FK");

    printf("--- INIZIO TEST ---\n");
    printf("Set di stringhe:\n");
    for(int i=0; i<n; i++) printf("S%d: %s\n", i, S[i]);

    /* --- 3. ESECUZIONE --- */
    int max_disgiunte = disgiunte(S, n);

    /* --- 4. STAMPA RISULTATO --- */
    printf("\nNumero massimo di stringhe disgiunte: %d\n", max_disgiunte);
    printf("Atteso: 4 (es. BA, CD, FE, JKLGHI)\n");

    /* --- 5. PULIZIA MEMORIA --- */
    // Prima libero le stringhe interne
    for (int i = 0; i < n; i++) {
        free(S[i]);
    }
    // Poi libero l'array contenitore
    free(S);

    return 0;
}