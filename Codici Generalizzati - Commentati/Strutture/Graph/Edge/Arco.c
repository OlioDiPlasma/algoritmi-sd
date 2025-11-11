/* =====================
 * Implementazione delle funzioni per la struttura Edge.
 */

#include <stdlib.h> // Necessario per malloc()
#include "Edge.h"   // Include la definizione di 'Edge'

/**
 * @brief Crea e restituisce un Arco (Edge) per valore.
 * * Questa funzione è utile quando non serve un puntatore
 * (es. 'return EDGEcreate(...)').
 */
Edge EDGEcreate(int v, int w, int wt) {
    Edge e; // Crea la struttura (sullo stack)
    
    // Inizializza i campi
    e.v = v;
    e.w = w;
    e.wt = wt;
    
    // Ritorna una COPIA della struttura
    return e;
}


/**
 * @brief Alloca dinamicamente un Arco (Edge) e restituisce un PUNTATORE.
 *
 * Questa funzione è utile quando l'arco deve "sopravvivere"
 * alla funzione che lo crea (es. per inserirlo in una lista linkata).
 */
Edge *EDGEalloc(int v, int w, int wt) {
    // 1. Alloca memoria sull'heap grande quanto la struttura Edge
    Edge *e_ptr = malloc(sizeof(Edge));

    // 2. Controlla se malloc() ha fallito (memoria esaurita)
    if (e_ptr == NULL) {
        return NULL; // Ritorna NULL per segnalare l'errore
    }
    
    // 3. Inizializza i campi della struttura (usando l'operatore '->')
    e_ptr->v = v;
    e_ptr->w = w;
    e_ptr->wt = wt;
    
    // 4. Ritorna il PUNTATORE alla memoria allocata
    return e_ptr;
}