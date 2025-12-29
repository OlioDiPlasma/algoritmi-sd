#include "grafo.h"

/* --- 1. GESTIONE TABELLA DI SIMBOLI --- */

/*
 * Scopo: Ottenere l'indice numerico di un vertice dato il suo nome.
 * Se il nome non esiste, lo aggiunge dinamicamente.
 * Parametri:
 * - st: puntatore alla tabella di simboli
 * - str: stringa con il nome del vertice
 * Ritorna: L'indice (int) associato alla stringa.
 */
int getIndex(SymbolTable *st, char *str) {
    int i;
    /* Cerchiamo se il nome esiste gia' */
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->nomi[i], str) == 0)
            return i;
    }

    /* Se non esiste, dobbiamo aggiungerlo.
     * Concetto chiave: REALLOC
     * Espandiamo l'array di puntatori a stringhe di 1 unita'.
     */
    st->size++;
    st->nomi = (char **)realloc(st->nomi, st->size * sizeof(char *));
    
    /* Allocazione della singola stringa */
    st->nomi[st->size - 1] = strdup(str); 

    return st->size - 1; /* Ritorna il nuovo indice */
}

/* --- 2. GESTIONE GRAFO (Allocazione e Caricamento) --- */

Graph* initGraph() {
    Graph *G = (Graph *)malloc(sizeof(Graph));
    G->V = 0;
    G->E = 0;
    G->ladj = NULL; /* Sara' allocato quando sapremo V finale o dinamicamente */
    G->st.nomi = NULL;
    G->st.size = 0;
    return G;
}

/*
 * Scopo: Legge il file, popola la ST e crea le liste di adiacenza.
 * Concetto chiave: Poiche' non sappiamo V all'inizio, leggiamo prima tutto
 * per costruire la ST, e poi creiamo gli archi. Oppure (piu' complesso)
 * ridimensioniamo la ladj dinamicamente. Qui usiamo un approccio misto:
 * siccome V non e' noto, usiamo la getIndex per contare V mentre leggiamo.
 * Nota: Questo approccio richiede di rileggere il file o memorizzare gli archi temporaneamente.
 * Per semplicita' didattica: Facciamo due passaggi sul file.
 */
void loadGraph(Graph *G, FILE *fp) {
    char id1[MAX_STR], id2[MAX_STR];
    int u, v;

    /* PASSO 1: Popolamento Tabella Simboli (Solo lettura vertici) */
    while (fscanf(fp, "%s %s", id1, id2) == 2) {
        getIndex(&G->st, id1);
        getIndex(&G->st, id2);
    }

    /* Ora sappiamo quanti vertici ci sono */
    G->V = G->st.size;

    /* ALLOCAZIONE LISTE DI ADIACENZA */
    /* Calloc inizializza tutto a NULL, fondamentale per le liste */
    G->ladj = (Node **)calloc(G->V, sizeof(Node *));

    /* PASSO 2: Creazione Archi (Riavvolgiamo il file) */
    rewind(fp);
    while (fscanf(fp, "%s %s", id1, id2) == 2) {
        u = getIndex(&G->st, id1); /* Recupera indice gia' esistente */
        v = getIndex(&G->st, id2);

        /* Inserimento in testa per u -> v (Grafo non orientato: anche v -> u) */
        /* Arco u-v */
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->v = v;
        newNode->next = G->ladj[u];
        G->ladj[u] = newNode;

        /* Arco v-u */
        newNode = (Node *)malloc(sizeof(Node));
        newNode->v = u;
        newNode->next = G->ladj[v];
        G->ladj[v] = newNode;
        
        G->E++;
    }
}

/* Funzione di utilita' per debuggare */
void printGraph(Graph *G) {
    int i;
    Node *x;
    printf("--- Visualizzazione Grafo ---\n");
    for (i = 0; i < G->V; i++) {
        printf("Vertice %s (%d):", G->st.nomi[i], i);
        for (x = G->ladj[i]; x != NULL; x = x->next) {
            printf(" -> %s", G->st.nomi[x->v]);
        }
        printf("\n");
    }
    printf("-----------------------------\n");
}

/* --- 3. VERIFICA INSIEME INDIPENDENTE (Richiesta Esercizio) --- */

/*
 * Scopo: Verifica se i vertici nel file formano un Independent Set.
 * Algoritmo:
 * 1. Legge i vertici dal file e li converte in indici.
 * 2. Li marca in un array 'presenza' (bool array).
 * 3. Per ogni vertice 'u' nel set, controlla i suoi vicini nel grafo.
 * Se un vicino 'v' e' anch'esso marcato come presente nel set -> NON e' indipendente.
 */
int verificaIndipendente(Graph *G, FILE *fp) {
    char nome[MAX_STR];
    int *inSet = (int *)calloc(G->V, sizeof(int)); /* Array di flag: 1 se nel set, 0 altrimenti */
    int *listaVertici = (int *)malloc(G->V * sizeof(int)); /* Per iterare solo su quelli del set */
    int count = 0, i, u, v_adj;
    Node *x;

    /* Lettura file vertici */
    while (fscanf(fp, "%s", nome) == 1) {
        int idx = getIndex(&G->st, nome); /* Nota: se il vertice non esiste nel grafo originale, getIndex lo aggiungerebbe. 
                                             In un caso reale dovremmo gestire l'errore, qui assumiamo input coerenti */
        if (idx < G->V) { /* Safety check */
            inSet[idx] = 1;
            listaVertici[count++] = idx;
        }
    }

    /* Verifica */
    for (i = 0; i < count; i++) {
        u = listaVertici[i];
        /* Scorri i vicini di u nel grafo originale */
        for (x = G->ladj[u]; x != NULL; x = x->next) {
            v_adj = x->v;
            /* Se anche il vicino e' nel set, abbiamo un arco interno -> FAIL */
            if (inSet[v_adj]) {
                printf("Conflitto trovato tra %s e %s\n", G->st.nomi[u], G->st.nomi[v_adj]);
                free(inSet);
                free(listaVertici);
                return 0; /* Falso */
            }
        }
    }

    free(inSet);
    free(listaVertici);
    return 1; /* Vero */
}

/* --- 4. IDENTIFICAZIONE INSIEME INDIPENDENTE MASSIMALE (Ottimizzazione) --- */

/*
 * Scopo: Trovare un insieme massimale (Greedy).
 * Algoritmo Greedy:
 * 1. Scorri tutti i vertici del grafo.
 * 2. Se un vertice non e' "bloccato", aggiungilo all'IS.
 * 3. Appena aggiunto, "blocca" tutti i suoi vicini (non possono essere presi).
 * 4. Ripeti.
 * Ritorna: Cardinalita' dell'insieme trovato.
 */
int trovaInsiemeMassimale(Graph *G, char *outputFilename) {
    int *isIndependent = (int *)calloc(G->V, sizeof(int)); /* 1 se scelto, 0 no */
    int *blocked = (int *)calloc(G->V, sizeof(int));       /* 1 se adiacente a uno scelto */
    int i, v_adj, cardinalita = 0;
    Node *x;
    FILE *fout;

    /* Logica Greedy */
    for (i = 0; i < G->V; i++) {
        if (!blocked[i]) {
            /* Prendo il nodo i */
            isIndependent[i] = 1;
            cardinalita++;
            
            /* Blocco i vicini */
            for (x = G->ladj[i]; x != NULL; x = x->next) {
                v_adj = x->v;
                blocked[v_adj] = 1; 
            }
        }
    }

    /* Scrittura su file */
    fout = fopen(outputFilename, "w");
    if (fout == NULL) {
        printf("Errore apertura file output.\n");
        return -1;
    }

    for (i = 0; i < G->V; i++) {
        if (isIndependent[i]) {
            fprintf(fout, "%s\n", G->st.nomi[i]);
        }
    }
    fclose(fout);

    /* Pulizia */
    free(isIndependent);
    free(blocked);

    return cardinalita;
}

/* --- 5. PULIZIA MEMORIA --- */
void freeGraph(Graph *G) {
    int i;
    Node *tmp;
    if (G == NULL) return;

    /* Libera le liste */
    for (i = 0; i < G->V; i++) {
        while (G->ladj[i] != NULL) {
            tmp = G->ladj[i];
            G->ladj[i] = G->ladj[i]->next;
            free(tmp);
        }
    }
    free(G->ladj);

    /* Libera Symbol Table */
    for (i = 0; i < G->st.size; i++) {
        free(G->st.nomi[i]);
    }
    free(G->st.nomi);

    free(G);
}