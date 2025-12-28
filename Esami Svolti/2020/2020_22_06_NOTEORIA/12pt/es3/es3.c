#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =================================================================================
 * DEFINIZIONE TIPI DI DATO (ADT e Quasi-ADT)
 * =================================================================================
 * Definiamo la struttura dell'oggetto come richiesto dalla traccia.
 * item_t contiene le 4 proprietà: costo, valore, tipo, quantità.
 */
typedef struct {
    char *name;
    int cost;
    int value;
    int type; // Identificativo numerico del tipo (es. 0=Cibo, 1=Elettronica...)
    int qty;  // Quantità disponibile (nota: nell'insieme delle parti, di solito si usa 1 per volta)
} item_t;

/* =================================================================================
 * FUNZIONE RICORSIVA: solve_r
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Esplora lo spazio delle soluzioni usando il modello combinatorio dell'Insieme delle Parti
 * (Powerset). Per ogni oggetto, decidiamo se PRENDERLO o LASCIARLO.
 *
 * PARAMETRI:
 * - item_t *v: Vettore degli oggetti disponibili.
 * - int n: Numero totale oggetti.
 * - int *sol: Vettore di interi (0/1) che rappresenta la soluzione corrente.
 * sol[i] = 1 significa "oggetto i preso", 0 "non preso".
 * - int pos: Livello della ricorsione (indice dell'oggetto corrente).
 * - int tot_c: Costo accumulato finora.
 * - int tot_v: Valore accumulato finora (non strettamente richiesto per il stop, ma utile).
 * - int C_max: (Parametro '...') Costo massimo consentito (budget).
 * - int *used_types: (Parametro '...') Array di flag per tenere traccia dei tipi usati.
 *
 * MODELLO COMBINATORIO:
 * Usiamo l'Insieme delle Parti (Powerset) con cardinalità variabile.
 * Albero binario di decisione: a ogni livello 'pos', due rami:
 * 1. Includo l'oggetto v[pos].
 * 2. Escludo l'oggetto v[pos].
 *
 * CRITERI DI PRUNING (Potatura):
 * 1. Vincolo di Budget: Se (tot_c + v[pos].cost > C_max), non posso prendere l'oggetto.
 * Taglio il ramo "prendi".
 * 2. Vincolo di Tipo: Se il tipo dell'oggetto (v[pos].type) è già presente
 * nell'array 'used_types', non posso prenderlo (devono essere tutti diversi).
 *
 * RITORNA:
 * 1 se abbiamo trovato una soluzione valida completa (o siamo arrivati in fondo validi),
 * 0 se questo ramo non porta a soluzione.
 * =================================================================================
 */
int solve_r(item_t *v, int n, int *sol, int pos, int tot_c, int tot_v, int C_max, int *used_types) {

    /* --- 1. CASO BASE (Terminazione) --- */
    // Se siamo arrivati alla fine del vettore degli oggetti (pos == n).
    // Nota: Poiché cerchiamo UNA soluzione valida e i vincoli sono controllati DURANTE
    // la discesa, se arriviamo qui significa che la configurazione in 'sol' è valida.
    if (pos >= n) {
        // Qui potremmo mettere un controllo extra (es. valore minimo richiesto),
        // ma la traccia dice "trovata una soluzione valida".
        // Ritorniamo 1 per dire "Successo, ferma tutto!".
        return 1;
    }

    /* --- 2. LOGICA RICORSIVA (Tentativi) --- */
    
    // --- TENTATIVO A: PRENDERE L'OGGETTO v[pos] ---
    
    // Verifichiamo i vincoli PRIMA di scendere (Pruning)
    int attuale_costo = v[pos].cost;
    int attuale_tipo = v[pos].type;
    
    // PRUNING 1: Il costo supera il budget?
    // PRUNING 2: Il tipo è già stato usato? (Accedo alla cella dell'array flags)
    if (tot_c + attuale_costo <= C_max && used_types[attuale_tipo] == 0) {
        
        // APPLICAZIONE SCELTA (Do move)
        sol[pos] = 1;                  // Segno che prendo l'oggetto
        used_types[attuale_tipo] = 1;  // Segno che questo tipo è ora occupato
        
        // RICORSIONE
        // Se la chiamata ricorsiva ritorna 1, significa che ha trovato la soluzione finale.
        // Propaghiamo l'1 verso l'alto per terminare immediatamente (Bubble Up).
        if (solve_r(v, n, sol, pos + 1, tot_c + attuale_costo, tot_v + v[pos].value, C_max, used_types)) {
            return 1;
        }
        
        // BACKTRACKING (Undo move)
        // Se siamo qui, il ramo "prendi" non ha portato a successo (o vogliamo esplorare altro,
        // ma qui ci fermiamo alla prima). Ripristiniamo lo stato.
        used_types[attuale_tipo] = 0;
        sol[pos] = 0;
    }

    // --- TENTATIVO B: NON PRENDERE L'OGGETTO v[pos] ---
    // Non prendere un oggetto è sempre lecito (non viola budget o tipi).
    sol[pos] = 0;
    
    if (solve_r(v, n, sol, pos + 1, tot_c, tot_v, C_max, used_types)) {
        return 1;
    }

    // Se né prenderlo né lasciarlo porta a una soluzione "di successo" (secondo i criteri di stop),
    // ritorniamo 0 (Backtrack).
    return 0;
}

/* =================================================================================
 * FUNZIONE WRAPPER
 * ---------------------------------------------------------------------------------
 * Scopo: Prepara le strutture dati ausiliarie (soluzione, flag tipi) e lancia la ricorsione.
 * =================================================================================
 */
void trova_soluzione(item_t *v, int n, int C_max) {
    /* --- 1. ALLOCAZIONE MEMORIA --- */
    
    // Allocazione vettore soluzione: n interi
    int *sol = (int *)calloc(n, sizeof(int));
    
    // Allocazione vettore per i tipi usati.
    // Assumiamo per semplicità che i tipi siano id da 0 a 99.
    // Usiamo calloc per inizializzare tutto a 0 (FALSE).
    int max_types = 100; 
    int *used_types = (int *)calloc(max_types, sizeof(int));

    if (sol == NULL || used_types == NULL) {
        printf("Errore allocazione memoria.\n");
        exit(1);
    }

    /* --- 2. CHIAMATA RICORSIVA --- */
    printf("Cerco soluzione con Costo Max %d...\n", C_max);
    
    // I parametri '...' sono C_max e used_types
    int trovato = solve_r(v, n, sol, 0, 0, 0, C_max, used_types);

    /* --- 3. STAMPA RISULTATI --- */
    if (trovato) {
        printf("Soluzione trovata:\n");
        int costo_tot = 0;
        int valore_tot = 0;
        for (int i = 0; i < n; i++) {
            if (sol[i] == 1) {
                printf("- %s (Costo: %d, Val: %d, Tipo: %d)\n", v[i].name, v[i].cost, v[i].value, v[i].type);
                costo_tot += v[i].cost;
                valore_tot += v[i].value;
            }
        }
        printf("Totale -> Costo: %d, Valore: %d\n", costo_tot, valore_tot);
    } else {
        printf("Nessuna soluzione trovata che rispetta i vincoli (caso improbabile con insieme vuoto valido).\n");
    }

    /* --- 4. FREE (Pulizia) --- */
    free(sol);
    free(used_types);
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * Crea un set di oggetti hardcoded e testa la funzione.
 * =================================================================================
 */
int main() {
    int n = 5;
    // Allocazione array di struct item_t
    item_t *items = (item_t *)malloc(n * sizeof(item_t));

    // Dati di prova:
    // Item 0: Spada (Tipo 1)
    items[0].name = "Spada"; items[0].cost = 10; items[0].value = 5; items[0].type = 1; items[0].qty = 1;
    
    // Item 1: Scudo (Tipo 2)
    items[1].name = "Scudo"; items[1].cost = 15; items[1].value = 8; items[1].type = 2; items[1].qty = 1;
    
    // Item 2: Pozione (Tipo 3)
    items[2].name = "Pozione"; items[2].cost = 5; items[2].value = 2; items[2].type = 3; items[2].qty = 5;
    
    // Item 3: Altra Spada (Tipo 1) - CONFLITTO DI TIPO con Item 0
    items[3].name = "SpadaMagica"; items[3].cost = 20; items[3].value = 15; items[3].type = 1; items[3].qty = 1;
    
    // Item 4: Elmo (Tipo 2) - CONFLITTO DI TIPO con Item 1
    items[4].name = "Elmo"; items[4].cost = 8; items[4].value = 4; items[4].type = 2; items[4].qty = 1;

    // Test 1: Budget limitato (20). Dovrebbe prendere Spada(10) e Pozione(5) = 15.
    // Non può prendere Scudo(15) insieme a Spada perchè 10+15=25 > 20.
    trova_soluzione(items, n, 20);

    printf("\n-------------------\n");

    // Test 2: Budget alto (50). Dovrebbe cercare di prendere tutto ma fermarsi per i TIPI.
    // Prenderà Spada (Tipo 1), Scudo (Tipo 2), Pozione (Tipo 3).
    // Quando arriva a SpadaMagica (Tipo 1), deve scartarla perché Tipo 1 è già usato.
    trova_soluzione(items, n, 50);

    // Pulizia array oggetti
    free(items);
    
    return 0;
}