#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * COSTANTI E DEFINIZIONI
 * ============================================================================
 * L'esercizio specifica che l'Universo U è composto dagli interi da 1 a 8.
 * MMAX è la dimensione fissa delle colonne specificata nel prototipo (9).
 */
#define U_MIN 1
#define U_MAX 8
#define MMAX 9

/* ============================================================================
 * FUNZIONE AUSILIARIA: check_coverage
 * ============================================================================
 * SCOPO:
 * Verifica se, dato lo stato attuale delle selezioni, abbiamo coperto tutti
 * i numeri da 1 a 8.
 *
 * PARAMETRI:
 * - count_array: Un array di contatori. La cella i-esima indica quante volte
 * il numero 'i' è presente nei sottoinsiemi scelti finora.
 *
 * RITORNA:
 * - 1 (true) se tutti i numeri da 1 a 8 hanno count > 0.
 * - 0 (false) se manca anche solo un numero.
 * ============================================================================
 */
int check_coverage(int *count_array) {
    /* Scorriamo l'universo U (da 1 a 8) */
    for (int i = U_MIN; i <= U_MAX; i++) {
        /* Se un numero ha contatore 0, non è coperto da nessuno */
        if (count_array[i] == 0) {
            return 0; 
        }
    }
    return 1; // Tutto coperto
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: update_coverage
 * ============================================================================
 * SCOPO:
 * Aggiorna l'array dei contatori aggiungendo o rimuovendo un sottoinsieme.
 *
 * PARAMETRI:
 * - row: L'array di interi che rappresenta il sottoinsieme (riga della matrice).
 * - count_array: L'array dei contatori da aggiornare.
 * - delta: +1 per aggiungere (do), -1 per rimuovere (undo/backtrack).
 *
 * DETTAGLIO SINTATTICO:
 * La riga termina con 0. Usiamo un ciclo while(row[j] != 0) per scorrere.
 * ============================================================================
 */
void update_coverage(int *row, int *count_array, int delta) {
    int j = 0;
    /* Ciclo finché non troviamo il terminatore 0 */
    while (row[j] != 0) {
        int val = row[j];
        /* Concetto Chiave: Accesso diretto.
           Usiamo il valore 'val' come indice dell'array count_array.
           Aggiorniamo il contatore corrispondente. */
        count_array[val] += delta;
        j++;
    }
}

/* ============================================================================
 * FUNZIONE RICORSIVA (KERNEL): cover_rec
 * ============================================================================
 * SCOPO:
 * Esplora le combinazioni di k righe per trovare una copertura valida.
 *
 * PARAMETRI:
 * - S: La matrice dei sottoinsiemi.
 * - n: Numero totale di righe disponibili.
 * - k: Numero di righe ancora da scegliere (decrementale).
 * - start_index: Indice da cui iniziare a cercare (per evitare permutazioni/duplicati).
 * - chosen: Array che memorizza gli indici delle righe scelte (per la stampa).
 * - chosen_count: Quante righe ho scelto finora (per indicizzare 'chosen').
 * - count_array: Array di stato per la copertura (backtracking).
 *
 * CONCETTI CHIAVE (BACKTRACKING):
 * 1. Scelta: Prendo una riga e aggiorno la copertura (+1).
 * 2. Ricorsione: Scendo di livello cercando k-1 righe.
 * 3. Backtrack: Tolgo la riga e ripristino la copertura (-1) per provare la successiva.
 * ============================================================================
 */
void cover_rec(int S[][MMAX], int n, int k, int start_index, 
               int *chosen, int chosen_count, int *count_array) {
    
    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se k == 0, abbiamo selezionato il numero richiesto di sottoinsiemi. */
    if (k == 0) {
        /* Verifichiamo se l'unione copre tutto U */
        if (check_coverage(count_array)) {
            printf("Soluzione trovata: { ");
            for (int i = 0; i < chosen_count; i++) {
                /* Stampiamo S seguito dall'indice della riga + 1 (per estetica 1-based) */
                printf("S%d ", chosen[i] + 1);
            }
            printf("}\n");
        }
        return;
    }

    /* --- 2. CICLO DI ESPLORAZIONE (Combinazioni) --- */
    /* Iteriamo sulle righe disponibili partendo da start_index fino alla fine.
       Ottimizzazione: i <= n - k (se mancano k elementi e ne rimangono meno di k, inutile continuare) */
    for (int i = start_index; i < n; i++) {
        
        /* A. AGGIORNA STATO (DO) */
        /* Aggiungiamo il contributo della riga S[i] alla copertura globale */
        update_coverage(S[i], count_array, 1);
        
        /* Memorizziamo l'indice della riga scelta */
        chosen[chosen_count] = i;

        /* B. PASSO RICORSIVO */
        /* Chiamiamo la funzione decrementando k, avanzando start_index (i+1) 
           e incrementando il conteggio dei scelti. */
        cover_rec(S, n, k - 1, i + 1, chosen, chosen_count + 1, count_array);

        /* C. RIPRISTINA STATO (UNDO / BACKTRACK) */
        /* Fondamentale: rimuoviamo il contributo di S[i] prima di passare
           alla prossima iterazione del ciclo for. */
        update_coverage(S[i], count_array, -1);
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER: cover
 * ============================================================================
 * SCOPO:
 * Interfaccia richiesta dall'esercizio. Prepara le strutture dati ausiliarie
 * e lancia la ricorsione.
 *
 * GESTIONE MEMORIA:
 * Allochiamo 'count_array' con calloc per avere tutto a 0.
 * Allochiamo 'chosen' per tenere traccia degli indici.
 * ============================================================================
 */
void cover(int S[][9], int n, int k) {
    
    /* --- 1. ALLOCAZIONE STRUTTURE DATI --- */
    /* Array per tenere traccia di quali numeri (1-8) sono coperti.
       Dimensione U_MAX + 1 per usare indici diretti 1..8. 
       Uso CALLOC per inizializzare a 0. */
    int *count_array = (int *)calloc(U_MAX + 1, sizeof(int));
    if (count_array == NULL) {
        printf("Errore malloc count_array\n");
        return;
    }

    /* Array per memorizzare gli indici delle righe della soluzione corrente */
    int *chosen = (int *)malloc(k * sizeof(int));
    if (chosen == NULL) {
        printf("Errore malloc chosen\n");
        free(count_array);
        return;
    }

    printf("Cerco coperture con k=%d sottoinsiemi...\n", k);

    /* --- 2. AVVIO RICORSIONE --- */
    /* Parametri iniziali: start_index=0, chosen_count=0 */
    cover_rec(S, n, k, 0, chosen, 0, count_array);

    /* --- 3. PULIZIA MEMORIA --- */
    free(chosen);
    free(count_array);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Ricrea l'esempio dell'immagine (i cerchi colorati) e testa la funzione.
 * ============================================================================
 */
int main() {
    /* DATI DELL'ESEMPIO NELL'IMMAGINE */
    /* U = {1,2,3,4,5,6,7,8}
       S1 = {1,2,3}
       S2 = {2,3,7}
       S3 = {7,8}
       S4 = {3,4}
       S5 = {4,5,6}
       
       Nella matrice, ogni riga deve terminare con 0.
    */
    
    int n = 5; // Numero di sottoinsiemi
    // Matrice statica 5x9 (compatibile con il parametro int S[][9])
    int S[5][9] = {
        {1, 2, 3, 0},       // S1 (indice 0)
        {2, 3, 7, 0},       // S2 (indice 1)
        {7, 8, 0},          // S3 (indice 2)
        {3, 4, 0},          // S4 (indice 3)
        {4, 5, 6, 0}        // S5 (indice 4) - Corretto dai dati immagine
    };

    /* CASO 1: k=3
       Soluzione visiva attesa: S1 U S5 U S3 
       S1={1,2,3}, S5={4,5,6}, S3={7,8} -> Totale {1..8}. OK. */
    printf("--- TEST 1 (k=3) ---\n");
    cover(S, n, 3);

    /* CASO 2: k=2
       Non dovrebbe esistere soluzione perché servono troppi numeri per sole 2 righe. */
    printf("\n--- TEST 2 (k=2) ---\n");
    cover(S, n, 2);

    return 0;
}