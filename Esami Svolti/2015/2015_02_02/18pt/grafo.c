#include "grafo.h"

/* --- 1. GESTIONE STRUTTURA E MEMORIA --- */

/*
 * Scopo: Creare un grafo vuoto.
 * Parametri: Nessuno.
 * Concetti Chiave:
 * - Inizializziamo i puntatori a NULL per sicurezza.
 * - Usiamo una dimensione iniziale fissa per semplicita' didattica (es. 100), 
 * ma in produzione useremmo realloc.
 */
Grafo *iniziaGrafo() {
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    g->n_nodi = 0;
    g->max_nodi = 100; // Assumiamo max 100 amici per questo esercizio

    /* Allocazione array di stringhe (i nomi) */
    g->nomi = (char **)malloc(g->max_nodi * sizeof(char *));
    
    /* Allocazione Matrice di Adiacenza: righe */
    g->adj = (int **)malloc(g->max_nodi * sizeof(int *));
    
    /* Allocazione Matrice di Adiacenza: colonne */
    for (int i = 0; i < g->max_nodi; i++) {
        /* calloc inizializza a 0, utile per dire "nessuna amicizia" all'inizio */
        g->adj[i] = (int *)calloc(g->max_nodi, sizeof(int)); 
        g->nomi[i] = NULL;
    }

    return g;
}

/*
 * Scopo: Trova l'indice di un nome o lo aggiunge se nuovo.
 * Parametri: 
 * - Grafo *g: puntatore al grafo.
 * - char *nome: stringa da cercare.
 * Ritorna: L'indice (int) del nodo associato al nome.
 */
int aggiungiNodo(Grafo *g, char *nome) {
    /* Cerca se esiste gia' */
    for (int i = 0; i < g->n_nodi; i++) {
        if (strcmp(g->nomi[i], nome) == 0) {
            return i;
        }
    }
    
    /* Se non esiste, lo aggiungiamo */
    int indice = g->n_nodi;
    /* Allocazione stringa specifica */
    g->nomi[indice] = strdup(nome); 
    g->n_nodi++;
    return indice;
}

/*
 * Scopo: Libera tutta la memoria per evitare Memory Leaks.
 * Concetti Chiave: L'ordine di liberazione e' inverso all'allocazione.
 */
void liberaGrafo(Grafo *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->max_nodi; i++) {
        if (g->nomi[i] != NULL) free(g->nomi[i]); /* Libera la stringa del nome */
        if (g->adj[i] != NULL) free(g->adj[i]);   /* Libera la riga della matrice */
    }
    free(g->nomi);
    free(g->adj);
    free(g);
}

/* --- 2. LETTURA INPUT (Punto 1) --- */

/*
 * Scopo: Legge il file delle amicizie e popola il grafo.
 */
Grafo *leggiGrafo(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Errore apertura file grafo.\n");
        return NULL;
    }

    Grafo *g = iniziaGrafo();
    char n1[MAX_STR], n2[MAX_STR];

    /* Legge coppie di stringhe fino alla fine del file */
    while (fscanf(f, "%s %s", n1, n2) == 2) {
        int id1 = aggiungiNodo(g, n1);
        int id2 = aggiungiNodo(g, n2);

        /* Grafo non orientato: amicizia vale in entrambe le direzioni */
        g->adj[id1][id2] = 1;
        g->adj[id2][id1] = 1;
    }

    fclose(f);
    return g;
}

/* --- 3. VERIFICA SOLUZIONE (Punto 2) --- */

/*
 * Scopo: Verifica se la soluzione manuale rispetta le regole.
 * Regole:
 * 1. Ogni amico ha UNO e UN SOLO regalo (garantito dalla struttura dati array).
 * 2. Due amici (connessi nel grafo) non devono avere lo stesso regalo.
 * * Concetti Chiave:
 * - Creiamo un array 'assegnazioni' dove assegnazioni[i] = ID_regalo.
 */
void verificaSoluzione(Grafo *g, char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;

    int N_tipologie;
    fscanf(f, "%d", &N_tipologie);

    /* Array per memorizzare quale regalo ha ricevuto ogni persona 
       Inizializziamo a -1 (nessun regalo) */
    int *regali = (int *)malloc(g->n_nodi * sizeof(int));
    for(int i=0; i<g->n_nodi; i++) regali[i] = -1;

    /* Lettura soluzione proposta */
    int valido = 1;
    char buff[MAX_STR];
    
    /* Ciclo sulle N tipologie di regalo */
    for (int i = 0; i < N_tipologie; i++) {
        int k; /* Numero amici per questo regalo */
        fscanf(f, "%d", &k);
        for (int j = 0; j < k; j++) {
            fscanf(f, "%s", buff);
            int id = aggiungiNodo(g, buff); /* Recupera ID (non dovrebbe aggiungere se i file sono coerenti) */
            
            /* Controllo: ha gia' un regalo? */
            if (regali[id] != -1) {
                printf("Errore: %s ha ricevuto piu' di un regalo.\n", buff);
                valido = 0;
            }
            regali[id] = i; /* Assegna ID regalo 'i' */
        }
    }
    fclose(f);

    /* Controllo vincolo adiacenza (Graph Coloring Constraint) */
    if (valido) {
        /* Scorro la matrice di adiacenza superiore */
        for (int i = 0; i < g->n_nodi; i++) {
            for (int j = i + 1; j < g->n_nodi; j++) {
                /* Se sono amici (adj=1) E hanno lo stesso regalo */
                if (g->adj[i][j] == 1 && regali[i] == regali[j] && regali[i] != -1) {
                    valido = 0;
                    printf("Errore: Amici %s e %s hanno lo stesso regalo.\n", g->nomi[i], g->nomi[j]);
                }
            }
        }
    }

    /* Output richiesto */
    printf("\n--- PUNTO 2: VERIFICA ---\n");
    if (valido) {
        printf("La soluzione manuale soddisfa le condizioni.\n");
    } else {
        printf("La soluzione manuale NON soddisfa le condizioni.\n");
    }

    free(regali);
}

/* --- 4. OTTIMIZZAZIONE (Punto 3) --- */

/*
 * Funzione Ausiliaria Ricorsiva (Backtracking).
 * Scopo: Prova ad assegnare un colore al nodo 'v' usando max 'm' colori.
 */
int graphColoringUtil(Grafo *g, int m, int *colori, int v) {
    /* Caso Base: Se tutti i nodi sono colorati, abbiamo finito */
    if (v == g->n_nodi)
        return 1; // Successo

    /* Prova ad assegnare ogni colore da 1 a m al nodo v */
    for (int c = 1; c <= m; c++) {
        
        /* Check Validita': Nessun vicino deve avere il colore 'c' */
        int safe = 1;
        for (int i = 0; i < g->n_nodi; i++) {
            /* Se c'e' un arco E il vicino ha gia' quel colore */
            if (g->adj[v][i] && colori[i] == c) {
                safe = 0;
                break;
            }
        }

        /* Se e' sicuro assegnare il colore c */
        if (safe) {
            colori[v] = c;

            /* Passo ricorsivo: colora il resto dei nodi */
            if (graphColoringUtil(g, m, colori, v + 1) == 1)
                return 1; // Trovata soluzione nel ramo
            
            /* Backtracking: Se non porta a soluzione, rimuovi colore */
            colori[v] = 0;
        }
    }

    return 0; // Nessun colore ha funzionato
}

/*
 * Scopo: Trova il MINIMO numero di regali (numero cromatico).
 * Logica: Proviamo con 1 colore, poi 2, poi 3... il primo che funziona e' il minimo.
 */
void calcolaMinimoColori(Grafo *g) {
    int *colori = (int *)calloc(g->n_nodi, sizeof(int)); // Inizializza a 0
    int m = 1; // Numero colori da testare

    /* Continua ad incrementare m finche' non troviamo una soluzione valida */
    while (!graphColoringUtil(g, m, colori, 0)) {
        m++;
        /* Reset array colori per il prossimo tentativo */
        for(int k=0; k<g->n_nodi; k++) colori[k] = 0;
    }

    printf("\n--- PUNTO 3: OTTIMIZZAZIONE ---\n");
    printf("Numero minimo di tipologie di regali N = %d\n", m);
    printf("Partizionamento suggerito:\n");

    /* Stampa i gruppi (Partizioni) */
    /* Ciclo per ogni colore usato (da 1 a m) */
    for (int c = 1; c <= m; c++) {
        printf("{");
        int primo = 1;
        /* Trova tutti i nodi con questo colore */
        for (int i = 0; i < g->n_nodi; i++) {
            if (colori[i] == c) {
                if (!primo) printf(", ");
                printf("%s", g->nomi[i]);
                primo = 0;
            }
        }
        printf("} ");
    }
    printf("\n");

    free(colori);
}