#include <stdio.h>
#include <stdlib.h>

/*
findBOUND e DEC NON VANNO FATTE 
SCRITTE QUA SOLO PER SIMULARE 
 */


/*
 * ======================================================================================
 * NOME FUNZIONE: findBound (Simulata per il test)
 * ======================================================================================
 * SCOPO:
 * Trova il sottovettore più lungo contenente solo numeri positivi (> 0).
 * Questa funzione simula quella descritta nel testo dell'esercizio.
 *
 * PARAMETRI:
 * - int *V: Il vettore da analizzare.
 * - int N: La dimensione del vettore.
 * - int *lp: Puntatore all'intero dove scriveremo l'indice sinistro (inizio intervallo).
 * - int *rp: Puntatore all'intero dove scriveremo l'indice destro (fine intervallo).
 *
 * RITORNA:
 * - 1 se ha trovato un intervallo positivo, 0 se il vettore è tutto a 0.
 *
 * CONCETTI CHIAVE:
 * - Passaggio per Riferimento (Puntatori): Poiché in C una funzione può ritornare
 * un solo valore, usiamo `*lp` e `*rp` per "esportare" due risultati aggiuntivi
 * (gli indici) modificando le variabili del chiamante.
 * ======================================================================================
 */
int findBound(int *V, int N, int *lp, int *rp) {
    int maxLen = 0;       // Lunghezza massima trovata finora
    int currentLen = 0;   // Lunghezza della sequenza corrente
    int startIdx = -1;    // Dove inizia la sequenza corrente
    int found = 0;        // Flag: abbiamo trovato almeno un numero > 0?

    int bestL = -1, bestR = -1; // Variabili temporanee per memorizzare il miglior intervallo

    for (int i = 0; i < N; i++) {
        /* Se il numero è positivo, estendiamo la sequenza corrente */
        if (V[i] > 0) {
            if (currentLen == 0) {
                startIdx = i; // Inizio di una nuova sequenza positiva
            }
            currentLen++;
            found = 1;
        } 
        /* Se troviamo uno 0 o siamo alla fine, controlliamo se la sequenza appena chiusa è la migliore */
        else {
            if (currentLen > maxLen) {
                maxLen = currentLen;
                bestL = startIdx;
                bestR = i - 1; // L'indice precedente era l'ultimo positivo
            }
            currentLen = 0; // Reset
        }
    }
    
    /* Controllo finale nel caso la sequenza termini esattamente alla fine dell'array */
    if (currentLen > maxLen) {
        maxLen = currentLen;
        bestL = startIdx;
        bestR = N - 1;
    }

    if (found) {
        /* * Dettaglio Sintattico: Dereferenziazione
         * *lp = ... significa: "Vai all'indirizzo di memoria contenuto in lp
         * e scrivi lì dentro il valore bestL".
         */
        *lp = bestL;
        *rp = bestR;
        return 1; // Intervallo trovato
    }
    
    return 0; // Nessun valore positivo trovato
}

/*
 * ======================================================================================
 * NOME FUNZIONE: dec (Simulata per il test)
 * ======================================================================================
 * SCOPO:
 * Decrementa di 1 tutti gli elementi nell'intervallo [l, r].
 *
 * PARAMETRI:
 * - int *V: Il vettore.
 * - int l, int r: Indici estremi (inclusi).
 * ======================================================================================
 */
void dec(int *V, int l, int r) {
    /* Ciclo semplice che scorre dall'indice l all'indice r */
    for (int i = l; i <= r; i++) {
        V[i] = V[i] - 1; // Decremento
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: azzeraVettore
 * ======================================================================================
 * SCOPO:
 * Trasformare il vettore in tutti zeri usando il minimo numero di chiamate
 * a findBound e dec.
 *
 * PARAMETRI:
 * - int *V: Il puntatore al vettore (array).
 * - int N: Dimensione.
 *
 * LOGICA:
 * L'approccio è "Greedy" (ingordo). Ad ogni passo chiediamo: "Qual è la montagna
 * più larga di numeri positivi?" (findBound). Trovata la montagna, la abbassiamo
 * di 1 livello (dec). Ripetiamo finché non ci sono più montagne.
 * ======================================================================================
 */
void azzeraVettore(int *V, int N) {
    int left, right; // Variabili locali per ospitare gli indici restituiti da findBound
    int contatoreChiamate = 0; // Solo per statistica didattica

    /* --- 1. CICLO PRINCIPALE --- */
    /*
     * La condizione del while è cruciale: chiamiamo findBound.
     * findBound fa due cose:
     * 1. Restituisce 1 (Vero) se c'è ancora lavoro da fare, 0 (Falso) se tutto è zero.
     * 2. Se restituisce 1, aggiorna le variabili 'left' e 'right' con le coordinate.
     *
     * Dettaglio Sintattico: &left
     * Passiamo l'indirizzo di 'left' (&left) perché findBound deve riempire questa variabile.
     */
    while (findBound(V, N, &left, &right)) {
        
        /* --- 2. AZIONE DI DECREMENTO --- */
        /* Abbiamo le coordinate del segmento positivo più lungo. Lo abbassiamo. */
        dec(V, left, right);
        
        contatoreChiamate++;
        
        /* Stampa didattica per vedere l'evoluzione passo passo */
        printf("Step %d (Intervallo [%d, %d]): ", contatoreChiamate, left, right);
        for(int i=0; i<N; i++) printf("%d ", V[i]);
        printf("\n");
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. ALLOCAZIONE DATI --- */
    /*
     * Uso malloc per simulare un contesto dinamico reale e allenare la gestione memoria,
     * anche se per un array fisso potremmo usare lo stack.
     */
    int N = 8;
    int *V = (int*)malloc(N * sizeof(int));

    /* Controllo malloc */
    if (V == NULL) {
        perror("Errore malloc");
        exit(1);
    }

    /* Inizializzazione hardcoded come da esempio nell'immagine: {0, 1, 2, 0, 0, 3, 4, 5} */
    int datiIniziali[] = {0, 1, 2, 0, 0, 3, 4, 5};
    for(int i=0; i<N; i++) V[i] = datiIniziali[i];

    printf("--- Vettore Iniziale ---\n");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n\n--- Inizio Elaborazione ---\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE SOLUZIONE --- */
    azzeraVettore(V, N);

    printf("\n--- Vettore Finale ---\n");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n");

    /* --- 3. PULIZIA MEMORIA --- */
    /*
     * Mai dimenticare la free. In un programma piccolo il sistema operativo
     * pulisce all'uscita, ma abituarsi a farlo è vitale per evitare memory leaks
     * in programmi server o a lunga esecuzione.
     */
    free(V);

    return 0;
}