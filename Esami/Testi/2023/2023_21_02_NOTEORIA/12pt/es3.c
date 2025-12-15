#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // Per abs()
#include <limits.h> // Per INT_MAX

/*
 * STRUTTURA DATI: Pezzo
 * Rappresenta un tipo di pezzo producibile.
 */
typedef struct {
    int valore;
    int tempo;
} Pezzo;

/*
 * ======================================================================================
 * FUNZIONE CORE: cerca_combinazione
 * ======================================================================================
 *
 * Scopo:
 * Funzione ricorsiva che esplora tutte le possibili quantità per ogni tipo di pezzo.
 * Mantiene traccia della "miglior configurazione" trovata finora (quella che avvicina
 * di più la somma valori a V senza sforare il tempo T).
 *
 * Parametri:
 * - Pezzo *tipi:        Array con le info sui pezzi (valore, tempo).
 * - int P:              Numero totale di tipi di pezzi.
 * - int T_max:          Tempo massimo disponibile (input T).
 * - int V_target:       Valore obiettivo (input V).
 * - int indice_tipo:    Livello della ricorsione (quale tipo di pezzo stiamo valutando ora).
 * - int tempo_usato:    Tempo accumulato nella ricorsione corrente.
 * - int valore_accum:   Valore accumulato nella ricorsione corrente.
 * - int *conteggi_curr: Array temporaneo con le quantità attuali per ogni pezzo.
 * - int *conteggi_best: Array dove salviamo la soluzione migliore trovata.
 * - int *min_diff:      Puntatore alla differenza minima trovata finora (per aggiornarla).
 *
 * Concetti Chiave:
 * 1. Ricorsione con Backtracking: Proviamo ad aggiungere 0, 1, 2... pezzi del tipo corrente,
 * poi chiamiamo la funzione ricorsivamente per il prossimo tipo.
 * 2. Pruning (Potatura): Se aggiungere un pezzo ci fa superare T_max, smettiamo di
 * esplorare quel ramo.
 * ======================================================================================
 */
void cerca_combinazione(Pezzo *tipi, int P, int T_max, int V_target,
                        int indice_tipo, int tempo_usato, int valore_accum,
                        int *conteggi_curr, int *conteggi_best, int *min_diff) {

    /* --- 1. CASO BASE: Abbiamo deciso le quantità per tutti i tipi di pezzi --- */
    if (indice_tipo == P) {
        // Calcoliamo quanto siamo distanti dall'obiettivo V
        int diff = abs(V_target - valore_accum);

        // Se questa soluzione è migliore di quella salvata in precedenza...
        if (diff < *min_diff) {
            *min_diff = diff; // Aggiorniamo la miglior differenza
            
            // Copiamo l'array dei conteggi attuali in quello dei migliori
            for (int i = 0; i < P; i++) {
                conteggi_best[i] = conteggi_curr[i];
            }
        }
        return; // Fine di questo ramo
    }

    /* --- 2. LOGICA RICORSIVA --- */
    
    // Dobbiamo decidere quanti pezzi del tipo 'indice_tipo' produrre.
    // Proviamo tutte le quantità possibili: da 0 finché il tempo lo permette.
    // Nota: 'q' è la quantità (quantity)
    
    int costo_tempo_pezzo = tipi[indice_tipo].tempo;
    int valore_pezzo = tipi[indice_tipo].valore;
    
    // Il ciclo parte da q=0 e continua finché non sforiamo il tempo totale
    for (int q = 0; ; q++) {
        
        // Calcolo nuovo tempo se aggiungessi 'q' pezzi
        int nuovo_tempo = tempo_usato + (q * costo_tempo_pezzo);
        
        // CONDIZIONE DI ARRESTO LOOP (Pruning)
        if (nuovo_tempo > T_max) {
            break; // Non posso produrre 'q' o più pezzi di questo tipo, esco dal for
        }

        // Se siamo qui, la quantità 'q' è valida. Impostiamola.
        conteggi_curr[indice_tipo] = q;
        int nuovo_valore = valore_accum + (q * valore_pezzo);

        // CHIAMATA RICORSIVA: Passiamo al prossimo tipo di pezzo (indice_tipo + 1)
        cerca_combinazione(tipi, P, T_max, V_target, 
                           indice_tipo + 1, // Passiamo al prossimo tipo
                           nuovo_tempo, 
                           nuovo_valore, 
                           conteggi_curr, conteggi_best, min_diff);
    }
    
    // Backtracking implicito: quando il ciclo 'for' finisce, la funzione termina 
    // e il controllo torna al chiamante, che proverà un'altra quantità per il pezzo precedente.
}

/*
 * ======================================================================================
 * FUNZIONE WRAPPER (Interfaccia per il chiamante)
 * ======================================================================================
 * Prepara la memoria e lancia la ricorsione.
 */
void ottimizza_produzione(Pezzo *tipi, int P, int T, int V) {
    
    // Allocazione array per i conteggi
    int *conteggi_curr = (int *)calloc(P, sizeof(int)); // calloc inizializza a 0
    int *conteggi_best = (int *)calloc(P, sizeof(int));
    
    if (!conteggi_curr || !conteggi_best) {
        printf("Errore malloc\n");
        return;
    }

    // Inizializziamo la miglior differenza a un numero molto alto
    int min_diff = INT_MAX;

    // Lanciamo la ricorsione
    // Parametri iniziali: tipo 0, tempo 0, valore 0
    cerca_combinazione(tipi, P, T, V, 0, 0, 0, conteggi_curr, conteggi_best, &min_diff);

    /* --- STAMPA RISULTATI --- */
    printf("\n=== RISULTATO OTTIMIZZAZIONE ===\n");
    printf("Obiettivo Valore: %d (Tempo Max: %d)\n", V, T);
    
    int somma_val = 0;
    int somma_tempo = 0;
    
    for (int i = 0; i < P; i++) {
        if (conteggi_best[i] > 0) {
            printf("- Pezzo Tipo %d: %d unita' (Val: %d, Tempo: %d cad.)\n", 
                   i, conteggi_best[i], tipi[i].valore, tipi[i].tempo);
            somma_val += conteggi_best[i] * tipi[i].valore;
            somma_tempo += conteggi_best[i] * tipi[i].tempo;
        }
    }
    printf("--------------------------------\n");
    printf("Totale Ottenuto -> Valore: %d, Tempo Usato: %d\n", somma_val, somma_tempo);
    printf("Scarto dall'obiettivo: %d\n", min_diff);

    // Pulizia
    free(conteggi_curr);
    free(conteggi_best);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // ESEMPIO DI TEST
    // Supponiamo di avere 3 tipi di pezzi (P=3)
    int P = 3;
    Pezzo catalogo[] = {
        {10, 5},   // Tipo 0: Valore 10, Tempo 5
        {40, 20},  // Tipo 1: Valore 40, Tempo 20
        {90, 50}  // Tipo 2: Valore 90, Tempo 50
    };

    // Obiettivi
    int T = 100; // Tempo massimo disponibile
    int V = 150; // Valore che vorremmo raggiungere

    /* * Analisi attesa a mente:
     * Vogliamo 150.
     * Potremmo prendere 1 di Tipo 2 (Val 100, Tempo 55). Resta T=45, V_mancante=50.
     * Prendiamo 2 di Tipo 1 (Val 80, Tempo 40). Totale Val 180. Tempo 95. Diff = 30.
     * ... L'algoritmo proverà tutte le combinazioni per trovare quella con diff minima.
     */

    ottimizza_produzione(catalogo, P, T, V);

    return 0;
}