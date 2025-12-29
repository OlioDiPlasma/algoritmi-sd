#include "esame.h"

/* * VARIABILE GLOBALE STATICA
 * "static" qui significa: questa variabile è visibile SOLO all'interno di questo file .c.
 * Serve per mantenere lo stato della "soluzione migliore trovata finora" attraverso
 * le varie chiamate ricorsive, senza doverla passare continuamente come parametro.
 */
static Soluzione sol_best; 

/* * =======================================================================================
 * FUNZIONE: trova_indice
 * =======================================================================================
 * Scopo: Convertire il nome di un oggetto (stringa) nel suo ID numerico (indice array).
 * Dettaglio Sintattico:
 * - strcmp(s1, s2): restituisce 0 se le stringhe sono identiche.
 * - objs.vettore[i].nome: Accedo alla struct Oggetti -> array vettore -> elemento i -> campo nome.
 */
int trova_indice(Oggetti objs, char *nome) {
    for (int i = 0; i < objs.n_oggetti; i++) {
        if (strcmp(objs.vettore[i].nome, nome) == 0) return i;
    }
    return -1; // Codice di errore standard se non trovato
}

/* * =======================================================================================
 * FUNZIONE: leggi_file
 * =======================================================================================
 * Scopo: Popolare le strutture dati leggendo da disco.
 * Parametri:
 * - fn_obj, fn_vinc: nomi dei file (stringhe)
 * - objs, vinc: PUNTATORI alle strutture da riempire (passaggio per riferimento).
 * Usiamo i puntatori perché la funzione deve modificare il contenuto
 * delle variabili dichiarate nel main.
 * * Gestione Memoria:
 * - malloc: Alloca memoria grezza.
 * - calloc: Alloca memoria E LA INIZIALIZZA A ZERO. Cruciale per la matrice di adiacenza.
 */
void leggi_file(char *fn_obj, char *fn_vinc, Oggetti *objs, Vincoli *vinc) {
    /* --- 1. APERTURA FILE OGGETTI --- */
    FILE *f = fopen(fn_obj, "r");
    if (!f) { printf("Errore apertura %s\n", fn_obj); exit(1); }

    /* --- 2. CONTEGGIO RIGHE (Per sapere quanta memoria allocare) --- */
    char buff[100];
    objs->n_oggetti = 0;
    // fgets legge una riga alla volta finché non finisce il file
    while(fgets(buff, 100, f)) objs->n_oggetti++;
    
    // IMPORTANTE: Riportiamo il puntatore del file all'inizio per rileggerlo
    rewind(f);

    /* --- 3. ALLOCAZIONE DINAMICA VETTORE --- */
    // objs è un puntatore, quindi uso la freccia '->' per accedere ai campi.
    // (Oggetto*) è un cast esplicito (buona norma in C).
    objs->vettore = (Oggetto*) malloc(objs->n_oggetti * sizeof(Oggetto));

    /* --- 4. PARSING DEI DATI --- */
    for (int i = 0; i < objs->n_oggetti; i++) {
        // Sintassi puntatori: &objs->vettore[i].costo
        // Passo l'INDIRIZZO (&) della variabile float perché fscanf deve scriverci dentro.
        // Per le stringhe (nome) non serve & perché il nome dell'array è già un indirizzo.
        fscanf(f, "%s %f %f %f", objs->vettore[i].nome, &objs->vettore[i].costo, &objs->vettore[i].peso, &objs->vettore[i].volume);
        objs->vettore[i].id = i; // Assegno l'ID sequenziale
    }
    fclose(f);

    /* --- 5. INIZIALIZZAZIONE MATRICE VINCOLI (Adjacency Matrix) --- */
    vinc->dim = objs->n_oggetti;
    
    // Allocazione delle RIGHE (array di puntatori a int)
    vinc->matrice = (int**) calloc(vinc->dim, sizeof(int*));
    
    for (int i = 0; i < vinc->dim; i++) {
        // Allocazione delle COLONNE per ogni riga (array di int)
        // calloc mette tutto a 0 (nessun vincolo inizialmente)
        vinc->matrice[i] = (int*) calloc(vinc->dim, sizeof(int));
        vinc->matrice[i][i] = 1; // Un oggetto è sempre vincolato a se stesso (auto-anello)
    }

    /* --- 6. LETTURA FILE VINCOLI --- */
    f = fopen(fn_vinc, "r");
    if (f) {
        char n1[MAX_STR], n2[MAX_STR];
        // Leggo coppie di stringhe
        while(fscanf(f, "%s %s", n1, n2) == 2) {
            int i1 = trova_indice(*objs, n1); // Dereferenzio *objs per passarlo per valore
            int i2 = trova_indice(*objs, n2);
            
            // Se entrambi gli oggetti esistono, segno 1 nella matrice simmetrica
            if (i1 != -1 && i2 != -1) {
                vinc->matrice[i1][i2] = 1;
                vinc->matrice[i2][i1] = 1; // Grafo non orientato: A-B implica B-A
            }
        }
        fclose(f);
    }
}

/* * =======================================================================================
 * FUNZIONE: dfs_gruppo (Ausiliaria per Domanda 6)
 * =======================================================================================
 * Scopo: Esegue una Visita in Profondità (DFS) sul grafo dei vincoli.
 * Serve a trovare tutti gli oggetti collegati a 'u' (Componente Connessa).
 * * Parametri:
 * - p_tot, v_tot: Puntatori a float. Servono per accumulare peso e volume totale
 * del gruppo man mano che visitiamo i nodi.
 * * Concetto Chiave: Ricorsione
 * Visitiamo u -> Troviamo v vicino -> Chiamiamo dfs(v) -> Troviamo z vicino...
 */
void dfs_gruppo(int u, int *visitati, Vincoli vinc, Oggetti objs, float *p_tot, float *v_tot) {
    visitati[u] = 1; // Marco nodo come "già visto" per evitare cicli infiniti
    
    // Accumulo i valori tramite dereferenziazione (*)
    *p_tot += objs.vettore[u].peso;
    *v_tot += objs.vettore[u].volume;

    // Scansiono la riga 'u' della matrice per trovare i vicini
    for (int v = 0; v < vinc.dim; v++) {
        // Se c'è un arco (1) e v non è stato visitato...
        if (vinc.matrice[u][v] == 1 && !visitati[v]) {
            dfs_gruppo(v, visitati, vinc, objs, p_tot, v_tot); // ...scendo in ricorsione
        }
    }
}

/* * =======================================================================================
 * FUNZIONE: verifica_validita (Domanda 6)
 * =======================================================================================
 * Scopo: Controlla se OGNI gruppo di oggetti vincolati può entrare nel camion.
 * Se un gruppo di oggetti indivisibili pesa troppo, il problema è impossibile.
 */
int verifica_validita(Oggetti objs, Vincoli vinc, float Pmax, float Vmax) {
    // Array di flag per tenere traccia dei nodi già processati
    int *visitati = (int*) calloc(objs.n_oggetti, sizeof(int));
    int valido = 1;

    for (int i = 0; i < objs.n_oggetti; i++) {
        if (!visitati[i]) {
            // Se trovo un nodo non visitato, è l'inizio di un nuovo gruppo
            float p_gruppo = 0, v_gruppo = 0;
            
            // Lancio la DFS per esplorare tutto il gruppo collegato a 'i'
            dfs_gruppo(i, visitati, vinc, objs, &p_gruppo, &v_gruppo);

            // Controllo "Hard": il gruppo supera la capacità?
            if (p_gruppo > Pmax || v_gruppo > Vmax) {
                valido = 0;
                break; // Inutile continuare, basta un gruppo non valido
            }
        }
    }
    free(visitati); // Mai dimenticare la free!
    return valido;
}

/* * =======================================================================================
 * FUNZIONE: valuta_soluzione (Ausiliaria per Domanda 7)
 * =======================================================================================
 * Scopo: Viene chiamata quando abbiamo una "Soluzione Completa" (tutti gli oggetti assegnati).
 * Calcola le metriche (n_giorni, delta_costo) e decide se è la migliore vista finora.
 */
void valuta_soluzione(int *sol_curr, int n_obj, Oggetti objs) {
    int n_giorni = 0;
    // 1. Calcolo quanti giorni sono stati usati (max index + 1)
    for(int i=0; i<n_obj; i++) if (sol_curr[i] + 1 > n_giorni) n_giorni = sol_curr[i] + 1;

    // 2. Calcolo costo per ogni giorno
    // Uso calloc per avere costi a 0.0 inizialmente
    float *costi = (float*) calloc(n_giorni, sizeof(float));
    for(int i=0; i<n_obj; i++) costi[sol_curr[i]] += objs.vettore[i].costo;

    // 3. Trovo Max e Min costo giornaliero
    float min_c = FLT_MAX, max_c = -1.0;
    for(int k=0; k<n_giorni; k++) {
        if (costi[k] < min_c) min_c = costi[k];
        if (costi[k] > max_c) max_c = costi[k];
    }
    float delta = max_c - min_c;
    free(costi);

    // 4. LOGICA DI AGGIORNAMENTO BEST (Gerarchica)
    int aggiorna = 0;
    if (!sol_best.valida) aggiorna = 1; // Prima soluzione valida trovata
    else if (n_giorni < sol_best.n_giorni_usati) aggiorna = 1; // Meno giorni è meglio
    else if (n_giorni == sol_best.n_giorni_usati && delta < sol_best.delta_costo) aggiorna = 1; // A parità di giorni, guardo il delta

    if (aggiorna) {
        sol_best.n_giorni_usati = n_giorni;
        sol_best.delta_costo = delta;
        sol_best.valida = 1;
        // memcpy è più veloce di un ciclo for per copiare array
        memcpy(sol_best.assegnazioni, sol_curr, n_obj * sizeof(int));
    }
}

/* * =======================================================================================
 * FUNZIONE: solve_recur (CORE ALGORITHM - Domanda 7)
 * =======================================================================================
 * Scopo: Algoritmo di Backtracking ricorsivo.
 * Concetto Chiave: Invece di provare a mettere oggetto per oggetto (che sarebbe lento),
 * proviamo a mettere "GRUPPI DI OGGETTI VINCOLATI" nei giorni.
 * * Parametri:
 * - gruppo_idx: Indice del gruppo che stiamo cercando di piazzare ora.
 * - gruppi_ids, dim_gruppi: Strutture dati che descrivono i gruppi (componenti connesse).
 * - sol_curr: Array che mantiene lo stato corrente delle assegnazioni.
 * - n_giorni_attuali: Quanti giorni (bin) abbiamo aperto finora.
 */
void solve_recur(int gruppo_idx, int n_gruppi, int **gruppi_ids, int *dim_gruppi, 
                 int *sol_curr, int n_giorni_attuali, 
                 Oggetti objs, float Pmax, float Vmax) {

    // --- CASO BASE ---
    // Abbiamo assegnato tutti i gruppi? Se sì, valutiamo la soluzione.
    if (gruppo_idx == n_gruppi) {
        valuta_soluzione(sol_curr, objs.n_oggetti, objs); 
        return;
    }

    // --- PRUNING (POTATURA) ---
    // Se stiamo usando già più giorni della soluzione migliore trovata finora,
    // è inutile continuare su questo ramo. Torniamo indietro.
    if (sol_best.valida && n_giorni_attuali > sol_best.n_giorni_usati) return;

    // Recupero gli ID degli oggetti che formano il gruppo corrente
    int *ids = gruppi_ids[gruppo_idx];
    int n_membri = dim_gruppi[gruppo_idx];
    
    // Pre-calcolo peso e volume totale di QUESTO gruppo (devono viaggiare insieme)
    float p_gruppo = 0, v_gruppo = 0;
    for(int i=0; i<n_membri; i++) {
        p_gruppo += objs.vettore[ids[i]].peso;
        v_gruppo += objs.vettore[ids[i]].volume;
    }

    // --- LOOP SULLE SCELTE POSSIBILI ---
    // Provo ad assegnare il gruppo ai giorni già aperti (0..n_giorni_attuali-1)
    // OPPURE apro un nuovo giorno (indice n_giorni_attuali).
    for (int g = 0; g <= n_giorni_attuali; g++) {
        
        // Calcolo quanto carico c'è già nel giorno 'g'
        float p_giorno = 0, v_giorno = 0;
        for (int i=0; i<objs.n_oggetti; i++) {
            if (sol_curr[i] == g) {
                p_giorno += objs.vettore[i].peso;
                v_giorno += objs.vettore[i].volume;
            }
        }

        // CONSTRAINT CHECK: Il gruppo ci sta nel camion insieme agli altri?
        if (p_giorno + p_gruppo <= Pmax && v_giorno + v_gruppo <= Vmax) {
            
            // 1. DO (Faccio la mossa): Assegno tutti i membri del gruppo al giorno g
            for(int i=0; i<n_membri; i++) sol_curr[ids[i]] = g;
            
            // Gestione del contatore giorni: se uso 'g' ed è uguale al max attuale, 
            // vuol dire che sto aprendo un nuovo camion.
            int nuovi_giorni = (g == n_giorni_attuali) ? n_giorni_attuali + 1 : n_giorni_attuali;
            
            // 2. RECURSE (Scendo in profondità)
            solve_recur(gruppo_idx + 1, n_gruppi, gruppi_ids, dim_gruppi, sol_curr, nuovi_giorni, objs, Pmax, Vmax);
            
            // 3. UNDO (Backtracking - Disfo la mossa):
            // Fondamentale! Devo riportare sol_curr allo stato precedente per
            // permettere al ciclo for di provare il giorno successivo (g+1).
            for(int i=0; i<n_membri; i++) sol_curr[ids[i]] = -1;
        }
    }
}

/* * =======================================================================================
 * FUNZIONE: calcola_ottimo (Entry Point Domanda 7)
 * =======================================================================================
 * Scopo: Prepara le strutture dati per la ricorsione ed esegue il preprocessing dei gruppi.
 */
Soluzione calcola_ottimo(Oggetti objs, Vincoli vinc, float Pmax, float Vmax) {
    // Inizializzazione della soluzione globale
    sol_best.assegnazioni = (int*) malloc(objs.n_oggetti * sizeof(int));
    sol_best.n_giorni_usati = objs.n_oggetti + 1; // Valore sentinella (pessimo)
    sol_best.delta_costo = FLT_MAX;
    sol_best.valida = 0;

    /* --- PREPROCESSING: IDENTIFICAZIONE GRUPPI CONNESSI --- */
    // Qui trasformiamo il grafo dei vincoli in una lista di gruppi.
    // Esempio: Se A-B e B-C, allora Gruppo 0 = {A,B,C}.
    
    int *visitati = (int*) calloc(objs.n_oggetti, sizeof(int));
    int **gruppi_ids = (int**) malloc(objs.n_oggetti * sizeof(int*));
    int *dim_gruppi = (int*) calloc(objs.n_oggetti, sizeof(int));
    int n_gruppi = 0;

    for(int i=0; i<objs.n_oggetti; i++) {
        if(!visitati[i]) {
            // Nuova componente connessa trovata. Uso una BFS (Breadth First Search) iterativa.
            int *buffer = (int*) malloc(objs.n_oggetti * sizeof(int));
            int count = 0;
            
            // Coda per la BFS
            int *q = (int*) malloc(objs.n_oggetti * sizeof(int));
            int head=0, tail=0;
            
            q[tail++] = i; // Enqueue
            visitati[i] = 1;
            
            while(head < tail) {
                int u = q[head++]; // Dequeue
                buffer[count++] = u; // Aggiungo al gruppo corrente
                
                // Cerco vicini non visitati
                for(int v=0; v<objs.n_oggetti; v++) {
                    if(vinc.matrice[u][v] && !visitati[v]) {
                        visitati[v] = 1;
                        q[tail++] = v;
                    }
                }
            }
            free(q);
            
            // Salvo il gruppo trovato
            gruppi_ids[n_gruppi] = buffer;
            dim_gruppi[n_gruppi] = count;
            n_gruppi++;
        }
    }
    free(visitati);

    /* --- LANCIO DELLA RICORSIONE --- */
    int *sol_curr = (int*) malloc(objs.n_oggetti * sizeof(int));
    for(int i=0; i<objs.n_oggetti; i++) sol_curr[i] = -1; // -1 = non assegnato

    // Parto dal Gruppo 0, Giorni usati 0.
    solve_recur(0, n_gruppi, gruppi_ids, dim_gruppi, sol_curr, 0, objs, Pmax, Vmax);

    /* --- PULIZIA MEMORIA TEMPORANEA --- */
    free(sol_curr);
    for(int i=0; i<n_gruppi; i++) free(gruppi_ids[i]);
    free(gruppi_ids);
    free(dim_gruppi);

    return sol_best;
}

// Libera tutta la memoria allocata nelle strutture principali
void free_tutto(Oggetti *o, Vincoli *v, Soluzione *s) {
    if (o->vettore) free(o->vettore);
    if (v->matrice) {
        for(int i=0; i<v->dim; i++) free(v->matrice[i]);
        free(v->matrice);
    }
    if (s->assegnazioni) free(s->assegnazioni);
}