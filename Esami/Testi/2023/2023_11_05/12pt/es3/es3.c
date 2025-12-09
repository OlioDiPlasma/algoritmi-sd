#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 9999 // Valore sentinella per indicare "nessuna soluzione ancora trovata"

/*
 * ======================================================================================
 * NOME FUNZIONE: solve_exact_cover
 * ======================================================================================
 * SCOPO:
 * Funzione ricorsiva (backtracking) per trovare la copertura esatta minima.
 *
 * PARAMETRI:
 * - int **M: La matrice binaria (O righe x S colonne).
 * - int O, int S: Dimensioni (Oggetti, Insiemi).
 * - int *covered: Array booleano (0/1). covered[i]=1 se l'oggetto i è già coperto.
 * - int count: Numero di insiemi selezionati nella ricorsione corrente.
 * - int *best_count: Puntatore al miglior numero di insiemi trovato finora (minimo).
 * - int *current_sol: Array degli indici dei set scelti nel percorso corrente.
 * - int *best_sol: Array dove salviamo la soluzione migliore.
 *
 * CONCETTI CHIAVE:
 * 1. Strategia "First Uncovered":
 * Invece di scorrere le colonne a caso, cerchiamo sempre il primo oggetto 'i'
 * che ha valore 0 in 'covered'. Questo forza l'algoritmo a fare progressi
 * verso la copertura completa.
 *
 * 2. Backtracking:
 * Facciamo una scelta (selezioniamo una colonna), andiamo avanti (ricorsione),
 * e se torniamo indietro annulliamo la scelta (togliamo la copertura).
 * ======================================================================================
 */
void solve_exact_cover(int **M, int O, int S, int *covered, int count, int *best_count, 
                       int *current_sol, int *best_sol) {

    /* --- 1. PRUNING PER OTTIMALITÀ --- */
    // Se stiamo già usando più set della soluzione migliore trovata prima, stop.
    // Esempio: Se ho già trovato una soluzione con 3 set, e ora sono a quota 4, inutile continuare.
    if (count >= *best_count) {
        return;
    }

    /* --- 2. RICERCA DEL PRIMO OGGETTO SCOPERTO --- */
    int first_uncovered = -1;
    for (int i = 0; i < O; i++) {
        if (covered[i] == 0) {
            first_uncovered = i;
            break; // Trovato, usciamo subito dal ciclo
        }
    }

    /* --- 3. CASO BASE: TUTTI COPERTI --- */
    // Se first_uncovered è rimasto -1, significa che tutti gli oggetti sono coperti.
    // Abbiamo trovato una soluzione valida!
    if (first_uncovered == -1) {
        // Poiché abbiamo passato il check di pruning all'inizio,
        // sappiamo per certo che count < *best_count. Aggiorniamo il migliore.
        *best_count = count;
        
        // Salviamo la configurazione vincente
        for(int k=0; k<count; k++) {
            best_sol[k] = current_sol[k];
        }
        return;
    }

    /* --- 4. PASSO RICORSIVO --- */
    // Cerchiamo tutte le colonne (set) 'j' che possono coprire 'first_uncovered'
    for (int j = 0; j < S; j++) {
        
        // Verifica preliminare: La colonna j contiene l'oggetto che stiamo cercando di coprire?
        if (M[first_uncovered][j] == 1) {

            // --- CHECK DI DISGIUNZIONE (PRUNING VALIDITÀ) ---
            // Dobbiamo verificare che questa colonna 'j' non contenga ALTRI oggetti
            // che sono GIÀ stati coperti da altri set scelti in precedenza.
            int is_disjoint = 1;
            for (int r = 0; r < O; r++) {
                if (M[r][j] == 1 && covered[r] == 1) {
                    is_disjoint = 0; // Conflitto! Oggetto r è in questa colonna ma era già coperto
                    break;
                }
            }

            if (is_disjoint) {
                // --- A. DO (Faccio la mossa) ---
                
                // Marco come coperti tutti gli oggetti presenti nella colonna j
                for (int r = 0; r < O; r++) {
                    if (M[r][j] == 1) covered[r] = 1;
                }
                
                // Aggiungo l'indice della colonna alla soluzione corrente
                current_sol[count] = j;

                // --- B. RECURSE (Scendo nel pozzo) ---
                solve_exact_cover(M, O, S, covered, count + 1, best_count, current_sol, best_sol);

                // --- C. UNDO (Backtracking - Annullo la mossa) ---
                // Importante: ripristino lo stato di 'covered' com'era prima.
                // Rimetto a 0 solo gli oggetti coperti da QUESTA colonna j.
                for (int r = 0; r < O; r++) {
                    if (M[r][j] == 1) covered[r] = 0;
                }
                // (Non serve pulire current_sol, verrà sovrascritta alla prossima iterazione)
            }
        }
    }
}

/*
 * ======================================================================================
 * FUNZIONE WRAPPER (Interfaccia pulita per il main)
 * ======================================================================================
 */
void trova_minima_copertura(int **M, int O, int S) {
    // Allocazione array ausiliari
    // calloc inizializza a 0 (molto comodo per l'array 'covered')
    int *covered = (int *)calloc(O, sizeof(int));
    int *current_sol = (int *)malloc(S * sizeof(int));
    int *best_sol = (int *)malloc(S * sizeof(int));

    int best_count = INF; // Inizializziamo con un valore altissimo

    // Chiamata alla funzione ricorsiva vera e propria
    solve_exact_cover(M, O, S, covered, 0, &best_count, current_sol, best_sol);

    // Stampa Risultati
    printf("\n--- RISULTATO ---\n");
    if (best_count == INF) {
        printf("Impossibile coprire l'insieme universo con sottoinsiemi disgiunti.\n");
    } else {
        printf("Cardinalita' minima trovata: %d insiemi.\n", best_count);
        printf("Indici delle colonne (insiemi) scelti: ");
        for (int i = 0; i < best_count; i++) {
            printf("%d ", best_sol[i]);
        }
        printf("\n");
    }

    // Pulizia
    free(covered);
    free(current_sol);
    free(best_sol);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Esempio Hardcoded
    // 3 Oggetti (Righe), 4 Insiemi (Colonne)
    // Cerchiamo di coprire gli oggetti 0, 1, 2 senza sovrapposizioni.
    
    int O = 3; 
    int S = 4;

    /*
       Matrice M:
          Set0 Set1 Set2 Set3
       O0  1    0    1    0
       O1  1    1    0    0
       O2  0    1    0    1
       
       Analisi a occhio:
       - Se scelgo Set0 (copre O0, O1), mi manca O2. Posso prendere Set3?
         Set3 copre O2. Set0 e Set3 sono disgiunti?
         Set0: {0,1}, Set3: {2}. Sì. Totale 2 set.
       
       - Se scelgo Set1 (copre O1, O2), mi manca O0. Posso prendere Set2?
         Set2 copre O0. Set1 e Set2 sono disgiunti?
         Set1: {1,2}, Set2: {0}. Sì. Totale 2 set.
         
       Entrambe le soluzioni sono valide e minime (cardinalità 2).
    */

    // Allocazione M
    int **M = (int **)malloc(O * sizeof(int *));
    for (int i = 0; i < O; i++) M[i] = (int *)calloc(S, sizeof(int));

    // Popolamento (Attenzione agli indici)
    // Colonna 0: {0, 1}
    M[0][0] = 1; M[1][0] = 1; 
    // Colonna 1: {1, 2}
    M[1][1] = 1; M[2][1] = 1;
    // Colonna 2: {0}
    M[0][2] = 1;
    // Colonna 3: {2}
    M[2][3] = 1;

    printf("Matrice di Adiacenza (Righe=Oggetti, Col=Insiemi):\n");
    for(int i=0; i<O; i++) {
        for(int j=0; j<S; j++) printf("%d ", M[i][j]);
        printf("\n");
    }

    // Avvio risoluzione
    trova_minima_copertura(M, O, S);

    // Free matrice
    for (int i = 0; i < O; i++) free(M[i]);
    free(M);

    return 0;
}