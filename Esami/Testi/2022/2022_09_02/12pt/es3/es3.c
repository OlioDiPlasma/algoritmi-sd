#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ============================================================================
 * FUNZIONE: check_validita
 * ============================================================================
 * SCOPO:
 * Verifica se un gruppo di persone candidato soddisfa il vincolo:
 * "Ogni persona nel gruppo deve essere amica di almeno altre k persone DEL GRUPPO".
 *
 * PARAMETRI:
 * - int *gruppo: Array contenente gli indici delle persone nel gruppo attuale.
 * - int size: Numero di persone nel gruppo.
 * - int **M: Matrice di adiacenza (relazioni di amicizia).
 * - int k: Numero minimo di amici richiesti all'interno del gruppo.
 *
 * RITORNA:
 * - 1 (true) se il gruppo è valido, 0 (false) altrimenti.
 * ============================================================================
 */
int check_validita(int *gruppo, int size, int **M, int k) {
    /* --- 1. CASO LIMITE --- */
    /* Se il gruppo ha meno di k+1 persone, è impossibile che ognuno abbia k amici. */
    if (size <= k) return 0;

    /* --- 2. VERIFICA PER OGNI MEMBRO --- */
    /* Iteriamo su ogni persona 'p1' presente nel gruppo candidato */
    for (int i = 0; i < size; i++) {
        int p1 = gruppo[i];
        int amici_nel_gruppo = 0;

        /* Contiamo quanti amici ha p1 TRA GLI ALTRI membri del gruppo */
        for (int j = 0; j < size; j++) {
            if (i == j) continue; /* Saltiamo noi stessi */

            int p2 = gruppo[j];
            
            /* * Accesso alla matrice M[p1][p2].
             * Se vale 1, c'è amicizia.
             */
            if (M[p1][p2] == 1) {
                amici_nel_gruppo++;
            }
        }

        /* Se anche solo una persona non raggiunge k amici, il gruppo è invalido */
        if (amici_nel_gruppo < k) return 0;
    }

    return 1; /* Tutti i controlli superati */
}

/* ============================================================================
 * FUNZIONE: solve_ricorsiva
 * ============================================================================
 * SCOPO:
 * Funzione ricorsiva (Backtracking) per generare tutti i sottoinsiemi di persone.
 *
 * PARAMETRI:
 * - int index: L'indice della persona che stiamo valutando (livello dell'albero).
 * - int *curr_sol: Array che memorizza il gruppo che stiamo costruendo.
 * - int curr_size: Numero di persone attualmente in curr_sol.
 * - int **M, int N, int k: Dati del problema (Matrice, Num persone, vincolo k).
 * - int *best_sol: Array dove copiamo la soluzione migliore trovata finora.
 * - int *best_size: Puntatore alla dimensione della miglior soluzione (input/output).
 *
 * CONCETTI CHIAVE:
 * - Passiamo 'best_size' come puntatore (int *) perché la funzione deve poter
 * aggiornare il record "globale" visibile a tutte le chiamate ricorsive.
 * ============================================================================
 */
void solve_ricorsiva(int index, int *curr_sol, int curr_size, 
                     int **M, int N, int k, 
                     int *best_sol, int *best_size) {

    /* --- 1. PRUNING (POTATURA) --- */
    /*
     * Se le persone che ho già preso (curr_size) + quelle che rimangono (N - index)
     * sono <= al record attuale (*best_size), è inutile continuare.
     * Non potrò mai fare meglio del record esistente.
     */
    if (curr_size + (N - index) <= *best_size) {
        return;
    }

    /* --- 2. BASE CASE (FOGLIA DELL'ALBERO) --- */
    if (index == N) {
        /* Abbiamo deciso per tutte le N persone (dentro o fuori). Verifichiamo il vincolo. */
        if (check_validita(curr_sol, curr_size, M, k)) {
            /* * Se valido e più grande del precedente best, aggiorniamo.
             * Nota: il controllo > *best_size è implicito grazie al pruning,
             * ma lo riesplicitiamo per chiarezza logica.
             */
            if (curr_size > *best_size) {
                *best_size = curr_size;
                /* Copiamo il contenuto del vettore corrente in quello best */
                for (int i = 0; i < curr_size; i++) {
                    best_sol[i] = curr_sol[i];
                }
            }
        }
        return;
    }

    /* --- 3. PASSO RICORSIVO: INCLUDO index --- */
    /* Aggiungo la persona 'index' al gruppo corrente */
    curr_sol[curr_size] = index;
    
    /* Ricorsione: vado al prossimo (index + 1) con dimensione aumentata */
    solve_ricorsiva(index + 1, curr_sol, curr_size + 1, M, N, k, best_sol, best_size);

    /* --- 4. PASSO RICORSIVO: ESCLUDO index --- */
    /* * Non c'è bisogno di "cancellare" esplicitamente curr_sol[curr_size],
     * verrà sovrascritto alla prossima iterazione utile.
     * Ricorsione: vado al prossimo (index + 1) mantenendo la stessa dimensione.
     */
    solve_ricorsiva(index + 1, curr_sol, curr_size, M, N, k, best_sol, best_size);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Creare la matrice dell'esempio, lanciare l'algoritmo e pulire la memoria.
 */
int main() {
    /* Dati dell'esempio: N=4, k=2 */
    int N = 4;
    int k = 2;

    /* --- 1. ALLOCAZIONE MATRICE --- */
    /* Allocazione dinamica di un array di puntatori (int**) */
    int **M = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        M[i] = (int *)malloc(N * sizeof(int));
    }

    /* Inizializzazione Hardcoded (come da immagine) */
    /* Riga 0: 1 1 0 1 */
    M[0][0]=1; M[0][1]=1; M[0][2]=0; M[0][3]=1;
    /* Riga 1: 1 1 0 1 */
    M[1][0]=1; M[1][1]=1; M[1][2]=0; M[1][3]=1;
    /* Riga 2: 0 0 1 0 */
    M[2][0]=0; M[2][1]=0; M[2][2]=1; M[2][3]=0;
    /* Riga 3: 1 1 0 1 */
    M[3][0]=1; M[3][1]=1; M[3][2]=0; M[3][3]=1;

    /* --- 2. PREPARAZIONE VARIABILI PER LA SOLUZIONE --- */
    /* Vettori di supporto allocati dinamicamente */
    int *curr_sol = (int *)malloc(N * sizeof(int));
    int *best_sol = (int *)malloc(N * sizeof(int));
    int best_size = 0; /* Inizialmente il miglior gruppo è vuoto */

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    /* * Passiamo &best_size (indirizzo) per permettere alla funzione 
     * di modificare l'intero 'best_size' dichiarato qui nel main.
     */
    printf("Cerco il gruppo piu' grande con k=%d...\n", k);
    solve_ricorsiva(0, curr_sol, 0, M, N, k, best_sol, &best_size);

    /* --- 4. STAMPA RISULTATI --- */
    printf("Dimensione massima trovata: %d\n", best_size);
    printf("Persone nel gruppo: { ");
    for (int i = 0; i < best_size; i++) {
        printf("p%d ", best_sol[i]);
    }
    printf("}\n");

    /* --- 5. GESTIONE MEMORIA (FREE) --- */
    /* * Regola d'oro: Per ogni malloc deve esistere una free.
     * Liberiamo prima le righe, poi il puntatore alle righe.
     */
    for (int i = 0; i < N; i++) free(M[i]);
    free(M);
    
    free(curr_sol);
    free(best_sol);

    return 0;
}