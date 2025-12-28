#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Serve per INT_MAX

/* =================================================================================
 * FUNZIONE RICORSIVA: solve_r
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Esplora tutte le combinazioni possibili di monete per raggiungere il 'resto' target.
 * Cerca di minimizzare il numero totale di monete usate.
 *
 * PARAMETRI:
 * - int pos: Indice del tipo di moneta che stiamo considerando nel vettore 'val'.
 * - int n: Numero totale di tipi di monete.
 * - int *val: Vettore valori monete.
 * - int *disp: Vettore disponibilità monete.
 * - int resto: Quanto manca per raggiungere l'obiettivo.
 * - int count: Quante monete abbiamo usato finora in questo ramo di ricorsione.
 * - int *curr_sol: Vettore temporaneo che conta quante monete di ogni tipo stiamo usando.
 * - int *best_sol: Vettore dove salviamo la soluzione migliore trovata finora.
 * - int *min_coins: Puntatore all'intero che memorizza il record attuale (minimo monete).
 *
 * CONCETTI CHIAVE:
 * 1. Modello Combinatorio: Per ogni tipo di moneta all'indice 'pos', possiamo
 * prenderne 0, 1, 2... fino al massimo disponibile (disp[pos]).
 * 2. Pruning (Potatura): Se in un qualsiasi momento il numero di monete che stiamo
 * usando (count) supera o eguaglia il record migliore già trovato (*min_coins),
 * ci fermiamo. Non ha senso continuare su una strada che è già peggiore della migliore.
 * =================================================================================
 */
void solve_r(int pos, int n, int *val, int *disp, int resto, int count, 
             int *curr_sol, int *best_sol, int *min_coins) {

    /* --- 1. PRUNING (Ottimizzazione) --- */
    // Se stiamo già usando più monete della soluzione migliore trovata in precedenza,
    // o se abbiamo superato il resto (resto < 0), tagliamo questo ramo.
    if (count >= *min_coins || resto < 0) {
        return;
    }

    /* --- 2. CASO BASE (Successo) --- */
    // Se il resto è esattamente 0, abbiamo trovato una combinazione valida.
    // Grazie al pruning sopra, sappiamo già che 'count' è < *min_coins.
    if (resto == 0) {
        *min_coins = count; // Aggiorniamo il nuovo record
        // Copiamo la soluzione attuale in quella "best" per ricordarcela
        for (int i = 0; i < n; i++) {
            best_sol[i] = curr_sol[i];
        }
        return;
    }

    /* --- 3. CASO BASE (Fine monete) --- */
    // Se abbiamo finito i tipi di monete e il resto non è 0, questo ramo è fallito.
    if (pos == n) {
        return;
    }

    /* --- 4. LOGICA RICORSIVA (Iterazione sulle quantità) --- */
    
    // Per il tipo di moneta corrente (val[pos]), proviamo a prenderne
    // da 0 fino al massimo disponibile (disp[pos]).
    // Inoltre, non ne prendiamo più di quante ne servano per coprire il resto (i * val[pos] <= resto).
    
    // NOTA: Spesso conviene provare dal massimo possibile verso 0 per trovare prima
    // soluzioni che riempiono il resto velocemente, ma qui iteriamo standard.
    for (int i = 0; i <= disp[pos]; i++) {
        
        int valore_totale_monete = i * val[pos];
        
        // Se troppe monete superano il resto, inutile continuare il ciclo for
        if (valore_totale_monete > resto) break;

        // DO MOVE (Prendo 'i' monete di tipo 'pos')
        curr_sol[pos] = i;

        // RICORSIONE
        // Passiamo al prossimo tipo di moneta (pos + 1)
        // Il resto diminuisce, il conteggio monete aumenta di 'i'
        solve_r(pos + 1, n, val, disp, 
                resto - valore_totale_monete, 
                count + i, 
                curr_sol, best_sol, min_coins);
        
        // BACKTRACKING (Undo move)
        // Non serve codice esplicito qui perché 'curr_sol[pos]' verrà sovrascritta
        // alla prossima iterazione del ciclo for (quando i diventa i+1)
        // o resettata uscendo dalla funzione.
        curr_sol[pos] = 0; 
    }
}

/* =================================================================================
 * FUNZIONE WRAPPER: calcola_resto
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Prepara la memoria e le variabili per la ricorsione.
 * =================================================================================
 */
void calcola_resto(int r, int n, int *val, int *disp) {
    
    /* --- 1. ALLOCAZIONE VETTORI SOLUZIONE --- */
    // curr_sol: terrà traccia di quante monete usiamo durante i tentativi
    // best_sol: memorizzerà la combinazione vincente
    // Usiamo calloc per inizializzare tutto a 0.
    int *curr_sol = (int *)calloc(n, sizeof(int));
    int *best_sol = (int *)calloc(n, sizeof(int));

    if (curr_sol == NULL || best_sol == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    // Inizializziamo il minimo con un valore altissimo (infinito)
    int min_coins = INT_MAX;

    /* --- 2. CHIAMATA AL CORE RICORSIVO --- */
    // Partiamo dalla moneta indice 0, con resto r e 0 monete usate.
    solve_r(0, n, val, disp, r, 0, curr_sol, best_sol, &min_coins);

    /* --- 3. STAMPA RISULTATI --- */
    if (min_coins == INT_MAX) {
        printf("Non e' possibile erogare il resto di %d con le monete disponibili.\n", r);
    } else {
        printf("Soluzione Ottima trovata (%d monete totali):\n", min_coins);
        for (int i = 0; i < n; i++) {
            if (best_sol[i] > 0) {
                printf("- %d monete da valore %d\n", best_sol[i], val[i]);
            }
        }
    }

    /* --- 4. PULIZIA MEMORIA --- */
    free(curr_sol);
    free(best_sol);
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Eseguire l'esempio esatto della traccia.
 * n=3, val={1, 10, 25}, disp={10, 3, 2}, r=30
 * =================================================================================
 */
int main() {
    printf("--- ESERCIZIO SISTEMA MONETAZIONE (NO GREEDY) ---\n\n");

    /* DATI DELL'ESEMPIO */
    int n = 3;
    int r = 30;

    // Definiamo i vettori staticamente per semplicità nel test,
    // ma in un programma reale potrebbero essere allocati dinamicamente.
    int val[] = {1, 10, 25};
    int disp[] = {10, 3, 2};

    printf("Target Resto: %d\n", r);
    printf("Monete disponibili:\n");
    for(int i=0; i<n; i++) {
        printf(" Valore %d (Disp: %d)\n", val[i], disp[i]);
    }
    printf("\nCalcolo...\n");

    // Invocazione Wrapper
    calcola_resto(r, n, val, disp);

    return 0;
}


/* 
//////
1. Perché non funziona il Greedy?
Nell'esempio $r=30$ con monete 25, 10, 1.L'algoritmo Greedy ordinerebbe le monete: 25, 10, 
1.Prendo 25 (Resto 5).10 non ci sta.
Prendo cinque monete da 
1.Risultato Greedy: 6 monete (1x25 + 5x1).
L'algoritmo ricorsivo (Backtracking) invece esplora tutte le combinazioni. 
Quando proverà a prendere zero monete da 25, scoprirà che può prendere tre monete da 10.Risultato Ottimo: 3 monete (3x10). $3 < 6$, quindi vince.
/////
 */