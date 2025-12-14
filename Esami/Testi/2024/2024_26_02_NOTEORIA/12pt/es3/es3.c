#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * FUNZIONE AUSILIARIA: backtracking
 * ======================================================================================
 * Scopo:
 * Funzione ricorsiva che costruisce il numero cifra per cifra.
 * * Parametri:
 * - int level: Il livello corrente dell'albero (ovvero: quale cifra stiamo decidendo ora? 0..NC-1).
 * - int *sol: Vettore che contiene le cifre scelte finora.
 * - int current_sum: Somma delle cifre inserite finora.
 * - int *freq: Vettore delle frequenze (count) per ogni cifra da 0 a B-1.
 * - int repeated_count: Contatore che dice QUANTE cifre appaiono più di una volta. 
 * (Il vincolo dice che questo valore deve essere <= 1).
 * - int B, NC, minS: I vincoli costanti del problema.
 * * Concetti Chiave:
 * - Backtracking: "Provo, vado avanti, se non va bene torno indietro e annullo".
 * - Pruning: Le istruzioni 'continue' nel ciclo for saltano interi rami dell'albero.
 */
void backtracking(int level, int *sol, int current_sum, int *freq, int repeated_count, 
                  int B, int NC, int minS) {
    
    /* --- 1. CASO BASE (Condizione di terminazione) --- */
    // Siamo arrivati a riempire tutte le posizioni (da 0 a NC-1).
    if (level == NC) {
        // Ultimo controllo: la somma soddisfa il requisito?
        if (current_sum >= minS) {
            // Stampa della soluzione
            for (int i = 0; i < NC; i++) {
                printf("%d", sol[i]);
            }
            printf("\n");
        }
        return; // Torniamo al livello precedente (backtrack)
    }

    /* --- 2. ESPLORAZIONE SCELTE (Ciclo sulle cifre possibili) --- */
    // Per la posizione corrente 'level', proviamo tutte le cifre da 0 a B-1
    for (int i = 0; i < B; i++) {
        
        // VINCOLO A: La cifra più significativa (level 0) non può essere 0
        if (level == 0 && i == 0) continue; 

        /* --- 3. PRUNING SULLE RIPETIZIONI --- */
        // Calcoliamo come cambierebbe lo stato delle ripetizioni se scegliessimo 'i'.
        int next_repeated_count = repeated_count;
        
        // Se freq[i] è 0: diventa 1 (nessuna nuova ripetizione).
        // Se freq[i] è 1: diventa 2 (ABBIAMO UNA NUOVA RIPETIZIONE).
        // Se freq[i] > 1: diventa >2 (la cifra era già ripetuta, non cambia il numero di cifre ripetute distinte).
        
        if (freq[i] == 1) {
            next_repeated_count++;
        }

        // VINCOLO B: "al massimo una delle cifre presenti può apparire più di una volta"
        // Se scegliendo 'i' portiamo il contatore delle cifre ripetute a 2, è ILLEGALE.
        // Esempio illegale: ho "11" (1 ripetuto), ora provo ad aggiungere "2" avendo già un "2". 
        // Avrei "1" ripetuto e "2" ripetuto -> repeated_count = 2 -> STOP.
        if (next_repeated_count > 1) {
            continue; // PRUNING: Taglia questo ramo!
        }

        /* --- 4. PRUNING SULLA SOMMA (Ottimizzazione Avanzata) --- */
        // Calcoliamo la somma massima che potremmo raggiungere se riempissimo 
        // tutte le caselle rimanenti con la cifra più alta possibile (B-1).
        int slots_remaining = NC - 1 - level;
        int max_potential_sum = current_sum + i + (slots_remaining * (B - 1));

        // Se anche nel caso migliore non raggiungiamo minS, inutile continuare.
        if (max_potential_sum < minS) {
            continue; // PRUNING
        }

        /* --- 5. PASSO RICORSIVO (Do - Recurse - Undo) --- */
        
        // DO: Applica la scelta
        sol[level] = i;      // Metti la cifra nel vettore soluzione
        freq[i]++;           // Aggiorna la frequenza
        
        // RECURSE: Scendi al livello successivo
        backtracking(level + 1, sol, current_sum + i, freq, next_repeated_count, B, NC, minS);
        
        // UNDO: Backtracking puro (annulla la scelta per provare la prossima 'i')
        freq[i]--;           // Decrementa frequenza
        // Non serve cancellare sol[level], verrà sovrascritta alla prossima iterazione
    }
}

/*
 * ======================================================================================
 * FUNZIONE: generaNumeri (WRAPPER)
 * ======================================================================================
 * Scopo:
 * Prepara le strutture dati necessarie e lancia la ricorsione.
 */
void generaNumeri(int B, int NC, int minS) {
    /* --- 1. ALLOCAZIONE MEMORIA --- */
    // Vettore per la soluzione corrente (lungo NC)
    int *sol = (int *)malloc(NC * sizeof(int));
    
    // Vettore per le frequenze (lungo B, perché le cifre sono 0..B-1)
    // Usiamo calloc per inizializzare tutto a 0
    int *freq = (int *)calloc(B, sizeof(int));

    if (sol == NULL || freq == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    printf("Generazione numeri Base %d, Cifre %d, Somma Min %d:\n", B, NC, minS);

    /* --- 2. LANCIO RICORSIONE --- */
    // Partiamo dal livello 0, somma 0, 0 ripetizioni trovate finora
    backtracking(0, sol, 0, freq, 0, B, NC, minS);

    /* --- 3. CLEANUP --- */
    free(sol);
    free(freq);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Esempio 1: Base 4, 3 Cifre, Somma minima 5
    // Cifre ammesse: 0, 1, 2, 3.
    // Esempio valido: 123 (somma 6, 0 rip), 313 (somma 7, 1 rip: il 3).
    // Esempio invalido: 012 (inizia con 0), 100 (somma 1 < 5).
    generaNumeri(4, 3, 5);
    
    printf("\n----------------\n");
    
    // Esempio 2: Vincolo ripetizioni
    // Base 3 (cifre 0,1,2), 4 Cifre.
    // 1122 -> Invalido (1 rip, 2 rip -> 2 cifre ripetute).
    // 1112 -> Valido (solo 1 è ripetuto).
    generaNumeri(3, 4, 1); 

    return 0;
}