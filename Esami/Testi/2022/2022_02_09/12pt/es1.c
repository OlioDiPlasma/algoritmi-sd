#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: differenza_vettori
 * ======================================================================================
 * SCOPO:
 * Crea un nuovo vettore che contiene gli elementi presenti in v1 ma NON in v2.
 * Poiché i vettori sono ordinati, usiamo una scansione lineare ottimizzata.
 * Gestisce inoltre l'unicità: niente duplicati nel vettore risultato.
 *
 * PARAMETRI:
 * - v1: Puntatore al primo vettore di interi (input, ordinato).
 * - v2: Puntatore al secondo vettore di interi (input, ordinato).
 * - dim1: Dimensione del primo vettore.
 * - dim2: Dimensione del secondo vettore.
 * - dim3: Puntatore a intero (OUTPUT). Qui scriveremo la dimensione effettiva di v3.
 *
 * RITORNO:
 * - int*: Puntatore al nuovo vettore allocato dinamicamente.
 *
 * CONCETTI CHIAVE:
 * - Puntatori per Output: Usiamo 'int *dim3' perché la funzione deve "comunicare" al main
 * quanti numeri ha trovato. Modificando '*dim3', la modifica persiste fuori dalla funzione.
 * - Allocazione Pessimistica: Non sapendo quanti elementi salveremo, allochiamo inizialmente
 * la dimensione massima possibile (dim1) e poi ridimensioniamo (realloc) alla fine.
 * - Logica a Due Indici: Usiamo 'i' per v1 e 'j' per v2 per scorrere entrambi i vettori
 * una sola volta.
 * ======================================================================================
 */
int* differenza_vettori(int *v1, int *v2, int dim1, int dim2, int *dim3) {

    /* --- 1. ALLOCAZIONE INIZIALE --- */
    /*
     * Dettaglio Sintattico:
     * malloc(bytes): Richiede al sistema operativo un blocco di memoria.
     * dim1 * sizeof(int): Calcoliamo i byte totali. "sizeof(int)" è cruciale
     * perché un int occupa solitamente 4 byte. Senza di esso, alloceremmo troppo poco spazio.
     * Caso peggiore: nessun elemento di v1 è in v2, quindi v3 sarà grande quanto v1.
     */
    int *v3 = (int *)malloc(dim1 * sizeof(int));

    /* Controllo difensivo: sempre verificare se la malloc ha fallito (ritorna NULL) */
    if (v3 == NULL) {
        printf("Errore: Memoria insufficiente!\n");
        exit(1);
    }

    /* --- 2. LOGICA DI SCANSIONE (Merge-like Logic) --- */
    int i = 0; // Indice per scorrere v1
    int j = 0; // Indice per scorrere v2
    int k = 0; // Indice per scrivere in v3 (sarà anche la nostra dimensione effettiva)

    /*
     * Ciclo principale: Scorriamo v1 finché non finisce.
     * Dobbiamo decidere per ogni v1[i] se tenerlo o scartarlo.
     */
    while (i < dim1) {

        /* CASO A: Trovata corrispondenza -> v1[i] è presente in v2.
         * Condizione: j non è finito E i valori sono uguali.
         */
        if (j < dim2 && v1[i] == v2[j]) {
            /* Scartiamo l'elemento di v1 (avanziamo i).
             * NON avanziamo necessariamente j, perché potrebbero esserci
             * duplicati in v1 (es: v1={2,2}, v2={2}). Entrambi i 2 di v1 vanno scartati.
             */
            i++;
        }
        /* CASO B: v2 è "indietro" -> v2[j] è più piccolo di v1[i].
         * Poiché sono ordinati, dobbiamo avanzare j per cercare di raggiungere v1[i].
         */
        else if (j < dim2 && v2[j] < v1[i]) {
            j++;
        }
        /* CASO C: v1[i] è "sicuro" -> o v2 è finito, o v1[i] < v2[j].
         * Questo significa che v1[i] NON esiste in v2.
         * Dobbiamo inserirlo in v3, ma ATTENZIONE ai duplicati interni a v1.
         */
        else {
            /*
             * Controllo Duplicati in Output:
             * Inseriamo v1[i] SOLO SE:
             * 1. È il primo elemento che inseriamo (k == 0)
             * OPPURE
             * 2. È diverso dall'ultimo elemento inserito (v3[k-1] != v1[i])
             */
            if (k == 0 || v3[k - 1] != v1[i]) {
                /*
                 * Dettaglio Sintattico: Assegnazione array
                 * Scriviamo il valore nella cella k e POI incrementiamo k.
                 */
                v3[k] = v1[i];
                k++;
            }
            /* In ogni caso, abbiamo "processato" questo v1[i], quindi avanziamo */
            i++;
        }
    }

    /* --- 3. AGGIORNAMENTO DIMENSIONE OUTPUT --- */
    /*
     * Dettaglio Sintattico: Dereferenziazione
     * *dim3 = k: Scriviamo il valore di k ALL'INDIRIZZO puntato da dim3.
     * Questo modifica la variabile 'dim_risultato' nel main.
     */
    *dim3 = k;

    /* --- 4. OTTIMIZZAZIONE MEMORIA (Realloc) --- */
    /*
     * Abbiamo allocato spazio per 'dim1' elementi, ma ne abbiamo usati solo 'k'.
     * realloc ridimensiona il blocco di memoria per risparmiare spazio.
     * Nota: se k è 0, realloc potrebbe comportarsi come free (dipende dall'implementazione),
     * ma qui gestiamo il caso generico.
     */
    if (k == 0) {
        free(v3);
        return NULL;
    } else {
        int *temp = realloc(v3, k * sizeof(int));
        if (temp != NULL) {
            v3 = temp;
        }
    }

    return v3;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DATI DI PROVA (Come nell'esempio dell'immagine) --- */
    int v1[] = {1, 2, 2, 3, 4, 5};
    int dim1 = 6;

    int v2[] = {1, 3};
    int dim2 = 2;

    int *v3 = NULL;     // Puntatore che accoglierà il nuovo array
    int dim3 = 0;       // Variabile per la dimensione del nuovo array

    printf("--- TEST ESERCIZIO ---\n");
    printf("Vettore 1: ");
    for(int i=0; i<dim1; i++) printf("%d ", v1[i]);
    printf("\n");

    printf("Vettore 2: ");
    for(int i=0; i<dim2; i++) printf("%d ", v2[i]);
    printf("\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /*
     * Dettaglio Sintattico: Operatore & (Address-of)
     * Passiamo &dim3 perché la funzione vuole un 'int *'.
     * Vuole l'indirizzo di dim3 per poterci scrivere dentro il risultato.
     */
    v3 = differenza_vettori(v1, v2, dim1, dim2, &dim3);

    /* --- 3. STAMPA RISULTATI --- */
    printf("\nRisultato v3 (dimensione %d): { ", dim3);
    for (int i = 0; i < dim3; i++) {
        /* Accesso classico all'array */
        printf("%d ", v3[i]);
    }
    printf("}\n");

    /* --- 4. GESTIONE MEMORIA (PULIZIA) --- */
    /*
     * IMPORTANTE: La memoria è stata allocata dentro la funzione con malloc/realloc.
     * È responsabilità del main (o di chi riceve il puntatore) liberarla.
     * Se non facciamo free(v3), creiamo un "Memory Leak".
     */
    if (v3 != NULL) {
        free(v3);
        v3 = NULL; // Buona norma per evitare "dangling pointers"
        printf("\nMemoria liberata correttamente.\n");
    }

    return 0;
}