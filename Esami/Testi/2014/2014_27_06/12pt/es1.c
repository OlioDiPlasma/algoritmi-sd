#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: ricerca
 * ============================================================================
 * SCOPO:
 * Cerca la prima occorrenza del vettore 'vet2' all'interno del vettore 'vet1'.
 * Restituisce l'indice in 'vet1' dove inizia la sequenza, oppure -1 se non trovata.
 *
 * PARAMETRI:
 * - int *vet1: Puntatore al primo elemento del vettore principale (dove cercare).
 * - int *vet2: Puntatore al primo elemento del sottovettore (cosa cercare).
 * - int d1:    Dimensione (numero di elementi) di vet1.
 * - int d2:    Dimensione (numero di elementi) di vet2.
 *
 * RITORNO:
 * - int: L'indice (0-based) della prima occorrenza, o -1.
 *
 * CONCETTI CHIAVE:
 * 1. Finestra Scorrevole (Sliding Window):
 * Immagina di sovrapporre 'vet2' sopra 'vet1'. Spostiamo 'vet2' di una posizione
 * alla volta verso destra e controlliamo se tutti i numeri combaciano.
 *
 * 2. Limite del Ciclo Esterno (d1 - d2):
 * È inutile cercare oltre l'indice (d1 - d2). Perché?
 * Se vet1 ha 10 elementi e vet2 ne ha 3, l'ultima posizione valida per iniziare
 * il confronto è l'indice 7 (elementi 7, 8, 9). Se iniziassimo all'indice 8,
 * avremmo solo 2 elementi rimasti in vet1, ma ne servono 3 per vet2.
 * Questo evita il famoso errore "Array Index Out of Bounds".
 * ============================================================================
 */
int ricerca(int *vet1, int *vet2, int d1, int d2) {

    /* --- 1. CONTROLLO VALIDITÀ INPUT --- */
    /*
     * Se i puntatori sono NULL o le dimensioni non hanno senso (es. vet2 più grande di vet1),
     * restituiamo subito errore.
     */
    if (vet1 == NULL || vet2 == NULL || d2 <= 0 || d1 < d2) {
        return -1;
    }

    /* --- 2. CICLO ESTERNO (SCORRIMENTO) --- */
    /*
     * Iteriamo su 'vet1' con l'indice 'i'.
     * Questo indice rappresenta il potenziale punto di INIZIO del sottovettore.
     * Condizione: i <= (d1 - d2).
     */
    int i;
    for (i = 0; i <= (d1 - d2); i++) {

        /* --- 3. LOGICA DI CONFRONTO (CICLO INTERNO) --- */
        /*
         * Assumiamo ottimisticamente che abbiamo trovato il match (match_found = 1).
         * Ora verifichiamo elemento per elemento.
         */
        int match_found = 1; // Flag booleano (1=Vero, 0=Falso)
        int j;

        /* Iteriamo su 'vet2' con l'indice 'j' */
        for (j = 0; j < d2; j++) {
            
            /* Dettaglio Sintattico:
             * vet1[i + j] -> Accedo all'elemento di vet1 spostato di 'j' posizioni rispetto a 'i'.
             * vet2[j]     -> Accedo all'elemento j-esimo del pattern.
             *
             * Se trovo ANCHE SOLO UN numero diverso, il match fallisce.
             */
            if (vet1[i + j] != vet2[j]) {
                match_found = 0; // Abbiamo trovato una discrepanza
                break;           // Interrompiamo subito il ciclo interno (ottimizzazione)
            }
        }

        /* --- 4. VERIFICA ESITO --- */
        /* Se dopo aver controllato tutti i 'd2' elementi il flag è ancora 1, abbiamo vinto! */
        if (match_found == 1) {
            return i; // Ritorniamo l'indice di inizio corrente
        }
    }

    /* Se il ciclo esterno finisce senza aver ritornato nulla, significa che non c'è match */
    return -1;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Testare la funzione con i dati dell'esempio fornito nell'immagine.
 * Gestione Memoria:
 * Anche se per vettori fissi potremmo usare array statici (int v[] = ...),
 * qui usiamo malloc/free per dimostrare la gestione rigorosa della memoria HEAP
 * come richiesto dallo stile "Professore".
 * ============================================================================
 */
int main() {
    
    /* Dati dell'esempio:
     * vet1: 0, 15, 12, 21, 7, 25, 32, 1 (Dimensione 8)
     * vet2: 21, 7, 25 (Dimensione 3)
     */
    int d1 = 8;
    int d2 = 3;

    /* --- 1. ALLOCAZIONE DINAMICA --- */
    /*
     * malloc(N * sizeof(int)):
     * Chiede al sistema operativo uno spazio contiguo di memoria capace di
     * contenere N interi. Restituisce un puntatore (indirizzo) al primo byte.
     */
    int *v1 = (int *)malloc(d1 * sizeof(int));
    int *v2 = (int *)malloc(d2 * sizeof(int));

    /* Controllo sempre se la malloc ha avuto successo (risultato != NULL) */
    if (v1 == NULL || v2 == NULL) {
        printf("Errore critico: memoria insufficiente.\n");
        return 1;
    }

    /* --- 2. POPOLAZIONE DATI (HARDCODED) --- */
    /* Popolo vet1 */
    v1[0] = 0;  v1[1] = 15; v1[2] = 12; v1[3] = 21;
    v1[4] = 7;  v1[5] = 25; v1[6] = 32; v1[7] = 1;

    /* Popolo vet2 */
    v2[0] = 21; v2[1] = 7;  v2[2] = 25;

    /* Stampa dei vettori per chiarezza */
    printf("Vettore 1 (Haystack): [ ");
    for(int k=0; k<d1; k++) printf("%d ", v1[k]);
    printf("]\n");

    printf("Vettore 2 (Needle):   [ ");
    for(int k=0; k<d2; k++) printf("%d ", v2[k]);
    printf("]\n");

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    int risultato = ricerca(v1, v2, d1, d2);

    /* --- 4. STAMPA RISULTATI --- */
    printf("\nRisultato atteso: 3\n");
    printf("Risultato ottenuto: %d\n", risultato);

    if (risultato == 3) {
        printf("--> TEST SUPERATO!\n");
    } else {
        printf("--> TEST FALLITO.\n");
    }

    /* --- 5. PULIZIA DELLA MEMORIA (FREE) --- */
    /*
     * Fondamentale: Restituiamo la memoria al sistema.
     * Senza questo passaggio, creiamo un 'Memory Leak'.
     */
    free(v1);
    free(v2);
    
    /* Buona norma: annullare i puntatori dopo la free per evitare accessi accidentali */
    v1 = NULL;
    v2 = NULL;

    return 0;
}