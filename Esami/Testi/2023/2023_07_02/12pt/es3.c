#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 99999

/* ============================================================================
 * STRUTTURA DATI PER LO STATO GLOBALE
 * ============================================================================
 * Scopo:
 * Mantenere traccia del "Record Mondiale" durante la ricorsione.
 * Invece di passare un semplice int *, usiamo una struct per chiarezza e
 * per poter estendere facilmente il programma in futuro (es. salvare anche
 * la configurazione dei gruppi, non solo il numero).
 */
typedef struct {
    int min_gruppi; // Il minor numero di gruppi trovato finora
} Soluzione;

/* ============================================================================
 * FUNZIONE AUSILIARIA: VERIFICA COMPATIBILITÀ (CLIQUE)
 * ============================================================================
 * Scopo:
 * Controllare se una città può entrare in un gruppo esistente.
 *
 * Concetto Chiave:
 * Un gruppo è valido solo se TUTTI i suoi membri sono compatibili tra loro
 * (struttura a "Clique"). Quando aggiungiamo una nuova città, dobbiamo
 * verificare che sia compatibile con *ogni* membro già presente nel gruppo.
 *
 * Parametri:
 * - citta_corrente: L'indice della città che stiamo provando a inserire.
 * - gruppo_id: L'ID del gruppo target (es. 0, 1, 2...).
 * - N: Numero totale città.
 * - assegnazioni: Array che ci dice a quale gruppo appartengono le città precedenti.
 * - connessioni: Matrice booleana pre-calcolata (True = compatibili).
 */
bool e_compatibile(int citta_corrente, int gruppo_id, int N, int *assegnazioni, bool **connessioni) {
    // Iteriamo su tutte le città precedenti alla corrente
    for (int i = 0; i < citta_corrente; i++) {
        
        // Se la città 'i' fa parte del gruppo in cui vogliamo entrare...
        if (assegnazioni[i] == gruppo_id) {
            
            // ...verifichiamo la connessione reciproca nella matrice pre-calcolata.
            // Dettaglio Sintattico: connessioni[u][v]
            // Accesso diretto alla cella di memoria. È O(1).
            if (!connessioni[citta_corrente][i]) {
                return false; // Violazione del vincolo: non possiamo unire queste città.
            }
        }
    }
    return true; // La città è compatibile con tutti i membri del gruppo.
}

/* ============================================================================
 * FUNZIONE CORE: BACKTRACKING RICORSIVO
 * ============================================================================
 * Scopo:
 * Esplorare l'albero delle decisioni per assegnare ogni città a un gruppo.
 *
 * Parametri:
 * - citta_idx: Indice della città che stiamo processando in questo step.
 * - num_gruppi_usati: Contatore dei gruppi creati nel ramo corrente.
 * - assegnazioni: Array di stato (indice città -> id gruppo).
 * - connessioni: Matrice di adiacenza logica.
 * - best: Puntatore alla struttura che contiene il record da battere.
 *
 * Concetto Chiave: Pruning (Potatura)
 * Se stiamo costruendo una soluzione che usa già più gruppi del nostro record
 * migliore, ci fermiamo subito. È inutile continuare a peggiorare.
 */
void trova_raggruppamenti(int citta_idx, int num_gruppi_usati, int N, 
                          int *assegnazioni, bool **connessioni, Soluzione *best) {
    
    /* --- 1. PRUNING (Ottimizzazione) --- */
    if (num_gruppi_usati >= best->min_gruppi) {
        return; // Taglio il ramo: non porterà a una soluzione migliore.
    }

    /* --- 2. CASO BASE (Tutte le città assegnate) --- */
    if (citta_idx == N) {
        // Se siamo qui, num_gruppi_usati è sicuramente < best->min_gruppi (grazie al pruning sopra)
        best->min_gruppi = num_gruppi_usati;
        return;
    }

    /* --- 3. TENTATIVO A: UNIRSI A UN GRUPPO ESISTENTE --- */
    // Proviamo a mettere la città corrente in ognuno dei gruppi già aperti.
    for (int g = 0; g < num_gruppi_usati; g++) {
        if (e_compatibile(citta_idx, g, N, assegnazioni, connessioni)) {
            
            // Azione (Do)
            assegnazioni[citta_idx] = g;
            
            // Ricorsione (Recurse)
            trova_raggruppamenti(citta_idx + 1, num_gruppi_usati, N, assegnazioni, connessioni, best);
            
            // Backtracking (Undo)
            // Ripristiniamo lo stato per la prossima iterazione del ciclo for.
            assegnazioni[citta_idx] = -1; 
        }
    }

    /* --- 4. TENTATIVO B: CREARE UN NUOVO GRUPPO --- */
    // Se non posso (o non voglio) unirmi agli altri, apro un nuovo gruppo.
    // L'ID del nuovo gruppo sarà uguale al numero di gruppi attuali (es: se ho 0 e 1, il nuovo è 2).
    
    assegnazioni[citta_idx] = num_gruppi_usati;
    
    // Ricorsione: notare 'num_gruppi_usati + 1'
    trova_raggruppamenti(citta_idx + 1, num_gruppi_usati + 1, N, assegnazioni, connessioni, best);
    
    // Backtracking
    assegnazioni[citta_idx] = -1;
}

/* ============================================================================
 * FUNZIONE WRAPPER (PREPARAZIONE DATI)
 * ============================================================================
 * Scopo:
 * 1. Allocare le strutture dati ausiliarie.
 * 2. Pre-calcolare la matrice di connettività (chi raggiunge chi).
 * 3. Lanciare la ricorsione.
 * 4. Pulire la memoria.
 */
int calcola_minimo_gruppi(int N, int **M) {
    
    /* --- 1. ALLOCAZIONE MATRICI DINAMICHE --- */
    // 'connessioni': Matrice finale simmetrica (bool).
    // 'raggiungibile': Matrice temporanea monodirezionale.
    
    // Dettaglio Sintattico: int **ptr = malloc(...)
    // Stiamo creando un array di puntatori. Ogni puntatore punterà a una riga.
    bool **connessioni = (bool **)malloc(N * sizeof(bool *));
    bool **raggiungibile = (bool **)malloc(N * sizeof(bool *));
    
    for (int i = 0; i < N; i++) {
        // calloc inizializza tutto a 0 (false), risparmiandoci un ciclo di init.
        connessioni[i] = (bool *)calloc(N, sizeof(bool));
        raggiungibile[i] = (bool *)calloc(N, sizeof(bool));
    }

    /* --- 2. PRE-CALCOLO RAGGIUNGIBILITÀ (Logica Business) --- */
    // Dobbiamo capire se i raggiunge j direttamente o con 1 scalo (k).
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            // Una città raggiunge sempre se stessa
            if (i == j) {
                raggiungibile[i][j] = true;
                continue;
            }
            
            // Check Connessione Diretta
            if (M[i][j] > 0) {
                raggiungibile[i][j] = true;
            } else {
                // Check Connessione con 1 Intermediario (k)
                // Cerchiamo UN nodo k tale che i->k E k->j
                for (int k = 0; k < N; k++) {
                    if (M[i][k] > 0 && M[k][j] > 0) {
                        raggiungibile[i][j] = true;
                        break; // Trovato un percorso valido, non serve cercarne altri
                    }
                }
            }
        }
    }

    /* --- 3. SIMMETRIZZAZIONE (Mutua Raggiungibilità) --- */
    // Il problema richiede che i e j siano nello stesso gruppo SOLO SE
    // i raggiunge j E j raggiunge i.
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Operatore && logico: entrambe le direzioni devono essere vere
            if (raggiungibile[i][j] && raggiungibile[j][i]) {
                connessioni[i][j] = true;
            } else {
                connessioni[i][j] = false;
            }
        }
    }

    /* --- 4. PREPARAZIONE RICORSIONE --- */
    int *assegnazioni = (int *)malloc(N * sizeof(int));
    // Inizializziamo a -1 (nessun gruppo assegnato)
    for(int i=0; i<N; i++) assegnazioni[i] = -1;

    Soluzione best;
    best.min_gruppi = N; // Worst case: ogni città in un gruppo separato

    // Lancio il solver
    trova_raggruppamenti(0, 0, N, assegnazioni, connessioni, &best);

    /* --- 5. GESTIONE DELLA MEMORIA (Free) --- */
    // Regola d'oro: Per ogni malloc, deve esserci una free.
    // Poiché abbiamo allocato matrici (array di array), dobbiamo liberare in ordine inverso.
    
    for (int i = 0; i < N; i++) {
        free(connessioni[i]);   // Libero le righe
        free(raggiungibile[i]); // Libero le righe
    }
    free(connessioni);   // Libero il vettore di puntatori
    free(raggiungibile); // Libero il vettore di puntatori
    free(assegnazioni);  // Libero l'array semplice

    return best.min_gruppi;
}

/* ============================================================================
 * AREA DI TEST (MAIN)
 * ============================================================================
 */
int main() {
    printf("--- TEST PARTIZIONAMENTO GRUPPI ---\n");

    int N = 4;
    
    // Allocazione Matrice Adiacenza Grafo (M)
    int **M = (int **)malloc(N * sizeof(int *));
    for(int i=0; i<N; i++) {
        M[i] = (int *)calloc(N, sizeof(int));
    }

    /*
     * CREAZIONE SCENARIO DI TEST
     * Grafo Lineare Bidirezionale: 0 <-> 1 <-> 2 <-> 3
     * Distanze (passaggi):
     * 0 -> 1 (1 passo) : OK
     * 0 -> 2 (2 passi: 0->1->2) : OK
     * 0 -> 3 (3 passi: 0->1->2->3) : NO (Max dist è 2)
     *
     * Gruppi attesi:
     * Non possiamo mettere 0,1,2,3 insieme perché 0 e 3 sono lontani.
     * Soluzione Ottima: 2 gruppi. Es: {0,1} e {2,3} oppure {0,1,2} e {3}.
     */
    
    // Connessioni bidirezionali (distanza 10 arbitraria > 0)
    M[0][1] = 10; M[1][0] = 10;
    M[1][2] = 10; M[2][1] = 10;
    M[2][3] = 10; M[3][2] = 10;

    printf("Calcolo in corso per N=%d citta'...\n", N);
    int risultato = calcola_minimo_gruppi(N, M);

    printf("Minimo numero di gruppi calcolato: %d\n", risultato);
    printf("Valore atteso: 2\n");

    if (risultato == 2) {
        printf(">>> TEST SUPERATO <<<\n");
    } else {
        printf(">>> TEST FALLITO <<<\n");
    }

    // Pulizia finale della matrice M allocata nel main
    for(int i=0; i<N; i++) free(M[i]);
    free(M);

    return 0;
}