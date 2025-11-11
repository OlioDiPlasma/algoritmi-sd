// Include il file header che definisce i prototipi
#include "UF.h"

// --- VARIABILI GLOBALI (STATICHE) ---
// Poiché sono globali, può esistere solo UN set di queste
// variabili (una sola struttura UF) in tutto il programma.

// 'id' (parent array): id[i] memorizza il genitore dell'elemento i.
// Se id[i] == i, allora 'i' è la radice (rappresentante) del suo insieme.
static int *id; 
// 'sz' (size array): sz[i] memorizza il numero di elementi
// nell'insieme radicato in 'i' (usato solo se 'i' è una radice).
static int *sz;

/**
 * @brief Inizializza la struttura Union-Find (globale) per N elementi.
 */
void UFinit(int N) {
    int i; // Variabile contatore per il ciclo
    
    // Alloca memoria sull'heap per l'array 'id' (N interi)
    id = malloc(N*sizeof(int));
    // Alloca memoria sull'heap per l'array 'sz' (N interi)
    sz = malloc(N*sizeof(int));
    
    // Cicla per ogni elemento, da 0 a N-1
    for(i = 0; i < N; i++) {
        // Inizialmente, ogni elemento è genitore di se stesso
        // (Crea N insiemi distinti, ognuno con 1 elemento)
        id[i] = i; 
        
        // !!! BUG CRITICO !!!
        // La dimensione di ogni nuovo insieme (che contiene solo 'i')
        // dovrebbe essere 1.
        // Questa riga imposta la dimensione all'indice 'i',
        // il che renderà la logica "ponderata" (weighted)
        // in UFunion completamente errata.
        sz[i] = i; 
        // (La riga corretta è: sz[i] = 1;)
    }
}

/**
 * @brief Funzione 'find' (helper, statica/privata)
 * Trova la radice (il rappresentante) dell'insieme che contiene 'x'.
 * (Implementazione "Quick-Find" semplice, senza path compression)
 */
static int find(int x) {
    // 'i' è il nodo corrente, parte da 'x'
    int i = x;

    // Cicla "risalendo" l'albero:
    // Finché 'i' non è la radice (cioè 'i' non è genitore di se stesso)...
    while (i != id[i]) 
        // ...passa al genitore (i = genitore di i)
        i = id[i];

    // Ritorna 'i' (che ora è la radice)
    return i;
}

/**
 * @brief Funzione 'UFfind' (pubblica)
 * Controlla se due elementi 'p' e 'q' sono nello STESSO insieme.
 * Ritorna 1 (vero) se sono connessi, 0 (falso) altrimenti.
 */
int UFfind(int p, int q) { 
    // Trova la radice di 'p' e la radice di 'q'.
    // Se le radici sono le STESSE, appartengono allo stesso insieme.
    return (find(p) == find(q)); 
}

/**
 * @brief Funzione 'UFunion' (pubblica)
 * Unisce i due insiemi che contengono 'p' e 'q'.
 * (Implementazione "Weighted Quick-Union")
 */
void UFunion(int p, int q) {
    // Trova la radice dell'insieme di 'p' (la chiamiamo 'i')
    int i = find(p);
    // Trova la radice dell'insieme di 'q' (la chiamiamo 'j')
    int j = find(q);

    // Se 'i' e 'j' sono già la stessa radice...
    if (i == j) 
        return; // ...sono già nello stesso insieme. Non fare nulla.
        
    // Logica "Ponderata" (Weighted):
    // Confronta le dimensioni dei due alberi (insiemi)
    
    // Se l'albero 'i' è PIÙ PICCOLO dell'albero 'j'...
    // (basandosi sul (buggato) array 'sz')
    if (sz[i] < sz[j]) {
        // ...attacca la radice dell'albero più piccolo ('i')
        // sotto la radice dell'albero più grande ('j').
        id[i] = j; 
        // Aggiorna la dimensione dell'albero 'j'
        sz[j] += sz[i];
    } 
    // Altrimenti (se l'albero 'i' è più grande o uguale)...
    else {
        // ...attacca la radice dell'albero più piccolo ('j')
        // sotto la radice dell'albero più grande ('i').
        id[j] = i; 
        // Aggiorna la dimensione dell'albero 'i'
        sz[i] += sz[j];
    }
}