/*
 * File: grafo.c
 * Scopo: Implementazione delle funzioni di gestione grafo e algoritmo di ricerca.
 */

#include "grafo.h"

/* --- FUNZIONI DI SUPPORTO (Interne, non visibili dal main) --- */

/*
 * Converte stringa colore in enum.
 * Utile per evitare di fare strcmp ovunque.
 */
Colore leggiColore(char *s) {
    if (strcmp(s, "ROSSO") == 0) return ROSSO;
    if (strcmp(s, "NERO") == 0) return NERO;
    return NON_DEFINITO;
}

/*
 * Gestione Tabella Simboli (ST).
 * Cerca un nome nell'array. Se non c'è, lo aggiunge.
 * SE C'È MA IL COLORE È DIVERSO -> ERRORE CONGRUENZA.
 */
int aggiungiVertice(Grafo *g, char *id, Colore col) {
    int i;
    /* 1. Cerco se il vertice esiste già */
    for (i = 0; i < g->nV; i++) {
        if (strcmp(g->vett[i].id, id) == 0) {
            /* --- VERIFICA DI CONGRUENZA (Richiesta 1) --- */
            if (g->vett[i].col != col) {
                printf("ERRORE DI CONGRUENZA: Il vertice %s appare sia come ROSSO che come NERO!\n", id);
                /* In un caso reale uscirei con exit(1), qui ritorno -1 per gestire l'errore */
                return -1;
            }
            return i; /* Trovato e congruente */
        }
    }

    /* 2. Se non esiste, devo aggiungerlo. Controllo capacità array */
    /* Nota didattica: Raddoppio la dimensione se piena (realloc) */
    if (g->nV == g->capacity) {
        g->capacity *= 2;
        g->vett = realloc(g->vett, g->capacity * sizeof(Vertice));
        /* Inizializzo a NULL i nuovi head per sicurezza */
        for(int k=g->nV; k<g->capacity; k++) g->vett[k].head = NULL;
    }

    /* 3. Inserimento nuovo vertice */
    strcpy(g->vett[g->nV].id, id);
    g->vett[g->nV].col = col;
    g->vett[g->nV].head = NULL; /* Nessun arco uscente per ora */
    g->nV++;
    
    return g->nV - 1; /* Ritorno l'indice del nuovo inserito */
}

/* Aggiunge arco in testa alla lista di adiacenza */
void nuovoArco(Grafo *g, int src, int dest, int peso) {
    NodoArco *t = malloc(sizeof(NodoArco));
    t->v_index = dest;
    t->peso = peso;
    t->next = g->vett[src].head; /* Inserimento in testa */
    g->vett[src].head = t;
}

/* --- FUNZIONI PUBBLICHE --- */

/*
 * Scopo: Legge il file, riempie la struttura.
 * Parametri: Nome del file.
 * Concetti Chiave: Parsing stringhe, gestione puntatori a file, popolamento grafo non orientato.
 */
Grafo* caricaGrafo(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file.\n");
        return NULL;
    }

    /* Allocazione Struttura Grafo */
    Grafo *g = malloc(sizeof(Grafo));
    g->nV = 0;
    g->nE = 0;
    g->capacity = 10; /* Partiamo da 10, poi realloc se serve */
    g->vett = malloc(g->capacity * sizeof(Vertice));

    char id1[MAXC], id2[MAXC], col1Str[MAXC], col2Str[MAXC];
    int peso;
    
    /* --- 1. LETTURA CICLICA --- */
    /* Formato: idV1 colV1 val idV2 colV2 */
    while (fscanf(fp, "%s %s %d %s %s", id1, col1Str, &peso, id2, col2Str) == 5) {
        Colore c1 = leggiColore(col1Str);
        Colore c2 = leggiColore(col2Str);

        /* Ottengo indici e VERIFICO CONGRUENZA internamente */
        int u = aggiungiVertice(g, id1, c1);
        int v = aggiungiVertice(g, id2, c2);

        if (u == -1 || v == -1) {
            /* Errore congruenza rilevato */
            fclose(fp);
            /* Qui bisognerebbe liberare memoria già allocata, semplifico uscendo */
            exit(1); 
        }

        /* Il grafo è NON orientato, aggiungo arco in entrambe le direzioni */
        nuovoArco(g, u, v, peso);
        nuovoArco(g, v, u, peso);
        g->nE++;
    }

    fclose(fp);
    return g;
}

/* * Funzione ricorsiva (DFS) per il backtracking 
 * Parametri:
 * - g: puntatore al grafo
 * - curr: indice vertice corrente
 * - pesoAttuale: somma pesi finora
 * - visited: array booleano per evitare cicli (cammino semplice)
 * - bestWeight: puntatore a intero per salvare il massimo globale (passato per riferimento!)
 * - path/bestPath: array per memorizzare la sequenza
 * - depth: profondità ricorsione
 * - lenBest: lunghezza del cammino migliore
 */
void dfsMaxPath(Grafo *g, int curr, int pesoAttuale, int *visited, 
                int *bestWeight, int *path, int *bestPath, int depth, int *lenBest) {
    
    /* Aggiorno il percorso corrente */
    path[depth] = curr;

    /* Check se ho trovato un nuovo massimo */
    if (pesoAttuale > *bestWeight) {
        *bestWeight = pesoAttuale;
        *lenBest = depth + 1;
        /* Copio il percorso corrente in quello migliore */
        for(int i=0; i<=depth; i++) {
            bestPath[i] = path[i];
        }
    }

    visited[curr] = 1; /* Marco come visitato */

    /* Itero sui vicini */
    for (NodoArco *arco = g->vett[curr].head; arco != NULL; arco = arco->next) {
        int nextV = arco->v_index;
        
        /* --- LOGICA VINCOLI DEL PROBLEMA --- */
        
        /* 1. Vincolo Cammino Semplice: non devo averlo già visitato */
        if (visited[nextV]) continue;

        /* 2. Vincolo Colore:
         * "Un vertice ROSSO può essere seguito SOLO da un vertice NERO"
         * "Un vertice NERO può essere seguito da NERO o ROSSO" (nessun vincolo)
         *
         * Quindi l'unico caso vietato è: Corrente ROSSO -> Prossimo ROSSO
         */
        if (g->vett[curr].col == ROSSO && g->vett[nextV].col == ROSSO) {
            continue; /* Salto questo vicino, arco non valido */
        }

        /* Se passo i controlli, scendo in ricorsione */
        dfsMaxPath(g, nextV, pesoAttuale + arco->peso, visited, 
                   bestWeight, path, bestPath, depth + 1, lenBest);
    }

    /* Backtracking: smarco il vertice risalendo */
    visited[curr] = 0; 
}

/* Funzione wrapper per l'ottimizzazione */
void cercaCamminoMassimo(Grafo *g) {
    if (g->nV == 0) return;

    /* Allocazioni per il backtracking */
    int *visited = calloc(g->nV, sizeof(int));
    int *path = malloc(g->nV * sizeof(int));
    int *bestPath = malloc(g->nV * sizeof(int));
    
    int maxWeight = -1;
    int lenBest = 0;

    /* * Il problema non specifica un nodo di partenza.
     * Dobbiamo provare a lanciare la DFS da OGNI nodo possibile
     * per trovare il cammino massimo assoluto nel grafo.
     */
    printf("\nAvvio ricerca cammino massimo...\n");

    for (int i = 0; i < g->nV; i++) {
        /* Pulisco visited per ogni nuova partenza (anche se il backtracking lo pulisce,
           è buona norma resettare se ci sono componenti disconnesse) */
        for(int k=0; k<g->nV; k++) visited[k] = 0;
        
        /* Chiamata ricorsiva partendo dal nodo i */
        dfsMaxPath(g, i, 0, visited, &maxWeight, path, bestPath, 0, &lenBest);
    }

    /* Stampa Risultati */
    printf("--- RISULTATO OTTIMIZZAZIONE ---\n");
    if (maxWeight >= 0) {
        printf("Peso Massimo Trovato: %d\n", maxWeight);
        printf("Cammino: ");
        for (int i = 0; i < lenBest; i++) {
            int idx = bestPath[i];
            char *nome = g->vett[idx].id;
            char *colore = (g->vett[idx].col == ROSSO) ? "R" : "N";
            printf("%s(%s) ", nome, colore);
            if (i < lenBest - 1) printf("-> ");
        }
        printf("\n");
    } else {
        printf("Nessun cammino trovato.\n");
    }

    /* Pulizia memoria locale */
    free(visited);
    free(path);
    free(bestPath);
}

void liberaGrafo(Grafo *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->nV; i++) {
        NodoArco *tmp = g->vett[i].head;
        while (tmp != NULL) {
            NodoArco *aux = tmp;
            tmp = tmp->next;
            free(aux);
        }
    }
    free(g->vett);
    free(g);
}