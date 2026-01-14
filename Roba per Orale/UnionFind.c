#include <stdio.h>
#include <stdlib.h>

// Struttura Union-Find
struct Subset {
    int parent; // Il genitore del nodo
    int rank;   // L'altezza approssimativa dell'albero (usata per il 'Weighted')
};

// Inizializzazione: Ogni nodo è genitore di se stesso (insiemi disgiunti)
void UFinit(struct Subset subsets[], int V) {
    for (int i = 0; i < V; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
}

// --- FIND con PATH COMPRESSION ---
// Trova il rappresentante (radice) dell'insieme a cui appartiene 'i'
// Mentre risale, appiattisce l'albero collegando i nodi direttamente alla radice.
int UFfind(struct Subset subsets[], int i) {
    // Se i non è il padre di se stesso, cerchiamo ricorsivamente
    if (subsets[i].parent != i) {
        // Path Compression: assegniamo direttamente il risultato alla parent
        subsets[i].parent = UFfind(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// --- UNION (Weighted / By Rank) ---
// Unisce due insiemi collegando la radice dell'albero più basso
// a quella dell'albero più alto.
void UFunion(struct Subset subsets[], int x, int y) {
    int xroot = UFfind(subsets, x);
    int yroot = UFfind(subsets, y);

    // Se sono già nello stesso insieme, non faccio nulla
    if (xroot == yroot) return;

    // Attacca l'albero più basso sotto quello più alto
    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        // Se hanno lo stesso rank, uno diventa figlio dell'altro
        // e il rank del nuovo padre aumenta
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Main di test per UnionFind (Opzionale)
/*
int main() {
    int V = 5;
    struct Subset* subsets = (struct Subset*)malloc(V * sizeof(struct Subset));
    UFinit(subsets, V);

    UFunion(subsets, 0, 2);
    UFunion(subsets, 4, 2); 
    // Ora 0, 2, 4 sono collegati
    
    if (UFfind(subsets, 0) == UFfind(subsets, 4))
        printf("0 e 4 sono nello stesso insieme\n");
    else
        printf("0 e 4 NON sono nello stesso insieme\n");

    return 0;
}
*/