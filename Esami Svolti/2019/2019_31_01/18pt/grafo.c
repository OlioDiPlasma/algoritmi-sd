/* grafo.c */
#include "grafo.h"

/* --- 1. GESTIONE BASE DEL GRAFO --- */

/*
 * Scopo: Crea un grafo vuoto pronto all'uso.
 * Memoria: Alloca la struttura principale.
 */
Grafo *inizializzaGrafo() {
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    if (g == NULL) { printf("Errore malloc grafo\n"); exit(1); }
    g->N = 0;
    g->nomi = NULL; // Alloceremo man mano o con realloc
    g->adj = NULL;
    return g;
}

/*
 * Scopo: Mappa una stringa ("nodoA") a un intero (0). 
 * Concetto Chiave: TABELLA DI SIMBOLI.
 * Se il nome esiste già, ritorna il suo indice.
 * Se è nuovo, espande le strutture dati (realloc) e lo aggiunge.
 */
int getIndice(Grafo *g, char *nome) {
    // 1. Cerca se esiste
    for (int i = 0; i < g->N; i++) {
        if (strcmp(g->nomi[i], nome) == 0) return i;
    }

    // 2. Se non esiste, aggiungi nuovo nodo
    int nuovoIndice = g->N;
    g->N++;

    // Espansione array nomi
    g->nomi = (char **)realloc(g->nomi, g->N * sizeof(char *));
    g->nomi[nuovoIndice] = strdup(nome); // strdup alloca e copia la stringa

    // Espansione Matrice di Adiacenza
    // Passo A: Espando le righe esistenti (aggiungo una colonna alla fine)
    // Passo B: Aggiungo una nuova riga completa
    // Per semplicità didattica: Riallochiamo tutto l'array di puntatori alle righe
    g->adj = (int **)realloc(g->adj, g->N * sizeof(int *));
    
    // Alloco la nuova riga
    g->adj[nuovoIndice] = (int *)calloc(g->N, sizeof(int)); // calloc inizializza a 0

    // Ora devo riallocare tutte le vecchie righe per aggiungere la nuova colonna
    for (int i = 0; i < nuovoIndice; i++) {
        g->adj[i] = (int *)realloc(g->adj[i], g->N * sizeof(int));
        g->adj[i][nuovoIndice] = 0; // Inizializzo la nuova cella a 0
    }

    return nuovoIndice;
}

/*
 * Scopo: Legge il file "idV1 idV2" e costruisce il grafo.
 */
Grafo *leggiGrafo(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Errore apertura %s\n", filename); return NULL; }

    Grafo *g = inizializzaGrafo();
    char u_str[MAX_STR], v_str[MAX_STR];

    // Legge coppie di stringhe fino a EOF
    while (fscanf(fp, "%s %s", u_str, v_str) == 2) {
        int u = getIndice(g, u_str);
        int v = getIndice(g, v_str);
        g->adj[u][v] = 1; // Arco orientato u -> v
    }

    fclose(fp);
    return g;
}

/*
 * Scopo: Pulisce tutta la memoria per evitare Memory Leaks.
 */
void liberaGrafo(Grafo *g) {
    if (!g) return;
    for (int i = 0; i < g->N; i++) {
        free(g->nomi[i]); // Libera stringa
        free(g->adj[i]);  // Libera riga matrice
    }
    free(g->nomi);
    free(g->adj);
    free(g);
}

/* --- 2. VERIFICA KERNEL (Punto 3) --- */

/*
 * Scopo: Funzione LOGICA pura. Verifica se un insieme di indici è Kernel.
 * Parametri:
 * - insieme: array di interi (indici dei nodi candidati)
 * - dim: quanti nodi ci sono nell'insieme
 */
int isKernel(Grafo *g, int *insieme, int dim) {
    // Creiamo un array di flag per accesso rapido O(1)
    // is_in_K[i] = 1 se il nodo i è nel Kernel proposto
    int *is_in_K = (int *)calloc(g->N, sizeof(int));
    for (int i = 0; i < dim; i++) is_in_K[insieme[i]] = 1;

    // CONDIZIONE 1: Indipendenza (Nessun arco tra nodi del Kernel)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (i == j) continue;
            int u = insieme[i];
            int v = insieme[j];
            // Se esiste arco u->v o v->u, fallisce
            if (g->adj[u][v] || g->adj[v][u]) {
                free(is_in_K); return 0;
            }
        }
    }

    // CONDIZIONE 2: Dominanza (Ogni nodo FUORI dal Kernel deve essere raggiunto da uno DENTRO)
    for (int u = 0; u < g->N; u++) {
        if (is_in_K[u] == 0) { // Se u non è nel Kernel
            int raggiunto = 0;
            for (int k_idx = 0; k_idx < dim; k_idx++) {
                int k = insieme[k_idx];
                if (g->adj[k][u]) { // Esiste arco da Kernel -> u ?
                    raggiunto = 1;
                    break;
                }
            }
            if (!raggiunto) {
                free(is_in_K); return 0; // Fallito: u non è dominato
            }
        }
    }

    free(is_in_K);
    return 1; // Successo
}

int verificaKernel(Grafo *g, char *fileCandidati) {
    FILE *fp = fopen(fileCandidati, "r");
    if (!fp) return 0;

    // Leggiamo i candidati in un array temporaneo
    int *candidati = malloc(g->N * sizeof(int));
    int dim = 0;
    char buff[MAX_STR];

    while (fscanf(fp, "%s", buff) == 1) {
        // Nota: dobbiamo trovare l'indice. Se il nodo non esiste nel grafo originale,
        // tecnicamente non può far parte del kernel. Qui assumiamo esistano.
        int idx = -1;
        for(int i=0; i<g->N; i++) {
            if(strcmp(g->nomi[i], buff)==0) idx = i;
        }
        if (idx != -1) candidati[dim++] = idx;
    }
    fclose(fp);

    int ris = isKernel(g, candidati, dim);
    free(candidati);
    return ris;
}

/* --- 3. KERNEL MINIMO (Punto 4) --- */

/*
 * Logica: Dobbiamo trovare il PIÙ PICCOLO insieme che sia un Kernel.
 * Approccio: Generiamo combinazioni di dimensione k=1, poi k=2, ecc.
 * Il primo che troviamo è garantito essere il minimo.
 * Useremo una funzione ricorsiva per generare combinazioni.
 */

// Variabili globali "static" per la ricorsione (visibili solo in questo file)
static int *bestK = NULL;
static int foundMin = 0;

void generaCombinazioni(Grafo *g, int *buffer, int bufIdx, int startIdx, int targetSize) {
    if (foundMin) return; // Ottimizzazione: se già trovato, esci

    if (bufIdx == targetSize) {
        // Abbiamo una combinazione di dimensione targetSize
        if (isKernel(g, buffer, targetSize)) {
            // Trovato! Poiché iteriamo dimensioni crescenti, è minimo.
            bestK = (int *)malloc(targetSize * sizeof(int));
            memcpy(bestK, buffer, targetSize * sizeof(int));
            foundMin = 1;
        }
        return;
    }

    for (int i = startIdx; i < g->N; i++) {
        buffer[bufIdx] = i;
        generaCombinazioni(g, buffer, bufIdx + 1, i + 1, targetSize);
        if (foundMin) return;
    }
}

void trovaEScriviKernelMinimo(Grafo *g, char *fileOutput) {
    int *buffer = malloc(g->N * sizeof(int));
    foundMin = 0;
    if(bestK) free(bestK); 
    bestK = NULL;

    int dimMinima = 0;
    // Provo tutte le dimensioni da 1 a N
    for (int k = 1; k <= g->N; k++) {
        generaCombinazioni(g, buffer, 0, 0, k);
        if (foundMin) {
            dimMinima = k;
            break;
        }
    }

    FILE *fp = fopen(fileOutput, "w");
    if (!fp) { perror("Errore scrittura"); free(buffer); return; }
    
    if (foundMin) {
        printf("Kernel Minimo trovato (dim %d): ", dimMinima);
        for (int i = 0; i < dimMinima; i++) {
            fprintf(fp, "%s\n", g->nomi[bestK[i]]);
            printf("%s ", g->nomi[bestK[i]]);
        }
        printf("\n");
        free(bestK);
    } else {
        fprintf(fp, "Nessun Kernel trovato\n");
    }
    fclose(fp);
    free(buffer);
}

/* --- 4. CAMMINO MASSIMO SU KERNEL (Punto 5) --- */

/*
 * Scopo: Trovare il cammino semplice (senza cicli) che tocca il maggior numero di nodi del Kernel.
 * Constraint: NO LIBRERIE. Quindi niente qsort o funzioni complesse, solo logica pura.
 */

// Variabili per la ricorsione DFS
static int maxKernelCount = -1;
static int lenForMaxKernel = 0;
static int *isNodeInK = NULL;   // Lookup rapido: 1 se il nodo i è nel kernel
static int *visited = NULL;     // Per evitare cicli (cammino semplice)

// DFS Ricorsiva
void dfsPath(Grafo *g, int u, int currentLen, int currentKCount) {
    visited[u] = 1; // Marca visitato
    
    int newKCount = currentKCount;
    if (isNodeInK[u]) newKCount++;

    // Aggiorna il massimo globale se necessario
    if (newKCount > maxKernelCount) {
        maxKernelCount = newKCount;
        lenForMaxKernel = currentLen; // Lunghezza in numero di archi (o nodi? Testo: "lunghezza cammino" solitamente archi, ma qui chiede "attraversa maggior numero". Usiamo archi).
        // Se currentLen conta i nodi, inizializzare a 1. Se archi, a 0.
        // Assumiamo currentLen = numero di archi.
    } else if (newKCount == maxKernelCount) {
        // Se parità di nodi kernel, prendo il cammino più lungo? Il testo non specifica, 
        // ma di solito "lunghezza del cammino... che attraversa".
        if (currentLen > lenForMaxKernel) {
            lenForMaxKernel = currentLen;
        }
    }

    // Esplora vicini
    for (int v = 0; v < g->N; v++) {
        if (g->adj[u][v] == 1 && !visited[v]) {
            dfsPath(g, v, currentLen + 1, newKCount);
        }
    }

    visited[u] = 0; // Backtrack: smarca per permettere altri percorsi
}

int calcolaMaxNodiKernel(Grafo *g, char *fileOutputPunto4) {
    // 1. Caricare il Kernel Minimo (o quello salvato al punto 4)
    // Dobbiamo sapere QUALI nodi sono nel Kernel per contarli.
    FILE *fp = fopen(fileOutputPunto4, "r");
    if (!fp) return 0;

    isNodeInK = (int *)calloc(g->N, sizeof(int));
    char buff[MAX_STR];
    while (fscanf(fp, "%s", buff) == 1) {
        for(int i=0; i<g->N; i++) {
            if(strcmp(g->nomi[i], buff)==0) isNodeInK[i] = 1;
        }
    }
    fclose(fp);

    // 2. Lanciare DFS da ogni nodo
    visited = (int *)calloc(g->N, sizeof(int));
    maxKernelCount = 0;
    lenForMaxKernel = 0;

    for (int i = 0; i < g->N; i++) {
        dfsPath(g, i, 0, 0); // start node, length 0, kernel count 0
    }

    free(visited);
    free(isNodeInK);

    // Ritorna il numero di nodi kernel attraversati (richiesto esplicitamente dal testo: 
    // "solo il numero di nodi del kernel che esso attraversa")
    // Aspetta, il punto 5 chiede: "calcolare la LUNGHEZZA del cammino... che attraversa il MAGGIOR numero di nodi kernel".
    // Quindi l'output è la LUNGHEZZA.
    return lenForMaxKernel; 
}