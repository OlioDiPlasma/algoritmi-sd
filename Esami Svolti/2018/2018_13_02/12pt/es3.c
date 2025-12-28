#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Per INT_MAX

/* ============================================================================
 * STRUTTURA DATI: Molo
 * ============================================================================
 * Concetto: Rappresentiamo ogni molo come un oggetto con due contatori.
 * - occ_sx: Quanto spazio è occupato sul lato sinistro.
 * - occ_dx: Quanto spazio è occupato sul lato destro.
 * Inizialmente entrambi sono a 0.
 * ============================================================================
 */
typedef struct {
    int occ_sx; // Occupazione Sinistra
    int occ_dx; // Occupazione Destra
} Molo;

/* ============================================================================
 * PROTOTIPI
 * ============================================================================
 */
void calcolaMinimoMoli(int *navi, int k, int lun_molo, int n_moli);
void rec_solve(int ship_idx, int k, int *navi, Molo *moli, int n_moli, int lun_molo, int moli_usati_attuali, int *best_moli);

/* ============================================================================
 * FUNZIONE: rec_solve (Core Ricorsivo)
 * ============================================================================
 * SCOPO:
 * Funzione di backtracking che tenta di posizionare la nave corrente (ship_idx)
 * in uno dei moli disponibili (a sinistra o a destra), cercando di minimizzare
 * il numero totale di moli aperti.
 *
 * PARAMETRI:
 * - ship_idx: Indice della nave che stiamo cercando di parcheggiare ora (0..k-1).
 * - k: Numero totale di navi.
 * - navi: Array contenente le dimensioni delle navi.
 * - moli: Array di struct Molo (lo stato attuale del porto).
 * - n_moli: Numero massimo di moli fisici disponibili nel porto.
 * - lun_molo: Lunghezza massima di ogni lato del molo.
 * - moli_usati_attuali: Quanti moli abbiamo "sporcato" (usato) finora in questo ramo.
 * - best_moli: Puntatore a un intero che conserva la miglior soluzione trovata finora
 * (il minimo numero di moli).
 *
 * CONCETTI CHIAVE:
 * 1. Backtracking: Proviamo una strada (es. nave 1 sul molo 0 sx), se porta a una
 * soluzione valida la valutiamo, poi torniamo indietro (backtrack) per provare
 * altre strade (es. nave 1 sul molo 0 dx, o sul molo 1).
 * 2. Pruning (Potatura): Se in un certo momento stiamo usando più moli della
 * migliore soluzione già trovata (*best_moli), smettiamo di scendere in profondità.
 * È inutile continuare, non troveremo un minimo migliore.
 * ============================================================================
 */
void rec_solve(int ship_idx, int k, int *navi, Molo *moli, int n_moli, int lun_molo, int moli_usati_attuali, int *best_moli) {

    /* --- 1. CONDIZIONE DI TERMINAZIONE (BASE CASE) --- */
    /*
     * Se ship_idx == k, significa che abbiamo piazzato con successo tutte le navi
     * (dalla 0 alla k-1). Abbiamo trovato una soluzione valida.
     */
    if (ship_idx == k) {
        /* Controlliamo se questa soluzione è migliore (usa meno moli) di quella attuale */
        if (moli_usati_attuali < *best_moli) {
            /*
             * DETTAGLIO SINTATTICO:
             * *best_moli = ...
             * Dereferenziamo il puntatore per sovrascrivere il valore nella variabile
             * 'min_result' dichiarata nella funzione chiamante.
             */
            *best_moli = moli_usati_attuali;
        }
        return;
    }

    /* --- 2. PRUNING (OTTIMIZZAZIONE) --- */
    /*
     * Se stiamo già usando un numero di moli >= al record attuale,
     * non ha senso continuare. Vogliamo MINIMIZZARE.
     */
    if (moli_usati_attuali >= *best_moli) {
        return;
    }

    /* --- 3. CICLO SUI MOLI DISPONIBILI --- */
    /*
     * Dobbiamo provare a mettere la nave corrente in ogni molo possibile.
     * Ma attenzione: non ha senso provare il molo 5 se il molo 4 è ancora vuoto.
     * Per efficienza, iteriamo fino a 'moli_usati_attuali + 1'.
     * Questo ci permette di usare i moli già aperti o di aprirne UNO nuovo.
     */
    int limite_ricerca = moli_usati_attuali + 1;
    if (limite_ricerca > n_moli) limite_ricerca = n_moli; // Non superiamo il limite fisico

    for (int i = 0; i < limite_ricerca; i++) {

        /* Dimensione della nave corrente */
        int dim_nave = navi[ship_idx];

        /* --- 4. TENTATIVO LATO SINISTRO --- */
        /*
         * Verifichiamo se c'è spazio a sinistra:
         * (spazio_occupato + dimensione_nave) <= lunghezza_massima
         */
        if (moli[i].occ_sx + dim_nave <= lun_molo) {
            /* AZIONE: Parcheggiamo la nave */
            moli[i].occ_sx += dim_nave;

            /*
             * Calcoliamo se abbiamo "aperto" un nuovo molo.
             * Se stiamo lavorando sul molo 'i' e 'i' è uguale all'indice del nuovo molo
             * (ovvero moli_usati_attuali), allora il conteggio aumenta.
             * Altrimenti resta uguale.
             */
            int nuovo_conteggio = (i == moli_usati_attuali) ? moli_usati_attuali + 1 : moli_usati_attuali;

            /* RICORSIONE: Passiamo alla nave successiva (ship_idx + 1) */
            rec_solve(ship_idx + 1, k, navi, moli, n_moli, lun_molo, nuovo_conteggio, best_moli);

            /* BACKTRACKING: Togliamo la nave per provare la prossima opzione nel ciclo */
            moli[i].occ_sx -= dim_nave;
        }

        /* --- 5. TENTATIVO LATO DESTRO --- */
        /*
         * Stessa logica per il lato destro.
         * Nota: Se il molo è vuoto (occ_sx == 0 e occ_dx == 0), il lato destro è
         * simmetrico al sinistro. Per evitare calcoli doppi inutili potremmo mettere un if,
         * ma per chiarezza didattica proviamo sempre entrambi.
         */
        if (moli[i].occ_dx + dim_nave <= lun_molo) {
            /* AZIONE */
            moli[i].occ_dx += dim_nave;

            int nuovo_conteggio = (i == moli_usati_attuali) ? moli_usati_attuali + 1 : moli_usati_attuali;

            /* RICORSIONE */
            rec_solve(ship_idx + 1, k, navi, moli, n_moli, lun_molo, nuovo_conteggio, best_moli);

            /* BACKTRACKING */
            moli[i].occ_dx -= dim_nave;
        }
    }
}

/* ============================================================================
 * FUNZIONE: calcolaMinimoMoli (Wrapper)
 * ============================================================================
 * SCOPO:
 * Prepara i dati per la ricorsione: alloca la struttura dei moli, inizializza
 * il valore "best" a un numero alto, lancia la ricorsione e stampa il risultato.
 *
 * PARAMETRI:
 * - navi, k, lun_molo, n_moli: Parametri del problema.
 *
 * GESTIONE MEMORIA:
 * - Usa malloc per creare l'array 'moli' dinamicamente in base a 'n_moli'.
 * - Fondamentale l'uso di calloc (o malloc + memset) per avere tutto a 0.
 * ============================================================================
 */
void calcolaMinimoMoli(int *navi, int k, int lun_molo, int n_moli) {

    /* --- 1. ALLOCAZIONE E INIZIALIZZAZIONE --- */
    /*
     * Allocazione dinamica dell'array di struct Molo.
     * Usiamo calloc invece di malloc perché calloc inizializza
     * automaticamente tutti i bit a 0.
     * In questo modo occ_sx e occ_dx partono da 0 senza cicli for manuali.
     * Sintassi: calloc(numero_elementi, dimensione_elemento)
     */
    Molo *moli = (Molo *)calloc(n_moli, sizeof(Molo));
    if (moli == NULL) {
        perror("Errore malloc moli");
        exit(1);
    }

    /*
     * Inizializziamo il risultato migliore con un valore "pessimo" (infinito).
     * Usiamo n_moli + 1 perché è impossibile usare più moli di quelli che abbiamo + 1.
     */
    int min_result = n_moli + 1;

    /* --- 2. CHIAMATA ALLA RICORSIONE --- */
    /*
     * Parametri iniziali:
     * - nave: 0 (la prima)
     * - moli_usati: 0 (all'inizio nessun molo è usato)
     * - &min_result: passiamo l'indirizzo così la ricorsione può aggiornarlo.
     */
    printf("Inizio calcolo per %d navi su moli lunghi %d...\n", k, lun_molo);
    rec_solve(0, k, navi, moli, n_moli, lun_molo, 0, &min_result);

    /* --- 3. STAMPA RISULTATO --- */
    if (min_result > n_moli) {
        printf("IMPOSSIBILE: Non ci sono abbastanza moli per tutte le navi.\n");
    } else {
        printf("SOLUZIONE TROVATA: Minimo numero di moli utilizzati = %d\n", min_result);
    }

    /* --- 4. LIBERAZIONE MEMORIA --- */
    /* Non dimentichiamo mai di liberare ciò che abbiamo allocato */
    free(moli);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Creare i dati come nell'esempio dell'esercizio e testare la funzione.
 * Esempio Immagine:
 * - 4 moli
 * - Lunghezza molo 10
 * - Navi: 8, 3, 5, 2, 5, 2, 9
 * ============================================================================
 */
int main() {
    /* Dati Hardcoded dell'esempio */
    int n_moli = 4;
    int lun_molo = 10;

    /* Array delle navi (dimensione k=7) */
    int navi[] = {8, 3, 5, 2, 5, 2, 9};
    // Calcolo dinamico della dimensione dell'array:
    // sizeof(navi) è il peso totale in byte dell'array.
    // sizeof(int) è il peso di un singolo elemento (4 byte).
    int k = sizeof(navi) / sizeof(int);

    /* Chiamata alla funzione principale */
    calcolaMinimoMoli(navi, k, lun_molo, n_moli);

    return 0;
}