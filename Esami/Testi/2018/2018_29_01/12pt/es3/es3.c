#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * STRUTTURE DATI
 * ======================================================================================
 * Definizione della struttura 'interv' come richiesto dal testo.
 * Rappresenta un intervallo [l, h) con estremo inferiore incluso e superiore escluso.
 */
typedef struct {
    int l; // Low: estremo inferiore
    int h; // High: estremo superiore
} interv;

/*
 * ======================================================================================
 * FUNZIONE: check_compatibility
 * ======================================================================================
 * SCOPO:
 * Verifica se due intervalli sono compatibili (ovvero NON si intersecano).
 *
 * PARAMETRI:
 * - interv a: Il primo intervallo.
 * - interv b: Il secondo intervallo.
 *
 * RITORNA:
 * - 1 (vero) se sono compatibili.
 * - 0 (falso) se si sovrappongono.
 *
 * CONCETTI CHIAVE:
 * - Logica di Intersezione: Il testo definisce l'intersezione come (li < hj) && (lj < hi).
 * Due intervalli sono compatibili se questa condizione è FALSA.
 * Esempio: (1,2) e (2,5). 
 * (1 < 5) VERO && (2 < 2) FALSO -> Risultato FALSO (non intersecano) -> Compatibili.
 * ======================================================================================
 */
int check_compatibility(interv a, interv b) {
    /* La condizione di sovrapposizione è: a inizia prima che b finisca E b inizia prima che a finisca */
    if ((a.l < b.h) && (b.l < a.h)) {
        return 0; // Si sovrappongono, non compatibili
    }
    return 1; // Compatibili
}

/*
 * ======================================================================================
 * FUNZIONE: is_compatible_with_solution
 * ======================================================================================
 * SCOPO:
 * Controlla se l'intervallo candidato 'val' è compatibile con TUTTI gli intervalli
 * già inseriti nella soluzione parziale corrente.
 *
 * PARAMETRI:
 * - interv *v: Il vettore completo degli intervalli originali.
 * - int *sol: Vettore di indici degli intervalli attualmente selezionati.
 * - int count: Numero di intervalli attualmente nella soluzione parziale.
 * - interv val: L'intervallo candidato da verificare.
 *
 * DETTAGLIO SINTATTICO:
 * - Accesso indiretto: v[sol[i]] accede all'intervallo originale usando l'indice salvato in sol.
 * ======================================================================================
 */
int is_compatible_with_solution(interv *v, int *sol, int count, interv val) {
    for (int i = 0; i < count; i++) {
        /* Recuperiamo l'intervallo già scelto all'indice i */
        interv current = v[sol[i]];
        if (!check_compatibility(current, val)) {
            return 0; // Trovata una collisione
        }
    }
    return 1; // Nessuna collisione trovata
}

/*
 * ======================================================================================
 * FUNZIONE: solve_rec (Funzione Ricorsiva)
 * ======================================================================================
 * SCOPO:
 * Esplora lo spazio delle soluzioni (Power Set) utilizzando il modello del "Pick or Not Pick"
 * (Prendo o non Prendo).
 *
 * PARAMETRI:
 * - int pos: L'indice dell'intervallo corrente che stiamo valutando nel vettore v.
 * - int N: Dimensione totale del problema.
 * - interv *v: Puntatore al vettore dei dati.
 * - int *sol: Vettore temporaneo che contiene gli indici della soluzione corrente.
 * - int count: Quanti elementi abbiamo messo in 'sol' finora.
 * - int curr_dur: La somma delle durate degli intervalli in 'sol'.
 * - int *best_sol: Puntatore al vettore dove copiamo la soluzione migliore trovata.
 * - int *best_count: Puntatore alla lunghezza della soluzione migliore.
 * - int *max_dur: Puntatore al valore massimo di durata trovato finora.
 *
 * CONCETTI CHIAVE:
 * - Backtracking: Proviamo ad aggiungere un elemento. Se valido, andiamo avanti (ricorsione).
 * Quando la ricorsione torna, "togliamo" l'elemento (backtrack) per provare altre strade.
 * - Pruning (Potatura): Se un intervallo non è compatibile con quelli scelti prima,
 * non esploriamo nemmeno il ramo in cui lo includiamo.
 * ======================================================================================
 */
void solve_rec(int pos, int N, interv *v, int *sol, int count, int curr_dur, 
               int *best_sol, int *best_count, int *max_dur) {
    
    /* --- 1. CASO BASE --- */
    /* Abbiamo valutato tutti gli N intervalli (siamo arrivati in fondo all'albero di ricorsione) */
    if (pos >= N) {
        /* Se la durata corrente è migliore del massimo globale registrato */
        if (curr_dur > *max_dur) {
            *max_dur = curr_dur;       // Aggiorniamo il record
            *best_count = count;       // Salviamo quanti elementi ci sono
            
            /* Copiamo la soluzione corrente in quella "best" per non perderla */
            for (int i = 0; i < count; i++) {
                best_sol[i] = sol[i];
            }
        }
        return;
    }

    /* --- 2. PASSO RICORSIVO (OPZIONE A: ESCLUDI) --- */
    /* Proviamo a NON includere l'intervallo v[pos] nella soluzione.
       Passiamo semplicemente al prossimo (pos + 1) senza cambiare sol o curr_dur. */
    solve_rec(pos + 1, N, v, sol, count, curr_dur, best_sol, best_count, max_dur);

    /* --- 3. PASSO RICORSIVO (OPZIONE B: INCLUDI) --- */
    /* Proviamo a includere v[pos], ma SOLO se è compatibile con quelli già presi. */
    if (is_compatible_with_solution(v, sol, count, v[pos])) {
        
        /* Aggiungiamo l'indice corrente alla soluzione temporanea */
        sol[count] = pos;
        
        /* Calcoliamo la durata del singolo intervallo (h - l) */
        int durata_singola = v[pos].h - v[pos].l;

        /* Chiamata ricorsiva con stato aggiornato:
           - pos + 1: andiamo al prossimo
           - count + 1: abbiamo un elemento in più
           - curr_dur + durata_singola: aggiorniamo la somma */
        solve_rec(pos + 1, N, v, sol, count + 1, curr_dur + durata_singola, 
                  best_sol, best_count, max_dur);
        
        /* BACKTRACKING IMPLICITO:
           Non serve "cancellare" sol[count], perché al prossimo giro quel valore
           verrà semplicemente sovrascritto. Tornando da questa chiamata, è come se
           avessimo rimosso l'elemento. */
    }
}

/*
 * ======================================================================================
 * FUNZIONE: intervSel (Wrapper)
 * ======================================================================================
 * SCOPO:
 * Funzione interfaccia richiesta dal testo. Prepara le strutture dati ausiliarie
 * e lancia la ricorsione.
 *
 * PARAMETRI:
 * - int N: Numero di intervalli.
 * - interv *v: Vettore degli intervalli.
 *
 * GESTIONE MEMORIA:
 * Allochiamo dinamicamente i vettori per la soluzione temporanea e quella ottima.
 * È fondamentale liberarli (free) prima di uscire.
 * ======================================================================================
 */
void intervSel(int N, interv *v) {
    /* --- 1. ALLOCAZIONE --- */
    /* sol: conterrà gli indici durante l'esplorazione.
       best_sol: conterrà gli indici della configurazione vincente.
       Dimensione N perché nel caso peggiore li prendiamo tutti. */
    int *sol = (int *)malloc(N * sizeof(int));
    int *best_sol = (int *)malloc(N * sizeof(int));

    if (sol == NULL || best_sol == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* Variabili per tracciare il risultato ottimo */
    int max_duration = 0;
    int best_count = 0;

    /* --- 2. LOGICA (Lancio Ricorsione) --- */
    /* Parametri iniziali: pos=0, count=0, duration=0 */
    solve_rec(0, N, v, sol, 0, 0, best_sol, &best_count, &max_duration);

    /* --- 3. VISUALIZZAZIONE RISULTATI --- */
    printf("Durata massima totale: %d\n", max_duration);
    printf("Intervalli selezionati:\n");
    for (int i = 0; i < best_count; i++) {
        /* best_sol contiene gli indici. Usiamoli per accedere a v. */
        int idx = best_sol[i];
        printf("(%d, %d) Durata: %d\n", v[idx].l, v[idx].h, v[idx].h - v[idx].l);
    }

    /* --- 4. LIBERAZIONE MEMORIA --- */
    free(sol);
    free(best_sol);
}

/*
 * ======================================================================================
 * FUNZIONE: main
 * ======================================================================================
 * SCOPO:
 * Main di test con dati hardcoded presi dall'esempio.
 * ======================================================================================
 */
int main() {
    printf("--- INIZIO TEST INTERVAL SCHEDULING ---\n");

    int N = 6;
    
    /* --- 1. ALLOCAZIONE DATI TEST --- */
    /* Uso malloc per simulare un vettore dinamico come avverrebbe in un caso reale */
    interv *v = (interv *)malloc(N * sizeof(interv));
    if (v == NULL) return 1;

    /* Popoliamo il vettore con i dati dell'esempio:
       (1,2), (2,4), (2,5), (3,5), (5,7), (6,8) */
    v[0].l = 1; v[0].h = 2;
    v[1].l = 2; v[1].h = 4;
    v[2].l = 2; v[2].h = 5;
    v[3].l = 3; v[3].h = 5;
    v[4].l = 5; v[4].h = 7;
    v[5].l = 6; v[5].h = 8;

    printf("Set di intervalli in input:\n");
    for(int i=0; i<N; i++) printf("(%d, %d) ", v[i].l, v[i].h);
    printf("\n\n");

    /* --- 2. CHIAMATA WRAPPER --- */
    intervSel(N, v);

    /* --- 3. FREE --- */
    free(v);

    printf("\n--- FINE TEST ---\n");
    return 0;
}