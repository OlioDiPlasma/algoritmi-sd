#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Per INT_MAX
#include "parts.h"

/* =========================================================================
 * ANALISI DEL MODELLO COMBINATORIO
 * =========================================================================
 * Il problema è una variante del "Set Cover" o "Exact Cover", ma con un vincolo
 * forte: ogni parte ha una posizione fissa (P[i].pos).
 *
 * Modello:
 * Immaginiamo la stringa 'target' come un percorso lineare da indice 0 a N.
 * Lo stato della ricorsione è definito dall'indice corrente 'pos_target' nel target.
 *
 * Transizioni:
 * Da 'pos_target', possiamo saltare a 'pos_target + lunghezza_parte' SE E SOLO SE:
 * 1. Esiste una parte P[i] tale che P[i].pos == pos_target.
 * 2. La stringa P[i].s coincide esattamente con i caratteri del target in quel punto.
 *
 * Giustificazione Criteri di Pruning:
 * Trattandosi di un problema di MINIMIZZAZIONE del costo:
 * 1. Pruning sul Costo: Se il costo attuale supera il 'best_cost' già trovato,
 * interrompiamo quel ramo (non potrà mai essere la soluzione ottima).
 * 2. Non usiamo pruning sulla lunghezza o conteggio parti durante la discesa,
 * perché il criterio secondario (massimizzare le parti) si valuta solo
 * a parità di costo minimo.
 * ========================================================================= */

/* =========================================================================
 * FUNZIONE RICORSIVA (CORE)
 * =========================================================================
 * SCOPO:
 * Esplora lo spazio delle soluzioni cercando di coprire il target carattere per carattere.
 *
 * PARAMETRI:
 * - target: Stringa obiettivo.
 * - len_target: Lunghezza pre-calcolata per evitare chiamate continue a strlen.
 * - P, nParts: Vettore delle parti.
 * - curr_idx: Indice attuale nel target che stiamo cercando di coprire.
 * - curr_sol: Vettore di indici delle parti usate finora.
 * - curr_count: Quante parti abbiamo usato finora.
 * - curr_cost: Costo accumulato finora.
 * - best_sol: Puntatore al vettore dove salveremo la soluzione migliore.
 * - best_count: Puntatore al numero di parti della soluzione migliore.
 * - min_cost: Puntatore al costo minimo globale trovato finora.
 */
void solve_rec(char *target, int len_target, part *P, int nParts,
               int curr_idx, int *curr_sol, int curr_count, int curr_cost,
               int *best_sol, int *best_count, int *min_cost) {

    /* --- 1. PRUNING (POTATURA) --- */
    /* Se abbiamo già superato il costo della soluzione migliore trovata finora,
       è inutile continuare. Nota: usiamo >= perché a parità di costo vogliamo
       controllare il numero di parti, ma se è STRETTAMENTE maggiore ci fermiamo. */
    if (curr_cost > *min_cost) {
        return;
    }

    /* --- 2. BASE CASE (SUCCESSO) --- */
    /* Siamo arrivati esattamente alla fine del target? */
    if (curr_idx == len_target) {
        /*
         * Abbiamo trovato una soluzione valida completa.
         * Valutiamo se è migliore dell'ottimo attuale.
         * Criterio 1: Costo minore.
         * Criterio 2: A parità di costo, numero di parti maggiore.
         */
        if (curr_cost < *min_cost || (curr_cost == *min_cost && curr_count > *best_count)) {
            /* Aggiorniamo l'ottimo globale */
            *min_cost = curr_cost;
            *best_count = curr_count;

            /* Salviamo la sequenza di parti.
             * memcpy è più efficiente di un for loop per copiare blocchi di memoria.
             * Copiamo 'curr_count' interi da curr_sol a best_sol. */
            memcpy(best_sol, curr_sol, curr_count * sizeof(int));
        }
        return;
    }

    /* --- 3. CICLO DI ESPLORAZIONE (RICORSIONE) --- */
    /* Scorriamo tutte le parti disponibili per trovare quelle compatibili
       con la posizione corrente. */
    for (int i = 0; i < nParts; i++) {

        /*
         * FILTRO VALIDITÀ (Vincoli del problema):
         * 1. La parte DEVE iniziare esattamente dove siamo ora (P[i].pos == curr_idx).
         * 2. La parte non deve sbordare oltre la fine del target.
         */
        if (P[i].pos == curr_idx) {
            int len_part = strlen(P[i].s);

            /* Controllo bounds: non andiamo oltre la fine del target */
            if (curr_idx + len_part <= len_target) {

                /*
                 * CONFRONTO STRINGHE (Logic):
                 * Verifichiamo se la sottostringa del target a partire da curr_idx
                 * coincide con la stringa della parte P[i].
                 *
                 * Sintassi: target + curr_idx è aritmetica dei puntatori.
                 * Sposta il puntatore all'inizio della sottostringa.
                 * strncmp restituisce 0 se i primi n caratteri sono uguali.
                 */
                if (strncmp(target + curr_idx, P[i].s, len_part) == 0) {

                    /* --- PASSO AVANTI (DO) --- */
                    curr_sol[curr_count] = i; // Aggiungo l'indice della parte alla soluzione

                    /* --- RICORSIONE --- */
                    solve_rec(target, len_target, P, nParts,
                              curr_idx + len_part, // Nuovo indice target (saltiamo la parola usata)
                              curr_sol,
                              curr_count + 1,      // Una parte in più
                              curr_cost + P[i].cost, // Aggiorno costo
                              best_sol, best_count, min_cost);

                    /* --- BACKTRACKING (UNDO) --- */
                    /* Non c'è bisogno di cancellare esplicitamente curr_sol[curr_count],
                       verrà sovrascritta alla prossima iterazione valida. */
                }
            }
        }
    }
}

/* =========================================================================
 * FUNZIONE WRAPPER
 * =========================================================================
 */
void solve(char *target, part *P, int nParts) {
    int len_target = strlen(target);
    int min_cost = INT_MAX; // Inizializziamo a "infinito"
    int best_count = 0;

    /*
     * GESTIONE MEMORIA DINAMICA:
     * Dobbiamo allocare vettori per tenere traccia delle soluzioni.
     * Dimensione massima? Nel caso peggiore, ogni parte è lunga 1 carattere,
     * quindi avremo al massimo 'len_target' parti.
     */
    int *curr_sol = (int *)malloc(len_target * sizeof(int));
    int *best_sol = (int *)malloc(len_target * sizeof(int));

    /* Controllo allocazione paranoico */
    if (curr_sol == NULL || best_sol == NULL) {
        printf("Errore malloc wrapper.\n");
        exit(-1);
    }

    printf("Cerco soluzione per target: \"%s\"...\n", target);

    /* Chiamata alla funzione ricorsiva */
    solve_rec(target, len_target, P, nParts, 0, curr_sol, 0, 0, best_sol, &best_count, &min_cost);

    /* --- STAMPA RISULTATI --- */
    if (min_cost != INT_MAX) {
        printf("Soluzione Ottima Trovata:\n");
        printf("- Costo Totale: %d\n", min_cost);
        printf("- Numero Parti: %d\n", best_count);
        printf("- Composizione: ");
        for (int i = 0; i < best_count; i++) {
            /* best_sol contiene gli indici del vettore P originale */
            int idx = best_sol[i];
            printf("\"%s\" (pos %d, costo %d) ", P[idx].s, P[idx].pos, P[idx].cost);
        }
        printf("\n");
    } else {
        printf("Nessuna soluzione possibile trovata.\n");
    }

    /* --- FREE DELLA MEMORIA --- */
    free(curr_sol);
    free(best_sol);
}