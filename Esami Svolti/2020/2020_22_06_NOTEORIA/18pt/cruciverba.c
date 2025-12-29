#include "cruciverba.h"

/* ============================================================================
 * SEZIONE 1: CARICAMENTO DATI E GESTIONE MEMORIA
 * ============================================================================ */

/*
 * Funzione: leggiSchema
 * Scopo: Legge il file dello schema, alloca la matrice e il vettore degli slot.
 * Parametri:
 * - filename: nome del file da aprire.
 * Ritorna: puntatore alla struttura schema_t allocata.
 */
schema_t *leggiSchema(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file schema: %s\n", filename);
        exit(1);
    }

    schema_t *s = (schema_t *)malloc(sizeof(schema_t)); // Alloco il contenitore principale
    
    // Leggo R, C e N
    if (fscanf(fp, "%d %d %d", &s->R, &s->C, &s->n_slot) != 3) {
        // Controllo errori di lettura base
        return NULL;
    }

    /* --- ALLOCAZIONE MATRICE DINAMICA (Concetto Chiave) --- */
    /*
     * Dobbiamo allocare una griglia R x C.
     * Passo 1: Alloco un vettore di R puntatori a char.
     * Passo 2: Per ogni puntatore, alloco un vettore di C char.
     */
    s->griglia = (char **)malloc(s->R * sizeof(char *));
    for (int i = 0; i < s->R; i++) {
        s->griglia[i] = (char *)malloc(s->C * sizeof(char));
        // Inizializzo a spazio vuoto o carattere nullo per pulizia
        for (int j = 0; j < s->C; j++) {
            s->griglia[i][j] = ' '; 
        }
    }

    /* --- ALLOCAZIONE VETTORE SLOT --- */
    s->slots = (slot_t *)malloc(s->n_slot * sizeof(slot_t));
    
    // Leggo i vincoli: lunghezza, riga, colonna, direzione
    for (int i = 0; i < s->n_slot; i++) {
        fscanf(fp, "%d %d %d %c", 
               &s->slots[i].lunghezza, 
               &s->slots[i].riga, 
               &s->slots[i].col, 
               &s->slots[i].direzione);
    }

    fclose(fp);
    return s;
}

/*
 * Funzione: leggiParole
 * Scopo: Legge le parole e le organizza per lunghezza per accesso O(1).
 * Concetto Chiave: "Bucket Sort" o indicizzazione diretta.
 * Invece di una lista unica, usiamo un array dove l'indice corrisponde alla lunghezza.
 */
parole_t *leggiParole(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) exit(1);

    parole_t *p = (parole_t *)malloc(sizeof(parole_t));
    int n_temp;
    fscanf(fp, "%d", &n_temp); // Leggo numero totale parole
    p->n_parole_tot = n_temp;

    // Inizializzo le strutture per lunghezza (0..MAX_STR)
    // Usiamo calloc per inizializzare tutto a 0/NULL automaticamente
    p->elenco_per_lunghezza = (char ***)calloc(MAX_STR, sizeof(char **));
    p->conta_per_lunghezza = (int *)calloc(MAX_STR, sizeof(int));
    p->usata = (int **)calloc(MAX_STR, sizeof(int *));

    /* * Nota: Poiché non so a priori quante parole ci sono per ogni lunghezza,
     * in un esame reale potrei fare due passaggi (uno per contare, uno per allocare).
     * Qui, per semplicità didattica, carico tutto in un buffer temporaneo, conto,
     * e poi alloco.
     */
    
    // Buffer temporaneo per salvare le parole lette e poi smistarle
    char **tempBuf = (char **)malloc(n_temp * sizeof(char *));
    for(int i=0; i<n_temp; i++) {
        tempBuf[i] = (char *)malloc(MAX_STR * sizeof(char));
        fscanf(fp, "%s", tempBuf[i]);
        int len = strlen(tempBuf[i]);
        if (len < MAX_STR)
            p->conta_per_lunghezza[len]++;
    }

    // Ora alloco i bucket per ogni lunghezza
    for (int len = 1; len < MAX_STR; len++) {
        int qty = p->conta_per_lunghezza[len];
        if (qty > 0) {
            // Alloco vettore di stringhe
            p->elenco_per_lunghezza[len] = (char **)malloc(qty * sizeof(char *));
            // Alloco vettore di flag "usata"
            p->usata[len] = (int *)calloc(qty, sizeof(int));
            
            // Riempio il bucket (Inefficiente O(N^2) ma chiaro didatticamente, si potrebbe ottimizzare)
            int k = 0;
            for (int i = 0; i < n_temp; i++) {
                if (strlen(tempBuf[i]) == len) {
                    p->elenco_per_lunghezza[len][k] = strdup(tempBuf[i]); // strdup duplica la stringa
                    k++;
                }
            }
        }
    }

    // Pulizia buffer temporaneo
    for(int i=0; i<n_temp; i++) free(tempBuf[i]);
    free(tempBuf);
    fclose(fp);

    return p;
}

/* ============================================================================
 * SEZIONE 2: VERIFICA (Punto 1.2.3)
 * ============================================================================ */

/*
 * Funzione: verificaSchema
 * Scopo: Verifica se le parole presenti nella griglia 'm' esistono nel dizionario 'p'
 * e rispettano le posizioni definite in 's'.
 */
int verificaSchema(schema_t *s, parole_t *p) { //
    char buffer[MAX_STR];

    // Itero su tutti gli slot definiti nello schema
    for (int i = 0; i < s->n_slot; i++) {
        slot_t sl = s->slots[i];
        int r = sl.riga;
        int c = sl.col;
        int len = sl.lunghezza;
        
        // Estraggo la parola dalla griglia
        for (int k = 0; k < len; k++) {
            if (sl.direzione == 'O') { // Orizzontale
                 // Controllo bounds
                 if (c + k >= s->C) return 0; 
                 buffer[k] = s->griglia[r][c + k];
            } else { // Verticale
                 if (r + k >= s->R) return 0;
                 buffer[k] = s->griglia[r + k][c];
            }
        }
        buffer[len] = '\0'; // Termino stringa

        // Verifico se 'buffer' esiste nel dizionario delle parole di lunghezza 'len'
        int trovata = 0;
        if (p->conta_per_lunghezza[len] > 0) {
            for (int j = 0; j < p->conta_per_lunghezza[len]; j++) {
                if (strcmp(buffer, p->elenco_per_lunghezza[len][j]) == 0) {
                    trovata = 1;
                    break;
                }
            }
        }
        
        if (!trovata) return 0; // Parola non valida
    }
    return 1; // Tutto ok
}

/* ============================================================================
 * SEZIONE 3: ALGORITMO DI RICERCA (SOLVE) (Punto 1.2.4)
 * ============================================================================ */

/*
 * Funzione: checkSol
 * Scopo: Controlla se una parola può essere inserita in uno slot specifico.
 * Logica: Controlla le INTERSEZIONI. Se nella griglia c'è già una lettera (messa da un'altra parola),
 * la nuova parola deve avere la stessa lettera in quella posizione.
 */
int checkSol(schema_t *s, int slot_idx, char *parola) { //
    slot_t sl = s->slots[slot_idx];
    int r = sl.riga;
    int c = sl.col;

    for (int k = 0; k < sl.lunghezza; k++) {
        char car_griglia;
        
        if (sl.direzione == 'O') 
            car_griglia = s->griglia[r][c + k];
        else 
            car_griglia = s->griglia[r + k][c];

        // Se la cella non è vuota (' ') E non corrisponde alla lettera della parola -> conflitto
        if (car_griglia != ' ' && car_griglia != parola[k]) {
            return 0; // Conflitto
        }
    }
    return 1; // Compatibile
}

/*
 * Funzione: scriviParola / cancellaParola
 * Scopo: Helper per scrivere o rimuovere una parola sulla griglia durante il backtracking.
 * Nota: Quando scriviamo, sovrascriviamo. Ma attenzione: quando cancelliamo, non possiamo
 * cancellare le lettere che appartengono ad ALTRE parole (incroci).
 * PER SEMPLICITA' DIDATTICA E DEL TESTO D'ESAME:
 * Spesso in questi problemi si assume di poter sovrascrivere. Per fare un backtracking perfetto
 * dovremmo sapere se la cella era vuota PRIMA di scriverci.
 * * Strategia Robusta: Non "cancello" mettendo spazi, ma mi affido al fatto che al ritorno
 * dalla ricorsione, se fallisce, sovrascrivo con la prossima parola o ripristino lo stato precedente.
 * * Tuttavia, il modo più pulito senza strutture di supporto complesse è:
 * Salvare lo stato della griglia prima della chiamata ricorsiva? Costoso.
 * * Approccio adottato: Scriviamo la parola. Se il backtracking torna su, dobbiamo "pulire".
 * Ma come distinguiamo una lettera messa da noi da una messa da un incrocio precedente?
 * Soluzione: Per questo esercizio, faremo una semplificazione. La `solve_r` proverà a riempire.
 * Se fallisce, ripristina la cella a ' ' SOLO SE non è incrociata? È complesso.
 * * TRUCCO DEL PROFESSORE:
 * Salviamo una copia locale delle celle che stiamo per modificare PRIMA di modificarle.
 * Al backtracking, ripristiniamo quelle copie.
 */

void solve_r(schema_t *s, parole_t *p, int slot_idx, int *trovato) { //
    // 1. Caso Base: Se abbiamo riempito tutti gli slot (0 a n-1), abbiamo finito.
    if (slot_idx == s->n_slot) {
        *trovato = 1;
        return;
    }

    slot_t sl = s->slots[slot_idx];
    int len = sl.lunghezza;

    // 2. Iterazione: Provo tutte le parole disponibili di lunghezza 'len'
    // Accesso O(1) all'elenco corretto grazie alla struttura dati ottimizzata
    for (int i = 0; i < p->conta_per_lunghezza[len]; i++) {
        
        if (*trovato) return; // Pruning globale: soluzione già trovata

        // Se la parola non è già usata
        if (!p->usata[len][i]) {
            char *parola_candidata = p->elenco_per_lunghezza[len][i];

            // 3. Pruning Locale: Controllo compatibilità incroci
            if (checkSol(s, slot_idx, parola_candidata)) {
                
                // --- PREPARA BACKTRACKING ---
                // Salvo i caratteri originali della griglia per poterli ripristinare
                char backup[MAX_STR];
                int r = sl.riga;
                int c = sl.col;
                
                for(int k=0; k<len; k++) {
                    if (sl.direzione == 'O') backup[k] = s->griglia[r][c+k];
                    else                     backup[k] = s->griglia[r+k][c];
                    
                    // Applico mossa (scrivo su griglia)
                    if (sl.direzione == 'O') s->griglia[r][c+k] = parola_candidata[k];
                    else                     s->griglia[r+k][c] = parola_candidata[k];
                }
                
                // Marco parola come usata
                p->usata[len][i] = 1;

                // 4. Passo Ricorsivo
                solve_r(s, p, slot_idx + 1, trovato);

                // 5. Backtracking
                if (!(*trovato)) {
                    // Ripristino parola come non usata
                    p->usata[len][i] = 0;
                    // Ripristino griglia allo stato precedente (backup)
                    for(int k=0; k<len; k++) {
                        if (sl.direzione == 'O') s->griglia[r][c+k] = backup[k];
                        else                     s->griglia[r+k][c] = backup[k];
                    }
                }
            }
        }
    }
}

void solve(schema_t *s, parole_t *p) { //
    int trovato = 0;
    // Chiamo la funzione ricorsiva partendo dallo slot 0
    solve_r(s, p, 0, &trovato);

    if (trovato) {
        printf("Soluzione Trovata!\n");
        stampaGriglia(s);
    } else {
        printf("Nessuna soluzione possibile.\n");
    }
}

/* ============================================================================
 * SEZIONE 4: UTILITY E PULIZIA
 * ============================================================================ */

void stampaGriglia(schema_t *s) {
    printf("\n--- GRIGLIA ---\n");
    for (int i = 0; i < s->R; i++) {
        for (int j = 0; j < s->C; j++) {
            // Se è vuoto stampo un punto per leggibilità
            printf("%c ", s->griglia[i][j] == ' ' ? '.' : s->griglia[i][j]);
        }
        printf("\n");
    }
    printf("---------------\n");
}

void liberaSchema(schema_t *s) {
    for (int i = 0; i < s->R; i++) free(s->griglia[i]);
    free(s->griglia);
    free(s->slots);
    free(s);
}

void liberaParole(parole_t *p) {
    for (int i = 1; i < MAX_STR; i++) {
        if (p->conta_per_lunghezza[i] > 0) {
            for (int j = 0; j < p->conta_per_lunghezza[i]; j++) {
                free(p->elenco_per_lunghezza[i][j]);
            }
            free(p->elenco_per_lunghezza[i]);
            free(p->usata[i]);
        }
    }
    free(p->elenco_per_lunghezza);
    free(p->conta_per_lunghezza);
    free(p->usata);
    free(p);
}