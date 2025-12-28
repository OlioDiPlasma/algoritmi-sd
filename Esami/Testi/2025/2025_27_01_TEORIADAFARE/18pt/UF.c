#include <stdlib.h>
#include "UF.h"

/* --- STRUTTURA INTERNA --- */
/*
   Utilizziamo due array per l'algoritmo Quick Union pesato:
   - id[]: il genitore del nodo
   - sz[]: la dimensione dell'albero radicato nel nodo
   Inoltre, serve uno stack per il backtrack. Salviamo quale nodo è stato
   attaccato a quale altro.
*/

typedef struct {
    int child;  // Chi è stato spostato
    int parent; // A chi è stato attaccato
} HistoryItem;

struct check_union_find {
    int *id;
    int *sz;
    int N;
    /* Stack per il backtrack */
    HistoryItem *history; 
    int historyIndex;     // Puntatore alla testa dello stack
};

/* --- 1. ALLOCAZIONE E INIZIALIZZAZIONE --- */
UF UFinit(int N) {
    int i;
    UF uf = malloc(sizeof(*uf));
    uf->N = N;
    uf->id = malloc(N * sizeof(int));
    uf->sz = malloc(N * sizeof(int));
    /* Allocazione stack: al massimo faremo N-1 union */
    uf->history = malloc(N * sizeof(HistoryItem)); 
    uf->historyIndex = 0;

    for (i = 0; i < N; i++) {
        uf->id[i] = i; // Ogni nodo è radice di se stesso
        uf->sz[i] = 1;
    }
    return uf;
}

void UFfree(UF uf) {
    free(uf->id);
    free(uf->sz);
    free(uf->history);
    free(uf);
}

/* --- 2. LOGICA CORE (FIND) --- */
/* Risaliamo l'albero fino alla radice */
static int findRoot(UF uf, int i) {
    while (i != uf->id[i]) {
        /* Compressione del percorso non implementata per semplicità di backtrack,
           ma si potrebbe fare salvando più stato. Qui rimaniamo standard. */
        i = uf->id[i];
    }
    return i;
}

int UFfind(UF uf, int p, int q) {
    return (findRoot(uf, p) == findRoot(uf, q));
}

/* --- 3. UNION CON SALVATAGGIO --- */
void UFunion(UF uf, int p, int q) {
    int i = findRoot(uf, p);
    int j = findRoot(uf, q);

    if (i == j) return; // Già connessi

    /* Weighted Quick Union: attacco l'albero più piccolo (i) a quello più grande (j) */
    /* Nota: Se sz[i] < sz[j] attacco i a j, altrimenti viceversa */
    
    if (uf->sz[i] < uf->sz[j]) {
        uf->id[i] = j;
        uf->sz[j] += uf->sz[i];
        
        /* Salvo nello stack cosa ho fatto */
        uf->history[uf->historyIndex].child = i;
        uf->history[uf->historyIndex].parent = j;
        uf->historyIndex++;
    } else {
        uf->id[j] = i;
        uf->sz[i] += uf->sz[j];
        
        /* Salvo nello stack */
        uf->history[uf->historyIndex].child = j;
        uf->history[uf->historyIndex].parent = i;
        uf->historyIndex++;
    }
}

/* --- 4. BACKTRACK (UNDO) --- */
/*
   Scopo: Ripristinare lo stato precedente all'ultima union.
   Fondamentale per l'algoritmo ricorsivo di esplorazione dello spazio delle soluzioni.
*/
void UFbacktrack(UF uf) {
    if (uf->historyIndex == 0) return; // Nulla da annullare

    uf->historyIndex--;
    int child = uf->history[uf->historyIndex].child;
    int parent = uf->history[uf->historyIndex].parent;

    /* Ripristino i puntatori e le dimensioni */
    uf->id[child] = child;         // Il figlio torna ad essere radice (o puntare a se stesso)
    uf->sz[parent] -= uf->sz[child]; // Tolgo la dimensione aggiunta al padre
}