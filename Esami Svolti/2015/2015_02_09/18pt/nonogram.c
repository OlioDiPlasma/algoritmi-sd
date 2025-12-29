/*
 * File: nonogram.c
 * Scopo: Implementazione di tutte le logiche del Nonogram.
 */

#include "nonogram.h"

/* =================================================================================
 * SEZIONE 1: GESTIONE MEMORIA E FILE (Punto 1)
 * ================================================================================= */

/*
 * Funzione: carica_vincoli
 * Scopo: Legge il file 1 (vincoli), alloca la struttura Schema e i relativi array.
 * Input: Nome del file.
 * Ritorna: Puntatore alla struttura Schema allocata.
 */
Schema* carica_vincoli(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Errore apertura file vincoli!\n");
        return NULL;
    }

    /* --- 1. Allocazione Struttura Principale --- */
    Schema *s = (Schema *)malloc(sizeof(Schema));

    /* --- 2. Lettura Righe (N) --- */
    fscanf(f, "%d", &s->R); // Legge N
    
    // Alloco l'array dei vincoli per le righe
    s->vincoli_righe = (Vincolo *)malloc(s->R * sizeof(Vincolo));

    // Ciclo per leggere i vincoli di ogni riga
    for (int i = 0; i < s->R; i++) {
        fscanf(f, "%d", &s->vincoli_righe[i].n_blocchi);
        // Alloco l'array delle lunghezze specifico per questa riga
        s->vincoli_righe[i].lunghezze = (int *)malloc(s->vincoli_righe[i].n_blocchi * sizeof(int));
        for (int j = 0; j < s->vincoli_righe[i].n_blocchi; j++) {
            fscanf(f, "%d", &s->vincoli_righe[i].lunghezze[j]);
        }
    }

    /* --- 3. Lettura Colonne (M) --- */
    fscanf(f, "%d", &s->C); // Legge M

    // Alloco l'array dei vincoli per le colonne
    s->vincoli_col = (Vincolo *)malloc(s->C * sizeof(Vincolo));

    for (int i = 0; i < s->C; i++) {
        fscanf(f, "%d", &s->vincoli_col[i].n_blocchi);
        s->vincoli_col[i].lunghezze = (int *)malloc(s->vincoli_col[i].n_blocchi * sizeof(int));
        for (int j = 0; j < s->vincoli_col[i].n_blocchi; j++) {
            fscanf(f, "%d", &s->vincoli_col[i].lunghezze[j]);
        }
    }

    /* --- 4. Allocazione Griglia (Matrice) --- */
    // Uso un doppio puntatore: un array di puntatori alle righe
    s->griglia = (int **)malloc(s->R * sizeof(int *));
    for (int i = 0; i < s->R; i++) {
        // Ogni riga è un array di interi di dimensione C
        // Uso calloc per inizializzare tutto a 0 (vuoto) di default
        s->griglia[i] = (int *)calloc(s->C, sizeof(int));
    }

    fclose(f);
    return s;
}

/*
 * Funzione: libera_schema
 * Scopo: Liberare TUTTA la memoria allocata con malloc per evitare memory leak.
 * Concetto chiave: L'ordine di liberazione è inverso all'allocazione (prima il profondo, poi il contenitore).
 */
void libera_schema(Schema *s) {
    if (s == NULL) return;

    // 1. Libero i sotto-array dei vincoli righe
    for (int i = 0; i < s->R; i++) {
        free(s->vincoli_righe[i].lunghezze);
    }
    free(s->vincoli_righe);

    // 2. Libero i sotto-array dei vincoli colonne
    for (int i = 0; i < s->C; i++) {
        free(s->vincoli_col[i].lunghezze);
    }
    free(s->vincoli_col);

    // 3. Libero la griglia
    for (int i = 0; i < s->R; i++) {
        free(s->griglia[i]);
    }
    free(s->griglia);

    // 4. Libero la struttura principale
    free(s);
}

/*
 * Funzione: leggi_soluzione_da_file
 * Scopo: Riempie la griglia leggendo da file (per il Punto 2).
 */
void leggi_soluzione_da_file(Schema *s, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return;

    for(int i=0; i<s->R; i++) {
        for(int j=0; j<s->C; j++) {
            fscanf(f, "%d", &s->griglia[i][j]);
        }
    }
    fclose(f);
}

/*
 * Funzione: salva_soluzione
 * Scopo: Scrive la griglia su file (per il Punto 3).
 */
void salva_soluzione(Schema *s, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) return;

    for(int i=0; i<s->R; i++) {
        for(int j=0; j<s->C; j++) {
            fprintf(f, "%d ", s->griglia[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

/* =================================================================================
 * SEZIONE 2: LOGICA DI VERIFICA (Punto 2)
 * ================================================================================= */

/*
 * Funzione Ausiliaria: check_array
 * Scopo: Verifica se un array di celle (una riga o una colonna) rispetta un vincolo.
 * Parametri:
 * - arr: l'array di interi (la riga o colonna estratta)
 * - len: lunghezza dell'array
 * - v: il vincolo da rispettare
 * Ritorna: 1 se valido, 0 se non valido.
 */
int check_array(int *arr, int len, Vincolo v) {
    int gruppi_trovati = 0;
    int indice_corrente = 0; // Dove siamo nel vincolo
    int contatore = 0;       // Conta celle nere consecutive

    for (int i = 0; i < len; i++) {
        if (arr[i] == 1) {
            contatore++;
        } else {
            // Se finisce un blocco nero
            if (contatore > 0) {
                // Se abbiamo trovato più gruppi di quelli previsti o la lunghezza non corrisponde
                if (indice_corrente >= v.n_blocchi || v.lunghezze[indice_corrente] != contatore) {
                    return 0; // Errore
                }
                indice_corrente++;
                contatore = 0;
            }
        }
    }
    // Controllo finale se l'array finisce con celle nere
    if (contatore > 0) {
        if (indice_corrente >= v.n_blocchi || v.lunghezze[indice_corrente] != contatore) {
            return 0;
        }
        indice_corrente++;
    }

    // Alla fine devo aver soddisfatto esattamente tutti i blocchi del vincolo
    return (indice_corrente == v.n_blocchi);
}

/*
 * Funzione: verifica_soluzione
 * Scopo: Controlla se l'intera griglia è valida.
 */
int verifica_soluzione(Schema *s) {
    // 1. Verifica Righe
    for (int i = 0; i < s->R; i++) {
        // La riga 'i' è già un array, posso passarla direttamente
        if (!check_array(s->griglia[i], s->C, s->vincoli_righe[i])) {
            return 0; // Falso
        }
    }

    // 2. Verifica Colonne
    // Le colonne NON sono array contigui in memoria, devo estrarle in un buffer temporaneo
    int *buffer_col = (int *)malloc(s->R * sizeof(int));
    for (int j = 0; j < s->C; j++) {
        for (int i = 0; i < s->R; i++) {
            buffer_col[i] = s->griglia[i][j]; // Copio la colonna j
        }
        if (!check_array(buffer_col, s->R, s->vincoli_col[j])) {
            free(buffer_col);
            return 0;
        }
    }
    free(buffer_col);
    
    return 1; // Tutto corretto
}

/* =================================================================================
 * SEZIONE 3: RICORSIONE E BACKTRACKING (Punto 3)
 * ================================================================================= */

/*
 * Funzione Ausiliaria: check_array_parziale
 * Scopo: Verifica se una riga/colonna NON viola i vincoli MENTRE la stiamo costruendo.
 * Differenza: Se il vincolo dice "3 neri" e io ne ho messi "2", non è errore finché la riga non è finita.
 * Ma se il vincolo dice "3 neri" e io ne ho messi "4", è errore subito.
 * NOTA: Per semplicità didattica, useremo una versione semplificata nel wrapper.
 */


/*
 * Funzione: solve_rec
 * Scopo: Funzione ricorsiva di backtracking.
 * Parametri: pos (da 0 a R*C - 1) indica la cella corrente linearizzata.
 */
int solve_rec(Schema *s, int pos) {
    // CASO BASE: Abbiamo riempito tutte le celle
    if (pos == s->R * s->C) {
        return verifica_soluzione(s);
    }

    // Calcolo coordinate riga (r) e colonna (c) dalla posizione lineare
    int r = pos / s->C;
    int c = pos % s->C;

    // TENTATIVO 1: Provo a mettere 0 (BIANCO)
    s->griglia[r][c] = 0;
    // OTTIMIZZAZIONE (Opzionale ma utile): Se sono a fine riga, controllo subito la riga
    // Se la riga appena finita è sbagliata, non scendo nel ramo ricorsivo (pruning)
    int procedi = 1;
    if (c == s->C - 1) {
        if (!check_array(s->griglia[r], s->C, s->vincoli_righe[r])) {
            procedi = 0;
        }
    }

    if (procedi) {
        if (solve_rec(s, pos + 1)) return 1; // Trovato! Risalgo
    }

    // TENTATIVO 2: Provo a mettere 1 (NERO)
    s->griglia[r][c] = 1;
    
    procedi = 1;
    if (c == s->C - 1) {
        if (!check_array(s->griglia[r], s->C, s->vincoli_righe[r])) {
            procedi = 0;
        }
    }

    if (procedi) {
        if (solve_rec(s, pos + 1)) return 1; // Trovato! Risalgo
    }

    // BACKTRACKING: Se né 0 né 1 vanno bene, rimetto a 0 (pulizia) e ritorno 0
    s->griglia[r][c] = 0;
    return 0;
}

int risolvi_nonogram(Schema *s) {
    // Pulisco la griglia per sicurezza
    for(int i=0; i<s->R; i++)
        for(int j=0; j<s->C; j++)
            s->griglia[i][j] = 0;
            
    return solve_rec(s, 0);
}