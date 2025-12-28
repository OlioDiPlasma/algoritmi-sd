#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: merge3
 * ============================================================================
 * SCOPO:
 * Fonde tre array di interi già ordinati in un unico array ordinato.
 * La fusione avviene simultaneamente (senza passaggi intermedi).
 *
 * PARAMETRI:
 * - int *a, *b, *c:  Puntatori ai tre array di input (sorgenti).
 * - int na, nb, nc:  Dimensioni (numero di elementi) dei rispettivi array.
 *
 * RITORNO:
 * - int *: Puntatore al nuovo array allocato dinamicamente contenente il risultato.
 *
 * CONCETTI CHIAVE:
 * 1. Algoritmo di Merge Simultaneo:
 * Dobbiamo confrontare le "teste" attuali di tutti e tre i vettori.
 * Il minimo tra a[ia], b[ib] e c[ic] vince e viene copiato nel risultato.
 * L'indice del vettore vincitore avanza.
 *
 * 2. Gestione dei Limiti (Bounds Checking):
 * È fondamentale controllare sempre se un array è "esaurito" (indice == dimensione)
 * prima di leggere il suo valore, per evitare Segmentation Fault.
 * Se un array è finito, non partecipa più al confronto per il minimo.
 * ============================================================================
 */
int *merge3(int *a, int *b, int *c, int na, int nb, int nc) {

    /* --- 1. CALCOLO DIMENSIONE E ALLOCAZIONE --- */
    /* Calcoliamo la dimensione totale necessaria per il vettore risultato */
    int total_len = na + nb + nc;

    /*
     * MALLOC:
     * Richiediamo memoria allo HEAP.
     * La dimensione in byte è: (numero_elementi) * (dimensione_di_un_intero).
     * sizeof(int) è solitamente 4 byte, ma usando sizeof siamo portabili.
     */
    int *result = (int *)malloc(total_len * sizeof(int));

    /* Controllo difensivo: Se la RAM è piena, malloc ritorna NULL */
    if (result == NULL) {
        printf("Errore: memoria insufficiente.\n");
        return NULL;
    }

    /* --- 2. INIZIALIZZAZIONE INDICI --- */
    /* Usiamo 3 indici indipendenti per tenere traccia della posizione corrente in ogni array */
    int ia = 0; // Indice corrente per vettore a
    int ib = 0; // Indice corrente per vettore b
    int ic = 0; // Indice corrente per vettore c
    int k = 0;  // Indice corrente per il vettore risultato

    /* --- 3. CICLO DI FUSIONE (LOGICA PRINCIPALE) --- */
    /*
     * Dobbiamo riempire il vettore 'result' dalla cella 0 alla cella total_len - 1.
     * Ad ogni iterazione, troviamo il valore più piccolo tra i candidati disponibili.
     */
    for (k = 0; k < total_len; k++) {
        
        /* * STRATEGIA DI CONFRONTO:
         * Dobbiamo capire chi è il "vincitore" (il minimo) tra a[ia], b[ib] e c[ic].
         * Un array può essere considerato candidato SOLO SE non è ancora finito (es: ia < na).
         */

        /* IPOTESI 1: Vince A?
         * A vince se:
         * 1. A non è finito (ia < na)
         * E
         * 2. (B è finito OPPURE A <= B)
         * E
         * 3. (C è finito OPPURE A <= C)
         */
        if (ia < na && (ib >= nb || a[ia] <= b[ib]) && (ic >= nc || a[ia] <= c[ic])) {
            
            /* Dettaglio sintattico:
             * result[k] = a[ia];  -> Copio il valore
             * ia++;               -> Incremento l'indice di A per "consumare" il numero
             */
            result[k] = a[ia];
            ia++;
        }
        
        /* IPOTESI 2: Vince B?
         * Se siamo qui, A non ha vinto (o era finito, o era più grande di B o C).
         * B vince se:
         * 1. B non è finito (ib < nb)
         * E
         * 2. (C è finito OPPURE B <= C)
         *
         * Nota: Non serve ricontrollare A, perché se B fosse stato >= A (e A valido),
         * saremmo entrati nell'if precedente.
         */
        else if (ib < nb && (ic >= nc || b[ib] <= c[ic])) {
            
            result[k] = b[ib];
            ib++;
        }
        
        /* IPOTESI 3: Vince C?
         * Se né A né B hanno vinto, deve per forza toccare a C (se valido).
         */
        else {
            result[k] = c[ic];
            ic++;
        }
    }

    /* --- 4. RITORNO AL MAIN --- */
    return result;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Verificare che la fusione avvenga correttamente mantenendo l'ordine.
 * Gestione Memoria: Creiamo array statici per l'input, ma dobbiamo ricordarci
 * di fare FREE sul risultato dinamico!
 * ============================================================================
 */
int main() {
    /* --- 1. DATI DI PROVA HARDCODED --- */
    /* Tre vettori ORDINATI (requisito fondamentale dell'esercizio) */
    int v1[] = {1, 10, 20};
    int na = 3;

    int v2[] = {5, 15, 25, 30};
    int nb = 4;

    int v3[] = {2, 3, 100};
    int nc = 3;

    printf("Vettore 1: [ 1, 10, 20 ]\n");
    printf("Vettore 2: [ 5, 15, 25, 30 ]\n");
    printf("Vettore 3: [ 2, 3, 100 ]\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    int *vettore_fuso;
    vettore_fuso = merge3(v1, v2, v3, na, nb, nc);

    /* Verifica se la funzione ha fallito l'allocazione */
    if (vettore_fuso == NULL) {
        return 1;
    }

    /* --- 3. STAMPA DEL RISULTATO --- */
    printf("Risultato Merge: [ ");
    int tot = na + nb + nc;
    for (int i = 0; i < tot; i++) {
        /* Accesso all'array dinamico come se fosse un array normale */
        printf("%d ", vettore_fuso[i]);
    }
    printf("]\n");

    /* --- 4. PULIZIA DELLA MEMORIA (CRUCIALE) --- */
    /* * Il vettore 'vettore_fuso' risiede nello HEAP. Se non chiamiamo free,
     * quella memoria rimane occupata fino alla chiusura del programma (Memory Leak).
     */
    free(vettore_fuso);
    printf("Memoria liberata correttamente.\n");

    return 0;
}