#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * STRUTTURE DATI
 * ======================================================================================
 * Definiamo la struttura del nodo e il tipo puntatore 'link' come standard
 * nei corsi di algoritmi in C.
 */
typedef struct node *link;

struct node {
    int item;   // Dato contenuto nel nodo (non essenziale per il calcolo, ma utile per debug)
    link l;     // Puntatore al sottoalbero sinistro (left)
    link r;     // Puntatore al sottoalbero destro (right)
};

/*
 * ======================================================================================
 * FUNZIONE: newNode
 * ======================================================================================
 * SCOPO:
 * Crea un nuovo nodo in memoria dinamica. Funzione di utilità per costruire l'albero.
 *
 * PARAMETRI:
 * - int item: Il valore intero da inserire nel nodo.
 * - link l: Puntatore al figlio sinistro.
 * - link r: Puntatore al figlio destro.
 *
 * GESTIONE MEMORIA:
 * Utilizza malloc per allocare lo spazio nell'heap. È fondamentale controllare
 * che malloc non ritorni NULL.
 */
link newNode(int item, link l, link r) {
    /* --- 1. ALLOCAZIONE --- */
    /* sizeof(*x) è una best practice: calcola la dimensione della struttura puntata da x
       indipendentemente dal tipo esplicito, riducendo errori in caso di refactoring. */
    link x = malloc(sizeof *x);
    
    /* Controllo difensivo: se la memoria è piena, terminiamo o gestiamo l'errore */
    if (x == NULL) {
        printf("Errore di allocazione memoria\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    /* Accesso ai campi tramite operatore freccia '->' perché x è un puntatore */
    x->item = item;
    x->l = l;
    x->r = r;
    
    return x;
}

/*
 * ======================================================================================
 * FUNZIONE: altezza_e_diametro_rec
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria ricorsiva che fa il lavoro pesante.
 * Visita l'albero e fa due cose contemporaneamente (ottimizzazione):
 * 1. Ritorna l'altezza del nodo corrente al chiamante (il genitore).
 * 2. Aggiorna il valore massimo del diametro trovato finora tramite un puntatore.
 *
 * PARAMETRI:
 * - link h: Il nodo corrente che stiamo analizzando.
 * - int *max_d: Puntatore a intero. Contiene il massimo diametro trovato finora in tutto l'albero.
 * Usiamo un puntatore per mantenere "memoria" tra le varie chiamate ricorsive.
 *
 * RITORNA:
 * - int: L'altezza del nodo 'h' (distanza massima in archi verso una foglia).
 *
 * CONCETTI CHIAVE:
 * - Post-order Traversal (Visita differita): Elaboriamo il nodo corrente SOLO DOPO
 * aver ottenuto i risultati dai figli.
 * - Altezza vs Diametro: L'altezza serve al genitore per calcolare la propria altezza.
 * Il diametro passante per il nodo corrente è (altezza_sinistra + altezza_destra + 2).
 * ======================================================================================
 */
int altezza_e_diametro_rec(link h, int *max_d) {
    /* --- 1. CASO BASE --- */
    /* Se il nodo è NULL, non esiste. La convenzione per l'altezza in archi è:
       altezza(foglia) = 0, quindi altezza(NULL) = -1. 
       Così quando facciamo -1 + 1 otteniamo 0 per la foglia. */
    if (h == NULL) {
        return -1;
    }

    /* --- 2. RICORSIONE (DISCESA) --- */
    /* Calcoliamo le altezze dei sottoalberi PRIMA di fare calcoli sul nodo corrente */
    int hl = altezza_e_diametro_rec(h->l, max_d); // Altezza sinistra
    int hr = altezza_e_diametro_rec(h->r, max_d); // Altezza destra

    /* --- 3. LOGICA DEL DIAMETRO (CORE) --- */
    /* Il diametro che passa esattamente per QUESTO nodo 'h' è dato dalla somma
       del cammino più lungo a sinistra e del cammino più lungo a destra.
       +2 rappresenta i due archi che collegano h ai suoi figli (se esistono).
       Esempio: se hl=0 (foglia a sx) e hr=0 (foglia a dx), diametro locale = 2. */
    int diametro_corrente = (hl + 1) + (hr + 1);

    /* Se questo percorso è il più lungo visto finora, aggiorniamo la variabile puntata */
    /* DETTAGLIO SINTATTICO: *max_d dereferenzia il puntatore per leggere/scrivere il valore intero */
    if (diametro_corrente > *max_d) {
        *max_d = diametro_corrente;
    }

    /* --- 4. RITORNO ALTEZZA --- */
    /* Ritorniamo al genitore l'altezza di questo nodo.
       L'altezza è 1 + il massimo tra l'altezza del figlio sinistro e destro. */
    return (hl > hr ? hl : hr) + 1;
}

/*
 * ======================================================================================
 * FUNZIONE: diameter (Wrapper richiesto dall'esercizio)
 * ======================================================================================
 * SCOPO:
 * Interfaccia pulita richiesta dal testo. Inizializza la variabile per il massimo
 * e lancia la ricorsione.
 *
 * PARAMETRI:
 * - link root: La radice dell'albero.
 *
 * RITORNA:
 * - int: Il diametro dell'albero.
 */
int diameter(link root) {
    int max_diametro = 0; // Variabile locale al wrapper per accumulare il risultato
    
    /* Passiamo l'indirizzo (&) di max_diametro affinché la funzione ricorsiva possa modificarla */
    altezza_e_diametro_rec(root, &max_diametro);
    
    return max_diametro;
}

/*
 * ======================================================================================
 * FUNZIONE: freeTree
 * ======================================================================================
 * SCOPO:
 * Libera la memoria allocata per l'intero albero per evitare Memory Leaks.
 *
 * GESTIONE MEMORIA:
 * È cruciale usare una visita in post-order (prima i figli, poi il padre).
 * Se liberassimo prima 'root', non potremmo più accedere a 'root->l' per liberarlo!
 */
void freeTree(link root) {
    if (root == NULL) return;
    freeTree(root->l);
    freeTree(root->r);
    free(root); // Libero il nodo solo dopo aver liberato i suoi discendenti
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * SCOPO:
 * Ricostruisce l'albero "verde" dell'esempio a sinistra e testa la funzione.
 * Struttura albero esempio (valori approssimativi basati sull'immagine):
 * 1
 * /   \
 * 2     3
 * / \
 * 4   5
 * /   /
 * 8   6
 * \
 * 7
 *
 * Il cammino più lungo è 8-4-2-5-6-7 (lunghezza 5 archi).
 */
int main() {
    printf("--- INIZIO TEST DIAMETRO ALBERO ---\n");

    /* --- 1. COSTRUZIONE ALBERO (BOTTOM-UP) --- */
    /* Costruiamo l'albero partendo dalle foglie per comodità, usando newNode */
    
    // Ramo sinistro estremo (sotto il 4)
    link n8 = newNode(8, NULL, NULL);
    
    // Ramo centrale sotto il 2 (il percorso 5-6-7)
    link n7 = newNode(7, NULL, NULL);
    link n6 = newNode(6, NULL, n7); // 6 ha destro 7
    link n5 = newNode(5, n6, NULL); // 5 ha sinistro 6
    
    // Nodo 4
    link n4 = newNode(4, n8, NULL); // 4 ha sinistro 8
    
    // Nodo 2 (snodo principale del diametro)
    link n2 = newNode(2, n4, n5);
    
    // Nodo 3 (lato destro radice)
    link n3 = newNode(3, NULL, NULL);
    
    // Radice 1
    link root = newNode(1, n2, n3);

    /* --- 2. ESECUZIONE --- */
    printf("Albero costruito. Calcolo diametro...\n");
    int d = diameter(root);

    /* --- 3. VERIFICA --- */
    printf("Diametro calcolato: %d\n", d);
    printf("Diametro atteso (dall'esempio): 5\n");
    
    if (d == 5) {
        printf("[SUCCESS] Il codice funziona correttamente.\n");
    } else {
        printf("[FAILURE] Risultato errato.\n");
    }

    /* --- 4. PULIZIA MEMORIA --- */
    printf("Liberazione memoria...\n");
    freeTree(root);
    printf("--- FINE TEST ---\n");

    return 0;
}