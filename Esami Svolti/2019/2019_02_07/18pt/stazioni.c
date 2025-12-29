#include "stazioni.h"

/* ==================================================================================
 * SEZIONE 1: GESTIONE DATI E MEMORIA
 * ================================================================================== */

/*
 * Scopo: Leggere il file principale e allocare dinamicamente le strutture.
 * Parametri:
 * - filename: nome del file dati.
 * Concetti Chiave:
 * - Malloc matrice: Allocare un array di puntatori (righe) e poi per ogni riga un array di interi.
 */
Mappa caricaDati(const char *filename) {
    FILE *f = fopen(filename, "r");
    Mappa m;
    m.N = 0; 
    
    if (f == NULL) {
        printf("Errore apertura file %s\n", filename);
        return m;
    }

    // Lettura N
    fscanf(f, "%d", &m.N);

    // Allocazione Vettori
    // sizeof(int) ci dà la dimensione in byte di un intero. Moltiplichiamo per N.
    m.pop = (int *)malloc(m.N * sizeof(int));
    m.stazComune = (int *)malloc(m.N * sizeof(int));
    m.dist = (int **)malloc(m.N * sizeof(int *)); // Array di puntatori alle righe

    // Lettura Popolazione
    for (int i = 0; i < m.N; i++) fscanf(f, "%d", &m.pop[i]);

    // Lettura Matrice e allocazione righe
    for (int i = 0; i < m.N; i++) {
        m.dist[i] = (int *)malloc(m.N * sizeof(int)); // Alloco la singola riga
        for (int j = 0; j < m.N; j++) {
            fscanf(f, "%d", &m.dist[i][j]);
        }
    }

    // Lettura stazComune (per Obj 2)
    for (int i = 0; i < m.N; i++) fscanf(f, "%d", &m.stazComune[i]);

    fclose(f);
    return m;
}

/*
 * Scopo: Pulire la memoria heap per evitare memory leaks.
 * Concetto: L'ordine di "free" è inverso all'ordine di "malloc". Prima le righe, poi il vettore di puntatori.
 */
void liberaMappa(Mappa m) {
    free(m.pop);
    free(m.stazComune);
    for (int i = 0; i < m.N; i++) {
        free(m.dist[i]);
    }
    free(m.dist);
}

/* ==================================================================================
 * SEZIONE 2: VERIFICA PROPOSTA (Requisito 1)
 * ================================================================================== */

/*
 * Scopo: Verifica se una soluzione letta da file rispetta la "distMax".
 * Logica:
 * 1. Legge quali comuni hanno una stazione.
 * 2. Per ogni comune della mappa, cerca la stazione più vicina.
 * 3. Se la distanza minima > distMax, la verifica fallisce.
 */
int verificaProposta(const char *filename, Mappa m, int distMax) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    int nStazProposte;
    fscanf(f, "%d", &nStazProposte);
    
    int *proposta = (int*)malloc(nStazProposte * sizeof(int));
    for(int i=0; i<nStazProposte; i++) {
        fscanf(f, "%d", &proposta[i]);
    }
    fclose(f);

    printf("\n[VERIFICA] Controllo copertura con distMax=%d...\n", distMax);
    
    // Algoritmo di verifica
    int tuttoCoperto = 1; // Flag booleano

    for (int i = 0; i < m.N; i++) { // Per ogni comune i
        int distMin = INT_MAX;
        
        // Trova la distanza alla stazione più vicina tra quelle proposte
        for (int k = 0; k < nStazProposte; k++) {
            int stazIndex = proposta[k];
            if (m.dist[i][stazIndex] < distMin) {
                distMin = m.dist[i][stazIndex];
            }
        }

        if (distMin > distMax) {
            printf(" -> Comune %d NON coperto (DistMin: %d > %d)\n", i, distMin, distMax);
            tuttoCoperto = 0;
            // Non faccio break perché potrei voler vedere tutti gli errori, 
            // ma per efficienza potrei fermarmi qui.
        }
    }

    free(proposta);
    return tuttoCoperto;
}

/* ==================================================================================
 * SEZIONE 3: OBIETTIVO 1 - MINIMIZZAZIONE STAZIONI (Set Cover)
 * ================================================================================== */

/*
 * Funzione ausiliaria per verificare la copertura di una soluzione parziale/totale
 * Restituisce 1 se tutti coperti, 0 altrimenti.
 */
int checkCopertura(Mappa m, int *soluzione, int distMax) {
    for (int i = 0; i < m.N; i++) {
        int coperto = 0;
        // Controllo se il comune i è coperto da almeno una stazione attivata in 'soluzione'
        for (int j = 0; j < m.N; j++) {
            if (soluzione[j] == 1) { // Se c'è una stazione in j
                if (m.dist[i][j] <= distMax) {
                    coperto = 1;
                    break;
                }
            }
        }
        if (!coperto) return 0;
    }
    return 1;
}

/*
 * Funzione Ricorsiva Obj 1 (Wrapper + Core)
 * Strategia: Generiamo le combinazioni (Power Set).
 * Stato Ricorsione:
 * - livello: comune attuale che stiamo decidendo (mettere o no stazione).
 * - count: quante stazioni abbiamo messo finora.
 * - bestCount: puntatore al miglior numero trovato finora (per Pruning).
 */
void recObj1(Mappa m, int livello, int *soluzione, int count, int *bestCount, int *bestSol, int distMax) {
    
    // PRUNING 1: Se ho già messo più stazioni della soluzione migliore trovata, mi fermo.
    // Non ha senso continuare, cerco il minimo.
    if (count >= *bestCount) return;

    // BASE CASE: Ho deciso per tutti i comuni (livello == N)
    if (livello == m.N) {
        // Verifico se questa configurazione copre tutti
        if (checkCopertura(m, soluzione, distMax)) {
            // Se copre ed è arrivata qui, grazie al Pruning 1 sappiamo che count < bestCount
            *bestCount = count;
            // Salvo la soluzione migliore
            for (int i = 0; i < m.N; i++) bestSol[i] = soluzione[i];
        }
        return;
    }

    // PASSO RICORSIVO
    
    // Opzione A: NON metto la stazione nel comune 'livello'
    soluzione[livello] = 0;
    recObj1(m, livello + 1, soluzione, count, bestCount, bestSol, distMax);

    // Opzione B: METTO la stazione nel comune 'livello'
    soluzione[livello] = 1;
    recObj1(m, livello + 1, soluzione, count + 1, bestCount, bestSol, distMax);
    
    // Backtracking: rimetto a 0 per pulizia (anche se sovrascritto al giro dopo)
    soluzione[livello] = 0; 
}

void risolviObiettivo1(Mappa m, int distMax) {
    int *soluzione = (int*)calloc(m.N, sizeof(int)); // Vettore temporaneo 0/1
    int *bestSol = (int*)calloc(m.N, sizeof(int));   // Vettore per salvare la best
    int bestCount = m.N + 1; // Inizializzo con valore pessimo (più grande del max possibile)

    printf("\n[OBIETTIVO 1] Calcolo configurazione ottima (DistMax: %d)...\n", distMax);

    recObj1(m, 0, soluzione, 0, &bestCount, bestSol, distMax);

    printf("Minimo stazioni necessarie: %d\n", bestCount);
    printf("Localizzazione: ");
    for(int i=0; i<m.N; i++) {
        if(bestSol[i]) printf("%d ", i);
    }
    printf("\n");

    free(soluzione);
    free(bestSol);
}

/* ==================================================================================
 * SEZIONE 4: OBIETTIVO 2 - MINIMIZZAZIONE FUNZIONE DI COSTO
 * ================================================================================== */

/*
 * Calcolo della formula complessa specificata nel testo.
 * Costo = sum( pop[i] * distMin[i] / numStazNelComunePiùVicino )
 */
double calcolaFunzioneObiettivo(Mappa m, int *stazioni) {
    double costoTotale = 0.0;

    for (int i = 0; i < m.N; i++) {
        int minDist = INT_MAX;
        int idStazionePiuVicina = -1;

        // 1. Trovo la distanza minima e l'ID del comune stazione più vicino
        for (int j = 0; j < m.N; j++) {
            if (stazioni[j] > 0) { // Se nel comune j ci sono stazioni
                if (m.dist[i][j] < minDist) {
                    minDist = m.dist[i][j];
                    idStazionePiuVicina = j;
                }
                // Nota: In caso di parità di distanza, la scelta del nodo potrebbe influenzare
                // il divisore (numero stazioni). Il testo non specifica la tie-breaking rule.
                // Assumiamo prenda il primo trovato o si debba gestire.
                // Qui prendiamo il primo che migliora strettamente.
            }
        }

        if (idStazionePiuVicina == -1) return 1e9; // Caso impossibile (nessuna stazione)

        // Applico formula: pop * dist / numStazioniInQuelNodo
        // Casting a double per la divisione
        double termine = (double)(m.pop[i] * minDist) / stazioni[idStazionePiuVicina];
        costoTotale += termine;
    }
    return costoTotale;
}

/*
 * Funzione Ricorsiva Obj 2
 * Problema: Distribuire un numero fisso di risorse (numStazTotali) nei contenitori (Comuni),
 * rispettando la capienza massima di ogni contenitore (stazComune[i]).
 * * Parametri:
 * - livello: Indice del comune che stiamo considerando.
 * - stazRimanenti: Quante stazioni devo ancora piazzare.
 */
void recObj2(Mappa m, int livello, int stazRimanenti, int *configurazioneAttuale, double *minCosto, int *bestConfig) {
    
    // PRUNING: Se sono finiti i comuni ma ho ancora stazioni in mano, è un ramo invalido.
    if (livello == m.N) {
        if (stazRimanenti == 0) {
            // Calcolo costo solo a foglia completa
            double costo = calcolaFunzioneObiettivo(m, configurazioneAttuale);
            if (costo < *minCosto) {
                *minCosto = costo;
                for(int i=0; i<m.N; i++) bestConfig[i] = configurazioneAttuale[i];
            }
        }
        return;
    }

    // Loop decisionale: quante stazioni metto nel comune 'livello'?
    // Posso metterne da 0 fino al massimo consentito (m.stazComune[livello]),
    // ma non più di quante me ne rimangono.
    int maxPossibili = m.stazComune[livello];
    if (stazRimanenti < maxPossibili) maxPossibili = stazRimanenti;

    for (int k = 0; k <= maxPossibili; k++) {
        configurazioneAttuale[livello] = k; // Assegno k stazioni qui
        
        // Passo al prossimo comune, sottraendo k dalle stazioni disponibili
        recObj2(m, livello + 1, stazRimanenti - k, configurazioneAttuale, minCosto, bestConfig);
    }
    
    configurazioneAttuale[livello] = 0; // Backtrack
}

void risolviObiettivo2(Mappa m, int numStazTotali) {
    int *configurazione = (int*)calloc(m.N, sizeof(int));
    int *bestConfig = (int*)calloc(m.N, sizeof(int));
    double minCosto = 1e9; // Valore sentinella molto alto

    printf("\n[OBIETTIVO 2] Ottimizzazione costo pesato con %d stazioni...\n", numStazTotali);

    recObj2(m, 0, numStazTotali, configurazione, &minCosto, bestConfig);

    if (minCosto >= 1e9) {
        printf("Impossibile allocare le stazioni rispettando i vincoli.\n");
    } else {
        printf("Costo Minimo: %.2f\n", minCosto);
        printf("Allocazione: ");
        for(int i=0; i<m.N; i++) {
            if (bestConfig[i] > 0) 
                printf("Comune %d (%d staz) ", i, bestConfig[i]);
        }
        printf("\n");
        
        // Verifica calcolo esempio (per debug didattico)
        // printf("Debug check formula sol(2,3): %f\n", minCosto); 
    }

    free(configurazione);
    free(bestConfig);
}