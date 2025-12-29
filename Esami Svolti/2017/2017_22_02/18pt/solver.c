#include "supermercato.h"

// Variabili globali per la ricorsione (semplificano la firma della funzione)
static int best_cost = INT_MAX;
static int *best_offer_selection = NULL; // Array di flag: 1 se presa, 0 se no
static int *current_offer_selection = NULL;

/*
 * Funzione Ausiliaria: Calcola il costo completando la spesa corrente con prodotti singoli.
 */
int calcola_costo_residuo(Catalog *C, ShoppingItem *list, int M, int *acquired_quantities) {
    int cost = 0;
    for (int i = 0; i < M; i++) {
        int needed = list[i].quantity_needed;
        int have = acquired_quantities[i]; // Nota: acquired_quantities è mappato su M, non su N totale
        
        if (have < needed) {
            int missing = needed - have;
            // Troviamo il prezzo unitario dal catalogo
            int unit_price = C->products[list[i].prod_index].unit_price;
            cost += missing * unit_price;
        }
    }
    return cost;
}

/*
 * Funzione Ricorsiva (Backtracking)
 * level: indice dell'offerta che stiamo considerando (0..n_off)
 * current_cost_offers: quanto abbiamo speso finora SOLO per le offerte
 * list: lista della spesa
 * M: numero elementi spesa
 * acquired: array temporaneo (mappato su M) che tiene conto di quanto abbiamo di ogni prodotto della spesa
 */
void solve_rec(Catalog *C, int level, int current_cost_offers, ShoppingItem *list, int M, int *acquired) {
    
    // Pruning (Potatura): Se abbiamo già speso più del best, ci fermiamo
    if (current_cost_offers >= best_cost) return;

    /* --- CASO BASE: Abbiamo deciso per tutte le offerte --- */
    if (level == C->n_off) {
        // Calcoliamo quanto costa comprare i pezzi mancanti singolarmente
        int missing_cost = calcola_costo_residuo(C, list, M, acquired);
        int total = current_cost_offers + missing_cost;

        if (total < best_cost) {
            best_cost = total;
            // Salviamo la configurazione vincente (copia del vettore flag)
            for(int i=0; i<C->n_off; i++) 
                best_offer_selection[i] = current_offer_selection[i];
        }
        return;
    }

    Offer *curr_offer = &C->offers[level];

    /* --- RAMO 1: NON PRENDO L'OFFERTA 'level' --- */
    current_offer_selection[level] = 0;
    solve_rec(C, level + 1, current_cost_offers, list, M, acquired);

    /* --- RAMO 2: PRENDO L'OFFERTA 'level' (Solo se utile) --- */
    // L'offerta è "utile" se contiene almeno un prodotto che ci serve? 
    // O possiamo prenderla anche se ridondante purche' convenga? 
    // Il testo dice "ottimo acquisto", quindi proviamo a prenderla sempre e vediamo se il costo scende.
    // L'unico vincolo è "ogni offerta al massimo una volta", già gestito dal livello ricorsivo.

    current_offer_selection[level] = 1;
    
    // Aggiorniamo le quantità acquisite
    // ATTENZIONE: Dobbiamo modificare 'acquired' ma poi ripristinarlo (backtracking)
    // O più semplicemente, passiamo una copia o incrementiamo/decrementiamo.
    
    // Aggiorno quantita'
    for (int i = 0; i < curr_offer->num_items; i++) {
        int prod_idx = curr_offer->items[i].id_prod_index;
        
        // Dobbiamo vedere se questo prodotto è nella lista della spesa
        // acquired è indicizzato da 0 a M-1 (su lista spesa), non su catalogo.
        for(int j=0; j<M; j++) {
            if(list[j].prod_index == prod_idx) {
                acquired[j] += curr_offer->items[i].quantity;
                break; 
            }
        }
    }

    solve_rec(C, level + 1, current_cost_offers + curr_offer->price, list, M, acquired);

    // Backtrack: Ripristino quantita' (tolgo quello che ho aggiunto)
    for (int i = 0; i < curr_offer->num_items; i++) {
        int prod_idx = curr_offer->items[i].id_prod_index;
        for(int j=0; j<M; j++) {
            if(list[j].prod_index == prod_idx) {
                acquired[j] -= curr_offer->items[i].quantity;
                break;
            }
        }
    }
}

void calcola_spesa_ottima(Catalog *C, char *filename_spesa) {
    /* --- 1. CARICAMENTO SPESA --- */
    FILE *fp = fopen(filename_spesa, "r");
    if (!fp) { printf("Errore file spesa\n"); return; }
    
    int M;
    fscanf(fp, "%d", &M);
    
    ShoppingItem *list = (ShoppingItem*)malloc(M * sizeof(ShoppingItem));
    for (int i = 0; i < M; i++) {
        fscanf(fp, "%s %d", list[i].id_name, &list[i].quantity_needed);
        list[i].prod_index = trova_indice_prodotto(C, list[i].id_name);
        if (list[i].prod_index == -1) {
            printf("Errore: Prodotto %s nella spesa non esiste nel catalogo!\n", list[i].id_name);
            exit(1);
        }
    }
    fclose(fp);

    /* --- 2. PREPARAZIONE RICORSIONE --- */
    int *acquired = (int*)calloc(M, sizeof(int)); // Quantità comprate finora (inizializzate a 0)
    best_offer_selection = (int*)malloc(C->n_off * sizeof(int));
    current_offer_selection = (int*)malloc(C->n_off * sizeof(int));
    best_cost = INT_MAX;

    printf("Calcolo soluzione ottima...\n");
    
    // Avvio Ricorsione
    solve_rec(C, 0, 0, list, M, acquired);

    /* --- 3. STAMPA RISULTATI --- */
    printf("\n=== RISULTATO OTTIMO ===\n");
    printf("Costo Totale Minimo: %d\n", best_cost);
    printf("Strategia:\n");
    
    // Ricalcoliamo acquired finale per stampare i singoli
    memset(acquired, 0, M * sizeof(int));

    // Stampa Offerte
    for (int i = 0; i < C->n_off; i++) {
        if (best_offer_selection[i] == 1) {
            printf("[OFFERTA] %s\n", C->offers[i].id_code);
            // Aggiorno acquired per calcolare i residui
            for (int k = 0; k < C->offers[i].num_items; k++) {
                int p_idx = C->offers[i].items[k].id_prod_index;
                for(int j=0; j<M; j++) {
                    if(list[j].prod_index == p_idx) acquired[j] += C->offers[i].items[k].quantity;
                }
            }
        }
    }

    // Stampa Singoli
    for (int i = 0; i < M; i++) {
        if (acquired[i] < list[i].quantity_needed) {
            int da_comprare = list[i].quantity_needed - acquired[i];
            printf("[SINGOLO] %d x %s\n", da_comprare, list[i].id_name);
        }
    }

    /* --- 4. PULIZIA --- */
    free(list);
    free(acquired);
    free(best_offer_selection);
    free(current_offer_selection);
}