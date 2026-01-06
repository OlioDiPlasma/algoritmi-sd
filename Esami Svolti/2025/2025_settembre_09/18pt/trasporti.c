#include "trasporti.h"

/* ============================================================================
 * FUNZIONE: verifica_validita (DOMANDA 6)
 * ============================================================================
 * Scopo:
 * Verifica se un insieme di persone può stare su un singolo minibus rispettando:
 * 1. Capienza massima persone (MP).
 * 2. Capienza massima valigie (MV).
 * 3. Vincoli "devono viaggiare insieme".
 *
 * Parametri:
 * - persone_nel_bus: array di indici delle persone caricate su questo bus.
 * - n_in_bus: quante persone sono in questo array.
 * - P, V: strutture dati con tutti i dati.
 * - MP, MV: limiti del bus.
 *
 * Concetti Chiave:
 * - Scansione Lineare: Controlliamo somme di pesi e volumi.
 * - Coerenza Vincoli: Se A è nel bus e ha un vincolo "stesso_bus" con B,
 * allora ANCHE B deve essere nel bus. Altrimenti la configurazione è invalida
 * perché abbiamo separato una coppia inseparabile.
 * ============================================================================
 */
int verifica_validita(int *persone_nel_bus, int n_in_bus, PersoneWrapper P, VincoliWrapper V, int MP, int MV) {
    
    /* --- 1. CONTROLLO CAPIENZA (MP e MV) --- */
    if (n_in_bus > MP) return 0; // Troppe persone

    int totale_valigie = 0;
    // Iteriamo sulle persone presenti nel bus
    for (int i = 0; i < n_in_bus; i++) {
        int id_curr = persone_nel_bus[i];
        totale_valigie += P.elenco[id_curr].num_valigie;
    }
    if (totale_valigie > MV) return 0; // Troppe valigie

    /* --- 2. CONTROLLO VINCOLI "STESSO BUS" --- */
    /*
     * Logica: Per ogni vincolo (A, B) con flag stesso_bus=1:
     * Se A è nel bus, dobbiamo verificare che anche B sia nel bus.
     * Nota: Questo controllo è costoso (O(N^2)) se fatto ingenuamente.
     * Qui usiamo un approccio didattico semplice.
     */

    // Creiamo un array di flag temporaneo per sapere velocemente chi è nel bus
    // calloc inizializza a 0.
    int *presenza = (int *)calloc(P.n_persone, sizeof(int));
    if (presenza == NULL) { printf("Errore malloc\n"); exit(1); }

    for(int i=0; i<n_in_bus; i++) {
        presenza[persone_nel_bus[i]] = 1; 
    }

    int valido = 1; // Assumiamo valido, proviamo a smentire
    for (int i = 0; i < V.n_vincoli; i++) {
        if (V.elenco[i].stesso_bus == 1) {
            int p1 = V.elenco[i].id_persona1;
            int p2 = V.elenco[i].id_persona2;

            // Caso di invalidità: Uno c'è, l'altro no.
            // XOR logico: (p1 c'è) != (p2 c'è)
            if (presenza[p1] != presenza[p2]) {
                valido = 0;
                break; // Usciamo subito
            }
        }
    }

    free(presenza); // Mai dimenticare di liberare la memoria di supporto!
    return valido;
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: calcola_costo_soluzione
 * ============================================================================
 * Scopo: Calcola i KM totali percorsi da una soluzione completa.
 * Logica:
 * Distanza Totale = (Somma distanze singole persone) - (Riduzioni per tratti comuni)
 * ============================================================================
 */
float calcola_costo_soluzione(Soluzione s, PersoneWrapper P, VincoliWrapper V) {
    float distanza = 0.0;
    
    // 1. Somma base: ogni persona aggiunge la sua distanza
    for (int i = 0; i < P.n_persone; i++) {
        distanza += P.elenco[i].distanza;
    }

    // 2. Sottrazione tratti comuni
    // Se due persone sono nello stesso bus (s.assegnazioni[p1] == s.assegnazioni[p2])
    // sottraiamo il tratto comune.
    for (int j = 0; j < V.n_vincoli; j++) {
        int p1 = V.elenco[j].id_persona1;
        int p2 = V.elenco[j].id_persona2;
        
        if (s.assegnazioni[p1] == s.assegnazioni[p2]) {
            // Sono sullo stesso bus, risparmiamo il tratto comune
            distanza -= V.elenco[j].tratto_comune;
        }
    }
    return distanza;
}

/* ============================================================================
 * CORE RICORSIVO (Modello: Partizioni di un Insieme)
 * ============================================================================
 */
void solve_recur(int livello, int n_persone, int n_bus_attuali, 
                 int *assegnazione_corrente, Soluzione *best_sol, 
                 PersoneWrapper P, VincoliWrapper V, int MP, int MV) {

    /* --- 1. CASO BASE --- */
    // Abbiamo assegnato un bus a tutte le persone (dalla 0 alla N-1)
    if (livello == n_persone) {
        
        // Calcoliamo i costi della soluzione trovata
        float dist_curr = calcola_costo_soluzione((Soluzione){assegnazione_corrente, n_bus_attuali, 0}, P, V);
        
        // Aggiorniamo il best se:
        // a) Usiamo MENO bus del record attuale
        // b) Usiamo GLI STESSI bus, ma con MENO distanza
        if (n_bus_attuali < best_sol->n_minibus_usati || 
           (n_bus_attuali == best_sol->n_minibus_usati && dist_curr < best_sol->distanza_totale)) {
            
            // Trovata soluzione migliore! Copiamo i dati.
            best_sol->n_minibus_usati = n_bus_attuali;
            best_sol->distanza_totale = dist_curr;
            // Copia profonda dell'array di assegnazioni
            for(int i=0; i<n_persone; i++) {
                best_sol->assegnazioni[i] = assegnazione_corrente[i];
            }
        }
        return;
    }

    /* --- 2. PRUNING (Ottimizzazione) --- */
    // Se stiamo già usando più bus del record attuale, è inutile continuare.
    if (n_bus_attuali > best_sol->n_minibus_usati) return;

    /* --- 3. PASSO RICORSIVO --- */
    // Tentiamo di mettere la persona 'livello' in uno dei bus già esistenti
    // O in un nuovo bus.

    int id_persona_corrente = livello; // Per chiarezza

    // A) Prova nei bus esistenti (0 ... n_bus_attuali - 1)
    for (int bus_idx = 0; bus_idx < n_bus_attuali; bus_idx++) {
        
        // Verifica capacità parziale
        // Dobbiamo contare quanti e quanto peso c'è GIA' in questo bus_idx
        int count = 0;
        int luggage = 0;
        // Check "Must be together": controlliamo se c'è qualcuno in ALTRI bus
        // che dovrebbe stare con me. Questo controllo è complesso in forward checking.
        // Per semplicità, faremo un check rigoroso sui vincoli "stesso bus"
        // SOLO alla fine o usando una verifica specifica qui.
        
        // Calcolo carico attuale del bus
        for(int i=0; i<livello; i++) {
            if (assegnazione_corrente[i] == bus_idx) {
                count++;
                luggage += P.elenco[i].num_valigie;
            }
        }

        // Se c'è spazio, proviamo l'assegnazione
        if (count < MP && (luggage + P.elenco[id_persona_corrente].num_valigie <= MV)) {
            
            // IMPORTANTE: Controllo Vincolo "STESSO BUS" (Look-back)
            // Se ho un vincolo con una persona PRECEDENTE (id < livello),
            // devo stare nel suo stesso bus.
            int violazione_vincolo = 0;
            for (int k=0; k<V.n_vincoli; k++) {
                // Se sono coinvolto in un vincolo "stesso bus"
                if (V.elenco[k].stesso_bus == 1) {
                    int altro_id = -1;
                    if (V.elenco[k].id_persona1 == id_persona_corrente) altro_id = V.elenco[k].id_persona2;
                    else if (V.elenco[k].id_persona2 == id_persona_corrente) altro_id = V.elenco[k].id_persona1;

                    // Se l'altra persona è già stata piazzata (id < livello)
                    if (altro_id != -1 && altro_id < livello) {
                        if (assegnazione_corrente[altro_id] != bus_idx) {
                            violazione_vincolo = 1; // Lui è altrove, non posso stare qui
                        }
                    }
                }
            }

            if (!violazione_vincolo) {
                assegnazione_corrente[id_persona_corrente] = bus_idx;
                solve_recur(livello + 1, n_persone, n_bus_attuali, assegnazione_corrente, best_sol, P, V, MP, MV);
                // Backtracking: non serve "cancellare" l'assegnazione perché verrà sovrascritta
            }
        }
    }

    // B) Prova in un NUOVO bus (bus_idx = n_bus_attuali)
    // Questo è sempre possibile per capacità (bus vuoto), ma devo controllare i vincoli "stesso bus"
    // con persone precedenti. Se devo stare con qualcuno precedente, NON posso aprire un nuovo bus da solo.
    int posso_nuovo_bus = 1;
    for (int k=0; k<V.n_vincoli; k++) {
        if (V.elenco[k].stesso_bus == 1) {
            int altro_id = -1;
            if (V.elenco[k].id_persona1 == id_persona_corrente) altro_id = V.elenco[k].id_persona2;
            else if (V.elenco[k].id_persona2 == id_persona_corrente) altro_id = V.elenco[k].id_persona1;

            if (altro_id != -1 && altro_id < livello) {
                // L'altro è già stato piazzato in un bus esistente. 
                // Quindi NON posso stare da solo in un nuovo bus.
                posso_nuovo_bus = 0;
            }
        }
    }

    if (posso_nuovo_bus) {
        assegnazione_corrente[id_persona_corrente] = n_bus_attuali; // Indice nuovo
        solve_recur(livello + 1, n_persone, n_bus_attuali + 1, assegnazione_corrente, best_sol, P, V, MP, MV);
    }
}

/* ============================================================================
 * FUNZIONE: calcola_distribuzione_ottima (DOMANDA 7)
 * ============================================================================
 * Wrapper per la funzione ricorsiva.
 */
void calcola_distribuzione_ottima(PersoneWrapper P, VincoliWrapper V, int MP, int MV, Soluzione *best_sol) {
    // Allocazione array di supporto per la ricorsione
    int *assegnazione_corrente = (int *)malloc(P.n_persone * sizeof(int));
    if (!assegnazione_corrente) exit(-1);

    // Inizializzazione best_sol con valori pessimi per trovare il minimo
    best_sol->n_minibus_usati = P.n_persone + 1; // Al massimo 1 persona per bus
    best_sol->distanza_totale = FLT_MAX;
    
    // Assegnazione iniziale (persona 0 nel bus 0) per rompere le simmetrie
    // Questo riduce drasticamente l'albero di ricorsione (N! invece di N^N)
    assegnazione_corrente[0] = 0;

    // Avvio ricorsione: partiamo dalla persona 1 (la 0 è fissata), con 1 bus usato
    solve_recur(1, P.n_persone, 1, assegnazione_corrente, best_sol, P, V, MP, MV);

    free(assegnazione_corrente);
}


/* --- FUNZIONI DI CARICAMENTO FILE (Implementazione Base) --- */

PersoneWrapper leggi_persone(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Errore file %s\n", filename); exit(1); }
    
    PersoneWrapper P;
    fscanf(f, "%d", &P.n_persone);
    P.elenco = (Persona *)malloc(P.n_persone * sizeof(Persona));

    char buffer[100];
    for(int i=0; i<P.n_persone; i++) {
        P.elenco[i].id = i;
        fscanf(f, "%s", buffer); // Nome
        P.elenco[i].nome = strdup(buffer);
        fscanf(f, "%s", buffer); // Indirizzo
        P.elenco[i].indirizzo = strdup(buffer);
        fscanf(f, "%d %f", &P.elenco[i].num_valigie, &P.elenco[i].distanza);
    }
    fclose(f);
    return P;
}

VincoliWrapper leggi_vincoli(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Errore file %s\n", filename); exit(1); }

    VincoliWrapper V;
    fscanf(f, "%d", &V.n_vincoli);
    V.elenco = (Vincolo *)malloc(V.n_vincoli * sizeof(Vincolo));

    for(int i=0; i<V.n_vincoli; i++) {
        fscanf(f, "%d %d %d %d %f", 
            &V.elenco[i].id_persona1, 
            &V.elenco[i].id_persona2,
            &V.elenco[i].stesso_indirizzo,
            &V.elenco[i].stesso_bus,
            &V.elenco[i].tratto_comune);
    }
    fclose(f);
    return V;
}

void libera_tutto(PersoneWrapper p, VincoliWrapper v, Soluzione s) {
    for(int i=0; i<p.n_persone; i++) {
        free(p.elenco[i].nome);
        free(p.elenco[i].indirizzo);
    }
    free(p.elenco);
    free(v.elenco);
    if(s.assegnazioni) free(s.assegnazioni);
}