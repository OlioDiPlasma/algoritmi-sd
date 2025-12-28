#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ========================================================================= */
/* MODULO 1: DEFINIZIONI E STRUTTURE DATI                                    */
/* In un progetto reale, questo andrebbe in un file header (es. data.h)      */
/* ========================================================================= */

/*
 * Struttura: Problema
 * Scopo: Contiene tutti i dati in ingresso letti dal file.
 * Non viene mai modificata dall'algoritmo (è Read-Only per il solver).
 */
typedef struct {
    int T;          // Numero di incarichi (Task)
    int *D;         // Vettore Difficoltà/Valore incarichi (dim T)
    int P;          // Numero di persone
    int *E;         // Vettore Esperienza persone (dim P)
    int **S;        // Matrice Sinergie (dim P x P)
} Problema;

/*
 * Struttura: Soluzione
 * Scopo: Incapsula il risultato. Invece di passare variabili sparse,
 * restituiamo un oggetto che contiene l'assegnazione e il punteggio totale.
 */
typedef struct {
    int *assegnazioni; // Vettore di dim P: indice persona -> indice incarico
    int resa_totale;   // Valore calcolato della funzione obiettivo
} Soluzione;


/* ========================================================================= */
/* MODULO 2: GESTIONE I/O E MEMORIA                                          */
/* In un progetto reale, questo andrebbe in input.c                          */
/* ========================================================================= */

/*
 * Nome: carica_dati
 * Scopo: Apre il file, legge le dimensioni, alloca memoria dinamica e popola la struct.
 * Parametri:
 * - filename: stringa col nome del file
 * Ritorna: Un puntatore a una struct Problema allocata.
 */
Problema* carica_dati(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", filename);
        exit(-1);
    }

    /* --- 1. ALLOCAZIONE STRUTTURA PRINCIPALE --- */
    Problema *p = (Problema *)malloc(sizeof(Problema));

    /* --- 2. LETTURA INCARICHI (T e vettore D) --- */
    fscanf(fp, "%d", &p->T);
    p->D = (int *)malloc(p->T * sizeof(int)); // Alloco vettore D
    for (int i = 0; i < p->T; i++) {
        fscanf(fp, "%d", &p->D[i]);
    }

    /* --- 3. LETTURA PERSONE (P e vettore E) --- */
    fscanf(fp, "%d", &p->P);
    p->E = (int *)malloc(p->P * sizeof(int)); // Alloco vettore E
    for (int i = 0; i < p->P; i++) {
        fscanf(fp, "%d", &p->E[i]);
    }

    /* --- 4. LETTURA MATRICE SINERGIE (S) --- */
    /* Allocazione dinamica matrice P x P */
    p->S = (int **)malloc(p->P * sizeof(int *)); // Vettore di puntatori alle righe
    for (int i = 0; i < p->P; i++) {
        p->S[i] = (int *)malloc(p->P * sizeof(int)); // Singola riga
        for (int j = 0; j < p->P; j++) {
            fscanf(fp, "%d", &p->S[i][j]);
        }
    }

    fclose(fp);
    return p;
}

/*
 * Nome: libera_problema
 * Scopo: Pulisce tutta la memoria allocata per il problema per evitare memory leaks.
 * Ordine: Prima le righe della matrice, poi il vettore righe, poi i vettori semplici, infine la struct.
 */
void libera_problema(Problema *p) {
    if (p == NULL) return;

    free(p->D);
    free(p->E);
    
    // Libera matrice
    for (int i = 0; i < p->P; i++) {
        free(p->S[i]);
    }
    free(p->S);
    
    free(p);
}

/* ========================================================================= */
/* MODULO 3: SOLVER (ALGORITMO)                                              */
/* In un progetto reale, questo andrebbe in solver.c                         */
/* ========================================================================= */

/*
 * Nome: valuta_soluzione
 * Scopo: Implementa le regole di business complesse (soglia 75%, sinergie, max value).
 * Ritorna: Intero rappresentante la resa totale della configurazione attuale.
 */
int valuta_soluzione(int *assegnazioni, Problema *p) {
    int resa_complessiva = 0;

    /* Analizzo un incarico alla volta */
    for (int t = 0; t < p->T; t++) {
        int somma_forza = 0;
        
        /* 1. Contributo Individuale (Esperienza) */
        /* Scorro tutte le persone e vedo chi lavora al task t */
        for (int i = 0; i < p->P; i++) {
            if (assegnazioni[i] == t) {
                somma_forza += p->E[i];
            }
        }

        /* 2. Contributo Sinergico */
        /* Scorro tutte le coppie uniche (i, j con j > i) */
        for (int i = 0; i < p->P; i++) {
            if (assegnazioni[i] == t) { // Ottimizzazione: entro nel secondo ciclo solo se i è nel task
                for (int j = i + 1; j < p->P; j++) {
                    if (assegnazioni[j] == t) {
                        somma_forza += p->S[i][j];
                    }
                }
            }
        }

        /* 3. Verifica Soglia e Calcolo Resa */
        /* Uso float per precisione sul 75% */
        float soglia = 0.75f * (float)p->D[t];
        
        if ((float)somma_forza >= soglia) {
            /* Se supera la soglia, la resa è il minimo tra la forza e il valore max dell'incarico */
            if (somma_forza > p->D[t])
                resa_complessiva += p->D[t];
            else
                resa_complessiva += somma_forza;
        }
        /* Se non supera la soglia, aggiungo 0 (contributo perso) */
    }
    return resa_complessiva;
}

/*
 * Nome: solve_rec
 * Scopo: Funzione ricorsiva (Disposizioni con ripetizione).
 * Parametri:
 * - livello: Indice della persona che stiamo allocando (0..P-1).
 * - curr_sol: Vettore temporaneo delle assegnazioni in costruzione.
 * - best: Puntatore alla struct Soluzione dove salviamo il meglio.
 * - p: Puntatore ai dati del problema.
 */
void solve_rec(int livello, int *curr_sol, Soluzione *best, Problema *p) {
    
    /* --- CASO BASE --- */
    if (livello == p->P) {
        /* Tutte le persone assegnate. Valutiamo. */
        int valore_attuale = valuta_soluzione(curr_sol, p);
        
        if (valore_attuale > best->resa_totale) {
            best->resa_totale = valore_attuale;
            /* Copia profonda del vettore soluzione */
            for (int i = 0; i < p->P; i++) {
                best->assegnazioni[i] = curr_sol[i];
            }
        }
        return;
    }

    /* --- PASSO RICORSIVO --- */
    /* La persona 'livello' può essere assegnata a qualsiasi incarico da 0 a T-1 */
    for (int t = 0; t < p->T; t++) {
        curr_sol[livello] = t; // Assegno
        solve_rec(livello + 1, curr_sol, best, p); // Ricorsione
        // Backtracking implicito: al prossimo giro del for sovrascrivo curr_sol[livello]
    }
}

/* ========================================================================= */
/* MAIN                                                                      */
/* ========================================================================= */

int main() {
    /* --- 1. CARICAMENTO DATI --- */
    printf("Caricamento dati da input.txt...\n");
    Problema *p = carica_dati("input.txt");

    /* --- 2. PREPARAZIONE STRUTTURE PER SOLVER --- */
    /* Alloco vettore temporaneo per la ricorsione */
    int *curr_sol = (int *)malloc(p->P * sizeof(int));
    
    /* Alloco e inizializzo la struttura per la soluzione migliore */
    Soluzione best;
    best.assegnazioni = (int *)malloc(p->P * sizeof(int));
    best.resa_totale = -1; // O 0, basta che sia basso

    /* --- 3. ESECUZIONE ALGORITMO --- */
    printf("Avvio calcolo ottimo...\n");
    solve_rec(0, curr_sol, &best, p);

    /* --- 4. STAMPA RISULTATI --- */
    printf("\n=== RISULTATO OTTIMO ===\n");
    printf("Resa Totale Massima: %d\n", best.resa_totale);
    printf("Dettaglio Assegnazioni:\n");
    for (int i = 0; i < p->P; i++) {
        printf("- Persona %d (Esp: %d) -> Incarico %d (Valore Max: %d)\n", 
               i, p->E[i], best.assegnazioni[i], p->D[best.assegnazioni[i]]);
    }

    /* --- 5. PULIZIA MEMORIA --- */
    free(curr_sol);
    free(best.assegnazioni);
    libera_problema(p);

    return 0;
}