#include "grafo.h"
#include <limits.h> /* Per INT_MAX */

/* =========================================================================
 * SEZIONE 1: GESTIONE DATI E MEMORIA
 * ========================================================================= */

/*
 * Scopo: Legge il file dei nodi e crea la struttura iniziale.
 * File di input: File testo con un nome per riga.
 * Parametri: 
 * - filename: percorso del file Nodi.txt
 * Concetti Chiave: 
 * - Realloc: Non sappiamo quanti nodi ci sono. Leggiamo e allarghiamo l'array.
 * - Strdup: Duplica la stringa letta allocando memoria automaticamente.
 */
Grafo *leggiNodi(char *filename) {
    FILE *f = fopen(filename, "r");
    char buffer[100]; /* Buffer temporaneo per leggere le righe */
    
    if (f == NULL) {
        printf("Errore apertura file nodi.\n");
        exit(1);
    }

    /* Allocazione della struttura "contenitore" del grafo */
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    g->N = 0;
    g->nomi = NULL; /* Inizialmente vuoto */
    g->mat = NULL;

    /* Lettura file */
    while (fscanf(f, "%s", buffer) == 1) {
        /*
         * Riallocazione dinamica dell'array di nomi.
         * (g->N + 1) * sizeof(char*) calcola la nuova dimensione necessaria.
         * Stiamo gestendo un array di PUNTATORI a char.
         */
        g->nomi = (char **)realloc(g->nomi, (g->N + 1) * sizeof(char *));
        
        /* * strdup alloca la memoria esatta per la stringa "buffer" e la copia.
         * E' fondamentale perche' buffer viene sovrascritto ad ogni giro.
         */
        g->nomi[g->N] = strdup(buffer);
        g->N++;
    }
    
    fclose(f);
    
    /* Ora che conosciamo N, possiamo allocare la matrice di adiacenza */
    inizializzaMatrice(g);
    
    return g;
}

/*
 * Scopo: Alloca una matrice quadrata N x N inizializzata a 0.
 * Parametri: g (puntatore al grafo)
 * Concetti Chiave: 
 * - Matrice dinamica: E' un array di puntatori (le righe), dove ogni puntatore
 * punta a un array di interi (le colonne).
 */
void inizializzaMatrice(Grafo *g) {
    int i, j;
    
    /* 1. Alloco l'array delle righe */
    g->mat = (int **)malloc(g->N * sizeof(int *));
    
    /* 2. Per ogni riga, alloco le colonne */
    for (i = 0; i < g->N; i++) {
        /* calloc inizializza direttamente a 0 */
        g->mat[i] = (int *)calloc(g->N, sizeof(int));
    }
}

/*
 * Scopo: Dato un nome, restituisce l'indice numerico (0...N-1).
 * Ritorna: indice o -1 se non trovato.
 */
int trovaIndice(Grafo *g, char *nome) {
    int i;
    for (i = 0; i < g->N; i++) {
        if (strcmp(g->nomi[i], nome) == 0) return i;
    }
    return -1;
}

/*
 * Scopo: Pulizia completa della memoria per evitare Memory Leaks.
 * Ordine di pulizia:
 * 1. Le stringhe dei nomi.
 * 2. L'array dei puntatori ai nomi.
 * 3. Le righe della matrice.
 * 4. L'array delle righe.
 * 5. La struttura Grafo stessa.
 */
void liberaGrafo(Grafo *g) {
    int i;
    if (g == NULL) return;

    /* Libera nomi */
    for (i = 0; i < g->N; i++) {
        free(g->nomi[i]);
    }
    free(g->nomi);

    /* Libera matrice */
    if (g->mat != NULL) {
        for (i = 0; i < g->N; i++) {
            free(g->mat[i]);
        }
        free(g->mat);
    }

    free(g);
}

/* =========================================================================
 * SEZIONE 2: FUNZIONI AUSILIARIE DI VERIFICA (Cuore Matematico)
 * ========================================================================= */

/*
 * Scopo: Controlla se il grafo e' CONNESSO (esiste un cammino tra ogni coppia).
 * Metodo: BFS (Breadth First Search) partendo dal nodo 0.
 * Se alla fine ho visitato N nodi, e' connesso.
 */
int checkConnesso(Grafo *g) {
    int *visitati = (int *)calloc(g->N, sizeof(int));
    int *coda = (int *)malloc(g->N * sizeof(int));
    int head = 0, tail = 0;
    int count = 0;
    int corrente, i;

    /* Parto dal nodo 0 */
    visitati[0] = 1;
    coda[tail++] = 0;
    count++;

    while (head < tail) {
        corrente = coda[head++];
        
        /* Controllo tutti i vicini */
        for (i = 0; i < g->N; i++) {
            if (g->mat[corrente][i] == 1 && visitati[i] == 0) {
                visitati[i] = 1;
                coda[tail++] = i;
                count++;
            }
        }
    }

    free(visitati);
    free(coda);
    
    return (count == g->N); /* 1 se vero, 0 se falso */
}

/*
 * Scopo: Controlla che nessun nodo superi grado m.
 */
int checkGradi(Grafo *g, int m) {
    int i, j, grado;
    for (i = 0; i < g->N; i++) {
        grado = 0;
        for (j = 0; j < g->N; j++) {
            if (g->mat[i][j]) grado++;
        }
        if (grado > m) return 0; /* Fallito */
    }
    return 1; /* Successo */
}

/*
 * Scopo: Calcola la distanza massima tra due nodi qualsiasi (Diametro).
 * Metodo: Algoritmo di Floyd-Warshall O(N^3).
 * Per N piccolo (es. < 100) e' perfetto e facile da scrivere.
 */
int checkDiametro(Grafo *g, int k) {
    int i, j, h;
    /* Matrice distanze locale */
    int **dist = (int **)malloc(g->N * sizeof(int *));
    for (i = 0; i < g->N; i++) dist[i] = (int *)malloc(g->N * sizeof(int));

    /* Inizializzazione Floyd-Warshall */
    for (i = 0; i < g->N; i++) {
        for (j = 0; j < g->N; j++) {
            if (i == j) 
                dist[i][j] = 0;
            else if (g->mat[i][j] == 1) 
                dist[i][j] = 1;
            else 
                dist[i][j] = g->N + 1; /* Infinito (o valore > k) */
        }
    }

    /* Core dell'algoritmo */
    for (h = 0; h < g->N; h++) {
        for (i = 0; i < g->N; i++) {
            for (j = 0; j < g->N; j++) {
                if (dist[i][h] + dist[h][j] < dist[i][j]) {
                    dist[i][j] = dist[i][h] + dist[h][j];
                }
            }
        }
    }

    /* Verifica vincolo k */
    int ok = 1;
    for (i = 0; i < g->N; i++) {
        for (j = 0; j < g->N; j++) {
            if (dist[i][j] > k) {
                ok = 0; /* Diametro troppo grande o nodi non connessi */
            }
        }
    }

    /* Pulizia memoria matrice locale */
    for (i = 0; i < g->N; i++) free(dist[i]);
    free(dist);

    return ok;
}

/* Funzione Wrapper che chiama tutti i controlli */
int verificaVincoli(Grafo *g, int k, int m) {
    if (!checkGradi(g, m)) return 0;
    if (!checkConnesso(g)) return 0;
    if (!checkDiametro(g, k)) return 0;
    return 1;
}

/* =========================================================================
 * SEZIONE 3: MODALITA' VERIFICA (Input File)
 * ========================================================================= */

void verificaSoluzione(Grafo *g, char *fileArchi, int k, int m) {
    FILE *f = fopen(fileArchi, "r");
    char n1[21], n2[21];
    int u, v;

    if (!f) {
        printf("Errore apertura file archi.\n");
        return;
    }

    /* Reset matrice (potrebbe essere sporca) */
    int i, j;
    for(i=0; i<g->N; i++) for(j=0; j<g->N; j++) g->mat[i][j] = 0;

    printf("\n--- INIZIO VERIFICA ---\n");
    /* Legge coppie di stringhe */
    while (fscanf(f, "%s %s", n1, n2) == 2) {
        u = trovaIndice(g, n1);
        v = trovaIndice(g, n2);
        
        if (u != -1 && v != -1) {
            /* Grafo non orientato: scrivo 1 in entrambe le direzioni */
            g->mat[u][v] = 1;
            g->mat[v][u] = 1;
        }
    }
    fclose(f);

    if (verificaVincoli(g, k, m)) {
        printf("La soluzione nel file e' VALIDA.\n");
    } else {
        printf("La soluzione nel file NON e' valida.\n");
    }
}

/* =========================================================================
 * SEZIONE 4: MODALITA' GENERAZIONE (Backtracking)
 * ========================================================================= */

/*
 * Logica:
 * Dobbiamo trovare il numero MINIMO di archi.
 * Un grafo connesso di N nodi ha minimo N-1 archi (albero).
 * Strategia:
 * 1. Proviamo a generare un grafo con (N-1) archi.
 * 2. Se troviamo una soluzione valida, e' ottima perche' minima. STOP.
 * 3. Se non troviamo nulla, proviamo con N archi, poi N+1...
 * * Usiamo il backtracking per selezionare "num_archi" da tutti i possibili archi.
 */

/* Variabile globale o statica per fermare la ricorsione appena trovo soluzione */
static int soluzioneTrovata = 0;

/*
 * Funzione ricorsiva di combinazioni semplici.
 * pos: indice nell'array dei "possibili archi"
 * count: quanti archi ho aggiunto finora
 * target: quanti archi devo aggiungere in totale
 * pool: array di tutti gli archi possibili
 * poolSize: quanti archi possibili esistono
 */
void combinaArchi(int pos, int count, int target, Arco *pool, int poolSize, Grafo *g, int k, int m) {
    
    /* Pruning: Se ho gia' trovato una soluzione altrove, esci subito */
    if (soluzioneTrovata) return;

    /* Base Case: Ho selezionato il numero target di archi */
    if (count == target) {
        /* Controllo se questa configurazione e' valida */
        if (verificaVincoli(g, k, m)) {
            soluzioneTrovata = 1; /* Trovato! Blocca tutto il resto */
        }
        return;
    }

    /* Base Case: Ho finito gli archi nel pool ma non ho raggiunto il target */
    if (pos == poolSize) return;

    /* * RICORSIONE - Modello "Prendo o Non Prendo"
     */

    int u = pool[pos].u;
    int v = pool[pos].v;

    /* --- RAMO 1: PRENDO l'arco pool[pos] --- */
    /* Check rapido: se aggiungendo questo arco supero il grado m per u o v, non lo metto */
    /* Nota: questo e' un pruning di ottimizzazione */
    int degU = 0, degV = 0, i;
    for(i=0; i<g->N; i++) { if(g->mat[u][i]) degU++; if(g->mat[v][i]) degV++; }
    
    if (degU < m && degV < m) {
        /* Do */
        g->mat[u][v] = 1;
        g->mat[v][u] = 1;
        
        /* Recurse */
        combinaArchi(pos + 1, count + 1, target, pool, poolSize, g, k, m);
        
        /* Undo (Backtrack) se non ho trovato la soluzione nel ramo profondo */
        if (!soluzioneTrovata) {
            g->mat[u][v] = 0;
            g->mat[v][u] = 0;
        } else {
            return; /* Se trovata, torno su mantenendo la matrice 'sporca' con la soluzione */
        }
    }

    /* --- RAMO 2: NON PRENDO l'arco pool[pos] --- */
    combinaArchi(pos + 1, count, target, pool, poolSize, g, k, m);
}

void generaSoluzioneOttima(Grafo *g, char *fileOutput, int k, int m) {
    int i, j;
    
    /* 1. Genero l'elenco di tutti gli archi possibili (Coppie uniche) */
    /* Massimo numero archi in grafo non orientato: N*(N-1)/2 */
    int maxArchi = g->N * (g->N - 1) / 2;
    Arco *pool = (Arco *)malloc(maxArchi * sizeof(Arco));
    int poolSize = 0;

    /* Reset matrice */
    for(i=0; i<g->N; i++) for(j=0; j<g->N; j++) g->mat[i][j] = 0;

    /* Riempio il pool con triangolo superiore della matrice */
    for (i = 0; i < g->N; i++) {
        for (j = i + 1; j < g->N; j++) {
            pool[poolSize].u = i;
            pool[poolSize].v = j;
            poolSize++;
        }
    }

    soluzioneTrovata = 0;
    
    printf("\n--- INIZIO GENERAZIONE OTTIMA ---\n");
    
    /* * Loop principale: Cerca soluzione con N-1 archi, poi N, poi N+1...
     * Garantisce la minimalita' del numero di archi.
     */
    for (int numArchi = g->N - 1; numArchi <= maxArchi; numArchi++) {
        printf("Provo a generare soluzione con %d archi...\n", numArchi);
        
        combinaArchi(0, 0, numArchi, pool, poolSize, g, k, m);
        
        if (soluzioneTrovata) {
            printf("Soluzione OTTIMA trovata con %d archi!\n", numArchi);
            
            /* Scrittura su file */
            FILE *fout = fopen(fileOutput, "w");
            if (fout) {
                /* Scorro la matrice per trovare gli archi selezionati */
                for (i = 0; i < g->N; i++) {
                    for (j = i + 1; j < g->N; j++) {
                        if (g->mat[i][j]) {
                            fprintf(fout, "%s %s\n", g->nomi[i], g->nomi[j]);
                            /* Stampo anche a video per debug */
                            printf("%s %s\n", g->nomi[i], g->nomi[j]);
                        }
                    }
                }
                fclose(fout);
                printf("Soluzione salvata in %s\n", fileOutput);
            } else {
                printf("Errore creazione file output.\n");
            }
            
            break; /* Esci dal loop for, ho trovato il minimo */
        }
    }

    if (!soluzioneTrovata) {
        printf("Nessuna soluzione trovata che rispetti i vincoli.\n");
    }

    free(pool);
}