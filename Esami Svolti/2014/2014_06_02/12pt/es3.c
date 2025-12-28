#include <stdio.h>
#include <stdlib.h>
#include <float.h>  // Necessario per FLT_MAX (il valore float più grande possibile)

/* ============================================================================
 * FUNZIONE HELPER: swap
 * ============================================================================
 * SCOPO:
 * Scambia due valori float in memoria. Fondamentale per l'algoritmo di
 * permutazione.
 *
 * PARAMETRI:
 * - float *a: Puntatore al primo valore.
 * - float *b: Puntatore al secondo valore.
 * ============================================================================
 */
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

/* ============================================================================
 * FUNZIONE HELPER: calcolaDifferenzaSaldo
 * ============================================================================
 * SCOPO:
 * Data una specifica sequenza di movimenti (permutazione), simula l'andamento
 * del conto corrente e calcola la metrica richiesta: (SaldoMax - SaldoMin).
 *
 * PARAMETRI:
 * - float *v: Il vettore con la sequenza attuale di movimenti.
 * - int n:    La lunghezza del vettore.
 *
 * RITORNO:
 * - float: La differenza tra il saldo massimo e minimo raggiunti.
 *
 * CONCETTI CHIAVE:
 * Simulazione Passo-Passo:
 * Partiamo da 0. Aggiorniamo il saldo corrente a ogni movimento.
 * Dobbiamo ricordare che il saldo iniziale (0) conta come un valore valido
 * per il minimo o il massimo.
 * ============================================================================
 */
float calcolaDifferenzaSaldo(float *v, int n) {
    float saldo_corrente = 0.0f;
    float saldo_max = 0.0f; // Il saldo parte da 0
    float saldo_min = 0.0f; // Il saldo parte da 0

    /* --- 1. SIMULAZIONE MOVIMENTI --- */
    for (int i = 0; i < n; i++) {
        /* Aggiungo il movimento corrente al saldo */
        saldo_corrente += v[i];

        /* Aggiorno i record se necessario */
        if (saldo_corrente > saldo_max) {
            saldo_max = saldo_corrente;
        }
        if (saldo_corrente < saldo_min) {
            saldo_min = saldo_corrente;
        }
    }

    /* --- 2. CALCOLO DELTA --- */
    return (saldo_max - saldo_min);
}

/* ============================================================================
 * FUNZIONE RICORSIVA (CORE): permuta
 * ============================================================================
 * SCOPO:
 * Genera ricorsivamente tutte le permutazioni possibili del vettore.
 * Quando trova una permutazione completa, calcola il costo e vede se è
 * la migliore trovata finora.
 *
 * PARAMETRI:
 * - float *v:        Il vettore di lavoro (viene mescolato ricorsivamente).
 * - int k:           Indice dell'elemento che stiamo fissando in questa chiamata.
 * - int n:           Dimensione totale del vettore.
 * - float *best_diff: Puntatore alla variabile che conserva la miglior differenza trovata (minimo globale).
 * - float *best_sol: Vettore di supporto dove copiamo la permutazione vincente.
 *
 * CONCETTI CHIAVE:
 * 1. Backtracking:
 * La struttura `swap -> permuta -> swap` è il cuore del backtracking.
 * Scambiamo un elemento, scendiamo nella ricorsione, e al ritorno RIMETTIAMO
 * l'elemento a posto (undo) per provare la combinazione successiva.
 * ============================================================================
 */
void permuta(float *v, int k, int n, float *best_diff, float *best_sol) {
    
    /* --- 1. CASO BASE (Permutazione Completa) --- */
    /* Abbiamo fissato tutti gli elementi dalla posizione 0 alla n-1 */
    if (k == n) {
        /* Calcoliamo il costo di QUESTA configurazione specifica */
        float current_diff = calcolaDifferenzaSaldo(v, n);

        /* Se questa configurazione è migliore (differenza più piccola) della migliore vista finora... */
        if (current_diff < *best_diff) {
            /* Aggiorniamo il record */
            *best_diff = current_diff;

            /* SALVIAMO la sequenza vincente nel vettore di backup */
            /* Copiamo n elementi da 'v' a 'best_sol' */
            for (int i = 0; i < n; i++) {
                best_sol[i] = v[i];
            }
        }
        return;
    }

    /* --- 2. PASSO RICORSIVO --- */
    /* Proviamo a mettere in posizione 'k' ogni elemento disponibile da 'k' in poi */
    for (int j = k; j < n; j++) {
        
        /* Scambio l'elemento corrente v[k] con il candidato v[j] */
        swap(&v[k], &v[j]);

        /* Ricorsione: Fisso k e vado a permutare il resto (k+1) */
        permuta(v, k + 1, n, best_diff, best_sol);

        /* BACKTRACKING: Anullo lo scambio per ripristinare l'ordine originale
           prima della prossima iterazione del ciclo for */
        swap(&v[k], &v[j]);
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER: ordinaMovimenti
 * ============================================================================
 * SCOPO:
 * Interfaccia principale richiesta dall'esercizio. Prepara le strutture dati
 * ausiliarie e lancia la ricorsione.
 *
 * PARAMETRI:
 * - float *v: Il vettore input/output. Alla fine conterrà la sequenza ottima.
 * - int n:    La lunghezza del vettore.
 *
 * GESTIONE MEMORIA:
 * Dobbiamo allocare un array temporaneo (`best_sol`) per salvare la permutazione
 * migliore mentre la ricorsione continua a mescolare `v`.
 * ============================================================================
 */
void ordinaMovimenti(float *v, int n) {
    /* Controllo input difensivo */
    if (v == NULL || n <= 0) return;

    /* --- 1. ALLOCAZIONE MEMORIA --- */
    /* Allocazione array di supporto per salvare la soluzione migliore */
    float *best_sol = (float *)malloc(n * sizeof(float));
    if (best_sol == NULL) {
        printf("Errore malloc\n");
        return;
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    /* Inizializziamo best_diff con il massimo float possibile,
       così la prima soluzione trovata sarà sicuramente minore e verrà salvata. */
    float best_diff = FLT_MAX;

    /* --- 3. LANCIO ALGORITMO --- */
    /* Chiamiamo la funzione ricorsiva partendo dall'indice 0 */
    permuta(v, 0, n, &best_diff, best_sol);

    /* --- 4. COPIA RISULTATO E PULIZIA --- */
    /* La ricorsione ha finito. La soluzione migliore è in 'best_sol'.
       L'esercizio chiede di ritornare la sequenza "sul vettore di partenza".
       Quindi sovrascriviamo v con best_sol. */
    for (int i = 0; i < n; i++) {
        v[i] = best_sol[i];
    }

    /* Liberiamo la memoria ausiliaria */
    free(best_sol);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Creiamo uno scenario bancario:
 * +100, -50, +20, -100.
 *
 * Scenario A (Pessimo): +100, +20, -50, -100
 * Saldo: 0 -> 100 -> 120 -> 70 -> -30.
 * Max: 120, Min: -30. Diff: 150.
 *
 * Scenario B (Ottimo?): +100, -100, +20, -50
 * Saldo: 0 -> 100 -> 0 -> 20 -> -30.
 * Max: 100, Min: -30. Diff: 130.
 * ============================================================================
 */
int main() {
    /* Dati di prova hardcoded */
    float movimenti[] = {100.0, -50.0, 20.0, -100.0};
    int n = 4;

    /* Stampa Iniziale */
    printf("--- Vettore Iniziale ---\n[ ");
    for (int i = 0; i < n; i++) printf("%.1f ", movimenti[i]);
    printf("]\n");

    /* Chiamata alla funzione */
    printf("\nCalcolo permutazione ottimale...\n");
    ordinaMovimenti(movimenti, n);

    /* Stampa Finale */
    printf("--- Vettore Riordinato (Minimizza Max-Min Saldo) ---\n[ ");
    for (int i = 0; i < n; i++) printf("%.1f ", movimenti[i]);
    printf("]\n");

    /* Verifica del risultato ottenuto */
    float diff_finale = calcolaDifferenzaSaldo(movimenti, n);
    printf("Differenza Saldo ottenuta: %.2f\n", diff_finale);

    return 0;
}