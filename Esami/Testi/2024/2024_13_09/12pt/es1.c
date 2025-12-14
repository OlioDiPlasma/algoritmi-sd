#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: diffVett
 * ============================================================================
 * SCOPO:
 * Crea un nuovo vettore contenente gli elementi presenti nel vettore 'a'
 * ma NON presenti nel vettore 'b'.
 *
 * PARAMETRI:
 * - a: Puntatore al primo vettore (ordinato, senza ripetizioni).
 * - na: Dimensione del vettore a.
 * - b: Puntatore al secondo vettore (ordinato, senza ripetizioni).
 * - nb: Dimensione del vettore b.
 * - nc: Puntatore a intero (output). Usato per restituire al chiamante
 * la dimensione del nuovo vettore creato.
 *
 * CONCETTI CHIAVE:
 * 1. Ordinamento: Poiché i vettori sono ordinati, non serve un doppio ciclo
 * annidato (che sarebbe lento, O(na*nb)). Possiamo scorrere i due vettori
 * parallelamente una sola volta (complessità lineare O(na+nb)).
 * 2. Passaggio per Riferimento: 'nc' è un int*. Lo usiamo perché la funzione
 * deve restituire DUE cose: il puntatore al nuovo vettore (tramite return)
 * e la sua dimensione (scrivendo nell'indirizzo puntato da nc).
 * 3. Allocazione Pessimistica: Non sappiamo a priori quanti elementi avrà 'c'.
 * Nel caso peggiore, nessun elemento di 'a' è in 'b', quindi 'c' sarà grande
 * quanto 'a'. Alochiamo 'na' e poi (opzionalmente) ridimensioniamo.
 * ============================================================================
 */
int* diffVett(const int *a, int na, const int *b, int nb, int *nc) {

    /* --- 1. ALLOCAZIONE MEMORIA --- */
    /*
     * Dobbiamo allocare lo spazio per il vettore risultato.
     * QUANTO SPAZIO? Nel caso peggiore (nessun elemento di 'a' è in 'b'),
     * il risultato avrà la stessa dimensione di 'a'.
     *
     * SINTASSI:
     * malloc(na * sizeof(int)) -> Richiede al sistema operativo un blocco di byte
     * pari a: numero elementi (na) MOLTIPLICATO PER la grandezza di un int (solitamente 4 byte).
     * Il cast (int*) converte il puntatore generico void* restituito da malloc.
     */
    int *c = (int*)malloc(na * sizeof(int));

    /* Controllo difensivo: se la malloc fallisce, restituiamo NULL e dimensione 0 */
    if (c == NULL) {
        *nc = 0; // Dereferenziamo nc per impostare la dimensione a 0
        return NULL;
    }

    /* --- 2. LOGICA DI SCANSIONE (ALGORITMO MERGE-LIKE) --- */
    int i = 0; // Indice per scorrere il vettore a
    int j = 0; // Indice per scorrere il vettore b
    int k = 0; // Indice per riempire il nuovo vettore c

    /*
     * Cicliamo finché entrambi i vettori hanno elementi da analizzare.
     * Confrontiamo l'elemento corrente di A con quello di B.
     */
    while (i < na && j < nb) {

        /* CASO A: a[i] < b[j]
         * Dato che i vettori sono ordinati, se a[i] è minore dell'attuale b[j],
         * significa che a[i] NON potrà mai trovarsi in b (perché i successivi b
         * saranno ancora più grandi).
         * QUINDI: a[i] fa parte della differenza. Lo salviamo.
         */
        if (a[i] < b[j]) {
            c[k] = a[i]; // Copiamo il valore nella cella k di c
            k++;         // Incrementiamo il contatore degli elementi trovati
            i++;         // Passiamo al prossimo elemento di a
        }
        /* CASO B: a[i] > b[j]
         * L'elemento di b è troppo piccolo. Dobbiamo avanzare in b per vedere
         * se troviamo un valore che pareggi a[i].
         */
        else if (a[i] > b[j]) {
            j++; // Avanziamo solo in b
        }
        /* CASO C: a[i] == b[j]
         * I valori sono uguali. L'elemento è presente in entrambi.
         * NON lo aggiungiamo a c (perché vogliamo a-b).
         * Avanziamo entrambi gli indici per scartare il valore.
         */
        else {
            i++;
            j++;
        }
    }

    /* --- 3. GESTIONE DEI RESIDUI --- */
    /*
     * Se il vettore 'b' finisce prima del vettore 'a' (j arriva a nb),
     * tutti gli elementi rimanenti in 'a' sono sicuramente non presenti in 'b'.
     * Dobbiamo copiarli tutti in coda a 'c'.
     */
    while (i < na) {
        c[k] = a[i]; // Copia valore
        k++;         // Incrementa indice destinazione
        i++;         // Incrementa indice sorgente
    }

    /* --- 4. OUTPUT E OTTIMIZZAZIONE MEMORIA --- */

    /*
     * SINTASSI: *nc = k;
     * Accediamo alla locazione di memoria puntata da 'nc' (nel main)
     * e ci scriviamo dentro il valore di k (numero effettivo di elementi trovati).
     */
    *nc = k;

    /*
     * OPZIONALE MA CONSIGLIATO (Realloc):
     * Abbiamo allocato spazio per 'na' elementi, ma ne abbiamo usati solo 'k'.
     * Se k < na, stiamo sprecando memoria. realloc ridimensiona il blocco.
     * Nota: se k è 0, realloc potrebbe comportarsi come free (dipende dall'implementazione),
     * quindi è bene gestirlo, ma per semplicità didattica qui ridimensioniamo solo se k > 0.
     */
    if (k > 0 && k < na) {
        int *temp = (int*)realloc(c, k * sizeof(int));
        if (temp != NULL) {
            c = temp;
        }
    }
    /* Se k è 0, potremmo liberare c e restituire NULL, ma restituire
       un puntatore valido (anche se a 0 elementi logici) è accettabile. */

    return c; // Restituisce l'indirizzo del vettore dinamico
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* Dati di prova hardcoded (ordinati e senza ripetizioni come da specifica) */
    int v1[] = {1, 2, 5, 7, 8, 10, 15};
    int n1 = 7; // Dimensione v1

    int v2[] = {2, 7, 9, 10, 12};
    int n2 = 5; // Dimensione v2

    int *risultato = NULL; // Puntatore che riceverà l'array dinamico
    int dim_ris = 0;       // Variabile per ospitare la dimensione del risultato

    printf("--- TEST FUNZIONE diffVett ---\n");
    printf("Vettore A: [ 1, 2, 5, 7, 8, 10, 15 ]\n");
    printf("Vettore B: [ 2, 7, 9, 10, 12 ]\n");
    printf("Atteso (A - B): [ 1, 5, 8, 15 ]\n\n");

    /*
     * CHIAMATA ALLA FUNZIONE
     * SINTASSI: &dim_ris
     * Passiamo l'INDIRIZZO della variabile dim_ris (operatore &).
     * Questo permette alla funzione di modificare il contenuto di dim_ris.
     */
    risultato = diffVett(v1, n1, v2, n2, &dim_ris);

    /* --- STAMPA E VERIFICA --- */
    printf("Risultato ottenuto (dimensione %d): [ ", dim_ris);
    for (int i = 0; i < dim_ris; i++) {
        /*
         * SINTASSI: risultato[i]
         * Accediamo all'i-esimo intero a partire dall'indirizzo 'risultato'.
         * Equivalente a *(risultato + i).
         */
        printf("%d ", risultato[i]);
    }
    printf("]\n");

    /* --- PULIZIA DELLA MEMORIA --- */
    /*
     * REGOLA D'ORO: Ogni malloc deve avere una corrispondente free.
     * Poiché 'diffVett' ha allocato memoria heap, il main è responsabile di liberarla
     * quando non serve più, per evitare Memory Leaks.
     */
    if (risultato != NULL) {
        free(risultato);
        printf("\nMemoria liberata correttamente.\n");
    }

    return 0;
}