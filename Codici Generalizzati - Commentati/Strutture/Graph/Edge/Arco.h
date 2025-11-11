/* =====================
 * Definizione della struttura dati "Edge" (Arco).
 E' di supporto ai graph
 */

#ifndef EDGE_H
#define EDGE_H

// La struttura Edge "pacchettizza" i tre componenti
// di un arco pesato.
typedef struct {
    int v;  // Vertice 1 (partenza, se orientato)
    int w;  // Vertice 2 (arrivo, se orientato)
    int wt; // Peso (Weight)
} Edge;


/* * Crea e restituisce un Arco (Edge) per valore.
 * La struct 'Edge' viene creata sullo stack e poi copiata.
 */
Edge EDGEcreate(int v, int w, int wt);

/*
 * Alloca dinamicamente un Arco (Edge) e restituisce un PUNTATORE.
 * La struct 'Edge' viene creata sull'heap.
 * Nota: L'interfaccia deve restituire 'Edge *' (puntatore)
 * per essere una funzione 'alloc' utile.
 */
Edge *EDGEalloc(int v, int w, int wt);


#endif // EDGE_H