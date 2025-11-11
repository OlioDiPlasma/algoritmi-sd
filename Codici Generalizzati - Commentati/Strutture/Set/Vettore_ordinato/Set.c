// Include il file header che definisce i tipi (SET, Item, Key) e i prototipi.
#include "Set.h"

// --- DEFINIZIONE STRUTTURA ---
// Questa è la definizione interna (nascosta) della struttura Set
struct set {
    Item *v; // Puntatore all'array (allocato dinamicamente)
    int N;   // Numero di elementi *attualmente* presenti nell'array
};

/**
 * @brief Inizializza (alloca) un nuovo set vuoto.
 * 'maxN' è la capacità massima iniziale dell'array.
 */
SET SETinit(int maxN) {
    // Alloca memoria per la struttura "wrapper" (il contenitore)
    SET s = malloc(sizeof *s);

    // Alloca memoria per l'array interno 'v'
    s->v = malloc(maxN*sizeof(Item));
    // Inizializza la dimensione corrente a 0 (set vuoto)
    s->N=0;

    // Ritorna il puntatore al nuovo set
    return s;
}

/**
 * @brief Libera tutta la memoria allocata per il set 's'.
 */
void SETfree(SET s) {
    // 1. Libera l'array interno 'v'
    free(s->v);
    // 2. Libera la struttura 'set' (il wrapper)
    free(s);
}

/**
 * @brief Cerca una chiave 'k' nel set 's' usando la RICERCA BINARIA.
 * (Presuppone che l'array s->v sia ORDINATO)
 * Ritorna 1 (vero) se trovato, 0 (falso) se non trovato.
 */
int SETsearch(SET s, Key k) {
    // l = indice sinistro, m = indice medio, r = indice destro
    int l = 0, m, r = s->N-1;

    // Cicla finché la finestra di ricerca è valida (l <= r)
    while (l <= r) {
        // Calcola l'indice medio (in modo sicuro per evitare overflow)
        m = l + (r-l)/2;
        
        // Controlla l'elemento centrale
        // (KEYeq/KEYget non mostrate)
        if (KEYeq(KEYget(s->v[m]), k))
            return 1; // Trovato!
            
        // Se l'elemento centrale è MINORE della chiave 'k'...
        if (KEYless(KEYget(s->v[m]), k)) // (KEYless non mostrata)
            // ...elimina la metà sinistra (cerca a destra)
            l = m+1;
        else // Altrimenti (l'elemento centrale è MAGGIORE)...
            // ...elimina la metà destra (cerca a sinistra)
            r = m-1;
    }

    // Se il loop finisce, la chiave non è stata trovata
    return 0;
}

/**
 * @brief Crea un nuovo set 's' che è l'UNIONE di s1 e s2.
 * (Presuppone che s1 e s2 siano ORDINATI, usa un algoritmo di MERGE)
 */
SET SETunion(SET s1, SET s2) {
    // i = indice per s1, j = indice per s2, k = indice per il new set s
    int i = 0, j = 0, k = 0, size1 = SETsize(s1), size2 = SETsize(s2);
    SET s; // Il nuovo set da restituire

    // Alloca il nuovo set (grande al massimo size1 + size2)
    s = SETinit(size1+size2);
    
    // Cicla finché 'k' non ha inserito tutti gli elementi
    // (il loop continua finché ALMENO uno dei due set ha elementi)
    for(k = 0; (i < size1) || (j < size2); k++)
        // Caso 1: s1 è finito, copia solo da s2
        if (i >= size1) 
            s->v[k] = s2->v[j++];
        // Caso 2: s2 è finito, copia solo da s1
        else if (j >= size2) 
            s->v[k] = s1->v[i++];
        // Caso 3: s1[i] < s2[j], copia s1[i]
        else if (ITEMless(s1->v[i], s2->v[j])) // (ITEMless non mostrata)
            s->v[k] = s1->v[i++];
        // Caso 4: s2[j] < s1[i], copia s2[j]
        else if (ITEMless(s2->v[j], s1->v[i])) 
            s->v[k] = s2->v[j++];
        // Caso 5: s1[i] == s2[j] (duplicato)
        else { 
            s->v[k] = s1->v[i++]; // Copia l'elemento una volta
            j++; // Salta il duplicato in s2
        }
        
    // Imposta la dimensione effettiva del new set 's'
    s->N = k;

    return s; // Ritorna il set unione
}

/**
 * @brief Crea un nuovo set 's' che è l'INTERSEZIONE di s1 e s2.
 * (Presuppone che s1 e s2 siano ORDINATI, usa un algoritmo di MERGE)
 */
SET SETintersection(SET s1, SET s2) {
    int i = 0, j = 0, k = 0, size1 = SETsize(s1), size2 = SETsize(s2), minsize;
    SET s;

    // Calcola la dimensione massima possibile (la più piccola dei due set)
    minsize = min(size1, size2); // (min non mostrata)
    // Alloca il new set
    s = SETinit(minsize);
    
    // Cicla solo finché ENTRAMBI i set hanno elementi
    while ((i < size1) && (j < size2)) {
        // Caso 1: Trovata un'intersezione (elementi uguali)
        if (ITEMeq(s1->v[i], s2->v[j])) { // (ITEMeq non mostrata)
            // Copia l'elemento in 's' e avanza tutti e tre gli indici
            s->v[k++] = s1->v[i++]; 
            j++;
        }
        // Caso 2: s1[i] < s2[j]
        else if (ITEMless(s1->v[i], s2->v[j]))
            // Avanza 'i' (l'elemento s1[i] non può essere in s2)
            i++;
        // Caso 3: s2[j] < s1[i]
        else 
            // Avanza 'j' (l'elemento s2[j] non può essere in s1)
            j++;
    }
    // Imposta la dimensione effettiva del new set 's'
    s->N = k;

    return s; // Ritorna il set intersezione
}