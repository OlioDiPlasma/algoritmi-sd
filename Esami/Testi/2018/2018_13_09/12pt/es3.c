#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * STRUTTURA DATI
 * ============================================================================
 * Come richiesto dal testo, definiamo la struct per i pasticcini.
 */
typedef struct {
    int codice;   // Identificativo (es. 101)
    int peso;     // Peso in grammi del singolo pasticcino
    int quantita; // Disponibilità massima in magazzino
} Pasticcino;

/* ============================================================================
 * PROTOTIPI
 * ============================================================================
 */
void componiVassoio(Pasticcino *paste, int n, int peso_target);
void rec_search(Pasticcino *paste, int n, int peso_target, int idx,
                int current_weight, int *current_sol,
                int *best_weight, int *best_sol);

/* ============================================================================
 * FUNZIONE: rec_search (Il Motore Ricorsivo)
 * ============================================================================
 * SCOPO:
 * Esplorare ricorsivamente le combinazioni di quantità per ogni tipo di pasticcino
 * per trovare quella che massimizza il peso totale senza superare il target.
 *
 * PARAMETRI:
 * - paste: Array delle struct (i dati del problema).
 * - n: Numero di tipologie di pasticcini.
 * - peso_target: Il peso massimo che il vassoio può sostenere.
 * - idx: L'indice del tipo di pasticcino che stiamo decidendo ORA (profondità ricorsione).
 * - current_weight: Il peso accumulato fino a questo punto della ricorsione.
 * - current_sol: Array temporaneo che dice "quanti pasticcini del tipo i ho preso".
 * - best_weight: Puntatore al miglior peso trovato finora (condiviso tra le chiamate).
 * - best_sol: Array che memorizza la composizione del vassoio migliore trovata.
 *
 * CONCETTI CHIAVE:
 * 1. Pruning (Potatura): Se superiamo il peso target, ci fermiamo.
 * 2. Aggiornamento dell'Ottimo: Ogni volta che troviamo una combinazione valida
 * con un peso maggiore del precedente record, salviamo tutto.
 * 3. Backtracking: Proviamo a prendere 0 pasticcini, poi 1, poi 2... quando
 * la ricorsione torna indietro, dobbiamo "pulire" o sovrascrivere le scelte fatte.
 * ============================================================================
 */
void rec_search(Pasticcino *paste, int n, int peso_target, int idx,
                int current_weight, int *current_sol,
                int *best_weight, int *best_sol) {

    /* --- 1. CONTROLLO MIGLIORAMENTO --- */
    /*
     * Prima di tutto: la soluzione attuale è migliore del record precedente?
     * Nota: controlliamo <= peso_target per sicurezza, anche se i cicli sotto
     * dovrebbero garantirlo.
     */
    if (current_weight <= peso_target && current_weight > *best_weight) {
        /*
         * Abbiamo trovato un nuovo "campione"!
         * Aggiorniamo il peso massimo trovato.
         */
        *best_weight = current_weight;

        /*
         * Copiamo l'array della soluzione corrente in quello della soluzione migliore.
         * Dobbiamo salvare "la fotografia" di quanti pasticcini abbiamo preso per ogni tipo.
         */
        for (int i = 0; i < n; i++) {
            best_sol[i] = current_sol[i];
        }
    }

    /* --- 2. CASO BASE --- */
    /*
     * Se abbiamo considerato tutte le tipologie di pasticcini (idx == n)
     * o se abbiamo raggiunto esattamente il peso target (ottimizzazione), ci fermiamo.
     */
    if (idx == n || *best_weight == peso_target) {
        return;
    }

    /* --- 3. CICLO DI SCELTA (Core del Backtracking) --- */
    /*
     * Per il pasticcino corrente (paste[idx]), quante unità ne prendiamo?
     * Possiamo prenderne da 0 fino alla sua disponibilità massima (paste[idx].quantita).
     */
    for (int q = 0; q <= paste[idx].quantita; q++) {

        /* Calcoliamo quanto peserebbero 'q' pasticcini di questo tipo */
        int peso_aggiunto = q * paste[idx].peso;

        /*
         * --- VINCOLO DI VALIDITÀ ---
         * Possiamo aggiungere questi pasticcini senza sfondare il vassoio?
         */
        if (current_weight + peso_aggiunto <= peso_target) {

            /* AZIONE: Segniamo che ne prendiamo 'q' */
            current_sol[idx] = q;

            /* RICORSIONE: Passiamo al prossimo tipo di pasticcino (idx + 1) */
            rec_search(paste, n, peso_target, idx + 1,
                       current_weight + peso_aggiunto, /* Nuovo peso accumulato */
                       current_sol,
                       best_weight, best_sol);

            /*
             * BACKTRACKING:
             * Non c'è bisogno di una istruzione esplicita qui (es. current_sol[idx] = 0)
             * perché al prossimo giro del ciclo for, sovrascriveremo current_sol[idx]
             * con il nuovo valore di 'q'.
             */

        } else {
            /*
             * OTTIMIZZAZIONE (Break):
             * Se con 'q' pasticcini supero il peso, sicuramente lo supererò anche
             * con 'q+1', 'q+2' ecc. Inutile continuare il ciclo su questo tipo.
             */
            break;
        }
    }
    /* --- CORREZIONE FONDAMENTALE QUI --- */
    /* * Prima di uscire dalla funzione e tornare al livello superiore (idx-1),
     * dobbiamo PULIRE la cella corrente. Altrimenti, se un'altra soluzione 
     * viene trovata in un ramo diverso, si ritroverà questo valore "sporco".
     */
    current_sol[idx] = 0;
}

/* ============================================================================
 * FUNZIONE: componiVassoio (Wrapper)
 * ============================================================================
 * SCOPO:
 * Preparare le strutture dati necessarie e lanciare la ricorsione.
 * Si occupa di allocare la memoria per le soluzioni e stampare il risultato finale.
 *
 * GESTIONE MEMORIA:
 * Utilizziamo `calloc` per creare gli array `curr_sol` e `best_sol`.
 * È fondamentale perché dobbiamo conservare il conteggio per N tipologie,
 * che non conosciamo a priori (è dinamico).
 * ============================================================================
 */
void componiVassoio(Pasticcino *paste, int n, int peso_target) {

    /* --- 1. ALLOCAZIONE MEMORIA --- */
    /*
     * Array per tenere traccia delle scelte durante la ricorsione.
     * calloc inizializza a 0 (nessun pasticcino preso all'inizio).
     */
    int *curr_sol = (int *)calloc(n, sizeof(int));
    int *best_sol = (int *)calloc(n, sizeof(int));

    if (curr_sol == NULL || best_sol == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    int best_weight = 0; // Inizialmente il miglior peso è 0

    /* --- 2. CHIAMATA ALLA LOGICA --- */
    printf("Cerco combinazione per target %d grammi...\n", peso_target);
    rec_search(paste, n, peso_target, 0, 0, curr_sol, &best_weight, best_sol);

    /* --- 3. STAMPA RISULTATI --- */
    printf("\n=== COMPOSIZIONE VASSOIO OTTIMALE ===\n");
    printf("Peso Totale Raggiunto: %d g (su %d g richiesti)\n", best_weight, peso_target);
    printf("Dettaglio:\n");

    int totale_pezzi = 0;
    for (int i = 0; i < n; i++) {
        if (best_sol[i] > 0) {
            printf("- Tipologia %d (Peso %dg): %d pezzi\n",
                   paste[i].codice, paste[i].peso, best_sol[i]);
            totale_pezzi += best_sol[i];
        }
    }
    if (totale_pezzi == 0) {
        printf("Nessun pasticcino selezionato (tutti troppo pesanti?).\n");
    }

    /* --- 4. LIBERAZIONE MEMORIA --- */
    /* Mai dimenticare le free per gli array allocati nel wrapper */
    free(curr_sol);
    free(best_sol);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Creare un inventario fittizio e testare l'algoritmo.
 * ============================================================================
 */
int main() {
    /* Creiamo un inventario di 4 tipologie */
    int n = 4;
    Pasticcino inventario[] = {
        {101, 50,  5},  // Cod 101, 50g l'uno, 5 disponibili (es. Bignè)
        {102, 30, 10},  // Cod 102, 30g l'uno, 10 disponibili (es. Cannoncini)
        {103, 80,  2},  // Cod 103, 80g l'uno, 2 disponibili (es. Babà)
        {104, 15, 20}   // Cod 104, 15g l'uno, 20 disponibili (es. Amaretti)
    };

    /*
     * Caso 1: Vassoio piccolo (Target 100g)
     * Ci aspettiamo magari due bignè (100g) oppure tre cannoncini e un amaretto (105 no, 90+10? no)
     */
    componiVassoio(inventario, n, 100);

    printf("\n-----------------------------------\n\n");

    /*
     * Caso 2: Vassoio medio (Target 295g)
     * Algoritmo dovrà mixare bene per avvicinarsi.
     */
    componiVassoio(inventario, n, 295);

    return 0;
}